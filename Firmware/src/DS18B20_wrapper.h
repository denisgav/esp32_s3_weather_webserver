#pragma once

#include <Arduino.h>
#include <stdbool.h>

// Include the libraries we need
#include <OneWire.h>
#include <DallasTemperature.h>

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// https://github.com/milesburton/Arduino-Temperature-Control-Library

#include "board_defines.h"

class DS18B20_wrapper
{
public:
    // Class constructor
    DS18B20_wrapper();
    // Initialize BME sensor
    bool init();
    // Sample date/time
    bool sample_sensor_data();
    // Returns true if values were sampled from sensor
    bool get_is_sampled() const;

    // get sampled datetime values
    float get_sampled_temperature() const;

    bool get_is_initialized() const;
    void print_sensor_info();

private:
    OneWire oneWire;
    DallasTemperature sensors;
    float sampled_temperature;
    bool is_sampled;
    bool is_initialized;
};