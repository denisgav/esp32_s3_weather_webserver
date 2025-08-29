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

//-------------------------------------------------
// ST7735
//-------------------------------------------------

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include "Adafruit_miniTFTWing.h"

//-------------------------------------------------

// uninitialized pointers to SPI objects
// SPIClass fspi = SPIClass(FSPI);
SPIClass hspi = SPIClass(HSPI);

// Adafruit_miniTFTWing ss;
Adafruit_ST7735 tft = Adafruit_ST7735(&hspi, TFT_CS, TFT_DC, TFT_RES);

int init_sd_mmc();
int init_tft();

void setup(void)
{
    Serial.begin(115200);

    //fspi.begin(FSPI_SCK, FSPI_MISO, FSPI_MOSI, FSPI_SS);
    hspi.begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI, HSPI_SS);

    if(init_sd_mmc() != 0){
        while(true){
            Serial.print("SD MMC init failed!");
            delay(1000);
        }
    }

    if(init_tft() != 0){
        while(true){
            Serial.print("TFT init failed!");
            delay(1000);
        }
    }

    Serial.print("Hello!");

    Serial.println("Initialized");
}

void loop()
{
    delay(500);
}

int init_sd_mmc()
{
    if (!SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0))
    {
        Serial.println("Pin change failed!");
        return -1;
    }
    if (!SD_MMC.begin("/sdcard", true, false, BOARD_MAX_SDMMC_FREQ, 5))
    {
        Serial.println("Card Mount Failed");
        return -1;
    }
    uint8_t cardType = SD_MMC.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD_MMC card attached");
        return -1;
    }

    Serial.print("SD_MMC Card Type: ");
    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    }
    else
    {
        Serial.println("UNKNOWN");
        return -1;
    }

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);
    return 0;
}

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
    tft.initR(INITR_MINI160x80); // initialize a ST7735S chip, mini display

    tft.setRotation(3);

    tft.setCursor(0, 0);
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(0);
    tft.println("Hello World!");
    return 0;
}