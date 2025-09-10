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

    String res = "{";

    // DS3231
    res += "\"ds3231_temperature\": \"" + String(multisensor.get_rtc().get_sampled_temperature()) + "\"";
    res += ",\"year\": \"" + String(now.year()) + "\"";
    res += ",\"month\": \"" + String(now.month()) + "\"";
    res += ",\"day\": \"" + String(now.day()) + "\"";
    res += ",\"dayOfTheWeek\": \"" + String(now.dayOfTheWeek()) + "\"";
    res += ",\"hour\": \"" + String(now.hour()) + "\"";
    res += ",\"minute\": \"" + String(now.minute()) + "\"";
    res += ",\"second\": \"" + String(now.second()) + "\"";

    // LM35
    res += ",\"lm35_temperature\": \"" + String(multisensor.get_lm35().get_sampled_temperature()) + "\"";

    // BME280
    res += ",\"bme280_temperature\": \"" + String(multisensor.get_bme280().get_sampled_temperature()) + "\"";
    res += ",\"bme280_pressure\": \"" + String(multisensor.get_bme280().get_sampled_pressure()) + "\"";
    res += ",\"bme280_humidity\": \"" + String(multisensor.get_bme280().get_sampled_humidity()) + "\"";

    // DHT11
    res += ",\"dht11_temperature\": \"" + String(multisensor.get_dht11().get_sampled_temperature()) + "\"";
    res += ",\"dht11_humidity\": \"" + String(multisensor.get_dht11().get_sampled_humidity()) + "\"";

    // ENS160
    res += ",\"ens160_AQI\": \"" + String(multisensor.get_ens160().get_sampled_AQI()) + "\"";
    res += ",\"ens160_TVOC\": \"" + String(multisensor.get_ens160().get_sampled_TVOC()) + "\"";
    res += ",\"ens160_eCO2\": \"" + String(multisensor.get_ens160().get_sampled_eCO2()) + "\"";

    // AHT2x
    res += ",\"aht2x_temperature\": \"" + String(multisensor.get_aht2x().get_sampled_temperature()) + "\"";
    res += ",\"aht2x_humidity\": \"" + String(multisensor.get_aht2x().get_sampled_humidity()) + "\"";

    // LDR
    res += ",\"ldr\": \"" + String(multisensor.get_ldr().get_sampled_ldr()) + "\"";

    res += "}";
    return res;
}