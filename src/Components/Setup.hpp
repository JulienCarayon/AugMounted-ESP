#ifndef _SETUP_HPP
#define _SETUP_HPP


#define BATTERY_PIN 35                      //named "SP" on wroom32 dev board
#define ROTARY_ENCODER_BUTTON_PIN 15
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_VCC_PIN 0 // 27 put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder 
#define ROTARY_ENCODER_STEPS 4

#define AA_FONT_SMALL "NotoSansBold15"
#define AA_FONT_LARGE "NotoSansBold36"

#define DISPLAY_RESOLUTION 240
#define DISPLAY_BACKGROUND_COLOR 0x0000
#define DISPLAY_MENU_COLOR 0x03E0
#define DISPLAY_DYNAMIC_MENU_COLOR 0x03E0

#define MAX_IMAGE_WDITH 20

#define DEV_MODE
#undef SHOW_BLE_DATA

typedef enum 
{
    MOUNTAIN = 0, 
    URBAN = 1, 
} MODE; 

/* typedef enum 
{
    LEFT = 0, 
    MIDDLE = 1, 
    RIGHT = 2,
} MOVE; */

typedef struct {
    std::string _get_speed;
    std::string _get_altitude;
    std::string _get_time;
    std::string _get_gps;
    std::string _get_phone_battery;
    std::string _get_local_temperature;
} Data;

#endif