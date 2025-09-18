#include "AHT2x_wrapper.h"

// Class constructor
AHT2x_wrapper ::AHT2x_wrapper() : aht(),
                                  sampled_temperature(nanf("")),
                                  sampled_humidity(nanf("")),
                                  is_sampled(false),
                                  is_initialized(false)

{
}
// Initialize BME sensor
bool AHT2x_wrapper ::init()
{
    is_initialized = aht.begin();
    return is_initialized;
}
// Sample date/time
bool AHT2x_wrapper ::sample_sensor_data()
{
    if (is_initialized)
    {
        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
        sampled_temperature = temp.temperature;
        sampled_humidity = humidity.relative_humidity;
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
bool AHT2x_wrapper ::get_is_sampled() const
{
    return is_sampled;
}

// get sampled datetime values
float AHT2x_wrapper ::get_sampled_temperature() const
{
    return sampled_temperature;
}
float AHT2x_wrapper ::get_sampled_humidity() const
{
    return sampled_humidity;
}

bool AHT2x_wrapper ::get_is_initialized() const
{
    return is_initialized;
}
