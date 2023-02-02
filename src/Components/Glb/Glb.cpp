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
  pinMode(ROTARY_ENCODER_BUTTON_PIN, INPUT_PULLUP);
  #ifdef COMMIT
  pinMode(SUICIDE_PIN, OUTPUT);
  digitalWrite(SUICIDE_PIN, HIGH);
  #endif

  _rotaryEncoder = AiEsp32RotaryEncoder(_encoderAPin, _encoderBPin, _encoderButtonPin, _encoderVccPin, _encoderSteps);
  _rotaryEncoder.begin();
  _rotaryEncoder.setup(readEncoderISR);

  init();
  drawMenu(false, MENU_THICKNESS);
  drawMenuTitle(MENU_TITLE_LEFT, MENU_TITLE_CENTER, MENU_TITLE_RIGHT);
}

void Glb::rotary_loop(Move cursor, bool encoderSwitch, bool menuDynamic)
{
  if(!menuDynamic) {
    if (cursor != _old_cursoSt)
    {
      Serial.println("rotary_loop2");
      Serial.print("cursor : ");
      Serial.println(cursor);
      Serial.print("_old_cursoSt : ");
      Serial.println(_old_cursoSt);

      cursorManagement(cursor, false, false);
      _old_cursoSt = cursor;

      Serial.print("_old_cursoStA : ");
      Serial.println(_old_cursoSt);
    }
  }
  else
  {
    if (cursor != _old_cursoStDynMenu)
    {
      cursorManagement(cursor, false, true);
      _old_cursoStDynMenu = cursor;
    }
  }
}

uint8_t Glb::get_battery_level(bool system, uint16_t phone_battery)
{
  //if system == true -> map voltage divider
  //else -> map phone battery

  if(system){
    _adc_read = analogRead(BATTERY_PIN);
    _div_voltage = (uint16_t)map(_adc_read, 0, 4095, 0, 3300);
    _bat_voltage = _div_voltage * 2;
    _soc = (uint16_t)map(_adc_read, 0, 4200, 1, 159);

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
  if ((_selected_mode && _dynamic_menu_set)||((!_state) && (_state != _temp) && _cursoSt == MIDDLE))
  {
    Serial.println("ok");
    _temp = digitalRead(ROTARY_ENCODER_BUTTON_PIN);
    if (_dynamic_menu_set && _selected_mode)
    {
      drawDynamicMenu(false, false, 40, MIDDLE);
      _dynamic_menu_set = false;
    }
    else
    {
      drawDynamicMenu(true, false, 40, MIDDLE);
      _dynamic_menu_set = true;
      _selected_mode = false;
    }
  }
  _temp = _state;

  //_cursoSt = _rotaryEncoder.encoderChangedState();

  //check if button is clicked
  if (_rotaryEncoder.isEncoderButtonClicked()) {
    _encoderSwitch = true;
  }
  else {
    _encoderSwitch = false;
  } 

  if(!_dynamic_menu_set)
  {
    _cursoSt = _rotaryEncoder.encoderChangedState();

    if (_encoderSwitch && _cursoSt == LEFT) {
      Serial.println("-> Turn OFF system");
      splashScreen(SPLASH_SCREEN_BLINKING, "AUGMOUNTED", "GOOD BYE !");
      #ifdef COMMIT
      digitalWrite(SUICIDE_PIN, LOW);
      #endif
    }
    if (_encoderSwitch && _cursoSt == RIGHT) {
      Serial.println("-> CONFIG");
    }
    //rotaryloop algo
    rotary_loop(_cursoSt, _encoderSwitch, _dynamic_menu_set);
  }
  else
  {
    _cursoSt = _rotaryEncoder.encoderChangedState();

    if(_cursoSt == MIDDLE && _encoderSwitch)
    {
      //ACTIVE URBAN MODE
      Serial.println("URBAN");
      _current_mode = URBAN;
      _selected_mode = true;
    }
    else if (_cursoSt == LEFT && _encoderSwitch)
    {
      //ACTIVE MOUNTAIN MODE
      Serial.println("MOUNTAIN");
      _current_mode = MOUNTAIN;
      _selected_mode = true;
    }
    else if (_cursoSt == RIGHT && _encoderSwitch)
    {
      //ACTIVE CUSTOM MODE
      Serial.println("CUSTOM");
      _current_mode = CUSTOM;
      _selected_mode = true;
    } 
    else 
    {
      _selected_mode = false;
    }
    //rotaryloop algo
    //rotary_loop(_cursoStDynMenu, _encoderSwitch, _dynamic_menu_set);
  }
  //rotary_loop(_cursoSt, _encoderSwitch, _dynamic_menu_set);
  rotary_loop(_cursoSt, _encoderSwitch, _dynamic_menu_set);
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
    drawData(convertDataToString(dt._get_speed), 0);
    _old_data._get_speed = dt._get_speed;
  }
  if(dt._get_altitude != _old_data._get_altitude) {
    drawData(convertDataToString(dt._get_altitude), 1);
    _old_data._get_altitude = dt._get_altitude;
  }
  if(dt._get_local_temperature != _old_data._get_local_temperature) {
    drawData(convertDataToString(dt._get_local_temperature), 2);
    _old_data._get_local_temperature = dt._get_local_temperature;
  }
  if(dt._get_time != _old_data._get_time) {
    drawTime(convertDataToString(dt._get_time));
    _old_data._get_time = dt._get_time;
  }
  if(dt._get_phone_battery != _old_data._get_phone_battery) {
    get_battery_level(PHONE_BATTERY, convertDataToInt(dt._get_phone_battery));
    _old_data._get_phone_battery = dt._get_phone_battery;
  }
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
