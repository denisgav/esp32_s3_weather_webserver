#pragma once

#include <Arduino.h>
#include <stdbool.h>

#include <Adafruit_AHTX0.h>

class AHT2x_wrapper
{
public:
    // Class constructor
    AHT2x_wrapper();
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

private:
    Adafruit_AHTX0 aht;
    float sampled_temperature;
    float sampled_humidity;
    bool is_sampled;
    bool is_initialized;
};