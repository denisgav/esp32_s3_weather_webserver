#include "BME280_wrapper.h"

// Class constructor
BME280_wrapper ::BME280_wrapper() : bme280(),
                                    sampled_temperature(nanf("")),
                                    sampled_humidity(nanf("")),
                                    sampled_pressure(nanf("")),
                                    is_sampled(false),
                                    is_initialized(false)

{
}

// Initialize BME sensor
bool BME280_wrapper ::init()
{
    is_initialized = bme280.begin(I2C_BME280_ADDR, &Wire);
    return is_initialized;
}

// Sample sensor values
bool BME280_wrapper ::sample_sensor_data()
{
    if (is_initialized)
    {
        sampled_temperature = bme280.readTemperature();
        sampled_humidity = bme280.readHumidity();
        sampled_pressure = bme280.readPressure();
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
bool BME280_wrapper ::get_is_sampled() const
{
    return is_sampled;
}

// get sampled sensor values
float BME280_wrapper ::get_sampled_temperature() const
{
    return sampled_temperature;
}

float BME280_wrapper ::get_sampled_humidity() const
{
    return sampled_humidity;
}

float BME280_wrapper ::get_sampled_pressure() const
{
    return sampled_pressure;
}

bool BME280_wrapper ::get_is_initialized() const
{
    return is_initialized;
}