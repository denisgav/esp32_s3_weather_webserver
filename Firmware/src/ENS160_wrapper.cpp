#include "ENS160_wrapper.h"

// Class constructor
ENS160_wrapper ::ENS160_wrapper() : ens160(ENS160_I2CADDR_0),
                                    sampled_AQI(0),
                                    sampled_TVOC(0),
                                    sampled_eCO2(0),
                                    is_initialized(false)
{
}
// Initialize BME sensor
bool ENS160_wrapper ::init()
{
    ens160.begin();
    is_initialized = ens160.available();
    if(is_initialized)
    {
        Serial.println(ens160.setMode(ENS160_OPMODE_STD) ? "done." : "failed!");
    }
    return is_initialized;
}
// Sample sensor values
bool ENS160_wrapper ::sample_sensor_data()
{
    if (is_initialized)
    {
        if (ens160.available())
        {
            ens160.measure(true);
            ens160.measureRaw(true);

            sampled_AQI = ens160.getAQI();
            sampled_TVOC = ens160.getTVOC();
            sampled_eCO2 = ens160.geteCO2();
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

// get sampled sensor values
uint8_t ENS160_wrapper ::get_sampled_AQI() const
{
    return sampled_AQI;
}
uint16_t ENS160_wrapper ::get_sampled_TVOC() const
{
    return sampled_TVOC;
}
uint16_t ENS160_wrapper ::get_sampled_eCO2() const
{
    return sampled_eCO2;
}

bool ENS160_wrapper ::get_is_initialized() const 
{
    return is_initialized;
}