#include "DS3231_wrapper.h"

// Class constructor
DS3231_wrapper ::DS3231_wrapper() : rtc(),
                                    now(0x0, 0x0, 0x0, 0x0, 0x0, 0x0),
                                    sampled_temperature(nanf("")),
                                    is_sampled(false),
                                    is_initialized(false)
{
}
// Initialize BME sensor
bool DS3231_wrapper ::init()
{
    is_initialized = rtc.begin();
    if (is_initialized && rtc.lostPower())
    {
        Serial.println("RTC lost power, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
    return is_initialized;
}
// Sample date/time
bool DS3231_wrapper ::sample_datetime_data()
{
    if (is_initialized)
    {
        sampled_temperature = rtc.getTemperature();
        now = rtc.now();
        is_sampled = true;
        return true;
    }
    else
    {
        is_sampled = false;
        return false;
    }
}

// Returns true if values were sampled from sensor
bool DS3231_wrapper ::get_is_sampled() const
{
    return is_sampled;
}

// get sampled datetime values
DateTime DS3231_wrapper ::get_sampled_datetime() const
{
    return now;
}

float DS3231_wrapper ::get_sampled_temperature() const
{
    return sampled_temperature;
}

bool DS3231_wrapper ::get_is_initialized() const
{
    return is_initialized;
}