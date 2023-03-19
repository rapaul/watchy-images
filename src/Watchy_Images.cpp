#include "Watchy_Images.h"
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

const uint8_t BATTERY_DISPLAY_START_Y = 34;
const uint8_t BATTERY_DISPLAY_START_X = 40;
const uint8_t BATTERY_DISPLAY_HEIGHT = 4;
const uint8_t BATTERY_SEGMENT_WIDTH = 20;

void WatchyImages::drawWatchFace(){
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_WHITE);

    syncTime();
    drawBackground();
    drawTime();
    drawDate();
    drawSteps();
    drawBattery();

    //turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
}

void WatchyImages::syncTime(){
    if(currentTime.Hour == 5 && currentTime.Minute == 0) {
        if(connectWiFi()){
            syncNTP();
        }
    }
}

void WatchyImages::drawBackground(){
    int gap = 10;
    int displaySize = 200;
    for (int x = -displaySize; x < displaySize; x = x + gap) {
        display.drawLine(x, 0, x+displaySize, displaySize, GxEPD_BLACK);
        display.drawLine(x+1, 0, x+displaySize+1, displaySize, GxEPD_BLACK); // make lines fat
        display.drawLine(x+2, 0, x+displaySize+2, displaySize, GxEPD_BLACK); // make lines fat
        display.drawLine(x+3, 0, x+displaySize+3, displaySize, GxEPD_BLACK); // make lines fat
        display.drawLine(x+4, 0, x+displaySize+4, displaySize, GxEPD_BLACK); // make lines fat
        display.drawLine(x+6, 0, x+displaySize+6, displaySize, GxEPD_BLACK); // make lines fat (with a gap)
    }
}

void WatchyImages::drawTime(){
    display.setFont(&FreeMonoBold24pt7b);
    display.setCursor(29, 70);
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
    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(29, 130);
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
    display.setFont(&FreeMonoBold18pt7b);
    display.setCursor(29, 170);
    display.println(sensor.getCounter());
}

void WatchyImages::drawBattery(){
    float batt = (getBatteryVoltage()-3.3)/0.9;
    display.fillRect(0, 197, 200*batt, 3, GxEPD_BLACK);
}
