#include <ArduinoJson.h>

#include "weather_web_server.h"

const char *WiFiCfgPath;

weather_web_server ::weather_web_server(multisensor_wrapper &multisensor) : multisensor(multisensor),
                                                                            server(80),
                                                                            WiFiCfg_path("/__WiFiCfg__.txt"),
                                                                            WiFiCfg_ssid("<UNDEFINED>"),
                                                                            WiFiCfg_pass("<UNDEFINED>"),
                                                                            WiFiCfg_hostname("<UNDEFINED>"),
                                                                            WiFiCfg_ip_address("<UNDEFINED>"),
                                                                            is_WiFiCfg_available(false),

                                                                            scanned_networks(""),

                                                                            running_softAP(false),
                                                                            force_reconnect(false)
{
}

void weather_web_server ::init()
{
    start_SPIFFS();

    Serial.println("loading configuration");
    load_configuration();
    Serial.println("loading configuration done");

    Serial.println("Scanning WiFi networks");
    scanned_networks = scan_networks();
    Serial.println("Scanning WiFi networks done");
}

void weather_web_server ::setup_wifi()
{
    if (is_WiFiCfg_available)
    {
        if (WiFiCfg_hostname != "")
        {
            WiFi.setHostname(WiFiCfg_hostname.c_str());
            Serial.println("Setting hostname " + String(WiFiCfg_hostname));

            if (MDNS.begin(WiFiCfg_hostname))
            {
                Serial.println("mDNS responder started");
            }
            else
            {
                Serial.println("Unable to start mDNS responder");
            }
        }
        else
        {
            Serial.println("No hostname configured");
        }

        Serial.println("Connecting to WiFi");
        WiFi.begin(WiFiCfg_ssid, WiFiCfg_pass);
        Serial.println("Waiting for connection");
        for (size_t i = 0; i < 100; i++)
        {
            if (WiFi.status() != WL_CONNECTED)
            {
                vTaskDelay(500);
                Serial.print(".");
            }
            else
            {
                break;
            }
        }
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("Connecting to WiFi done");
    }
    else
    {
        Serial.println("Can not connect to WiFi");

        // Fixes issue with mDNS where hostname was not set (v1.0.1) and mDNS crashed (v1.0.2)
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
        
        WiFi.mode(WIFI_AP);
        //MDNS.addService("http", "tcp", 80); // Advertises an HTTP service on port 80
        WiFi.softAP("ESP32-S3-Weather-Webserver");
        running_softAP = true;
    }
    force_reconnect = false;
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

const char *weather_web_server ::get_WiFiCfg_ssid() const
{
    return WiFiCfg_ssid.c_str();
}

const char *weather_web_server ::get_WiFiCfg_pass() const
{
    return WiFiCfg_pass.c_str();
}

const char *weather_web_server ::get_WiFiCfg_hostname() const
{
    return WiFiCfg_hostname.c_str();
}

const char *weather_web_server ::get_WiFiCfg_ip_address() const
{
    return WiFiCfg_ip_address.c_str();
}

bool weather_web_server ::get_is_WiFiCfg_available() const
{
    return is_WiFiCfg_available;
}

void weather_web_server ::configure(
    const char *ssid,
    const char *pass,
    const char *hostname,
    const char *ip_address)
{
    JsonDocument doc;

    doc["ssid"] = ssid;
    doc["pass"] = pass;
    doc["hostname"] = hostname;
    doc["ip_address"] = ip_address;

    File file = SPIFFS.open(WiFiCfg_path, "w", true);
    if (!file)
    {
        Serial.println("Failed to open file \"" + String(WiFiCfg_path) + "\"  for writing");
        return;
    }

    if (serializeJson(doc, file) == 0)
    {
        Serial.println("Failed to write to file");
        file.close();
    }
    else
    {
        Serial.println("JSON written to file successfully");
    }
    file.close();

    load_configuration();
    force_reconnect = true;
}

void weather_web_server ::load_configuration()
{
    File file = SPIFFS.open(WiFiCfg_path, "r");
    if (!file)
    {
        Serial.println("Failed to open \"" + String(WiFiCfg_path) + "\" file for reading");

        is_WiFiCfg_available = false;
        return;
    }

    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        file.close();

        is_WiFiCfg_available = false;
        return;
    }

    const char *ssid = doc["ssid"];
    WiFiCfg_ssid = ssid;
    const char *pass = doc["pass"];
    WiFiCfg_pass = pass;
    const char *hostname = doc["hostname"];
    WiFiCfg_hostname = hostname;
    const char *ip_address = doc["ip_address"];
    WiFiCfg_ip_address = ip_address;

    is_WiFiCfg_available = true;
    file.close();

    // Serial.println("Load configuration ssid:" + String(ssid));
    // Serial.println("Load configuration password:" + String(pass));
    // Serial.println("Load configuration hostname:" + String(hostname));
    // Serial.println("Load configuration ip:" + String(ip_address));

    running_softAP = false;
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
                request->send(200, "application/json", "{\"status\":\"success\"}"); },
              NULL, // No file upload handler in this case
              [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
              { set_rtc_time(request, data, len, index, total); });

    server.on("/api/wifi/save", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                // This callback is called after onBody, if needed for response
                request->send(200, "application/json", "{\"status\":\"success\"}"); },
              NULL, // No file upload handler in this case
              [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
              { set_wifi_cfg(request, data, len, index, total); });

    server.on("/api/wifi/scan", HTTP_GET, [this](AsyncWebServerRequest *request)
              { request->send_P(200, "application/json", scanned_networks.c_str()); });

    server.on("/api/wifi/status", HTTP_GET, [this](AsyncWebServerRequest *request)
              { request->send_P(200, "application/json", fetch_wifi_status().c_str()); });
    

    server.begin();
}

bool weather_web_server ::get_is_running_softAP() const
{
    return running_softAP;
}

bool weather_web_server ::get_force_reconnect() const
{
    return force_reconnect;
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

String weather_web_server ::fetch_wifi_status() const
{
    JsonDocument doc;

    if(is_WiFiCfg_available){
        doc["ssid"] = WiFiCfg_ssid;
        doc["hostname"] = WiFiCfg_hostname;
        doc["ip"] = WiFiCfg_ip_address;
    }

    // Lastly, you can print the resulting JSON to a String
    String output;
    serializeJson(doc, output);

    return output;
}

String weather_web_server ::fetch_sensor_data() const
{
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

void weather_web_server ::set_wifi_cfg(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
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
        const char* ssid = doc["ssid"];
        const char* password = doc["password"];
        const char* hostname = doc["hostname"];
        const char* ip = doc["ip"];

        // Serial.println("Configure ssid:" + String(ssid));
        // Serial.println("Configure password:" + String(password));
        // Serial.println("Configure hostname:" + String(hostname));
        // Serial.println("Configure ip:" + String(ip));

        configure(ssid, password, hostname, ip);
        // You can perform actions based on the received JSON data here
    }
    // If the body is larger and comes in multiple chunks, you'd handle
    // appending data to a buffer and then parsing it when `index + len == total`.
}