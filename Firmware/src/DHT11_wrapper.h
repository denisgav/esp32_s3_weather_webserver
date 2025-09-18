#pragma once

#include <Arduino.h>
#include <stdbool.h>

#include <DHT.h>
#include <DHT_U.h>

#include "board_defines.h"

#define DHTTYPE DHT11 // DHT 11

class DHT11_wrapper
{
public:
    // Class constructor
    DHT11_wrapper();
    // Initialize BME sensor
    bool init();
    // Sample date/time
    bool sample_sensor_data();
    // Returns true if values were sampled from sensor
    bool get_is_sampled() const;

    // get sampled datetime values
    float get_sampled_temperature() const;
    float get_sampled_humidity() const;

    bool get_is_initialized() const;
    void print_sensor_info();

private:
    DHT_Unified dht;
    float sampled_temperature;
    float sampled_humidity;
    bool is_sampled;
    bool is_initialized;
};