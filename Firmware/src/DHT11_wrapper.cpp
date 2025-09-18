#include "DHT11_wrapper.h"

// Class constructor
DHT11_wrapper ::DHT11_wrapper() : dht(DHT11_PIN, DHTTYPE),
                                  sampled_temperature(nanf("")),
                                  sampled_humidity(nanf("")),
                                  is_initialized(false)
{
}

// Initialize BME sensor
bool DHT11_wrapper ::init()
{
    dht.begin();
    is_initialized = true;
    return true;
}

// Sample date/time
bool DHT11_wrapper ::sample_sensor_data()
{
    if (is_initialized)
    {
        sensors_event_t event;

        dht.temperature().getEvent(&event);

        sampled_temperature = event.temperature;
        if (isnan(event.temperature)) {
             Serial.println(F("Error reading temperature!"));
             return false;
        }

        dht.humidity().getEvent(&event);
        sampled_humidity = event.relative_humidity;
        if (isnan(event.relative_humidity)) {
            Serial.println(F("Error reading humidity!"));
            return false;
        }

        return true;
    }
    else
        return false;
}

// get sampled datetime values
float DHT11_wrapper ::get_sampled_temperature() const
{
    return sampled_temperature;
}

float DHT11_wrapper ::get_sampled_humidity() const
{
    return sampled_humidity;
}

bool DHT11_wrapper ::get_is_initialized() const
{
    return is_initialized;
}

void DHT11_wrapper ::print_sensor_info()
{
    Serial.println(F("DHTxx Unified Sensor Example"));
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(sensor.max_value);
    Serial.println(F("°C"));
    Serial.print(F("Min Value:   "));
    Serial.print(sensor.min_value);
    Serial.println(F("°C"));
    Serial.print(F("Resolution:  "));
    Serial.print(sensor.resolution);
    Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(sensor.max_value);
    Serial.println(F("%"));
    Serial.print(F("Min Value:   "));
    Serial.print(sensor.min_value);
    Serial.println(F("%"));
    Serial.print(F("Resolution:  "));
    Serial.print(sensor.resolution);
    Serial.println(F("%"));
    Serial.println(F("------------------------------------"));
    // Set delay between sensor readings based on sensor details.
}