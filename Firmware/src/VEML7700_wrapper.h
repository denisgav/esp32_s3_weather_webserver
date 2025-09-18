#pragma once

#include <Arduino.h>
#include <stdbool.h>

#include <Adafruit_VEML7700.h>

#include "board_defines.h"


class VEML7700_wrapper{
public:
    // Class constructor
    VEML7700_wrapper();
    // Initialize BME sensor
    bool init();
    // Sample date/time
    bool sample_sensor_data();

    // get sampled datetime values
    float get_sampled_lux() const;
    uint8_t get_sampled_gain() const;
    String get_sampled_gain_s() const;
    uint8_t get_sampled_integration_time() const;
    String get_sampled_integration_time_s() const;

    bool get_is_initialized() const;
private:
    Adafruit_VEML7700 veml;
    float sampled_lux;
    uint8_t sampled_gain;
    uint8_t sampled_integration_time;
    bool is_initialized;
};