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
    String res = "{";
    res += "\"bme280_temperature\": \"" + String(multisensor.get_bme280().get_sampled_temperature()) + "\",";
    res += "\"bme280_pressure\": \"" + String(multisensor.get_bme280().get_sampled_pressure()) + "\",";
    res += "\"bme280_humidity\": \"" + String(multisensor.get_bme280().get_sampled_humidity()) + "\"";
    res += "}";
    return res;
}