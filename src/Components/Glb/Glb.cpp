#include "Glb.hpp"

Glb::Glb(uint8_t resolution, uint64_t backgroundColor, uint8_t encoderAPin, uint8_t encoderBPin, uint8_t encoderButtonPin, uint8_t encoderVccPin, uint8_t encoderSteps) : DisplayGC9A01(resolution, backgroundColor)
{
  _encoderAPin = encoderAPin;
  _encoderBPin = encoderBPin;
  _encoderButtonPin = encoderButtonPin;
  _encoderVccPin = encoderVccPin;
  _encoderSteps = encoderSteps;
}

void Glb::begin(void)
{
  _rotaryEncoder = AiEsp32RotaryEncoder(_encoderAPin, _encoderBPin, _encoderButtonPin, _encoderVccPin, _encoderSteps);
  _rotaryEncoder.begin();
  _rotaryEncoder.setup(readEncoderISR);

  init();
  drawMenu(false, MENU_THICKNESS);
  drawMenuTitle("OFF", "MENU", "CONF");
  //drawMenuTitle("OFF", "MENU", "CONF");
}
const unsigned char data [6] = {urban[1442] };

void Glb::rotary_loop(Move cursor, bool sw)
{
  _cursoSt = cursor;
  _sw = sw;
  if (_cursoSt != _old_cursoSt)
  {
    if (_cursoSt == RIGHT)
      Serial.println("                  RIGHT");
    else if (_cursoSt == LEFT)
      Serial.println("LEFT");
    else if (_cursoSt == MIDDLE)
      Serial.println("        MIDDLE");

    cursorManagement(_cursoSt, false);
    _old_cursoSt = _cursoSt;
  }
  if (_sw && _cursoSt == LEFT) {
    Serial.println("-> Turn OFF system");
    //splashScreen(false, "AUGMOUNTED", "GOOD BYE !");
  }
  if (_sw && _cursoSt == RIGHT) {
    Serial.println("-> CONFIG");
  }
}

uint8_t Glb::get_battery_level(bool system, uint16_t phone_battery)
{
  //if system == true -> map voltage divider
  //else -> map phone battery

  if(system){
    _adc_read = analogRead(BATTERY_PIN);
    //uint16_t div_voltage = (uint16_t)map(adc_read, 0, 4095, 0, 3300);
    //uint16_t bat_voltage = (div_voltage * 1500) / 500;
    //static uint16_t old_soc = 0;
    //uint8_t soc = (uint8_t)map(bat_voltage, 0, 8400, 0, 100);

    _soc = (uint16_t)map(_adc_read, 0, 4095, 1, 159);
    // for voltage divider 
    if (_soc < (_old_soc - 3) || _soc > (_old_soc + 3))
    {
      deviceBatteryManagement(_soc, ARC_ROUNDED_END, BATTERY_THICKNESS);
      _old_soc = _soc;
    }
  } else {
    _phone_battery = (uint8_t)map(phone_battery, 0, 100, 1, 159);

    if (_phone_battery < (_old_phone_battery ) || _phone_battery > (_old_phone_battery))
    {
      Serial.println("test");
      phoneBatteryManagement(_phone_battery, ARC_ROUNDED_END, BATTERY_THICKNESS);
      _old_phone_battery = _phone_battery;
    }
  }  
  return _soc;
}

void Glb::dynamic_menu_management(void)
{
  _state = digitalRead(ROTARY_ENCODER_BUTTON_PIN);
  if ((!_state) && (_state != _temp) && _cursoSt == MIDDLE)
  {
    _temp = digitalRead(ROTARY_ENCODER_BUTTON_PIN);
    if (_dynamic_menu_set)
    {
      drawDynamicMenu(false, false, 40, _cursoSt);
      _dynamic_menu_set = false;
    }
    else
    {
      drawDynamicMenu(true, false, 40, _cursoSt);
      _dynamic_menu_set = true;
    }
  }
  _temp = _state;

  if (!_dynamic_menu_set)
  {
    _cursoSt = _rotaryEncoder.encoderChangedState();
  }
  else {  //dynamic menu

  }

  if (_cursoSt != MIDDLE && _rotaryEncoder.isEncoderButtonClicked())
  {
    _sw = true;
  }
  else
  {
    _sw = false;
  }

  rotary_loop(_cursoSt, _sw);
}
void IRAM_ATTR Glb::readEncoderISR()
{
  _rotaryEncoder.readEncoder_ISR();
}

void Glb::updateData(Data dt)
{
  //  placement :
  //                _______
  //               |       |           ___
  //               |   0   |          |   |
  //               |_______|          | 3 |
  //       _______          _______   |___|
  //      |       |        |       |
  //      |   1   |        |   2   |
  //      |_______|        |_______|
  //
  // placement[0] : 0 -> moutain(SPEED)       / urban(GPS)         / custom(VERTICAL SPEED)
  // placement[1] : 1 -> moutain(ALTITUDE)    / urban(SPEED)       / custom(WIND)
  // placement[2] : 2 -> moutain(TEMPERATURE) / urban(TEMPERATURE) / custom(TEMPERATURE)
  // placement[3] : 3 -> for_each_mode(CURRENT_TIME)

  //moutain mode
  if(dt._device_connected != _old_data._device_connected) {
    if(dt._device_connected == "yes") {
      deviceStatus(deviceConnected);
      deviceConnected = true;
    }
    else {
      deviceStatus(false);
      deviceConnected = false;
    }
  } 
  if(dt._get_speed != _old_data._get_speed) {
    //arrayData[0] = convertDataToString(dt._get_speed);
    drawData(convertDataToString(dt._get_speed), 0);
    _old_data._get_speed = dt._get_speed;
  }
  if(dt._get_altitude != _old_data._get_altitude) {
    arrayData[1] = convertDataToString(dt._get_altitude);
    drawData(convertDataToString(dt._get_altitude), 1);
    _old_data._get_altitude = dt._get_altitude;
  }
  if(dt._get_local_temperature != _old_data._get_local_temperature) {
    //arrayData[2] = convertDataToString(dt._get_local_temperature);
    drawData(convertDataToString(dt._get_local_temperature), 2);
    _old_data._get_local_temperature = dt._get_local_temperature;
  }
  if(dt._get_time != _old_data._get_time) {
    //arrayData[3] = convertDataToString(dt._get_time);
    drawTime(convertDataToString(dt._get_time));
    _old_data._get_time = dt._get_time;
  }
  if(dt._get_phone_battery != _old_data._get_phone_battery) {
    get_battery_level(false, convertDataToInt(dt._get_phone_battery));
    _old_data._get_phone_battery = dt._get_phone_battery;
  }

/*   
  Serial.println("----------------------------------------------------------------------");
  Serial.print("data 1 : ");
  Serial.println( convertDataToString(dt._get_speed));
  Serial.print("data 2 : ");
  Serial.println( convertDataToString(dt._device_connected) );
  Serial.print("data 3 : ");
  Serial.println( convertDataToString(dt._get_altitude) );
  Serial.print("data 4 : ");
  Serial.println("GPS");
  //Serial.println( convertDataToString(dt._get_gps));
  Serial.print("data 5 : ");
  Serial.println( convertDataToString(dt._get_local_temperature));
  Serial.print("data 6 : ");
  Serial.println(convertDataToString(dt._get_phone_battery)); 
*/
}

String Glb::convertDataToString(std::string data) {
  String convertedDataStr;
  for (int i = 0; i < data.length(); i++) {
    convertedDataStr += data[i];
  }
  return convertedDataStr;
}

uint8_t Glb::convertDataToInt(std::string data) { 
  return stoi(data);
}

