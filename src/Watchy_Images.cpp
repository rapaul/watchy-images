#include "Watchy_Images.h"
#include <Arduino_JSON.h>
#include <Fonts/FreeMonoBold12pt7b.h>

const uint8_t BATTERY_DISPLAY_START_Y = 34;
const uint8_t BATTERY_DISPLAY_START_X = 40;
const uint8_t BATTERY_DISPLAY_HEIGHT = 4;
const uint8_t BATTERY_SEGMENT_WIDTH = 20;
const int8_t NO_VALUE = 255;
RTC_DATA_ATTR int8_t officeTemperature = NO_VALUE;

const bool DARKMODE = false;

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 50240)
const int albumCount = 9;
const unsigned char* albums[albumCount] = {
	aphex_twin_selected_ambient_works_85_92,
	dfa_records,
	discovery,
	french79_olympic,
	joy_division_unknown_pleasures,
	kraftwerk_computer_world,
	lautundluise,
	sound_of_silver,
	the_xx_xx,
};

void WatchyImages::drawWatchFace(){
    display.fillScreen(DARKMODE ? GxEPD_BLACK : GxEPD_WHITE);
    display.setTextColor(DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setFont(&FreeMonoBold12pt7b); // ? width, 15px high
    drawBackground();
    drawTime();
    drawDate();
    drawSteps();
    drawBattery();
    drawTemperature();

    //turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
}

void WatchyImages::drawBackground(){
    const unsigned char *image = albums[currentTime.Minute % albumCount];
    display.drawBitmap(0, 0, image, DISPLAY_WIDTH, DISPLAY_HEIGHT, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.fillRect(0, 180, 200, 20, DARKMODE ? GxEPD_BLACK : GxEPD_WHITE); // space for dynamic content
}

void WatchyImages::drawTime(){
    display.setCursor(0, 195);
    if(currentTime.Hour < 10){
        display.print('0');
    }
    display.print(currentTime.Hour);
    display.print(':');
    if(currentTime.Minute < 10){
        display.print('0');
    }
    display.print(currentTime.Minute);
}

void WatchyImages::drawDate(){
    display.setCursor(131, 195);
    uint8_t day = currentTime.Day;
    if (day < 10){
        display.print("0");
    }
    display.print(day);
    display.print("/");
    uint8_t month = currentTime.Month;
    if (month < 10){
        display.print("0");
    }
    display.print(month);
}

String leftPad(uint32_t num, uint8_t digits) {
    String input = String(num);
    uint8_t needed = digits - input.length();
    String padded = "";
    for (int i = 0; i < needed; i++) {
        padded += " ";
    }
    padded += input;
    return padded;
}

void WatchyImages::drawSteps(){
    // reset step counter at midnight
    if (currentTime.Hour == 0 && currentTime.Minute == 0){
      sensor.resetStepCounter();
    }
    display.setCursor(0, 15);
    display.println(sensor.getCounter());
}

void WatchyImages::drawBattery(){
    float batt = (getBatteryVoltage()-3.3)/0.9;
    display.drawLine(0,199,200*batt,199,GxEPD_BLACK);
}

void WatchyImages::drawTemperature(){
    // Update every 30 minutes
    if(currentTime.Minute % 30 == 0) {
        officeTemperature = NO_VALUE;
        // Pull data from Prometheus on Raspberry Pi (via Grafana)
        if(connectWiFi()){
            HTTPClient http;
            http.setConnectTimeout(3000);//3 second max timeout
            String pibPromQuery = "http://pib:3000/api/datasources/proxy/1/api/v1/query?query=am2302_temp";
            http.begin(pibPromQuery.c_str());
            int httpResponseCode = http.GET();
            if(httpResponseCode == 200) {
                String payload = http.getString();
                JSONVar responseObject = JSON.parse(payload);
                float_t temp_float = String((const char*)(responseObject["data"]["result"][0]["value"][1])).toFloat();
                officeTemperature = (int) round(temp_float);
            }
            http.end();
        }
    }

    if(officeTemperature != NO_VALUE){
        display.setCursor(131, 15);
        String padded = leftPad(officeTemperature, 5);
        display.println(padded);
    }
}
