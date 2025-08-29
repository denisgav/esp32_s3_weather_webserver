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

//-------------------------------------------------
// BME280
//-------------------------------------------------
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
//-------------------------------------------------

//-------------------------------------------------
// DS3231
//-------------------------------------------------
#include "RTClib.h"
//-------------------------------------------------

// uninitialized pointers to SPI objects
// SPIClass fspi = SPIClass(FSPI);
SPIClass hspi = SPIClass(HSPI);

// Adafruit_miniTFTWing ss;
Adafruit_ST7735 tft = Adafruit_ST7735(&hspi, TFT_CS, TFT_DC, TFT_RES);

// BME280
Adafruit_BME280 bme; // I2C

// RTC3231
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int init_sd_mmc();
int init_tft();

void setup(void)
{
    Serial.begin(115200);

    // fspi.begin(FSPI_SCK, FSPI_MISO, FSPI_MOSI, FSPI_SS);
    hspi.begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI, HSPI_SS);

    if (init_sd_mmc() != 0)
    {
        while (true)
        {
            Serial.print("SD MMC init failed!");
            delay(1000);
        }
    }

    if (init_tft() != 0)
    {
        while (true)
        {
            Serial.print("TFT init failed!");
            delay(1000);
        }
    }

    if (!bme.begin(I2C_BME280_ADDR, &Wire))
    {
        while (true)
        {
            Serial.println("Could not find a valid BME280 sensor, check wiring!");
            delay(1000);
        }
    }

    if (!rtc.begin())
    {
        while (true)
        {
            Serial.println("Couldn't find RTC");
            delay(1000);
        }
    }

    if (rtc.lostPower())
    {
        Serial.println("RTC lost power, let's set the time!");
        // When time needs to be set on a new device, or after a power loss, the
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }

    Serial.print("Hello!");

    Serial.println("Initialized");

    tft.fillScreen(ST77XX_BLACK);
}

void loop()
{
    delay(1000);

    DateTime now = rtc.now();

    String date_s = String(now.year()) + '/' + String(now.month()) + '/' + String(now.day()) + ", " + daysOfTheWeek[now.dayOfTheWeek()];
    String time_s = String(now.hour()) + ':' + String(now.minute()) + ':' + String(now.second());
    String temperature1_s = "Temperature: " + String(rtc.getTemperature()) + " C";

    Serial.println(date_s + " " + time_s + " " + temperature1_s);

    String temperature2_s = "Temperature: " + String(bme.readTemperature()) + " C";
    String pressure_s = "Pressure: " + String(bme.readPressure() / 100.0F) + " hPa";
    String humidity_s = "Humidity: " + String(bme.readHumidity()) + " %";

    Serial.println(temperature2_s + ", " + pressure_s + ", " + humidity_s);

    // tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(0);

    tft.setCursor(8, 8);
    tft.println(date_s);
    tft.setCursor(8, 16);
    tft.println(time_s);
    tft.setCursor(8, 24);
    tft.println(temperature1_s);

    tft.setCursor(8, 32);
    tft.println(temperature2_s);
    tft.setCursor(8, 40);
    tft.println(pressure_s);
    tft.setCursor(8, 48);
    tft.println(humidity_s);
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
    tft.invertDisplay(true);
    return 0;
}