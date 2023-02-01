#ifndef GLB_HPP
    #define GLB_HPP

    #include <Arduino.h>
    #include <TFT_eSPI.h>
    #include "Components/Display_GC9A01/Display_GC9A01.hpp"
    #include "AiEsp32RotaryEncoder.h"
    #include "Components/Setup.hpp"
    
    using namespace std;

    static AiEsp32RotaryEncoder _rotaryEncoder;

    class Glb: public DisplayGC9A01
    {
    public:
        Glb(uint8_t resolution, uint64_t backgroundColor, uint8_t encoderAPin, uint8_t encoderBPin, uint8_t encoderButtonPin, uint8_t encoderVccPin, uint8_t encoderSteps);
        void rotary_loop(Move cursor, bool sw);
        uint8_t get_battery_level(bool system, uint16_t phone_battery = 0);
        //uint8_t get_battery_level(uint16_t phone_battery = 0);
        void dynamic_menu_management(void);
        void begin(void);
        void updateData(Data dt);
        //void updateData(String* new_data);
        String convertDataToString(std::string data);
        uint8_t convertDataToInt(std::string data);

    private:
        //String _old_data[6];
        Data _old_data;
        MODE _mode = URBAN;

        uint16_t _adc_read = 0;
        uint8_t _soc = 0;
        uint8_t _old_soc = 0;
        uint8_t _phone_battery = 0;
        uint8_t _old_phone_battery = 0;

        String arrayData[6];


        bool _dynamic_menu_set = false;
        bool _temp = HIGH;
        int _old_state = 0;
        int _state = 0;
        Move _cursoSt = MIDDLE;
        Move _old_cursoSt = MIDDLE;
        bool _sw = false;
        int _data = 0;

        uint8_t _encoderAPin= 0;
        uint8_t _encoderBPin = 0;
        uint8_t _encoderButtonPin= 0;
        uint8_t _encoderVccPin= 0;
        uint8_t _encoderSteps = 0;

        bool _initdone = false;
        
        static void IRAM_ATTR readEncoderISR();

        /* 
        uint8_t pinInt;                     // The interrupt pin used
        static volatile uint16_t count; // Encoder interrupt counter
 */
    };
#endif