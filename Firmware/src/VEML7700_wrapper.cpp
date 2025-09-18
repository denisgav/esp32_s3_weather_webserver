#include "VEML7700_wrapper.h"

// Class constructor
VEML7700_wrapper ::VEML7700_wrapper() : veml(),
                                        sampled_lux(nanf("")),
                                        sampled_gain(0),
                                        sampled_integration_time(0),
                                        is_sampled(false),
                                        is_initialized(false)
{
}
// Initialize BME sensor
bool VEML7700_wrapper ::init()
{
    is_initialized = veml.begin();
    return is_initialized;
}
// Sample date/time
bool VEML7700_wrapper ::sample_sensor_data()
{
    if (is_initialized)
    {
        sampled_lux = veml.readLux(VEML_LUX_AUTO);
        sampled_gain = veml.getGain();
        sampled_integration_time = veml.getIntegrationTime();
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
bool VEML7700_wrapper ::get_is_sampled() const
{
    return is_sampled;
}

// get sampled datetime values
float VEML7700_wrapper ::get_sampled_lux() const
{
    return sampled_lux;
}

uint8_t VEML7700_wrapper ::get_sampled_gain() const
{
    return sampled_gain;
}

String VEML7700_wrapper ::get_sampled_gain_s() const
{
    switch (sampled_gain)
    {
    case VEML7700_GAIN_1:
        return String("1");
    case VEML7700_GAIN_2:
        return String("2");
    case VEML7700_GAIN_1_4:
        return String("1/4");
    case VEML7700_GAIN_1_8:
        return String("1/8");
    default:
        return String("Unknown(") + String(sampled_gain) + ")";
    }
}

uint8_t VEML7700_wrapper ::get_sampled_integration_time() const
{
    return sampled_integration_time;
}

String VEML7700_wrapper ::get_sampled_integration_time_s() const
{
    switch (sampled_integration_time)
    {
    case VEML7700_IT_25MS:
        return String("25");
        break;
    case VEML7700_IT_50MS:
        return String("50");
        break;
    case VEML7700_IT_100MS:
        return String("100");
        break;
    case VEML7700_IT_200MS:
        return String("200");
        break;
    case VEML7700_IT_400MS:
        return String("400");
        break;
    case VEML7700_IT_800MS:
        return String("800");
        break;
    default:
        return String("Unknown(") + String(sampled_integration_time) + ")";
    }
}

bool VEML7700_wrapper ::get_is_initialized() const
{
    return is_initialized;
}