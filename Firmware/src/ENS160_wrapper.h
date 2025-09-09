#pragma once

#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include "board_defines.h"

#include <ScioSense_ENS160.h>  // ENS160 library

class ENS160_wrapper{
public:
    // Class constructor
    ENS160_wrapper();
    // Initialize BME sensor
    bool init();
    // Sample sensor values
    bool sample_sensor_data();

    // get sampled sensor values
    uint8_t get_sampled_AQI() const;
    uint16_t get_sampled_TVOC() const;
    uint16_t get_sampled_eCO2() const;
    
    bool get_is_initialized() const;
private:
    ScioSense_ENS160 ens160; // I2C
    uint8_t sampled_AQI;
    uint16_t sampled_TVOC;
    uint16_t sampled_eCO2;
    bool is_initialized;
};