#pragma once

#include <Arduino.h>
#include <stdbool.h>
#include "board_defines.h"

//-------------------------------------------------
// Multisensor
//-------------------------------------------------
#include "multisensor_wrapper.h"
//-------------------------------------------------

//-------------------------------------------------
// ST7735
//-------------------------------------------------
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include "Adafruit_miniTFTWing.h"
//-------------------------------------------------

class LCD_wrapper{
public:
    // Class constructor
    LCD_wrapper(multisensor_wrapper &multisensor, SPIClass &spi);
    bool init();

    void on_left_button_press();
    void on_user_button_press();
    void on_right_button_press();
    void loop();    
private:
    multisensor_wrapper &multisensor;
    Adafruit_ST7735 tft;
    // Adafruit_miniTFTWing ss;
    bool is_initialized;
};