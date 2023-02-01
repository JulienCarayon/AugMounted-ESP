#include "SPIFFS.h"
#include "BLE_Hud_Service.hpp"
#include "Components/Glb/Glb.hpp"

Glb global = Glb(DISPLAY_RESOLUTION, DISPLAY_BACKGROUND_COLOR, ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);
BLE_Hud_Service ble_com_hud = BLE_Hud_Service();
Data dt;
uint8_t bt;

void setup()
{
  Serial.begin(115200);
  ble_com_hud.initserviceUUID();
  ble_com_hud.createService();

  pinMode(ROTARY_ENCODER_BUTTON_PIN, INPUT_PULLUP);

  global.begin();
}

void loop()
{
  bt = global.get_battery_level(true);
  dt = ble_com_hud.hudconnected2(bt);
  global.dynamic_menu_management();
  global.updateData(dt);
}