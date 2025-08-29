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
const int HSPI_SS    = 10;
const int HSPI_MOSI  = 11;
const int HSPI_SCK   = 12;
const int HSPI_MISO  = 13;

const int FSPI_SS    = 39;
const int FSPI_MOSI  = 35;
const int FSPI_SCK   = 36;
const int FSPI_MISO  = 37;
//--------------------------

//--------------------------
// TFT Display
//--------------------------
const int TFT_SCL  = HSPI_SCK;  // SPI clock signal
const int TFT_SDA  = HSPI_MOSI; // Serial data input pin sda
const int TFT_RES  = 40; // Reset pin
const int TFT_DC   = 41; // Data selection signal
const int TFT_CS   = HSPI_SS; // LCD chip select signal, for SPI protocol
const int TFT_BLK  = -1; //
//--------------------------

//--------------------------
// SD SPI pins option
//--------------------------
const int SD_SCK  = FSPI_SCK;
const int SD_MISO = FSPI_MISO;
const int SD_MOSI = FSPI_MOSI;
const int SD_CS   = FSPI_SS; /* D3 */
//--------------------------

//--------------------------
// SD MMC pins option
//--------------------------
const int SD_MMC_CLK = FSPI_SCK;  // SCK OTG
const int SD_MMC_CMD = FSPI_MOSI; // MOSI OTG
const int SD_MMC_D0  = FSPI_MISO; // MISO OTG
//--------------------------

