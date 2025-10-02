#pragma once

#include <Arduino.h>
#include <stdbool.h>
#include "board_defines.h"
#include "string_utils.h"

#include <WiFi.h>

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

enum LCD_Window_e {
  LCD_WINDOW_NONE,
  LCD_WINDOW_MESSAGES,
  LCD_WINDOW_WIFI,
  LCD_WINDOW_RTC,
  LCD_WINDOW_BME280,
  LCD_WINDOW_ENS160,
  LCD_WINDOW_AHT2x,
  LCD_WINDOW_VEML7700,
  LCD_WINDOW_DHT11,
  LCD_WINDOW_LM35,
  LCD_WINDOW_LDR,
  LCD_WINDOW_TOTAL
}; 

enum LCD_BUTTON_e {
  LCD_BUTTON_LEFT,
  LCD_BUTTON,
  LCD_BUTTON_RIGHT
}; 

enum LCD_BUTTON_EVENT_e {
  LCD_BUTTON_PRESS,
  LCD_BUTTON_RELEASE
}; 



class LCD_wrapper{
public:
    // Class constructor
    LCD_wrapper(multisensor_wrapper &multisensor, SPIClass &spi);
    bool init();

    void on_button_event(LCD_BUTTON_e button, LCD_BUTTON_EVENT_e event);
    
    void refresh();
private:
    multisensor_wrapper &multisensor;
    Adafruit_ST7735 tft;
    // Adafruit_miniTFTWing ss;
    LCD_Window_e cur_window;
    bool is_initialized;

    void display_WINDOW_MESSAGES();
    void display_WINDOW_RTC();
    void display_WINDOW_WIFI();
    void display_WINDOW_BME280();
    void display_WINDOW_ENS160();
    void display_WINDOW_AHT2x();
    void display_WINDOW_VEML7700();
    void display_WINDOW_DHT11();
    void display_WINDOW_LM35();
    void display_WINDOW_LDR();

    void display_full_refresh();
    void display_background();
    void display_header_footer();

    String LCD_Window_e_ToString(LCD_Window_e window) const;
    LCD_Window_e get_next_window(LCD_Window_e window) const;
    LCD_Window_e get_prev_window(LCD_Window_e window) const;
};