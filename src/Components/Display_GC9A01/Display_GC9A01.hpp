#ifndef Display_GC91A01_H
#define Display_GC91A01_H

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "AiEsp32RotaryEncoder.h"
#include "Components/Setup.hpp"

#include <PNGdec.h>


using namespace std;

enum BATTERYCOLOR
{
    RED = TFT_RED,
    ORANGE = TFT_ORANGE,
    YELLOW = TFT_YELLOW,
    GREEN = TFT_GREEN
};

static PNG _png;
static TFT_eSPI _display;
static int16_t _xpos = 50;
static int16_t _ypos = 50;

class DisplayGC9A01
{
public:
    // DisplayGC9A01();
    DisplayGC9A01(uint8_t resolution, uint64_t backgroundColor);

    void init(void);
    void splashScreen(bool inOut, String str1, String str2);
    void drawMenu(bool arcRoundedEnd, uint8_t thickness);
    void drawDynamicMenu(bool inOut, bool arcRoundedEnd, uint8_t thickness, Move cursorSt);
    void drawDynamicMenuIcons(bool state);
    void deviceBatteryManagement(uint8_t batteryLevel, bool arcRoundedEnd, uint8_t thickness);
    void phoneBatteryManagement(uint8_t batteryLevel, bool arcRoundedEnd, uint8_t thickness);
    void drawDataString(String str, int32_t x, int32_t y);
    void drawMenuTitle(String leftTitle, String centerTitle, String rightTitle);
    void drawArcString(void);
    void drawTime(String actual_time);
    void cursorManagement(Move current_state_menu, bool afterDynamicMenu);
    void drawData(String str, int placement);
    void clearData(int placement);
    void deviceStatus(bool state);
    void logo(uint8_t* logo);
    void setFontSprite(void);
    void initSprite(void);
    void drawUnit(MODE actualMode);
    static void pngDrawBLELogoStatus(PNGDRAW *pDraw);
    static void pngDrawLogo(PNGDRAW *pDraw);
    static void pngDrawMountainIcon(PNGDRAW *pDraw);
    static void pngDrawUrbanIcon(PNGDRAW *pDraw);
    static void pngDrawCustomIcon(PNGDRAW *pDraw);

    bool deviceConnected = false;

private:
    //TFT_eSPI _display;
    TFT_eSprite plane_s = TFT_eSprite(&_display);
    TFT_eSprite _sprite = TFT_eSprite(&_display);
    
    TFT_eSprite _splashScreenSprite = TFT_eSprite(&_display);
    TFT_eSprite _splashScreenSubTitleSprite = TFT_eSprite(&_display);
    
 
    TFT_eSprite _primaryDataSprite = TFT_eSprite(&_display);
    TFT_eSprite _secondaryDataSprite = TFT_eSprite(&_display);
    TFT_eSprite _tertiaryDataSprite = TFT_eSprite(&_display);

    TFT_eSprite _primaryDataUnitSprite = TFT_eSprite(&_display);
    TFT_eSprite _secondaryDataUnitSprite = TFT_eSprite(&_display);
    TFT_eSprite _tertiaryDataUnitSprite = TFT_eSprite(&_display);

    TFT_eSprite _menuTitleLeftSprite = TFT_eSprite(&_display);
    TFT_eSprite _menuTitleCenterSprite = TFT_eSprite(&_display);
    TFT_eSprite _menuTitleRightSprite = TFT_eSprite(&_display);

    TFT_eSprite _currentTimeSprite = TFT_eSprite(&_display);

    uint8_t _centerX;
    uint8_t _centerY;
    uint64_t _backgroundColor;
    uint8_t _resolution;
    uint8_t _radius;
    BATTERYCOLOR _arcBatteryColor;
    int16_t _rc;

    MODE _mode = MOUNTAIN;
    uint8_t _inner_radius;
};

#endif
