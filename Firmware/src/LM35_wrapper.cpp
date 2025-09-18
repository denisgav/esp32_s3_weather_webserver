#include "LM35_wrapper.h"

// Class constructor
LM35_wrapper ::LM35_wrapper() : sampled_temperature(nanf("")),
                                is_sampled(false),
                                is_initialized(false)

{
}

// Initialize BME sensor
bool LM35_wrapper ::init()
{
    is_initialized = true;
    return is_initialized;
}

// Sample sensor values
bool LM35_wrapper ::sample_sensor_data()
{
    if (is_initialized)
    {
        double value = analogRead(LM35_PIN);
        // VOUT (Volts) = (AnalogReading / 4096.0) * VREF
        double vout = (double(value) * ((3.3 / 4096.0) / 0.01));
        double celcius = vout;
        sampled_temperature = celcius;
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
bool LM35_wrapper ::get_is_sampled() const
{
    return is_sampled;
}

// get sampled sensor values
float LM35_wrapper ::get_sampled_temperature() const
{
    return sampled_temperature;
}

bool LM35_wrapper ::get_is_initialized() const
{
    return is_initialized;
}