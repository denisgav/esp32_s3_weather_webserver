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
SemaphoreHandle_t xtftSemaphore;

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

  /* Create a mutex type semaphore. */
  xtftSemaphore = xSemaphoreCreateMutex();

  // Create freeRTOS tasks
  xTaskCreate(
      MultisensorSampleTask,       // Task function
      "MultisensorSampleTask",     // Task name
      10000,                       // Stack size (bytes)
      NULL,                        // Parameters
      1,                           // Priority
      &MultisensorSampleTaskHandle // Task handle
  );

  xTaskCreate(
      WebServerTask,       // Task function
      "WebServerTask",     // Task name
      10000,               // Stack size (bytes)
      NULL,                // Parameters
      1,                   // Priority
      &WebServerTaskHandle // Task handle
  );

  xTaskCreate(
      TFT_Task,       // Task function
      "TFT_Task",     // Task name
      10000,          // Stack size (bytes)
      NULL,           // Parameters
      1,              // Priority
      &TFT_TaskHandle // Task handle
  );

  xTaskCreate(
      ButtonsTask,       // Task function
      "ButtonsTask",     // Task name
      10000,             // Stack size (bytes)
      NULL,              // Parameters
      1,                 // Priority
      &ButtonsTaskHandle // Task handle
  );
}

void loop()
{
  // Empty because FreeRTOS scheduler runs the task
}

void MultisensorSampleTask(void *parameter)
{
  for (;;)
  { // Infinite loop
    vTaskDelay(2500);
    multisensor.sample_sensor_data();
    multisensor.print_to_serial();
  }
}

void WebServerTask(void *parameter)
{
  // WiFi
  Serial.println("Init web_server");
  web_server.init();
  web_server.setup_wifi();
  web_server.setup_server();
  Serial.println("Init web_server done");

  for (;;)
  { // Infinite loop
    vTaskDelay(2500);

    if (web_server.get_force_reconnect())
    {
      WiFi.disconnect();
      web_server.setup_wifi();
    }
    else
    {
      wifi_mode_t mode = WiFi.getMode();
      switch(mode)
      {
        case WIFI_MODE_AP:
        {
          String AP_info = "(Use AP)";
          String ip_addr_s = WiFi.softAPIP().toString();
          Serial.println("[WiFi]" + AP_info + " IP address: " + ip_addr_s);
          break;
        }
        case WIFI_MODE_STA:
        {
          String AP_info = "(STA)";
          String ip_addr_s = WiFi.localIP().toString();
          Serial.println("[WiFi]" + AP_info + " IP address: " + ip_addr_s);
          break;
        }
        default:
        {
          Serial.println("[WiFi] Config error");
          break;
        }
      }

      bool is_connection_ok = (mode == WIFI_MODE_AP) || ((mode == WIFI_MODE_STA) && (WiFi.status() == WL_CONNECTED));
      
      if (is_connection_ok == false)
      {
        web_server.setup_wifi();
      }
    }
  }
}

void TFT_Task(void *parameter)
{
  for (;;)
  { // Infinite loop
    vTaskDelay(1000);

    /* See if we can obtain the semaphore. If the semaphore is not
          available wait 10 ticks to see if it becomes free. */
    if (xSemaphoreTake(xtftSemaphore, (TickType_t)1000) == pdTRUE)
    {
      /* We were able to obtain the semaphore and can now access the
          shared resource. */

      tft.refresh();

      /* We have finished accessing the shared resource. Release the
          semaphore. */
      xSemaphoreGive(xtftSemaphore);
    }
    else
    {
      /* We could not obtain the semaphore and can therefore not access
          the shared resource safely. */
    }
  }
}

void on_button_event(LCD_BUTTON_e button, LCD_BUTTON_EVENT_e event)
{
  /* See if we can obtain the semaphore. If the semaphore is not
      available wait 10 ticks to see if it becomes free. */
  if (xSemaphoreTake(xtftSemaphore, (TickType_t)1000) == pdTRUE)
  {
    /* We were able to obtain the semaphore and can now access the
        shared resource. */

    tft.on_button_event(button, event);

    /* We have finished accessing the shared resource. Release the
        semaphore. */
    xSemaphoreGive(xtftSemaphore);
  }
  else
  {
    /* We could not obtain the semaphore and can therefore not access
        the shared resource safely. */
  }
}

void ButtonsTask(void *parameter)
{
  static bool btn_left_state_prev = false;
  static bool btn_state_prev = false;
  static bool btn_right_state_prev = false;

  pinMode(USR_BTN_LEFT_PIN, INPUT);
  pinMode(USR_BTN_PIN, INPUT);
  pinMode(USR_BTN_RIGHT_PIN, INPUT);

  for (;;)
  { // Infinite loop
    vTaskDelay(50);

    bool btn_left_state = digitalRead(USR_BTN_LEFT_PIN);
    bool btn_state = digitalRead(USR_BTN_PIN);
    bool btn_right_state = digitalRead(USR_BTN_RIGHT_PIN);

    if ((btn_left_state == true) && (btn_left_state_prev == false))
    {
      on_button_event(LCD_BUTTON_LEFT, LCD_BUTTON_PRESS);
    }

    if ((btn_left_state == false) && (btn_left_state_prev == true))
    {
      on_button_event(LCD_BUTTON_LEFT, LCD_BUTTON_RELEASE);
    }

    if ((btn_state == true) && (btn_state_prev == false))
    {
      on_button_event(LCD_BUTTON, LCD_BUTTON_PRESS);
    }

    if ((btn_state == false) && (btn_state_prev == true))
    {
      on_button_event(LCD_BUTTON, LCD_BUTTON_RELEASE);
    }

    if ((btn_right_state == true) && (btn_right_state_prev == false))
    {
      on_button_event(LCD_BUTTON_RIGHT, LCD_BUTTON_PRESS);
    }

    if ((btn_right_state == false) && (btn_right_state_prev == true))
    {
      on_button_event(LCD_BUTTON_RIGHT, LCD_BUTTON_RELEASE);
    }

    btn_left_state_prev = btn_left_state;
    btn_state_prev = btn_state;
    btn_right_state_prev = btn_right_state;
  }
}
