#pragma once

#include <Arduino.h>
#include <stdbool.h>
#include "board_defines.h"

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define I2C_BME280_ADDR 0x76

class BME280_wrapper{
public:
    // Class constructor
    BME280_wrapper();
    // Initialize BME sensor
    bool init();
    // Sample sensor values
    bool sample_sensor_data();
    // Returns true if values were sampled from sensor
    bool get_is_sampled() const;

    // get sampled sensor values
    float get_sampled_temperature() const;
    float get_sampled_humidity() const;
    float get_sampled_pressure() const;
    
    bool get_is_initialized() const;
private:
    Adafruit_BME280 bme280; // I2C
    float sampled_temperature;
    float sampled_humidity;
    float sampled_pressure;
    bool is_sampled;
    bool is_initialized;
};