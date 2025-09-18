#include "LCD_wrapper.h"

// Class constructor
LCD_wrapper ::LCD_wrapper(multisensor_wrapper &multisensor, SPIClass &spi) : multisensor(multisensor),
                                                                             tft(&spi, TFT_CS_PIN, TFT_DC_PIN, TFT_RES_PIN),
                                                                             is_initialized(false)
{
}
bool LCD_wrapper ::init() {
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
  is_initialized = true;

  return is_initialized;
}

void LCD_wrapper ::on_left_button_press() {}
void LCD_wrapper ::on_user_button_press() {}
void LCD_wrapper ::on_right_button_press() {}
void LCD_wrapper ::loop() {}