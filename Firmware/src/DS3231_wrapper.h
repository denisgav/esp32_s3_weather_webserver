#pragma once

#include <Arduino.h>
#include <stdbool.h>
#include <RTClib.h>

class DS3231_wrapper{
public:
    // Class constructor
    DS3231_wrapper();
    // Initialize BME sensor
    bool init();
    // Sample date/time
    bool sample_datetime_data();
    // Returns true if values were sampled from sensor
    bool get_is_sampled() const;

    void adjust(const DateTime &dt);

    // get sampled datetime values
    DateTime get_sampled_datetime() const;
    float get_sampled_temperature() const;

    bool get_is_initialized() const;
private:
    RTC_DS3231 rtc; // I2C
    DateTime now;
    float sampled_temperature;
    bool is_sampled;
    bool is_initialized;
};