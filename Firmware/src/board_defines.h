#pragma once
// SPI	        |MOSI	    |MISO	    |CLK	    |CS
// HSPI (SPI 1)	|GPIO 11	|GPIO 13	|GPIO 12	|GPIO 10
// FSPI (SPI 0)	|GPIO 35	|GPIO 37	|GPIO 36	|GPIO 39

// SD pins
// SD -> SPI
// D0 -> DO
// D1 -> X
// D2 -> X
// D3 -> CS
// CMD -> DI
// CLK -> SCLK

//--------------------------
// SPI buses
//--------------------------
const int HSPI_SS_PIN    = 10;
const int HSPI_MOSI_PIN  = 11;
const int HSPI_SCK_PIN   = 12;
const int HSPI_MISO_PIN  = 13;

const int FSPI_SS_PIN    = 39;
const int FSPI_MOSI_PIN  = 35;
const int FSPI_SCK_PIN   = 36;
const int FSPI_MISO_PIN  = 37;
//--------------------------

//--------------------------
// I2C bus
//--------------------------
const int I2C_SDA_PIN  = 8;
const int I2C_SCL_PIN  = 9;

const int DS3231_INT_PIN = 42;
//--------------------------

//--------------------------
// DHT11
//--------------------------
const int DHT11_PIN = 7;
//--------------------------

//--------------------------
// User buttons
//--------------------------
const int USR_BTN_PIN = 1;
const int USR_BTN_LEFT_PIN = 17;
const int USR_BTN_RIGHT_PIN = 18;
//--------------------------

//--------------------------
// Analog pins
//--------------------------
const int LDR_PIN = 4;
const int DS18B20_PIN = 5;
const int LM35_PIN = 6;
//--------------------------

//--------------------------
// TFT Display
//--------------------------
const int TFT_SCL_PIN  = HSPI_SCK_PIN;  // SPI clock signal
const int TFT_SDA_PIN  = HSPI_MOSI_PIN; // Serial data input pin sda
const int TFT_RES_PIN  = 40; // Reset pin
const int TFT_DC_PIN   = 41; // Data selection signal
const int TFT_CS_PIN   = HSPI_SS_PIN; // LCD chip select signal, for SPI protocol
const int TFT_BLK_PIN  = -1; //
//--------------------------

//--------------------------
// SD SPI pins option
//--------------------------
const int SD_SCK_PIN  = FSPI_SCK_PIN;
const int SD_MISO_PIN = FSPI_MISO_PIN;
const int SD_MOSI_PIN = FSPI_MOSI_PIN;
const int SD_CS_PIN   = FSPI_SS_PIN; /* D3 */
//--------------------------

//--------------------------
// SD MMC pins option
//--------------------------
const int SD_MMC_CLK_PIN = FSPI_SCK_PIN;  // SCK OTG
const int SD_MMC_CMD_PIN = FSPI_MOSI_PIN; // MOSI OTG
const int SD_MMC_D0_PIN  = FSPI_MISO_PIN; // MISO OTG
//--------------------------

