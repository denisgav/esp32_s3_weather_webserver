#include "multisensor_wrapper.h"

char daysOfTheWeek[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SUN"};

// Class constructor
multisensor_wrapper ::multisensor_wrapper() : rtc(),
                                              bme280(),
                                              ens160(),
                                              aht2x(),
                                              veml7700(),
                                              dht11(),
                                              lm35(),
                                              ldr(),
                                              is_initialized(false)
{
}
// Initialize BME sensor
bool multisensor_wrapper ::init()
{
    Serial.println("Init RTC");
    rtc.init();
    Serial.println("Init RTC Done");

    Serial.println("Init BME280");
    bme280.init();
    Serial.println("Init BME280 Done");

    Serial.println("Init ENS160");
    ens160.init();
    Serial.println("Init ENS160 Done");

    Serial.println("Init AHT2x");
    aht2x.init();
    Serial.println("Init AHT2x Done");

    Serial.println("Init VEML770");
    veml7700.init();
    Serial.println("Init VEML770 Done");

    Serial.println("Init DHT11");
    dht11.init();
    Serial.println("Init DHT11 Done");

    Serial.println("Init LM35");
    lm35.init();
    Serial.println("Init LM35 Done");

    Serial.println("Init LDR");
    ldr.init();
    Serial.println("Init LDR Done");

    is_initialized = rtc.get_is_initialized() && 
        bme280.get_is_initialized() && ens160.get_is_initialized() && 
        aht2x.get_is_initialized() && veml7700.get_is_initialized() && 
        dht11.get_is_initialized() && 
        lm35.get_is_initialized() && ldr.get_is_initialized();

    return is_initialized;
}

// Sample date/time
bool multisensor_wrapper ::sample_sensor_data()
{
    bool sample_error_happen = false;
    if (rtc.sample_datetime_data() == false)
    {
        Serial.println("Couldn't find RTC");
        sample_error_happen = true;
    }
    else
    {
        DateTime now = rtc.get_sampled_datetime();
        float ds3231_temperature = rtc.get_sampled_temperature();

        String date_s = String(now.year()) + '/' + String(now.month()) + '/' + String(now.day()) + ", " + daysOfTheWeek[now.dayOfTheWeek()];
        String time_s = String(now.hour()) + ':' + String(now.minute()) + ':' + String(now.second());
        String temperature_s = "Temperature: " + String(ds3231_temperature) + " C";

        Serial.println("[DS3231] " + date_s + " " + time_s + " " + temperature_s);
    }

    if (lm35.sample_sensor_data() == false)
    {
        Serial.println("Couldn't find LM35");
        sample_error_happen = true;
    }
    else
    {
        float lm35_temperature = lm35.get_sampled_temperature();

        String temperature_s = "Temperature: " + String(lm35_temperature) + " C";

        Serial.println("[LM35] " + temperature_s);
    }

    if (bme280.sample_sensor_data() == false)
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        sample_error_happen = true;
    }
    else
    {
        float bme280_temperature = bme280.get_sampled_temperature();
        float bme280_pressure = bme280.get_sampled_pressure();
        float bme280_humidity = bme280.get_sampled_humidity();

        String temperature_s = "Temperature: " + String(bme280_temperature) + " C";
        String pressure_s = "Pressure: " + String(bme280_pressure / 100.0F) + " hPa";
        String humidity_s = "Humidity: " + String(bme280_humidity) + " %";

        Serial.println("[BME280] " + temperature_s + " " + pressure_s + " " + humidity_s);
    }

    if (ens160.sample_sensor_data() == false)
    {
        Serial.println("[ENS160] did not sample values!");
        sample_error_happen = true;
    }
    else
    {
        uint8_t ens160_AQI = ens160.get_sampled_AQI();
        uint16_t ens160_TVOC = ens160.get_sampled_TVOC();
        uint16_t ens160_eCO2 = ens160.get_sampled_eCO2();

        String AQI_s = "AQI: " + String(ens160_AQI);
        String TVOC_s = "TVOC: " + String(ens160_TVOC) + " ppb";
        String eCO2_s = "eCO2: " + String(ens160_eCO2) + " ppm";

        Serial.println("[ENS160] " + AQI_s + " " + TVOC_s + " " + eCO2_s);
    }

    if (aht2x.sample_sensor_data() == false)
    {
        Serial.println("Couldn't find AHT2x");
        sample_error_happen = true;
    }
    else
    {
        float aht2x_temperature = aht2x.get_sampled_temperature();
        float aht2x_humidity = aht2x.get_sampled_humidity();

        String temperature_s = "Temperature: " + String(aht2x_temperature) + " C";
        String humidity_s = "Humidity: " + String(aht2x_humidity) + " %";

        Serial.println("[AHT2x] " + temperature_s + " " + humidity_s);
    }

    if (veml7700.sample_sensor_data() == false)
    {
        Serial.println("Couldn't find VEML7700");
        sample_error_happen = true;
    }
    else
    {
        float lux = veml7700.get_sampled_lux();
        float gain = veml7700.get_sampled_gain();
        float integration_time = veml7700.get_sampled_integration_time();

        String lux_s = "Lux: " + String(lux);
        String gain_s = "Gain: " + veml7700.get_sampled_gain_s();
        String integration_time_s = "Integration Time: " + veml7700.get_sampled_integration_time_s() + "MS";

        Serial.println("[VEML7700] " + lux_s + " " + gain_s + " " + integration_time_s);
    }

    if (dht11.sample_sensor_data() == false)
    {
        Serial.println("Couldn't find DHT11");
        sample_error_happen = true;
    }
    else
    {
        float dht11_temperature = bme280.get_sampled_temperature();
        float dht11_humidity = bme280.get_sampled_humidity();

        String temperature_s = "Temperature: " + String(dht11_temperature) + " C";
        String humidity_s = "Humidity: " + String(dht11_humidity) + " %";

        Serial.println("[DHT11] " + temperature_s + " " + humidity_s);
    }

    if (ldr.sample_sensor_data() == false)
    {
        Serial.println("Couldn't find LDR");
        sample_error_happen = true;
    }
    else
    {
        float sampled_ldr = ldr.get_sampled_ldr();

        String ldr_s = String(sampled_ldr) + " mV";

        Serial.println("[LDR] " + ldr_s);
    }

    return sample_error_happen == false;
}

// RTC3231
DS3231_wrapper &multisensor_wrapper ::get_rtc()
{
    return rtc;
}
// BME280
BME280_wrapper &multisensor_wrapper ::get_bme280()
{
    return bme280;
}
// ENS160
ENS160_wrapper &multisensor_wrapper ::get_ens160()
{
    return ens160;
}
// AHT2x
AHT2x_wrapper &multisensor_wrapper ::get_aht2x()
{
    return aht2x;
}
// AHT2x
VEML7700_wrapper &multisensor_wrapper ::get_veml7700()
{
    return veml7700;
}
// DHT11
DHT11_wrapper &multisensor_wrapper ::get_dht11()
{
    return dht11;
}
// LM35
LM35_wrapper &multisensor_wrapper ::get_lm35()
{
    return lm35;
}

// LDR
LDR_wrapper &multisensor_wrapper ::get_ldr()
{
    return ldr;
}

bool multisensor_wrapper ::get_is_initialized() const
{
    return is_initialized;
}