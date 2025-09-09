#pragma once

#include <Arduino.h>

#include "board_defines.h"

//-------------------------------------------------
// DS3231
//-------------------------------------------------
#include "DS3231_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// BME280
//-------------------------------------------------
#include "BME280_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// ENS160
//-------------------------------------------------
#include "ENS160_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// AHT2x
//-------------------------------------------------
#include "AHT2x_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// DHT11
//-------------------------------------------------
#include "DHT11_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// LM35
//-------------------------------------------------
#include "LM35_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// LDR
//-------------------------------------------------
#include "LDR_wrapper.h"
//-------------------------------------------------


class multisensor_wrapper{
public:
    // Class constructor
    multisensor_wrapper();
    // Initialize BME sensor
    bool init();
    // Sample date/time
    bool sample_sensor_data();

    // RTC3231
    DS3231_wrapper& get_rtc();
    // BME280
    BME280_wrapper& get_bme280();
    // ENS160
    ENS160_wrapper& get_ens160();
    // AHT2x
    AHT2x_wrapper& get_aht2x();
    // DHT11
    DHT11_wrapper& get_dht11();
    // LM35
    LM35_wrapper& get_lm35();
    // LDR
    LDR_wrapper& get_ldr();

    bool get_is_initialized() const;
private:
    // RTC3231
    DS3231_wrapper rtc;

    // BME280
    BME280_wrapper bme280;

    // ENS160
    ENS160_wrapper ens160;

    // AHT2x
    AHT2x_wrapper aht2x;

    // DHT11
    DHT11_wrapper dht11;

    // LM35
    LM35_wrapper lm35;

    // LDR
    LDR_wrapper ldr;
    
    bool is_initialized;
};