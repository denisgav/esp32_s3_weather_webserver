// Link to the board:
// https://mischianti.org/esp32-s3-devkitc-1-high-resolution-pinout-and-specs/
// Link to SD MMC card test:
// https://github.com/espressif/arduino-esp32/blob/master/libraries/SD_MMC/README.md
// Link to the SD test:
// https://github.com/espressif/arduino-esp32/blob/master/libraries/SD/README.md

#include <Arduino.h>

#include "board_defines.h"
#include "string_utils.h"

#include <FS.h>
#include <SD_MMC.h>
#include <SPIFFS.h>

//-------------------------------------------------
// ST7735
//-------------------------------------------------
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include "Adafruit_miniTFTWing.h"
//-------------------------------------------------

//-------------------------------------------------
// DS3231
//-------------------------------------------------
#include "DS3231_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// BME280
//-------------------------------------------------
#include "BME280_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// DHT11
//-------------------------------------------------
#include "DHT11_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// LM35
//-------------------------------------------------
#include "LM35_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// WiFi
//-------------------------------------------------
// #include <WiFi.h>
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>
//-------------------------------------------------

// uninitialized pointers to SPI objects
// SPIClass fspi = SPIClass(FSPI);
SPIClass hspi = SPIClass(HSPI);

// Adafruit_miniTFTWing ss;
Adafruit_ST7735 tft = Adafruit_ST7735(&hspi, TFT_CS_PIN, TFT_DC_PIN, TFT_RES_PIN);

// RTC3231
DS3231_wrapper rtc;
char daysOfTheWeek[7][4] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SUN"};

// BME280
BME280_wrapper bme280;

// DHT11
DHT11_wrapper dht11;

// LM35
LM35_wrapper lm35;

// Replace with your network credentials
// const char *ssid = "";
// const char *password = "";

// Create AsyncWebServer object on port 80
// AsyncWebServer server(80);
// AsyncEventSource events("/events");

int init_sd_mmc();
int init_tft();

// String get_live_sensor_values();

void setup(void)
{
  Serial.begin(115200);

  // fspi.begin(FSPI_SCK, FSPI_MISO, FSPI_MOSI, FSPI_SS);
  hspi.begin(HSPI_SCK_PIN, HSPI_MISO_PIN, HSPI_MOSI_PIN, HSPI_SS_PIN);

  // // Initialize SPIFFS
  // if (!SPIFFS.begin())
  // {
  //   while (true)
  //   {
  //     Serial.println("An Error has occurred while mounting SPIFFS");
  //     delay(1000);
  //   }
  // }

  // Serial.println("Init SD MMC");
  // if (init_sd_mmc() != 0)
  // {
  //   while (true)
  //   {
  //     Serial.print("SD MMC init failed!");
  //     delay(1000);
  //   }
  // }
  // Serial.println("Init SD MMC Done");

  Serial.println("Init TFT");
  if (init_tft() != 0)
  {
    while (true)
    {
      Serial.print("TFT init failed!");
      delay(1000);
    }
  }
  Serial.println("Init TFT Done");

  Serial.println("Init BME");
  bme280.init();
  Serial.println("Init BME Done");

  Serial.println("Init RTC");
  rtc.init();
  Serial.println("Init RTC Done");

  Serial.println("Init DHT11");
  dht11.init();
  Serial.println("Init DHT11 Done");

  Serial.println("Init LM35");
  lm35.init();
  Serial.println("Init LM35 Done");

  // Serial.println("Connecting to WiFi");
  // WiFi.begin(ssid, password);
  // Serial.println("Waiting for connection");
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("Connecting to WiFi done");

  // // Print local IP address and start web server
  // Serial.println("");
  // Serial.println("WiFi connected.");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP().toString());

  Serial.print("Hello!");

  Serial.println("Initialized");

  tft.fillScreen(ST77XX_BLACK);

  // -----------------------------
  // Web server initialization
  // -----------------------------

  // Route for root / web page
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send(SPIFFS, "/index.html", String(), false /*, processor*/); });

  // server.on("/live_sensor_values", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send_P(200, "application/json", get_live_sensor_values().c_str()); });

  // // Handle Web Server Events
  // events.onConnect([](AsyncEventSourceClient *client)
  //                  { client->send("hello!", NULL, millis(), 10000); });
  // server.addHandler(&events);

  // server.begin();
}

void loop()
{
  static int sensorUpdateMillisPrev = 0;
  // delay(1000);
  if (((sensorUpdateMillisPrev == 0) || ((millis() - sensorUpdateMillisPrev) >= 1000)))
  {
    if (rtc.sample_datetime_data() == false)
    {
      Serial.println("Couldn't find RTC");
    }
    else
    {
      DateTime now = rtc.get_sampled_datetime();
      float ds3231_temperature = rtc.get_sampled_temperature();

      String date_s = int_to_string(now.year(), 4) + '/' + int_to_string(now.month(), 2) + '/' + int_to_string(now.day(), 2) + ", " + daysOfTheWeek[now.dayOfTheWeek()];
      String time_s = int_to_string(now.hour(), 2) + ':' + int_to_string(now.minute(), 2) + ':' + int_to_string(now.second(), 2);
      String temperature_s = "Temperature: " + string_extend(String(ds3231_temperature), 6) + " C";

      Serial.println("[DS3231] " + date_s + " " + time_s + " " + temperature_s);
    }

    if (bme280.sample_sensor_data() == false)
    {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
    }
    else
    {
      float bme280_temperature = bme280.get_sampled_temperature();
      float bme280_pressure = bme280.get_sampled_pressure();
      float bme280_humidity = bme280.get_sampled_humidity();

      String temperature_s = "Temperature: " + string_extend(String(bme280_temperature), 6) + " C";
      String pressure_s = "Pressure: " + string_extend(String(bme280_pressure / 100.0F), 8) + " hPa";
      String humidity_s = "Humidity: " + string_extend(String(bme280_humidity), 6) + " %";

      Serial.println("[BME280] " + temperature_s + " " + pressure_s + " " + humidity_s);
    }

    if (dht11.sample_sensor_data() == false)
    {
      Serial.println("Couldn't find DHT11");
    }
    else
    {
      float dht11_temperature = bme280.get_sampled_temperature();
      float dht11_humidity = bme280.get_sampled_humidity();

      String temperature_s = "Temperature: " + string_extend(String(dht11_temperature), 6) + " C";
      String humidity_s = "Humidity: " + string_extend(String(dht11_humidity), 6) + " %";

      Serial.println("[DHT11] " + temperature_s + " " + humidity_s);
    }

    if (lm35.sample_sensor_data() == false)
    {
      Serial.println("Couldn't find LM35");
    }
    else
    {
      float lm35_temperature = lm35.get_sampled_temperature();

      String temperature_s = "Temperature: " + string_extend(String(lm35_temperature), 6) + " C";

      Serial.println("[LM35] " + temperature_s);
    }

    // String ip_addr_s = "IP address: " + WiFi.localIP().toString();

    // // tft.fillScreen(ST77XX_BLACK);
    // tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    // tft.setTextSize(0);

    // tft.setCursor(8, 8);
    // tft.println(date_s);
    // tft.setCursor(8, 16);
    // tft.println(time_s);
    // tft.setCursor(8, 24);
    // tft.println(temperature1_s);

    // tft.setCursor(8, 32);
    // tft.println(temperature2_s);
    // tft.setCursor(8, 40);
    // tft.println(pressure_s);
    // tft.setCursor(8, 48);
    // tft.println(humidity_s);

    // tft.setCursor(8, 56);
    // tft.println(ip_addr_s);

    sensorUpdateMillisPrev = millis();

    // String event_data = get_live_sensor_values();
    // events.send(event_data.c_str(), "data", millis());
  }
}

// String get_live_sensor_values()
// {
//   String res = "{";
//   res += "\"bme280_temperature\": \"" + String(bme280_temperature) + "\",";
//   res += "\"bme280_pressure\": \"" + String(bme280_pressure) + "\",";
//   res += "\"bme280_humidity\": \"" + String(bme280_humidity) + "\"";
//   res += "}";
//   return res;
// }

// int init_sd_mmc()
// {
//   if (!SD_MMC.setPins(SD_MMC_CLK_PIN, SD_MMC_CMD_PIN, SD_MMC_D0_PIN))
//   {
//     Serial.println("Pin change failed!");
//     return -1;
//   }
//   if (!SD_MMC.begin("/sdcard", true, false, BOARD_MAX_SDMMC_FREQ, 5))
//   {
//     Serial.println("Card Mount Failed");
//     return -1;
//   }
//   uint8_t cardType = SD_MMC.cardType();

//   if (cardType == CARD_NONE)
//   {
//     Serial.println("No SD_MMC card attached");
//     return -1;
//   }

//   Serial.print("SD_MMC Card Type: ");
//   if (cardType == CARD_MMC)
//   {
//     Serial.println("MMC");
//   }
//   else if (cardType == CARD_SD)
//   {
//     Serial.println("SDSC");
//   }
//   else if (cardType == CARD_SDHC)
//   {
//     Serial.println("SDHC");
//   }
//   else
//   {
//     Serial.println("UNKNOWN");
//     return -1;
//   }

//   uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
//   Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
//   return 0;
// }

int init_tft()
{
  //   if (!ss.begin()) {
  //     Serial.println("seesaw init error!");
  //     while(1);
  //   }
  //   else Serial.println("seesaw started");

  //   ss.tftReset();
  //   ss.setBacklight(0x0); //set the backlight fully on

  // Use this initializer (uncomment) if you're using a 0.96" 180x60 TFT
  tft.initR(INITR_MINI160x80_PLUGIN); // initialize a ST7735S chip, mini display
  // tft.invertDisplay(true);

  tft.setRotation(3);

  tft.setCursor(0, 0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(0);
  tft.println("Hello World!");

  return 0;
}
