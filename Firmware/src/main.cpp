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
// ST7735
//-------------------------------------------------
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include "Adafruit_miniTFTWing.h"
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



// uninitialized pointers to SPI objects
// SPIClass fspi = SPIClass(FSPI);
SPIClass hspi = SPIClass(HSPI);

// Adafruit_miniTFTWing ss;
Adafruit_ST7735 tft = Adafruit_ST7735(&hspi, TFT_CS_PIN, TFT_DC_PIN, TFT_RES_PIN);


// Multisensor
multisensor_wrapper multisensor;

// WiFi
weather_web_server web_server(multisensor);

// Replace with your network credentials
// const char *ssid = "";
// const char *password = "";

// Create AsyncWebServer object on port 80
// AsyncWebServer server(80);

//int init_sd_mmc();
int init_tft();

void setup(void)
{
  Serial.begin(115200);

  // fspi.begin(FSPI_SCK, FSPI_MISO, FSPI_MOSI, FSPI_SS);
  hspi.begin(HSPI_SCK_PIN, HSPI_MISO_PIN, HSPI_MOSI_PIN, HSPI_SS_PIN);



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

  Serial.println("Init multisensor");
  multisensor.init();
  Serial.println("Init multisensor done");

  // WiFi
  Serial.println("Init web_server");
  web_server.init();
  Serial.println("Init web_server done");

  



  Serial.print("Hello!");

  Serial.println("Initialized");

  tft.fillScreen(ST77XX_BLACK);

  // -----------------------------
  // Web server initialization
  // -----------------------------

  
}

void loop()
{
  static int sensorUpdateMillisPrev = 0;
  // delay(1000);
  if (((sensorUpdateMillisPrev == 0) || ((millis() - sensorUpdateMillisPrev) >= 1000)))
  {
    
    multisensor.sample_sensor_data();

    String ip_addr_s = "IP address: " + WiFi.localIP().toString();
    Serial.println(ip_addr_s);

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
