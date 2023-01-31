#include "Display_GC9A01.hpp"

DisplayGC9A01::DisplayGC9A01(uint8_t resolution, uint64_t backgroundColor)
{
    _backgroundColor = backgroundColor;
    _resolution = resolution;
    _radius = resolution / 2;
    _centerX = resolution / 2;
    _centerY = resolution / 2;
}
      
void DisplayGC9A01::setFontSprite(void)
{
    _splashScreenSprite.setFreeFont(&Syncopate_Bold_16);
    _splashScreenSubTitleSprite.setFreeFont(&Syncopate_Bold_12);
    _splashScreenSprite.setTextColor(TFT_GOLD, TFT_BLACK, true);
    _splashScreenSubTitleSprite.setTextColor(TFT_LIGHTGREY, TFT_BLACK, true);
    _splashScreenSprite.setTextDatum(MC_DATUM);
    _splashScreenSubTitleSprite.setTextDatum(MC_DATUM);

    _primaryDataSprite.setFreeFont(&Orbitron_Bold_35);
    _primaryDataUnitSprite.setFreeFont(&Roboto_Bold_15);
    _primaryDataUnitSprite.setTextColor(UNIT_TEXT_COLOR, TFT_BLACK, true);
    _primaryDataSprite.setTextDatum(MC_DATUM);
    _primaryDataUnitSprite.setTextDatum(MC_DATUM);

    _secondaryDataSprite.setFreeFont(&Orbitron_Bold_17);
    _secondaryDataUnitSprite.setFreeFont(&Roboto_Bold_15);
    _secondaryDataUnitSprite.setTextColor(UNIT_TEXT_COLOR, TFT_BLACK, true);
    _secondaryDataSprite.setTextDatum(MC_DATUM);
    _secondaryDataUnitSprite.setTextDatum(MC_DATUM);
    
    _tertiaryDataSprite.setFreeFont(&Orbitron_Bold_17);
    _tertiaryDataUnitSprite.setFreeFont(&Roboto_Bold_15);
    _tertiaryDataUnitSprite.setTextColor(UNIT_TEXT_COLOR, TFT_BLACK, true);
    _tertiaryDataSprite.setTextDatum(MC_DATUM);
    _tertiaryDataUnitSprite.setTextDatum(MC_DATUM);
}

void DisplayGC9A01::initSprite(void)
{
    _splashScreenSprite.createSprite(SPLASH_SCREEN_SPRITE_WIDTH, SPLASH_SCREEN_SPRITE_HEIGHT);
    _splashScreenSubTitleSprite.createSprite(SPLASH_SCREEN_SUB_SPRITE_WIDTH, SPLASH_SCREEN_SUB_SPRITE_HEIGHT);

    _primaryDataSprite.createSprite(PRIMARY_DATA_SPRITE_WIDTH,PRIMARY_DATA_SPRITE_HEIGHT);
    _primaryDataUnitSprite.createSprite(PRIMARY_DATA_UNIT_SPRITE_WIDTH, PRIMARY_DATA_UNIT_SPRITE_HEIGHT);

    _secondaryDataSprite.createSprite(SECONDARY_DATA_SPRITE_WIDTH,SECONDARY_DATA_SPRITE_HEIGHT);
    _secondaryDataUnitSprite.createSprite(SECONDARY_DATA_UNIT_SPRITE_WIDTH, SECONDARY_DATA_UNIT_SPRITE_HEIGHT);

    _tertiaryDataSprite.createSprite(TERTIARY_DATA_SPRITE_WIDTH,TERTIARY_DATA_SPRITE_HEIGHT);
    _tertiaryDataUnitSprite.createSprite(TERTIARY_DATA_UNIT_SPRITE_WIDTH, TERTIARY_DATA_UNIT_SPRITE_HEIGHT);

    setFontSprite();

    _splashScreenSprite.setPivot(SPLASH_SCREEN_SPRITE_POSX, SPLASH_SCREEN_SPRITE_POSY);
    _splashScreenSubTitleSprite.setPivot(SPLASH_SCREEN_SUB_SPRITE_POSX, SPLASH_SCREEN_SUB_SPRITE_POSY);

    _primaryDataSprite.setPivot(PRIMARY_DATA_SPRITE_POSX, PRIMARY_DATA_SPRITE_POSY);
    _primaryDataUnitSprite.setPivot(PRIMARY_DATA_UNIT_SPRITE_POSX, PRIMARY_DATA_UNIT_SPRITE_POSY);

    _secondaryDataSprite.setPivot(SECONDARY_DATA_SPRITE_POSX, SECONDARY_DATA_SPRITE_POSY);
    _secondaryDataUnitSprite.setPivot(SECONDARY_DATA_UNIT_SPRITE_POSX, SECONDARY_DATA_UNIT_SPRITE_POSY);

    _tertiaryDataSprite.setPivot(TERTIARY_DATA_SPRITE_POSX, TERTIARY_DATA_SPRITE_POSY);
    _tertiaryDataUnitSprite.setPivot(TERTIARY_DATA_UNIT_SPRITE_POSX, TERTIARY_DATA_UNIT_SPRITE_POSY);
}

void DisplayGC9A01::init(void)
{
    _display.init();
    _display.setRotation(2);
    _display.setPivot(_radius, _radius);
    _display.setFreeFont(&Roboto_Black_19);
    _display.setTextColor(TFT_WHITE, TFT_BLACK, true);
    initSprite();
    splashScreen(true, "AUGMOUNTED", "LOADING ...");
    drawUnit(_mode);
    deviceStatus(deviceConnected);
    cursorManagement(MIDDLE, false);
}

void DisplayGC9A01::splashScreen(bool inOut, String str1, String str2)
{
    _display.fillScreen(_backgroundColor);

    _rc = _png.openFLASH((uint8_t*)logo_mounted, sizeof(logo_mounted), pngDrawLogo);
    if (_rc == PNG_SUCCESS) {
        _display.startWrite();
        _rc = _png.decode(NULL, 0);
        _display.endWrite();
        _png.close();
    }

    _splashScreenSprite.drawString(str1, _splashScreenSprite.width()/2, _splashScreenSprite.height()/2);
    _splashScreenSprite.pushRotated(270, TFT_BLACK);

    _splashScreenSubTitleSprite.drawString(str2, _splashScreenSubTitleSprite.width()/2, _splashScreenSubTitleSprite.height()/2);
    _splashScreenSubTitleSprite.pushRotated(270, TFT_BLACK);

    for (int i = 0; i < 360; i++)
    {   
        _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 1+i, TFT_GOLD, _backgroundColor, false);
        if(i<10)
            delay(5);
        else
            delay(1);
    }

    if(inOut) {
        for(int i = 0; i<4; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 360, TFT_BLACK, _backgroundColor, false);
            delay(100);
            _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 360, TFT_LIGHTGREY, _backgroundColor, false);
            delay(100); 
        }
    }
    else {
        _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 360, TFT_LIGHTGREY, _backgroundColor, false);
        delay(1000);
    }
    _display.fillScreen(_backgroundColor);
}   

void DisplayGC9A01::deviceStatus(bool state)
{
    if(state) {
        _rc = _png.openFLASH((uint8_t*)ble_connected, sizeof(ble_connected), pngDrawBLELogoStatus);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }
    } else {
        _rc = _png.openFLASH((uint8_t*)ble_disconnected, sizeof(ble_disconnected), pngDrawBLELogoStatus);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }
    }
}

void DisplayGC9A01::drawMenu(bool arcRoundedEnd, uint8_t thickness)
{
    _inner_radius = _radius - (thickness);
    _display.drawSmoothArc(_centerX, _centerY, _radius, _inner_radius, 0, 160, DISPLAY_MENU_COLOR, _backgroundColor, arcRoundedEnd);
}

void DisplayGC9A01::drawDynamicMenu(bool inOut, bool arcRoundedEnd, uint8_t thickness, Move cursorSt)
{
    Serial.println("DisplayGC9A01 drawDynamicMenu");
    _inner_radius = ((_radius) - (thickness / 2)) - 3; // Calculate inner radius (can be 0 for circle segment)
    if (inOut) {
        //  fade in
        for (int i = 0; i < 20; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, _inner_radius, _inner_radius - i, 40, 140, DISPLAY_DYNAMIC_MENU_COLOR, _backgroundColor, arcRoundedEnd);
        }
        drawDynamicMenuIcons(true);
     /*   
    _xpos = 100;
    _ypos = 70;
    
    int16_t rc = _png.openFLASH((uint8_t*)urban, sizeof(urban), pngDraw);
    Serial.println("logo function");
    Serial.println(rc);
    if (rc == PNG_SUCCESS) {
        Serial.println("succes");
        _display.startWrite();
        rc = _png.decode(NULL, 0);
        _display.endWrite();
        _png.close(); // not needed for memory->memory decode
    }

    _xpos = 40;
    _ypos = 40;
    
    rc = _png.openFLASH((uint8_t*)mountain, sizeof(mountain), pngDraw);
    Serial.println("logo function");
    Serial.println(rc);
    if (rc == PNG_SUCCESS) {
        Serial.println("succes");
        _display.startWrite();
        rc = _png.decode(NULL, 0);
        _display.endWrite();
        _png.close(); // not needed for memory->memory decode
    }
    */
    }
    else
    {
        drawDynamicMenuIcons(false);
        // fade out
        for (int i = 0; i <= 20; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, (_inner_radius - 20) + i, _inner_radius - 20, 40, 140, _backgroundColor, _backgroundColor, arcRoundedEnd);
        }
        cursorManagement(cursorSt, true);
    }
}

void DisplayGC9A01::drawDynamicMenuIcons(bool state)
{
    if(state) {
        _rc = _png.openFLASH((uint8_t*)urban, sizeof(urban), pngDrawUrbanIcon);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }

        _rc = _png.openFLASH((uint8_t*)mountain, sizeof(mountain), pngDrawMountainIcon);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }

        _rc = _png.openFLASH((uint8_t*)custom, sizeof(custom), pngDrawCustomIcon);
        if (_rc == PNG_SUCCESS) {
            _display.startWrite();
            _rc = _png.decode(NULL, 0);
            _display.endWrite();
            _png.close();
        }
    } else {
        
    }
}

void DisplayGC9A01::deviceBatteryManagement(uint8_t batteryLevel, bool arcRoundedEnd, uint8_t thickness)
{
    _inner_radius = _radius - (thickness / 2);

    if (batteryLevel > 110)
        _arcBatteryColor = GREEN;
    else if (batteryLevel > 65 && batteryLevel <= 109)
        _arcBatteryColor = YELLOW;
    else if (batteryLevel > 40 && batteryLevel <= 65)
        _arcBatteryColor = ORANGE;
    else
        _arcBatteryColor = RED;

    // 160 because batteryLevel is mapped from 0->4096 to 1->159
    _display.drawSmoothArc(_centerX, _centerY, _radius, _inner_radius, 200 + (160 - batteryLevel), 360, _arcBatteryColor, _backgroundColor, arcRoundedEnd);
    _display.drawSmoothArc(_centerX, _centerY, _radius, _inner_radius, 200, 200 + (160 - batteryLevel), TFT_BLACK, _backgroundColor, arcRoundedEnd);
}

void DisplayGC9A01::phoneBatteryManagement(uint8_t batteryLevel, bool arcRoundedEnd, uint8_t thickness)
{
    _inner_radius = _radius - (thickness / 2);

    if (batteryLevel > 110)
        _arcBatteryColor = GREEN;
    else if (batteryLevel > 65 && batteryLevel <= 109)
        _arcBatteryColor = YELLOW;
    else if (batteryLevel > 40 && batteryLevel <= 65)
        _arcBatteryColor = ORANGE;
    else
        _arcBatteryColor = RED;

    _display.drawSmoothArc(_centerX, _centerY, _radius - BATTERY_PHONE_ARC_OFFSET, _inner_radius - BATTERY_PHONE_ARC_OFFSET, 200 + (160 - batteryLevel), 360, _arcBatteryColor, _backgroundColor, arcRoundedEnd);
    _display.drawSmoothArc(_centerX, _centerY, _radius - BATTERY_PHONE_ARC_OFFSET, _inner_radius - BATTERY_PHONE_ARC_OFFSET, 200, 200 + (160 - batteryLevel), TFT_BLACK, _backgroundColor, arcRoundedEnd);
}

void DisplayGC9A01::drawDataString(String str, int32_t x, int32_t y)
{
    _display.drawString(str, x, y, 4);
}

void DisplayGC9A01::drawArcString(void)
{
    //_display.drawString("CO3", 110, 110, 6);

    _display.setPivot(120, 120); // Set pivot to middle of screen

    _sprite.loadFont(AA_FONT_SMALL);
    _sprite.createSprite(40, 40); // Create a new Sprite 40x30

    _sprite.setPivot(20, 130); // Set Sprite pivot at 20,80

    _sprite.setTextColor(TFT_WHITE); // Red text in Sprite
    _sprite.setTextSize(2);
    _sprite.setTextDatum(BR_DATUM); // Middle centre datum

    for (int16_t angle = 250; angle <= 360; angle += 10)
    {
        _sprite.fillSprite(TFT_BLACK); // Clear the Sprite
        // spr.drawNumber(num, 20, 15, 4);    // Plot number, in Sprite at 20,15 and with font 4
        if (angle == 250)
        {
            _sprite.loadFont(AA_FONT_SMALL);
            //_sprite.drawChar(77,20,15);
            _sprite.drawString("M", 0, 0, 2);
        }
        else if (angle == 260)
            _sprite.drawChar(69, 20, 15);
        else if (angle == 270)
            _sprite.drawChar(78, 20, 15);
        else if (angle == 280)
            _sprite.drawChar(85, 20, 15);

        _sprite.pushRotated(angle, TFT_BLACK); // Plot rotated Sprite, black being transparent
    }
}

void DisplayGC9A01::drawMenuTitle(void)
{

    _sprite.deleteSprite();
    _sprite.setColorDepth(8);      // Create an 8bpp Sprite of 60x30 pixels
    _sprite.createSprite(70, 30);  // 8bpp requires 64 * 30 = 1920 bytes
    _sprite.setPivot(35, _radius); // Set pivot relative to top left corner of Sprite
    _sprite.loadFont(AA_FONT_SMALL);
    _sprite.setTextColor(TFT_WHITE);    // Green text
    _sprite.setTextDatum(MC_DATUM);     // Middle centre datum
    _sprite.setTextSize(1);
    _sprite.drawString("MENU", 35, 12); // Plot text, font 4, in Sprite at 30, 15
    _sprite.pushRotated(270, TFT_BLACK);

    _sprite.deleteSprite();
    _sprite.setColorDepth(8);
    _sprite.createSprite(70, 30);
    _sprite.setPivot(35, _radius);
    _sprite.loadFont(AA_FONT_SMALL);
    _sprite.setTextColor(TFT_WHITE);
    _sprite.setTextDatum(MC_DATUM);    // Middle centre datum
    _sprite.drawString("OFF", 35, 12); // Plot text, font 4, in Sprite at 30, 15
    _sprite.pushRotated(230, TFT_BLACK);

    _sprite.deleteSprite();
    _sprite.setColorDepth(8);
    _sprite.createSprite(70, 30);
    _sprite.setPivot(35, _radius);
    _sprite.loadFont(AA_FONT_SMALL);
    _sprite.setTextColor(TFT_WHITE);
    _sprite.setTextDatum(MC_DATUM);     // Middle centre datum
    _sprite.drawString("CONF", 35, 12); // Plot text, font 4, in Sprite at 30, 15
    _sprite.pushRotated(310, TFT_BLACK);
}

void DisplayGC9A01::drawTime(String actual_time)
{
    // time format : XX:YY:ZZ
    // we just need to keep hours (XX), and minutes (YY)

    Serial.println("Time");
    clearData(3);

    String hours = actual_time.substring(0, 2);
    String minutes = actual_time.substring(3, 5);

    _sprite.deleteSprite();
    _sprite.loadFont(AA_FONT_SMALL);
    _sprite.createSprite(40, 50);
    _sprite.setPivot(-90, 25);
    _sprite.setTextDatum(MC_DATUM);
    _sprite.drawString(hours, _sprite.width() / 2, 15);
    _sprite.drawLine(10, _sprite.height() / 2, _sprite.width() - 8, _sprite.height() / 2, TFT_DARKGREY);
    _sprite.drawString(minutes, _sprite.width() / 2, _sprite.height() - 10);
    _sprite.pushRotated(270, TFT_BLACK);
}

void DisplayGC9A01::cursorManagement(Move current_state_menu, bool afterDynamicMenu)
{
    // position[0] : LEFT      30 -70
    // position[1] : MIDDLE    70 - 110
    // position[2] : RIGHT     110 - 150
    const uint8_t position[] = {30, 70, 110};
    static Move old_value = MIDDLE;
    const int size = 40;

    if (afterDynamicMenu)
    {
        _display.drawSmoothArc(_centerX, _centerY, 97, 95, position[1], position[1] + size, TFT_RED, _backgroundColor, false);
        return;
    }
    if (current_state_menu == MIDDLE)
    {
        for (int i = 0; i < size; i++)
        {
            if (old_value == LEFT)
            {
                _display.drawSmoothArc(_centerX, _centerY, 97, 95, position[old_value], position[old_value] + i, TFT_BLACK, _backgroundColor, false);
                _display.drawSmoothArc(_centerX, _centerY, 97, 95, position[old_value] + i, position[old_value] + size + i, TFT_RED, _backgroundColor, false);
            }
            else
            {
                _display.drawSmoothArc(_centerX, _centerY, 97, 95, position[old_value] + size - i, position[old_value] + size, TFT_BLACK, _backgroundColor, false);
                _display.drawSmoothArc(_centerX, _centerY, 97, 95, position[old_value] - i, position[old_value] + size - i, TFT_RED, _backgroundColor, false);
            }
        }
    }
    else if (current_state_menu == LEFT)
    {
        for (int i = 0; i < size; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, 97, 95, position[old_value] + size - i, position[old_value] + size, TFT_BLACK, _backgroundColor, false);
            _display.drawSmoothArc(_centerX, _centerY, 97, 95, position[old_value] - i, position[old_value] + size - i, TFT_RED, _backgroundColor, false);
        }
    }
    else if (current_state_menu == RIGHT)
    {
        for (int i = 0; i < size; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, 97, 95, position[old_value], position[old_value] + i, TFT_BLACK, _backgroundColor, false);
            _display.drawSmoothArc(_centerX, _centerY, 97, 95, position[old_value] + i, position[old_value] + size + i, TFT_RED, _backgroundColor, false);
        }
    }
    old_value = current_state_menu;
}

void DisplayGC9A01::drawData(String str, int placement)
{
    clearData(placement);
    switch (placement)
    {
    case 0:
        _primaryDataSprite.drawString(str, _primaryDataSprite.width()/2, _primaryDataSprite.height()/2);
        _primaryDataSprite.pushRotated(270,TFT_BLACK);
        break;

    case 1:
        _secondaryDataSprite.drawString(str, _secondaryDataSprite.width()/2, _secondaryDataSprite.height()/2);
        _secondaryDataSprite.pushRotated(270, TFT_TRANSPARENT);
        break;

    case 2:
        _tertiaryDataSprite.drawString(str, _tertiaryDataSprite.width()/2, _tertiaryDataSprite.height()/2);
        _tertiaryDataSprite.pushRotated(270, TFT_BLACK);
        break;
    }
}

void DisplayGC9A01::drawUnit(MODE actualMode)
{
    drawData("--", 0);
    drawData("--", 1);
    drawData("--", 2);

    if(actualMode == MOUNTAIN){
        _primaryDataUnitSprite.drawString("KM/H", _primaryDataUnitSprite.width()/2, _primaryDataUnitSprite.height()/2);
        _primaryDataUnitSprite.pushRotated(270, TFT_BLACK);

        _secondaryDataUnitSprite.drawString("M", _secondaryDataUnitSprite.width()/2, _secondaryDataUnitSprite.height()/2);
        _secondaryDataUnitSprite.pushRotated(270, TFT_BLACK);

        _tertiaryDataUnitSprite.drawString("C",_tertiaryDataUnitSprite.width()/2, _tertiaryDataUnitSprite.height()/2);
        _tertiaryDataUnitSprite.pushRotated(270, TFT_BLACK);
    }
    else if(actualMode == URBAN) {
        _primaryDataUnitSprite.drawString("GPS", 0, 0);
        _primaryDataUnitSprite.pushRotated(270, TFT_BLACK);

        _secondaryDataUnitSprite.drawString("KM/H", 0, 0);
        _secondaryDataUnitSprite.pushRotated(270, TFT_BLACK);

        _tertiaryDataUnitSprite.drawString("C", 0, 0);
        _tertiaryDataUnitSprite.pushRotated(270, TFT_BLACK);
    } else {
        _primaryDataUnitSprite.drawString("VS", 0, 0);
        _primaryDataUnitSprite.pushRotated(270, TFT_BLACK);

        _secondaryDataUnitSprite.drawString("M", 0, 0);
        _secondaryDataUnitSprite.pushRotated(270, TFT_BLACK);

        _tertiaryDataUnitSprite.drawString("C", 0, 0);
        _tertiaryDataUnitSprite.pushRotated(270, TFT_BLACK);
    }
}

void DisplayGC9A01::clearData(int placement)
{
    switch(placement)
    {
        case 0: 
            _primaryDataSprite.fillSprite(TFT_BLACK);
            _primaryDataSprite.pushRotated(270, TFT_BLUE);
            break;

         case 1: 
            _secondaryDataSprite.fillSprite(TFT_BLACK);
            _secondaryDataSprite.pushRotated(270, TFT_BLUE);
            break;

        case 2:
            _tertiaryDataSprite.fillSprite(TFT_BLACK);
            _tertiaryDataSprite.pushRotated(270, TFT_BLUE); 
            break;

        case 3:
            //_sprite.createSprite(40, 50);
            //_sprite.setPivot(-90, 25); 
            break;
        case 4:
           // _sprite.createSprite(120, 50);
            //_sprite.setPivot(60, _sprite.height()/2);
            break;
    }
}


void DisplayGC9A01::pngDrawBLELogoStatus(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(BLE_LOGO_POSX, BLE_LOGO_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
void DisplayGC9A01::pngDrawLogo(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(LOGO_MOUNTED_POSX, LOGO_MOUNTED_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
//MENU ICONS
void DisplayGC9A01::pngDrawMountainIcon(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(MOUTAIN_ICON_POSX, MOUTAIN_ICON_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
void DisplayGC9A01::pngDrawUrbanIcon(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(URBAN_ICON_POSX, URBAN_ICON_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}
void DisplayGC9A01::pngDrawCustomIcon(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(CUSTOM_ICON_POSX, CUSTOM_ICON_POSY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}