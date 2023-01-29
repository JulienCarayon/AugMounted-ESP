#include "Display_GC9A01.hpp"

DisplayGC9A01::DisplayGC9A01(uint8_t resolution, uint64_t backgroundColor)
{
    TFT_eSPI _display = TFT_eSPI();

    _backgroundColor = backgroundColor;
    _resolution = resolution;
    _radius = resolution / 2;
    _centerX = resolution / 2;
    _centerY = resolution / 2;
}

void DisplayGC9A01::init(void)
{
    _display.init();
    _display.setRotation(2);
    _display.setPivot(_radius, _radius);
    _display.setTextColor(TFT_WHITE, TFT_BLACK, true);
    splashScreen(true, "LOADING ...", "WELCOME !");
   //logo((uint8_t *) urban);    //not working
    /* _sprite.deleteSprite();
    _sprite.createSprite(240,240);
    _sprite.setPivot(120,120);
    plane_s.createSprite(121,190); 
    //plane_s.setPivot(100,100);
    plane_s.pushImage(00,00,20,20,urban);
    plane_s.pushToSprite(&_sprite, 60,7, 0);
    _sprite.pushSprite(40,40);
    delay(4000); */
}

void DisplayGC9A01::splashScreen(bool inOut, String str1, String str2)
{
    _display.fillScreen(_backgroundColor);

    _sprite.deleteSprite();
    _sprite.createSprite(120, 50);
    _sprite.loadFont(AA_FONT_SMALL);
    _sprite.setPivot(60, _sprite.height()/2);
    _sprite.setTextColor(TFT_WHITE);
    _sprite.drawString("AUGMOUNTED", 0, _sprite.height()/2 - 20);
    _sprite.setTextColor(TFT_DARKCYAN);
    _sprite.drawString(str1, 13, _sprite.height()/2 + 10);
    _sprite.pushRotated(270, TFT_BLACK);

    for (int i = 0; i < 360; i++)
    {   
        _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 1+i, TFT_GREENYELLOW, _backgroundColor, false);
        if(i<10)
            delay(5);
        else
            delay(1);
    }

    clearPrincipalData2(4);

    _sprite.createSprite(120, 50);
    _sprite.setPivot(60, _sprite.height()/2);
    _sprite.setTextDatum(BL_DATUM);
    _sprite.setTextColor(TFT_DARKCYAN);
    _sprite.drawString(str2, 15, _sprite.height()/2, 5);
    _sprite.pushRotated(270, TFT_BLACK);

    if(inOut) {
        for(int i = 0; i<4; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 360, TFT_BLACK, _backgroundColor, false);
            delay(100);
            _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 360, TFT_GREENYELLOW, _backgroundColor, false);
            delay(100); 
        }
    }
    else {
        _display.drawSmoothArc(_centerX, _centerY, _radius, _radius-5, 0, 360, TFT_GREENYELLOW, _backgroundColor, false);
        delay(1000);
    }
    _display.fillScreen(_backgroundColor);
}   

void DisplayGC9A01::developmentMode(void)
{
    _display.drawLine(120, 110, 120, 130, TFT_WHITE);
    _display.drawLine(110, 120, 130, 120, TFT_WHITE);
}

void DisplayGC9A01::deviceStatus(void)
{

}

void DisplayGC9A01::drawMenu(bool arcRoundedEnd, uint8_t thickness)
{
    Serial.println("DisplayGC9A01 drawMenu");

    //uint16_t fg_color = random(0x10000);
    uint16_t fg_color = TFT_TRANSPARENT;

    // _radius = arc outer radius, inner_radius = arc inner radius. Inclusive, so arc thickness = r-ir+1
    uint8_t inner_radius = _radius - (thickness); // Calculate inner radius (can be 0 for circle segment)

    // tft.drawSmoothArc(x, y, radius, inner_radius, start_angle, end_angle, fg_color, bg_color, arc_end);
    _display.drawSmoothArc(_centerX, _centerY, _radius, inner_radius, 0, 160, fg_color, _backgroundColor, arcRoundedEnd);
}

void DisplayGC9A01::drawDynamicMenu(bool inOut, bool arcRoundedEnd, uint8_t thickness, Move cursorSt)
{
    Serial.println("DisplayGC9A01 drawDynamicMenu");

    // uint16_t fg_color = random(0x10000);
    uint32_t fg_color = 0xf6b036;

    // _radius = arc outer radius, inner_radius = arc inner radius. Inclusive, so arc thickness = r-ir+1
    uint8_t inner_radius = ((_radius) - (thickness / 2)) - 3; // Calculate inner radius (can be 0 for circle segment)
    if (inOut)
    {
        // tft.drawSmoothArc(x, y, radius, inner_radius, start_angle, end_angle, fg_color, bg_color, arc _end);
        //  fade in
        for (int i = 0; i < 20; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, (inner_radius), inner_radius - i, 40, 140, fg_color, _backgroundColor, arcRoundedEnd);
        }
        
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
    }
    else
    {
        // fade out
        for (int i = 0; i <= 20; i++)
        {
            _display.drawSmoothArc(_centerX, _centerY, (inner_radius - 20) + i, inner_radius - 20, 40, 140, _backgroundColor, _backgroundColor, arcRoundedEnd);
        }
        cursorManagement(cursorSt, true);
    }
}

void DisplayGC9A01::batteryManagement(uint8_t batteryLevel, bool arcRoundedEnd, uint8_t thickness)
{
    // Serial.println("DisplayGC9A01 batteryManagement");
    uint8_t inner_radius = _radius - (thickness / 2);

    if (batteryLevel > 110)
        _arcBatteryColor = GREEN;
    else if (batteryLevel > 65 && batteryLevel <= 109)
        _arcBatteryColor = YELLOW;
    else if (batteryLevel > 40 && batteryLevel <= 65)
        _arcBatteryColor = ORANGE;
    else
        _arcBatteryColor = RED;

    // tft.drawSmoothArc(x, y, radius, inner_radius, start_angle, end_angle, fg_color, bg_color, arc_end);
    // 160 because batteryLevel is mapped from 0->4096 to 1->159
    _display.drawSmoothArc(_centerX, _centerY, _radius, inner_radius, 200 + (160 - batteryLevel), 360, _arcBatteryColor, _backgroundColor, arcRoundedEnd);
    _display.drawSmoothArc(_centerX, _centerY, _radius, inner_radius, 200, 200 + (160 - batteryLevel), TFT_BLACK, _backgroundColor, arcRoundedEnd);
}

void DisplayGC9A01::batteryManagement2(uint8_t batteryLevel, bool arcRoundedEnd, uint8_t thickness)
{
    uint8_t inner_radius = _radius - (thickness / 2);

    if (batteryLevel > 110)
        _arcBatteryColor = GREEN;
    else if (batteryLevel > 65 && batteryLevel <= 109)
        _arcBatteryColor = YELLOW;
    else if (batteryLevel > 40 && batteryLevel <= 65)
        _arcBatteryColor = ORANGE;
    else
        _arcBatteryColor = RED;

    _display.drawSmoothArc(_centerX, _centerY, _radius - 12, inner_radius - 12, 200 + (160 - batteryLevel), 360, _arcBatteryColor, _backgroundColor, arcRoundedEnd);
    _display.drawSmoothArc(_centerX, _centerY, _radius - 12, inner_radius - 12, 200, 200 + (160 - batteryLevel), TFT_BLACK, _backgroundColor, arcRoundedEnd);
}

void DisplayGC9A01::test()
{
    Serial.println("DisplayGC9A01 batteryManagement");
    uint8_t inner_radius = _radius - (40 / 2);

    // tft.drawSmoothArc(x, y, radius, inner_radius, start_angle, end_angle, fg_color, bg_color, arc_end);
    // 160 because batteryLevel is mapped from 0->4096 to 1->159
    for (int i = 0; i < 8; i++)
    {
        _display.setRotation(i);
        _display.drawSmoothArc(_centerX, _centerY, _radius, inner_radius, 0, 160, TFT_GREEN, _backgroundColor, false);
        _display.drawSmoothArc(_centerX, _centerY, _radius, inner_radius, 200, 360, TFT_BLUE, _backgroundColor, false);
        drawData(i);
        delay(3000);
        //_display.drawSmoothArc(_centerX, _centerY, _radius, inner_radius, 200, 360, _backgroundColor, _backgroundColor, false);
        _display.frameViewport(TFT_BLACK, 200);
        delay(300);
    }
}

void DisplayGC9A01::drawData(uint8_t speed)
{
    _display.drawNumber(speed, 110, 110, 6);
    //_display.drawString("CO3", 110, 110, 6);
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
    clearPrincipalData2(3);

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

void DisplayGC9A01::drawPrincipalData(String str, int placement)
{
    //  placement :
    //                _______
    //               |       |
    //               |   1   |
    //               |_______|
    //       _______          _______
    //      |       |        |       |
    //      |   2   |        |   3   |
    //      |_______|        |_______|

    _sprite.deleteSprite();
    //_sprite.setColorDepth(8);
    _sprite.createSprite(70, 70);
    _sprite.loadFont(AA_FONT_LARGE);
    //_sprite.fillSprite(TFT_BLUE);
    _sprite.fillSprite(TFT_BLACK);
    _sprite.setPivot(35, 80);
    _sprite.setTextColor(TFT_WHITE);
    _sprite.setTextDatum(MC_DATUM);
    _sprite.drawString(str, _sprite.width() / 2, 30);
    _sprite.pushRotated(270, TFT_BLACK);

    //_sprite.deleteSprite();
    _sprite.loadFont(AA_FONT_SMALL);
    _sprite.createSprite(70, 15);
    //_sprite.fillSprite(TFT_RED);
    //_sprite.loadFont(AA_FONT_SMALL);
    _sprite.fillSprite(TFT_BLACK);
    _sprite.setPivot(35, 25);
    //_sprite.setTextSize(2);
    _sprite.setTextColor(TFT_DARKGREY);
    _sprite.setTextDatum(MC_DATUM);
    _sprite.drawString("KM/H", _sprite.width() / 2, 10, 2);
    _sprite.pushRotated(270, TFT_BLACK);

    // 2
    _sprite.deleteSprite();
    //_sprite.setColorDepth(8);
    _sprite.createSprite(50, 50);
    //_sprite.fillSprite(TFT_RED);
    _sprite.fillSprite(TFT_BLACK);
    _sprite.setPivot(65, -10);
    // _sprite.setTextSize(2);
    _sprite.setTextColor(TFT_WHITE);
    _sprite.setTextDatum(MC_DATUM);
    _sprite.drawString("1450", _sprite.width() / 2, 25);
    _sprite.pushRotated(270, TFT_BLACK);

    _sprite.deleteSprite();
    _sprite.setColorDepth(8);
    _sprite.createSprite(50, 15);
    _sprite.fillSprite(TFT_BLACK);
    _sprite.setPivot(65, -50);
    _sprite.setTextColor(TFT_DARKGREY);
    //_sprite.setTextSize(1);
    _sprite.setTextDatum(CL_DATUM);
    _sprite.drawString("M", _sprite.width() / 2, _sprite.height() / 2);
    _sprite.pushRotated(270, TFT_BLACK);

    // 3
    _sprite.deleteSprite();
    _sprite.setColorDepth(8);
    _sprite.createSprite(50, 50);
    //_sprite.fillSprite(TFT_RED);
    _sprite.fillSprite(TFT_BLACK);
    _sprite.setPivot(-15, -10);
    //_sprite.setTextSize(2);
    _sprite.setTextColor(TFT_WHITE);
    _sprite.setTextDatum(MC_DATUM);
    _sprite.drawString("19", _sprite.width() / 2, 25, 4);
    _sprite.pushRotated(270, TFT_BLACK);

    _sprite.deleteSprite();
    _sprite.setColorDepth(8);
    _sprite.createSprite(50, 15);
    //_sprite.fillSprite(TFT_BLUE);
    _sprite.setPivot(-15, -50);
    //_sprite.setTextSize(1);
    _sprite.setTextColor(TFT_DARKGREY);
    _sprite.setTextDatum(MR_DATUM);
    _sprite.drawString("C", _sprite.width() / 2, _sprite.height() / 2);
    _sprite.pushRotated(270, TFT_BLACK);
}

void DisplayGC9A01::drawPrincipalData2(String str, int placement)
{
    //  placement :
    //                _______
    //               |       |
    //               |   0   |
    //               |_______|
    //       _______          _______
    //      |       |        |       |
    //      |   1   |        |   2   |
    //      |_______|        |_______|
    //
    // placement[0] : 0 -> moutain(SPEED)       / urban(GPS)         / custom(VERTICAL SPEED)
    // placement[1] : 1 -> moutain(ALTITUDE)    / urban(SPEED)       / custom(WIND)
    // placement[2] : 2 -> moutain(TEMPERATURE) / urban(TEMPERATURE) / custom(TEMPERATURE)
    Serial.println("data");
    clearPrincipalData2(placement);
    switch (placement)
    {
    case 0:
        _sprite.deleteSprite();
        _sprite.createSprite(70, 50);
        _sprite.loadFont(AA_FONT_LARGE);
        _sprite.fillSprite(TFT_BLACK);
        _sprite.setPivot(35, 80);
        _sprite.setTextColor(TFT_WHITE);
        _sprite.setTextDatum(MC_DATUM);
        _sprite.drawString(str, _sprite.width() / 2, 30);
        _sprite.pushRotated(270, TFT_BLUE);

        _sprite.loadFont(AA_FONT_SMALL);
        _sprite.createSprite(70, 15);
        _sprite.fillSprite(TFT_BLACK);
        _sprite.setPivot(35, 25);
        _sprite.setTextColor(TFT_DARKGREY);
        _sprite.setTextDatum(MC_DATUM);
        _sprite.drawString("KM/H", _sprite.width() / 2, 10, 2);
        _sprite.pushRotated(270, TFT_BLACK);
        break;

    case 1:
        _sprite.deleteSprite();
        _sprite.createSprite(50, 50);
        _sprite.fillSprite(TFT_BLACK);
        _sprite.setPivot(65, -10);
        // _sprite.setTextSize(2);
        _sprite.setTextColor(TFT_WHITE);
        _sprite.setTextDatum(MC_DATUM);
        _sprite.drawString(str, _sprite.width() / 2, 25);
        _sprite.pushRotated(270, TFT_BLACK);

        _sprite.deleteSprite();
        _sprite.setColorDepth(8);
        _sprite.createSprite(50, 15);
        _sprite.fillSprite(TFT_BLACK);
        _sprite.setPivot(65, -50);
        _sprite.setTextColor(TFT_DARKGREY);
        //_sprite.setTextSize(1);
        _sprite.setTextDatum(CL_DATUM);
        _sprite.drawString("M", _sprite.width() / 2, _sprite.height() / 2);
        _sprite.pushRotated(270, TFT_BLACK);
        break;

    case 2:
        _sprite.deleteSprite();
        _sprite.setColorDepth(8);
        _sprite.createSprite(50, 50);
        //_sprite.fillSprite(TFT_RED);
        _sprite.fillSprite(TFT_BLACK);
        _sprite.setPivot(-15, -10);
        //_sprite.setTextSize(2);
        _sprite.setTextColor(TFT_WHITE);
        _sprite.setTextDatum(MC_DATUM);
        _sprite.drawString(str, _sprite.width() / 2, 25, 4);
        _sprite.pushRotated(270, TFT_BLACK);

        _sprite.deleteSprite();
        _sprite.setColorDepth(8);
        _sprite.createSprite(50, 15);
        //_sprite.fillSprite(TFT_BLUE);
        _sprite.setPivot(-15, -50);
        _sprite.setTextColor(TFT_DARKGREY);
        _sprite.setTextDatum(MR_DATUM);
        _sprite.drawString("C", _sprite.width() / 2, _sprite.height() / 2);
        _sprite.pushRotated(270, TFT_BLACK);
        break;
    }
}

void DisplayGC9A01::clearPrincipalData2(int placement)
{
    _sprite.deleteSprite();
    _sprite.fillSprite(TFT_BLACK);

    switch(placement)
    {
        case 0: 
            _sprite.createSprite(70, 70);
            _sprite.setPivot(35, 80); 
            break;

         case 1: 
            _sprite.createSprite(50, 50);
            _sprite.setPivot(65, -10); 
            break;

        case 2:
            _sprite.createSprite(50, 50);
            _sprite.setPivot(-15, -10); 
            break;

        case 3:
            _sprite.createSprite(40, 50);
            _sprite.setPivot(-90, 25); 
            break;
        case 4:
            _sprite.createSprite(120, 50);
            _sprite.setPivot(60, _sprite.height()/2);
            break;
    }
    _sprite.pushRotated(270, TFT_BLUE); //why a fucking blue ?
}


void DisplayGC9A01::pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(_xpos, _ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void DisplayGC9A01::pngDraw3(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WDITH];
  _png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0x00000000);
  _display.pushImage(100, 89 + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void DisplayGC9A01::logo(uint8_t* logo)
{
    int16_t rc = _png.openFLASH(logo, sizeof(logo), pngDraw);
    Serial.println("logo function");
    Serial.println(rc);
    if (rc == PNG_SUCCESS) {
        Serial.println("success");
        _display.startWrite();
        rc = _png.decode(NULL, 0);
        _display.endWrite();
        // png.close(); // not needed for memory->memory decode
    }
} 
