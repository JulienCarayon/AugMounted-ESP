#ifndef BLE_Hud_Service_H
#define BLE_Hud_Service_H

#include <Arduino.h>
#include <../lib/BLE/src/BLEDevice.h>
#include <../lib/BLE/src/BLEUtils.h>
#include <../lib/BLE/src/BLEServer.h>
#include <../lib/BLE/src/BLE2902.h>
#include "Components/Display_GC9A01/Display_GC9A01.hpp"
#include "Components/Glb/Glb.hpp"

using namespace std;
static bool deviceConnected;

class BLE_Hud_Service
{
public:
    BLE_Hud_Service();
    //BLE_Hud_Service(uint8_t reso, uint64_t col);
    void initserviceUUID(void);
    void createService(void);
    std::string* hudconnected(uint8_t battery_level);
    Data hudconnected2(uint8_t battery_level);
    bool deviceDisconnected;
    String new_data[6];
    std::string new_data2[6];
    Data new_data3;

private:
    BLEServer *pServer;
    BLEServer *pServiceUserData;
    BLEServer *pServiceDataService;
    // Service User data
    BLECharacteristic *pCharacteristic;
    BLECharacteristic *pCharacteristicSpeed;
    BLECharacteristic *pCharacteristicAltitude;
    BLECharacteristic *pCharacteristicTime;
    BLECharacteristic *pCharacteristicGps;
    BLECharacteristic *pCharacteristicPhoneBattery;
    BLECharacteristic *pCharacteristicWeatherTemp;
    // Service Data service
    BLECharacteristic *pCharacteristicSw;
    BLECharacteristic *pCharacteristicHw;
    BLECharacteristic *pCharacteristicBattery;

    BLEAdvertising *pAdvertising;

    std::string _get_speed;
    std::string _get_altitude;
    std::string _get_time;
    std::string _get_gps;
    std::string _get_phone_battery;
    std::string _get_local_temperature;

    String _current_speed;
    String _current_altitude;
    String _local_temperature;
    String _current_time;
    String _phone_battery;

    char _batteryLevel[8];
    char _old_batteryLevel[8] = "100";

    int _bL;
    int _old_bL;
    int testbat;
};

#endif