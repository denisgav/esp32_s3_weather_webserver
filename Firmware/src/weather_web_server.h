#pragma once

#include <Arduino.h>

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
#include <ESPmDNS.h>
//-------------------------------------------------

class weather_web_server{
public:
    weather_web_server(multisensor_wrapper &multisensor);
    void init();

    const char * get_WiFiCfg_ssid() const;
	const char * get_WiFiCfg_pass() const;
	const char * get_WiFiCfg_hostname() const;
    const char * get_WiFiCfg_ip_address() const;
    bool get_is_WiFiCfg_available() const;

    void load_configuration();

    void configure(
        const char *ssid , 
        const char *pass, 
        const char *hostname, 
        const char *ip_address);

    void setup_wifi();

    void setup_server();

    bool get_is_running_softAP() const;
    bool get_force_reconnect() const;

protected:
    multisensor_wrapper &multisensor;
    AsyncWebServer server;

    const char *WiFiCfgPath;
    const char *WiFiCfg_path;
    String WiFiCfg_ssid;
	String WiFiCfg_pass;
	String WiFiCfg_hostname;
    String WiFiCfg_ip_address;
    bool is_WiFiCfg_available;
    String scanned_networks;

    bool running_softAP;
    bool force_reconnect;

    void start_SPIFFS();
    String scan_networks();
private:
    String fetch_sensor_data() const;
    String fetch_wifi_status() const;
    void set_rtc_time(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    void set_wifi_cfg(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
};