// Link to the board:
// https://mischianti.org/esp32-s3-devkitc-1-high-resolution-pinout-and-specs/
// Link to SD MMC card test:
// https://github.com/espressif/arduino-esp32/blob/master/libraries/SD_MMC/README.md
// Link to the SD test:
// https://github.com/espressif/arduino-esp32/blob/master/libraries/SD/README.md

#include <Arduino.h>

#include "board_defines.h"

#include <FS.h>
#include <SD_MMC.h>
#include <SPIFFS.h>

//-------------------------------------------------
// freertos
//-------------------------------------------------
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//-------------------------------------------------

//-------------------------------------------------
// Multisensor
//-------------------------------------------------
#include "multisensor_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// WiFi
//-------------------------------------------------
#include "weather_web_server.h"
//-------------------------------------------------

//-------------------------------------------------
// LCD
//-------------------------------------------------
#include "LCD_wrapper.h"
//-------------------------------------------------

// SPIClass fspi(FSPI);
SPIClass hspi(HSPI);

// Multisensor
multisensor_wrapper multisensor;

// WiFi
weather_web_server web_server(multisensor);

// LCD
LCD_wrapper tft(multisensor, hspi);

// Declare task handle
TaskHandle_t MultisensorSampleTaskHandle = NULL;
TaskHandle_t WebServerTaskHandle = NULL;
TaskHandle_t TFT_TaskHandle = NULL;
TaskHandle_t ButtonsTaskHandle = NULL;

void MultisensorSampleTask(void *parameter);
void WebServerTask(void *parameter);
void TFT_Task(void *parameter);
void ButtonsTask(void *parameter);

void setup(void)
{
  Serial.begin(115200);
 
  // SPI
  // fspi.begin(FSPI_SCK, FSPI_MISO, FSPI_MOSI, FSPI_SS);
  hspi.begin(HSPI_SCK_PIN, HSPI_MISO_PIN, HSPI_MOSI_PIN, HSPI_SS_PIN);

  // TFT
  Serial.println("Init TFT");
  tft.init();
  Serial.println("Init TFT Done");

  // multisensor
  Serial.println("Init multisensor");
  multisensor.init();
  Serial.println("Init multisensor done");

  // Create freeRTOS tasks
  xTaskCreate(
    MultisensorSampleTask,         // Task function
    "MultisensorSampleTask",       // Task name
    10000,                         // Stack size (bytes)
    NULL,                          // Parameters
    1,                             // Priority
    &MultisensorSampleTaskHandle  // Task handle
  );

  xTaskCreate(
    WebServerTask,                 // Task function
    "WebServerTask",               // Task name
    10000,                         // Stack size (bytes)
    NULL,                          // Parameters
    1,                             // Priority
    &WebServerTaskHandle           // Task handle
  );

  xTaskCreate(
    TFT_Task,                      // Task function
    "TFT_Task",                    // Task name
    10000,                         // Stack size (bytes)
    NULL,                          // Parameters
    1,                             // Priority
    &TFT_TaskHandle                // Task handle
  );

  xTaskCreate(
    ButtonsTask,                   // Task function
    "ButtonsTask",                 // Task name
    10000,                         // Stack size (bytes)
    NULL,                          // Parameters
    1,                             // Priority
    &ButtonsTaskHandle             // Task handle
  );
 
}

void loop()
{
  // Empty because FreeRTOS scheduler runs the task
  
}

void MultisensorSampleTask(void *parameter){
  for (;;) { // Infinite loop
    vTaskDelay(2500);
    multisensor.sample_sensor_data();
    multisensor.print_to_serial();
  }
}

void WebServerTask(void *parameter){
  // WiFi
  Serial.println("Init web_server");
  web_server.init();
  web_server.setup_wifi();
  web_server.setup_server();
  Serial.println("Init web_server done");

  for (;;) { // Infinite loop
    vTaskDelay(2500);

    if(web_server.get_force_reconnect())
    {
      WiFi.disconnect();
      web_server.setup_wifi();
    }
    else
    {
      if((web_server.get_is_running_softAP()) || (WiFi.status() == WL_CONNECTED))
      {
        String AP_info = web_server.get_is_running_softAP() ? "(Use AP)" : "";
        String ip_addr_s = web_server.get_is_running_softAP() ? WiFi.softAPIP().toString() : WiFi.localIP().toString();
        Serial.println("IP address: " + ip_addr_s + AP_info);
      }
      else 
      {
        web_server.setup_wifi();
      }
    }
  }
}

void TFT_Task(void *parameter){
  for (;;) { // Infinite loop
    vTaskDelay(1000);
    tft.refresh();
  }
}

void ButtonsTask(void *parameter){
  for (;;) { // Infinite loop
    vTaskDelay(50);
  }
}

