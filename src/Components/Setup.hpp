#ifndef _SETUP_HPP
#define _SETUP_HPP

//HARDWARE PIN
#define BATTERY_PIN 35                      //named "SP" on wroom32 dev board
#define ROTARY_ENCODER_BUTTON_PIN 15
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_VCC_PIN 0 // 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder 
#define ROTARY_ENCODER_STEPS 4

#define AA_FONT_SMALL "NotoSansBold15"
#define AA_FONT_LARGE "NotoSansBold36"

#define DISPLAY_RESOLUTION 240
#define DISPLAY_BACKGROUND_COLOR TFT_BLACK
#define DISPLAY_MENU_COLOR TFT_NAVY
#define DISPLAY_DYNAMIC_MENU_COLOR TFT_NAVY
#define ARC_ROUNDED_END false

//LOGO MAX SIZE
#define MAX_IMAGE_WDITH 60

//MENU and BATTERY ARC CIRCLE THICKNESS
#define MENU_THICKNESS 15
#define BATTERY_THICKNESS 6
#define BATTERY_PHONE_ARC_OFFSET 12

//TEXT COLOR
#define SPLASHSCREEN_TEXT_COLOR TFT_ORANGE
#define SPLASHSCREEN_SUBTITTLE_TEXT_COLOR TFT_GOLD
#define UNIT_TEXT_COLOR TFT_GREEN

//SPLASH_SCREEN TITLE
#define SPLASH_SCREEN_SPRITE_WIDTH 180
#define SPLASH_SCREEN_SPRITE_HEIGHT 40
#define SPLASH_SCREEN_SPRITE_POSX 90
#define SPLASH_SCREEN_SPRITE_POSY 20
//SPLASH_SCREEN SUBTITLE
#define SPLASH_SCREEN_SUB_SPRITE_WIDTH 180
#define SPLASH_SCREEN_SUB_SPRITE_HEIGHT 40
#define SPLASH_SCREEN_SUB_SPRITE_POSX 90
#define SPLASH_SCREEN_SUB_SPRITE_POSY -10

//PRIMARY_DATA
#define PRIMARY_DATA_SPRITE_WIDTH 57
#define PRIMARY_DATA_SPRITE_HEIGHT 35
#define PRIMARY_DATA_SPRITE_POSX 30
#define PRIMARY_DATA_SPRITE_POSY 60
//PRIMARY_UNIT
#define PRIMARY_DATA_UNIT_SPRITE_WIDTH 57
#define PRIMARY_DATA_UNIT_SPRITE_HEIGHT 15
#define PRIMARY_DATA_UNIT_SPRITE_POSX 30
#define PRIMARY_DATA_UNIT_SPRITE_POSY 15

//SECONDARY_DATA
#define SECONDARY_DATA_SPRITE_WIDTH 50
#define SECONDARY_DATA_SPRITE_HEIGHT 20
#define SECONDARY_DATA_SPRITE_POSX 65
#define SECONDARY_DATA_SPRITE_POSY -20
//SECONDARY_UNIT
#define SECONDARY_DATA_UNIT_SPRITE_WIDTH 50
#define SECONDARY_DATA_UNIT_SPRITE_HEIGHT 15
#define SECONDARY_DATA_UNIT_SPRITE_POSX 65
#define SECONDARY_DATA_UNIT_SPRITE_POSY -50

//TERTIARY_DATA
#define TERTIARY_DATA_SPRITE_WIDTH 50
#define TERTIARY_DATA_SPRITE_HEIGHT 20
#define TERTIARY_DATA_SPRITE_POSX -15
#define TERTIARY_DATA_SPRITE_POSY -20
//TERTIARY_UNIT
#define TERTIARY_DATA_UNIT_SPRITE_WIDTH 50
#define TERTIARY_DATA_UNIT_SPRITE_HEIGHT 15
#define TERTIARY_DATA_UNIT_SPRITE_POSX -15
#define TERTIARY_DATA_UNIT_SPRITE_POSY -50

//LOGOS POSITION
#define LOGO_MOUNTED_POSX 50
#define LOGO_MOUNTED_POSY 97
//BLE 
#define BLE_LOGO_POSX 100
#define BLE_LOGO_POSY 190
//MOUTAIN
#define MOUTAIN_ICON_POSX 38
#define MOUTAIN_ICON_POSY 160
//URBAN
#define URBAN_ICON_POSX 25
#define URBAN_ICON_POSY 112
//CUSTOM
#define CUSTOM_ICON_POSX 38
#define CUSTOM_ICON_POSY 66

//DEBUG
#define DEV_MODE
#undef SHOW_BLE_DATA

//FONT
#include "../include/fonts/font.h"
#include "../include/fonts/numberFont.h"
#include "../include/fonts/unitFont.h"
#include "../include/fonts/secondaryDataFont.h"
#include "../include/fonts/splashscreenTitle.h"
#include "../include/fonts/splashscreenSubTitle.h"

//LOGO
#include "../include/logos_hex/logo_mounted.h"
#include "../include/logos_hex/urban.h"
#include "../include/logos_hex/mountain.h"
#include "../include/logos_hex/custom.h"
#include "../include/logos_hex/ble_connected.h"
#include "../include/logos_hex/ble_disconnected.h"

typedef enum 
{
    MOUNTAIN = 0, 
    URBAN = 1, 
    CUSTOM = 2,
} MODE; 

/* typedef enum 
{
    LEFT = 0, 
    MIDDLE = 1, 
    RIGHT = 2,
} MOVE; */

typedef struct {
    std::string _device_connected;
    std::string _get_speed;
    std::string _get_altitude;
    std::string _get_time;
    std::string _get_gps;
    std::string _get_phone_battery;
    std::string _get_local_temperature;
} Data;

#endif