#include "LDR_wrapper.h"

// Class constructor
LDR_wrapper ::LDR_wrapper() : sampled_ldr(nanf("")),
                              is_initialized(false)

{
}

// Initialize BME sensor
bool LDR_wrapper ::init()
{
    is_initialized = true;
    return is_initialized;
}

// Sample sensor values
bool LDR_wrapper ::sample_sensor_data()
{
    if (is_initialized)
    {
        double value = analogRead(LDR_PIN);
        double vout = (double(value) * ((3.3 / 4096.0) / 0.01));
        sampled_ldr = vout;
        return true;
    }
    else
        return false;
}

// get sampled sensor values
float LDR_wrapper ::get_sampled_ldr() const
{
    return sampled_ldr;
}

bool LDR_wrapper ::get_is_initialized() const
{
    return is_initialized;
}