#include <ArduinoJson.h>

#include "weather_web_server.h"

weather_web_server ::weather_web_server(multisensor_wrapper &multisensor) : multisensor(multisensor),
                                                                            server(80)
{
}

void weather_web_server ::init()
{
    // Initialize SPIFFS
    if (!SPIFFS.begin())
    {
        while (true)
        {
            Serial.println("An Error has occurred while mounting SPIFFS");
            delay(1000);
        }
    }

    Serial.println("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.println("Waiting for connection");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("Connecting to WiFi done");

    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP().toString());

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", String(), false /*, processor*/); });
    server.on("/page_script.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/page_script.js", String(), false /*, processor*/); });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/style.css", String(), false /*, processor*/); });

    server.on("/api/fetch_sensor_data", HTTP_GET, [this](AsyncWebServerRequest *request)
              { request->send_P(200, "application/json", fetch_sensor_data().c_str()); });

    server.begin();
}

String weather_web_server ::fetch_sensor_data() const
{
    DateTime now = multisensor.get_rtc().get_sampled_datetime();

    JsonDocument doc;

    // Sampled RTC date time
    doc["year"] = String(now.year());
    doc["month"] = String(now.month());
    doc["day"] = String(now.day());
    doc["dayOfTheWeek"] = String(now.dayOfTheWeek());
    doc["hour"] = String(now.hour());
    doc["minute"] = String(now.minute());
    doc["second"] = String(now.second());

    // DS3231
    doc["ds3231_temperature"] = String(multisensor.get_rtc().get_sampled_temperature());

    // LM35
    doc["lm35_temperature"] = String(multisensor.get_lm35().get_sampled_temperature());

    // BME280
    doc["bme280_temperature"] = String(multisensor.get_bme280().get_sampled_temperature());
    doc["bme280_pressure"] = String(multisensor.get_bme280().get_sampled_pressure());
    doc["bme280_humidity"] = String(multisensor.get_bme280().get_sampled_humidity());

    // DHT11
    doc["dht11_temperature"] = String(multisensor.get_dht11().get_sampled_temperature());
    doc["dht11_humidity"] = String(multisensor.get_dht11().get_sampled_humidity());

    // ENS160
    doc["ens160_AQI"] = String(multisensor.get_ens160().get_sampled_AQI());
    doc["ens160_TVOC"] = String(multisensor.get_ens160().get_sampled_TVOC());
    doc["ens160_eCO2"] = String(multisensor.get_ens160().get_sampled_eCO2());

    // AHT2x
    doc["aht2x_temperature"] = String(multisensor.get_aht2x().get_sampled_temperature());
    doc["aht2x_humidity"] = String(multisensor.get_aht2x().get_sampled_humidity());

    // LDR
    doc["ldr"] = String(multisensor.get_ldr().get_sampled_ldr());

    
    // Lastly, you can print the resulting JSON to a String
    String output;
    serializeJson(doc, output);

    return output;
}