#include "LCD_wrapper.h"

const char daysOfTheWeek[7][12] = {
    "Sunday", "Monday",
    "Tuesday", "Wednesday",
    "Thursday", "Friday",
    "Saturday"};

const int SCREEN_WIDTH = 160;
const int SCREEN_HEIGHT = 80;
const int FONT_WIDTH = 6;
const int FONT_HEIGHT = 10;

// Class constructor
LCD_wrapper ::LCD_wrapper(multisensor_wrapper &multisensor, SPIClass &spi) : multisensor(multisensor),
                                                                             tft(&spi, TFT_CS_PIN, TFT_DC_PIN, TFT_RES_PIN),
                                                                             cur_window(LCD_WINDOW_RTC),
                                                                             is_initialized(false)
{
}
bool LCD_wrapper ::init()
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
  is_initialized = true;

  tft.setTextSize(1);

  return is_initialized;
}

void LCD_wrapper ::on_left_button_press()
{
  cur_window = (LCD_Window_e)(cur_window - 1);
  if (cur_window <= LCD_WINDOW_NONE)
  {
    cur_window = (LCD_Window_e)(LCD_WINDOW_TOTAL - 1);
  }
  tft.fillScreen(ST77XX_BLACK);
  refresh();
}

void LCD_wrapper ::on_user_button_press()
{
}

void LCD_wrapper ::on_right_button_press()
{
  cur_window = (LCD_Window_e)(cur_window + 1);
  if (cur_window >= LCD_WINDOW_TOTAL)
  {
    cur_window = (LCD_Window_e)(LCD_WINDOW_NONE + 1);
  }

  tft.fillScreen(ST77XX_BLACK);
  refresh();
}

void LCD_wrapper ::refresh()
{
  switch (cur_window)
  {
  case LCD_WINDOW_MESSAGES:
  {
    display_WINDOW_MESSAGES();
    break;
  }
  case LCD_WINDOW_RTC:
  {
    display_WINDOW_RTC();
    break;
  }
  case LCD_WINDOW_WIFI:
  {
    display_WINDOW_WIFI();
    break;
  }
  case LCD_WINDOW_BME280:
  {
    display_WINDOW_BME280();
    break;
  }
  case LCD_WINDOW_ENS160:
  {
    display_WINDOW_ENS160();
    break;
  }
  case LCD_WINDOW_AHT2x:
  {
    display_WINDOW_AHT2x();
    break;
  }
  case LCD_WINDOW_VEML7700:
  {
    display_WINDOW_VEML7700();
    break;
  }
  case LCD_WINDOW_DHT11:
  {
    display_WINDOW_DHT11();
    break;
  }
  case LCD_WINDOW_LM35:
  {
    display_WINDOW_LM35();
    break;
  }
  case LCD_WINDOW_LDR:
  {
    display_WINDOW_LDR();
    break;
  }
  }
}

void LCD_wrapper ::display_WINDOW_MESSAGES()
{
}

void LCD_wrapper ::display_WINDOW_RTC()
{
  // tft.fillScreen(ST77XX_BLACK);

  String caption_s = "RTC (DS3231)";
  String footer_s = "<--" + LCD_Window_e_ToString(get_prev_window(cur_window)) + " | " + LCD_Window_e_ToString(get_next_window(cur_window)) + " -->";

  
  tft.fillRect(0, 0, SCREEN_WIDTH, FONT_HEIGHT+2, ST77XX_ORANGE);

  tft.setTextColor(ST77XX_BLACK, ST77XX_ORANGE);
  tft.setCursor(SCREEN_WIDTH / 2 - (caption_s.length() / 2) * FONT_WIDTH, (FONT_HEIGHT + 2) * 0 + 2);
  tft.println("RTC (DS3231)");

  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);

  tft.fillRect(0, FONT_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - FONT_HEIGHT - FONT_HEIGHT, ST77XX_BLACK);

  // Get the current time from the RTC
  if (multisensor.get_rtc().get_is_sampled())
  {
    DateTime now = multisensor.get_rtc().get_sampled_datetime();

    // Getting each time field in individual variables
    // And adding a leading zero when needed;
    String yearStr = String(now.year(), DEC);
    String monthStr = (now.month() < 10 ? "0" : "") + String(now.month(), DEC);
    String dayStr = (now.day() < 10 ? "0" : "") + String(now.day(), DEC);
    String hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC);
    String minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
    String secondStr = (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
    String dayOfWeek = daysOfTheWeek[now.dayOfTheWeek()];

    // Complete time string
    String formattedDate = yearStr + "-" + monthStr + "-" + dayStr;
    String formattedTime = hourStr + ":" + minuteStr + ":" + secondStr;
    String formattedTemperature = String(multisensor.get_rtc().get_sampled_temperature()) + " C";

    tft.setCursor(12, (FONT_HEIGHT+ 2) * 1);
    tft.println(dayOfWeek);
    tft.setCursor(12, (FONT_HEIGHT+ 2) * 2);
    tft.println(formattedDate);
    tft.setCursor(12, (FONT_HEIGHT+ 2) * 3);
    tft.println(formattedTime);
    tft.setCursor(12, (FONT_HEIGHT+ 2) * 4);
    tft.println(formattedTemperature);
  }
  else
  {
    tft.setCursor(12, (FONT_HEIGHT + 2) * 1);
    tft.println("No data available");
  }

  tft.fillRect(0, SCREEN_HEIGHT - FONT_HEIGHT, SCREEN_WIDTH, FONT_HEIGHT+2, ST77XX_ORANGE);

  tft.setTextColor(ST77XX_BLACK, ST77XX_ORANGE);
  tft.setCursor(SCREEN_WIDTH / 2 - (footer_s.length() / 2) * FONT_WIDTH, SCREEN_HEIGHT - FONT_HEIGHT+2);
  tft.println(footer_s);
}

void LCD_wrapper ::display_WINDOW_WIFI()
{
}

void LCD_wrapper ::display_WINDOW_BME280()
{
}

void LCD_wrapper ::display_WINDOW_ENS160()
{
}

void LCD_wrapper ::display_WINDOW_AHT2x()
{
}

void LCD_wrapper ::display_WINDOW_VEML7700()
{
}

void LCD_wrapper ::display_WINDOW_DHT11()
{
}

void LCD_wrapper ::display_WINDOW_LM35()
{
}

void LCD_wrapper ::display_WINDOW_LDR()
{
}

String LCD_wrapper ::LCD_Window_e_ToString(LCD_Window_e window) const
{
  switch (window)
  {
  case LCD_WINDOW_MESSAGES:
    return "MESSAGES";
  case LCD_WINDOW_WIFI:
    return "WIFI";
  case LCD_WINDOW_RTC:
    return "RTC";
  case LCD_WINDOW_BME280:
    return "BME280";
  case LCD_WINDOW_ENS160:
    return "ENS160";
  case LCD_WINDOW_AHT2x:
    return "AHT2x";
  case LCD_WINDOW_VEML7700:
    return "VEML7700";
  case LCD_WINDOW_DHT11:
    return "DHT11";
  case LCD_WINDOW_LM35:
    return "LM35";
  case LCD_WINDOW_LDR:
    return "LDR";
  default:
    return "UNKNOWN";
  }
  return "UNKNOWN";
}

LCD_Window_e LCD_wrapper ::get_next_window(LCD_Window_e window) const
{
  LCD_Window_e res = (LCD_Window_e)(window + 1);
  if (res >= LCD_WINDOW_TOTAL)
  {
    res = (LCD_Window_e)(LCD_WINDOW_NONE + 1);
  }
  return res;
}

LCD_Window_e LCD_wrapper ::get_prev_window(LCD_Window_e window) const
{
  LCD_Window_e res = (LCD_Window_e)(window - 1);
  if (res <= LCD_WINDOW_NONE)
  {
    res = (LCD_Window_e)(LCD_WINDOW_TOTAL - 1);
  }
  return res;
}