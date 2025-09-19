#include <ArduinoJson.h>

#include "weather_web_server.h"

weather_web_server ::weather_web_server(multisensor_wrapper &multisensor) : multisensor(multisensor),
                                                                            server(80),
                                                                            scanned_networks("")
{
}

void weather_web_server ::init()
{
    start_SPIFFS();

    Serial.println("Scanning WiFi networks");
    scanned_networks = scan_networks();
    Serial.println("Scanning WiFi networks done");

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

    setup_server();
}

void weather_web_server ::start_SPIFFS()
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
}

void weather_web_server ::setup_server()
{
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", String(), false /*, processor*/); });
    server.on("/page_script.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/page_script.js", String(), false /*, processor*/); });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/style.css", String(), false /*, processor*/); });

    server.on("/api/fetch_sensor_data", HTTP_GET, [this](AsyncWebServerRequest *request)
              { request->send_P(200, "application/json", fetch_sensor_data().c_str()); });

    server.on("/api/set_rtc_time", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                // This callback is called after onBody, if needed for response
                request->send(200, "application/json", "{\"status\":\"success\"}"); 
              },
              NULL, // No file upload handler in this case
              [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
              { set_rtc_time(request, data, len, index, total); });

    server.on("/api/wifi/scan", HTTP_GET, [this](AsyncWebServerRequest *request)
              { request->send_P(200, "application/json", scanned_networks.c_str()); });

    server.begin();
}

String weather_web_server ::scan_networks()
{
    // We need to make sure we are disconnected
    // before trying to scan for networks. We do
    // not care about the return value from it.
    WiFi.disconnect();

    byte networks = WiFi.scanNetworks();

    String json = "[";
    String separator = "";

    // If negative value is returned from the scan we will
    // just return the empty list as the loop will not
    // run. This is in case of WIFI_SCAN_FAILED or similar.
    for (int i = 0; i < networks; i++)
    {
        String network = "\"" + WiFi.SSID(i) + "\"";

        if (json.indexOf(network) == -1)
        {
            json += separator + network;
            separator = ",";
        }
    }

    json += "]";

    return json;
}

String weather_web_server ::fetch_sensor_data() const
{
    // JsonDocument json_doc;
    JsonDocument doc;

    // Sampled RTC date time
    if (multisensor.get_rtc().get_is_sampled())
    {
        DateTime now = multisensor.get_rtc().get_sampled_datetime();

        // Sampled RTC date time
        doc["rtc"]["year"] = String(now.year());
        doc["rtc"]["month"] = String(now.month());
        doc["rtc"]["day"] = String(now.day());
        doc["rtc"]["dayOfTheWeek"] = String(now.dayOfTheWeek());
        doc["rtc"]["hour"] = String(now.hour());
        doc["rtc"]["minute"] = String(now.minute());
        doc["rtc"]["second"] = String(now.second());

        doc["rtc"]["temperature"] = String(multisensor.get_rtc().get_sampled_temperature());
    }

    // LM35
    if (multisensor.get_rtc().get_is_sampled())
    {
        doc["lm35"]["temperature"] = String(multisensor.get_lm35().get_sampled_temperature());
    }

    // BME280
    if (multisensor.get_bme280().get_is_sampled())
    {
        doc["bme280"]["temperature"] = String(multisensor.get_bme280().get_sampled_temperature());
        doc["bme280"]["pressure"] = String(multisensor.get_bme280().get_sampled_pressure());
        doc["bme280"]["humidity"] = String(multisensor.get_bme280().get_sampled_humidity());
    }

    // DHT11
    if (multisensor.get_dht11().get_is_sampled())
    {
        doc["dht11"]["temperature"] = String(multisensor.get_dht11().get_sampled_temperature());
        doc["dht11"]["humidity"] = String(multisensor.get_dht11().get_sampled_humidity());
    }

    // ENS160
    if (multisensor.get_dht11().get_is_sampled())
    {
        doc["ens160"]["AQI"] = String(multisensor.get_ens160().get_sampled_AQI());
        doc["ens160"]["TVOC"] = String(multisensor.get_ens160().get_sampled_TVOC());
        doc["ens160"]["eCO2"] = String(multisensor.get_ens160().get_sampled_eCO2());
    }

    // AHT2x
    if (multisensor.get_dht11().get_is_sampled())
    {
        doc["aht2x"]["temperature"] = String(multisensor.get_aht2x().get_sampled_temperature());
        doc["aht2x"]["humidity"] = String(multisensor.get_aht2x().get_sampled_humidity());
    }

    // VEML7700
    if (multisensor.get_dht11().get_is_sampled())
    {
        doc["veml7700"]["lux"] = String(multisensor.get_veml7700().get_sampled_lux());
        doc["veml7700"]["gain"] = multisensor.get_veml7700().get_sampled_gain_s();
        doc["veml7700"]["integration_time"] = multisensor.get_veml7700().get_sampled_integration_time_s();
    }

    // LDR
    if (multisensor.get_ldr().get_is_sampled())
    {
        doc["ldr"]["ldr"] = String(multisensor.get_ldr().get_sampled_ldr());
    }

    // Lastly, you can print the resulting JSON to a String
    String output;
    serializeJson(doc, output);

    return output;
}

void weather_web_server ::set_rtc_time(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
    // This is the onBody callback, where you receive the request body data
    if (index == 0)
    { // First chunk of data
        // Initialize a buffer for the JSON document
        // Using StaticJsonDocument for fixed size, or DynamicJsonDocument for dynamic size
        JsonDocument doc; // Adjust size as needed

        // Deserialize the JSON data
        DeserializationError error = deserializeJson(doc, (const char *)data, len);

        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            request->send(400, "text/plain", "Invalid JSON");
            return;
        }

        // Access the parsed JSON data
        // For example, if your JSON is {"key": "value"}
        int year = doc["year"];
        int month = doc["month"];
        int day = doc["day"];
        int hour = doc["hour"];
        int minute = doc["minute"];
        int second = doc["second"];

        DateTime dt(year, month, day, hour, minute, second);
        multisensor.get_rtc().adjust(dt);

        // You can perform actions based on the received JSON data here
    }
    // If the body is larger and comes in multiple chunks, you'd handle
    // appending data to a buffer and then parsing it when `index + len == total`.
}