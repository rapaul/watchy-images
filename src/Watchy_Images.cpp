#include "Watchy_Images.h"
#include "nk57_monospace_cd_bd12pt7b.h"




// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 50240)
const int albumCount = 83;
const unsigned char* albums[albumCount] = {
	aphex_twin_selected_ambient_works_85_92,
	dfa_records,
	discovery,
	french79_olympic,
	joy_division_unknown_pleasures,
	kraftwerk_computer_world,
	nin,
	sound_of_silver,
	the_xx_xx,
	rx7,
	epd_bitmap_Afar_tell_me_more,
	epd_bitmap_Animal_Collective_Merriweather_Post_Pavilion,
	epd_bitmap_Aphex_Twin_Collapse_EP,
	epd_bitmap_Arcade_Fire_The_Suburbs,
	epd_bitmap_Arctic_Monkeys_AM,
	epd_bitmap_Arctic_Monkeys_The_Car,
	epd_bitmap_Babe_Rainbow_The_Babe_Rainbow,
	epd_bitmap_Basement_Jaxx_The_Singles,
	epd_bitmap_Battles_Mirrored,
	epd_bitmap_Beach_House_Bloom,
	epd_bitmap_Bloc_Party_A_Weekend_In_the_City,
	epd_bitmap_Bon_Iver_Bon_Iver,
	epd_bitmap_Bonobo_The_North_Borders,
	epd_bitmap_Brian_Eno_Small_Craft_on_a_Milk_Sea,
	epd_bitmap_Broken_Bells_Broken_Bells,
	epd_bitmap_Canti_Human_music,
	epd_bitmap_Caribou_Swim,
	epd_bitmap_Charli_xcx_BRAT,
	epd_bitmap_Chet_Faker_Thinking_in_Textures,
	epd_bitmap_DJ_Shadow_Endtroducing_,
	epd_bitmap_Deftones_White_Pony,
	epd_bitmap_Dev_Lemons_In_The_Party,
	epd_bitmap_Digitalism_Idealism,
	epd_bitmap_Dizzee_Rascal_Tongue_n_Cheek,
	epd_bitmap_Duck_Sauce_Barbra_Streisand,
	epd_bitmap_Dwig_Big_Tales,
	epd_bitmap_Floating_Points_King_Bromeliad_Montparnasse,
	epd_bitmap_Flume_Flume,
	epd_bitmap_Foo_Fighters_The_Colour_and_the_Shape,
	epd_bitmap_Fred_again_Actual_Life_April_14_December_17_2020_,
	epd_bitmap_Friendly_Fires_Paris,
	epd_bitmap_Goodshirt_Good,
	epd_bitmap_Gorillaz_Demon_Days,
	epd_bitmap_Green_Day_Dookie,
	epd_bitmap_Grimes_Visions,
	epd_bitmap_Hot_Chip_In_Our_Heads,
	epd_bitmap_Jamie_xx_In_Colour,
	epd_bitmap_Jamie_xx_It_s_So_Good,
	epd_bitmap_Joe_Goddard_Electric_Lines_Bonus_Edition_,
	epd_bitmap_Jos_Gonz_lez_Veneer,
	epd_bitmap_Kendrick_Lamar_GNX,
	epd_bitmap_Khruangbin_Texas_Sun,
	epd_bitmap_Kings_of_Leon_Because_of_the_Times,
	epd_bitmap_Massive_Attack_Mezzanine,
	epd_bitmap_Metronomy_Nights_Out,
	epd_bitmap_Minotaur_Shock_Maritime,
	epd_bitmap_Mylo_Destroy_Rock_Roll,
	epd_bitmap_Peggy_Gou_Starry_Night,
	epd_bitmap_Radiohead_Kid_A,
	epd_bitmap_Red_Hot_Chili_Peppers_Blood_Sugar_Sex_Magik_Deluxe_Edition_,
	epd_bitmap_Rival_Consoles_Odyssey_Sonne,
	epd_bitmap_S3BZS_MONTAGEM_PR_FUNK,
	epd_bitmap_SBTRKT_SBTRKT,
	epd_bitmap_SJD_Songs_From_A_Dictaphone,
	epd_bitmap_Shapeshifter_Riddim_Wise,
	epd_bitmap_Simian_Mobile_Disco_Attack_Decay_Sustain_Release,
	epd_bitmap_Slowdive_everything_is_alive,
	epd_bitmap_Tame_Impala_Currents,
	epd_bitmap_The_Beatles_Revolver_Remastered_,
	epd_bitmap_The_Dare_What_s_Wrong_With_New_York_,
	epd_bitmap_The_Mint_Chicks_Crazy_Yes_Dumb_No_2016_Remastered_,
	epd_bitmap_The_Prodigy_The_Fat_of_the_Land,
	epd_bitmap_The_Shins_Wincing_the_Night_Away,
	epd_bitmap_The_Smashing_Pumpkins_Mellon_Collie_and_the_Infinite_Sadness,
	epd_bitmap_The_Streets_Original_Pirate_Material,
	epd_bitmap_The_Strokes_Angles,
	epd_bitmap_The_Whitest_Boy_Alive_Dreams,
	epd_bitmap_Thom_Yorke_The_Eraser,
	epd_bitmap_Tool_Lateralus,
	epd_bitmap_Underworld_Beaucoup_Fish,
	epd_bitmap_Vesky_Reverie,
	epd_bitmap_mindchatter_Kerosene,
	epd_bitmap_warner_case_summer_on_the_inside
};

void WatchyImages::drawWatchFace(){
    display.fillScreen(GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setFont(&nk57_monospace_cd_bd12pt7b); // ? width, 15px high

    syncTime();
    drawBackground();
    drawTime();
    drawDate();
    
    // drawBattery();

    //turn off radios
    WiFi.mode(WIFI_OFF);
    btStop();
}

void WatchyImages::syncTime(){
    if(currentTime.Hour == 4 && currentTime.Minute == 0) {
        if(connectWiFi()){
            getWeatherData();
        }
    }
}

void WatchyImages::drawBackground(){
    const unsigned char *image = albums[random(albumCount)];
    display.drawBitmap(0, 0, image, DISPLAY_WIDTH, DISPLAY_HEIGHT, GxEPD_BLACK);
    display.fillRect(0, 178, 200, 22, GxEPD_WHITE); // space for dynamic content
}

void WatchyImages::drawTime(){
    display.setCursor(0, 199);
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
    display.setCursor(137, 199);
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



void WatchyImages::drawBattery(){
    float batt = (getBatteryVoltage()-3.3)/0.9;
    display.drawLine(0,199,200*batt,199,GxEPD_BLACK);
}
