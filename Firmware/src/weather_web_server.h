#pragma once

#include <Arduino.h>

#include "credentials.h"
// Contains following lines:
// #define WIFI_SSID      ""
// #define WIFI_PASSWORD  ""

#include "board_defines.h"

#include <FS.h>
#include <SD_MMC.h>
#include <SPIFFS.h>

//-------------------------------------------------
// Multisensor
//-------------------------------------------------
#include "multisensor_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// WiFi
//-------------------------------------------------
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
//-------------------------------------------------

class weather_web_server{
public:
    weather_web_server(multisensor_wrapper &multisensor);
    void init();
protected:
    void start_SPIFFS();
    void setup_server();
    String scan_networks();
private:
    multisensor_wrapper &multisensor;
    AsyncWebServer server;
    String scanned_networks;

    String fetch_sensor_data() const;
    void set_rtc_time(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    
};