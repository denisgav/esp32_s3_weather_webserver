#include "DS18B20_wrapper.h"

// Class constructor
DS18B20_wrapper ::DS18B20_wrapper() : oneWire(DS18B20_PIN),
                                      sensors(&oneWire),
                                      sampled_temperature(nanf("")),
                                      is_sampled(false),
                                      is_initialized(false)
{
}

// Initialize BME sensor
bool DS18B20_wrapper ::init()
{
    // Start up the library
    sensors.begin();
    is_initialized = true;
    return true;
}

// Sample date/time
bool DS18B20_wrapper ::sample_sensor_data()
{
    if (is_initialized)
    {
        sampled_temperature = sensors.getTempCByIndex(0);
        sensors.requestTemperatures();
        if (sampled_temperature != DEVICE_DISCONNECTED_C)
        {
            is_sampled = true;
            return true;
        }
        else
        {
            is_sampled = false;
            return false;
        }
    }
    else
    {
        is_sampled = false;
        return false;
    }
}

// Returns true if values were sampled from sensor
bool DS18B20_wrapper ::get_is_sampled() const
{
    return is_sampled;
}

// get sampled datetime values
float DS18B20_wrapper ::get_sampled_temperature() const
{
    return sampled_temperature;
}

bool DS18B20_wrapper ::get_is_initialized() const
{
    return is_initialized;
}

void DS18B20_wrapper ::print_sensor_info()
{
    
}