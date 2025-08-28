#pragma once
// SPI	        |MOSI	    |MISO	    |CLK	    |CS
// HSPI (SPI 2)	|GPIO 11	|GPIO 13	|GPIO 12	|GPIO 10
// VSPI (SPI 3)	|GPIO 35	|GPIO 37	|GPIO 36	|GPIO 39

// SD pins
// SD -> SPI
// D0 -> DO
// D1 -> X
// D2 -> X
// D3 -> CS
// CMD -> DI
// CLK -> SCLK

// SD MMC pins
const int SD_MMC_CLK = 36; // SCK OTG
const int SD_MMC_CMD = 35; // MOSI OTG
const int SD_MMC_D0  = 37; // MISO OTG

// Used VSPI bus
const int SD_SCK  = SD_MMC_CLK;
const int SD_MISO = SD_MMC_D0;
const int SD_MOSI = SD_MMC_CMD;
const int SD_CS   = 39; /* D3 */