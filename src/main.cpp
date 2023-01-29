#include "SPIFFS.h"
#include "BLE_Hud_Service.hpp"
#include "Components/Glb/Glb.hpp"

Glb global = Glb(DISPLAY_RESOLUTION, DISPLAY_BACKGROUND_COLOR, ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
BLE_Hud_Service ble_com_hud = BLE_Hud_Service();

void setup()
{
  Serial.begin(115200);
  ble_com_hud.initserviceUUID();
  ble_com_hud.createService();

  pinMode(ROTARY_ENCODER_BUTTON_PIN, INPUT_PULLUP);

  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS initialisation failed!");
    while (1)
      yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nSPIFFS available!");

  // ESP32 will crash if any of the fonts are missing, so check
  bool font_missing = false;
  if (SPIFFS.exists("/NotoSansBold15.vlw") == false)
    font_missing = true;
  if (SPIFFS.exists("/NotoSansBold36.vlw") == false)
    font_missing = true;

  if (font_missing)
  {
    Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
    while (1)
      yield();
  }
  else
    Serial.println("\r\nFonts found OK.");

  global.begin();
}

void loop()
{
  global.updateData(ble_com_hud.hudconnected2(global.get_battery_level(true)));
  global.dynamic_menu_management();
}