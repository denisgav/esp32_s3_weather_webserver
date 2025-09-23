#include "LCD_wrapper.h"

const char daysOfTheWeek_s[7][4] = {
    "SUN", "MON",
    "TUE", "WED",
    "THU", "FRI",
    "SAT"};

const char month_s[12][4] = {
    "JAN", "FEB",
    "MAR", "APR", "MAY",
    "JUN", "JUL", "AUG",
    "SEP", "OCT", "NOW",
    "DEC"};

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

void LCD_wrapper ::on_button_event(LCD_BUTTON_e button, LCD_BUTTON_EVENT_e event)
{
  switch (button)
  {
  case LCD_BUTTON_LEFT:
  {
    if (event == LCD_BUTTON_PRESS)
    {
      cur_window = (LCD_Window_e)(cur_window - 1);
      if (cur_window <= LCD_WINDOW_NONE)
      {
        cur_window = (LCD_Window_e)(LCD_WINDOW_TOTAL - 1);
      }
      display_full_refresh();
      refresh();
    }
    break;
  }
  case LCD_BUTTON:
  {
    break;
  }
  case LCD_BUTTON_RIGHT:
  {
    if (event == LCD_BUTTON_PRESS)
    {
      cur_window = (LCD_Window_e)(cur_window + 1);
      if (cur_window >= LCD_WINDOW_TOTAL)
      {
        cur_window = (LCD_Window_e)(LCD_WINDOW_NONE + 1);
      }

      display_full_refresh();
      refresh();
    }
    break;
  }
  }
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
  static bool is_data_sampled_prev = false;

  bool is_data_sampled = multisensor.get_rtc().get_is_sampled();

  if (is_data_sampled_prev != is_data_sampled)
  {
    display_full_refresh();
  }

  is_data_sampled_prev = is_data_sampled;

  const int TEXT_BLOCK_HEIGHT = SCREEN_HEIGHT - (FONT_HEIGHT + 2) * 2;

  // Get the current time from the RTC
  if (is_data_sampled)
  {
    DateTime now = multisensor.get_rtc().get_sampled_datetime();

    // Getting each time field in individual variables
    // And adding a leading zero when needed;
    String yearStr = String(now.year(), DEC);
    String monthStr = month_s[now.month() - 1];
    String dayStr = (now.day() < 10 ? "0" : "") + String(now.day(), DEC);
    String hourStr = (now.hour() < 10 ? "0" : "") + String(now.hour(), DEC);
    String minuteStr = (now.minute() < 10 ? "0" : "") + String(now.minute(), DEC);
    String secondStr = (now.second() < 10 ? "0" : "") + String(now.second(), DEC);
    String dayOfWeek = daysOfTheWeek_s[now.dayOfTheWeek()];

    // Complete time string
    String formattedDate = monthStr + " " + dayStr;
    String formattedTime = hourStr + ":" + minuteStr;
    String formattedTemperature = String(multisensor.get_rtc().get_sampled_temperature()) + " C";

    formattedDate = string_extend(formattedDate, 8, STRING_ALIGN_LEFT);
    formattedTime = string_extend(formattedTime, 8, STRING_ALIGN_LEFT);
    formattedTemperature = string_extend(formattedTemperature, 8, STRING_ALIGN_LEFT);

    tft.setTextSize(2);

    tft.setCursor(12, (FONT_HEIGHT + 2));
    tft.println(formattedDate);
    tft.setCursor(12, (FONT_HEIGHT + 2) + (TEXT_BLOCK_HEIGHT * 1) / 3);
    tft.println(formattedTime);
    tft.setCursor(12, (FONT_HEIGHT + 2) + (TEXT_BLOCK_HEIGHT * 2) / 3);
    tft.println(formattedTemperature);

    tft.setTextSize(1);
  }
  else
  {
    tft.setCursor(12, (FONT_HEIGHT + 2) * 1);
    tft.println("No data available");
  }
}

void LCD_wrapper ::display_WINDOW_WIFI()
{
}

void LCD_wrapper ::display_WINDOW_BME280()
{
  static bool is_data_sampled_prev = false;

  bool is_data_sampled = multisensor.get_bme280().get_is_sampled();

  if (is_data_sampled_prev != is_data_sampled)
  {
    display_full_refresh();
  }

  is_data_sampled_prev = is_data_sampled;

  const int TEXT_BLOCK_HEIGHT = SCREEN_HEIGHT - (FONT_HEIGHT + 2) * 2;

  // Get the current time from the RTC
  if (is_data_sampled)
  {
    String formattedTemperature = "T:" + String(multisensor.get_bme280().get_sampled_temperature()) + " C";
    String formattedHumidity = "H:" + String(multisensor.get_bme280().get_sampled_humidity()) + " %";
    String formattedPressure = "P:" + String(multisensor.get_bme280().get_sampled_pressure()) + "hPa";

    formattedTemperature = string_extend(formattedTemperature, 13, STRING_ALIGN_LEFT);
    formattedHumidity = string_extend(formattedHumidity, 13, STRING_ALIGN_LEFT);
    formattedPressure = string_extend(formattedPressure, 13, STRING_ALIGN_LEFT);

    tft.setTextSize(2);

    tft.setCursor(2, (FONT_HEIGHT + 2));
    tft.println(formattedTemperature);
    tft.setCursor(2, (FONT_HEIGHT + 2) + (TEXT_BLOCK_HEIGHT * 1) / 3);
    tft.println(formattedHumidity);
    tft.setCursor(2, (FONT_HEIGHT + 2) + (TEXT_BLOCK_HEIGHT * 2) / 3);
    tft.println(formattedPressure);

    tft.setTextSize(1);
  }
  else
  {
    tft.setCursor(12, (FONT_HEIGHT + 2) * 1);
    tft.println("No data available");
  }
}

void LCD_wrapper ::display_WINDOW_ENS160()
{
  static bool is_data_sampled_prev = false;

  bool is_data_sampled = multisensor.get_ens160().get_is_sampled();

  if (is_data_sampled_prev != is_data_sampled)
  {
    display_full_refresh();
  }

  is_data_sampled_prev = is_data_sampled;

  const int TEXT_BLOCK_HEIGHT = SCREEN_HEIGHT - (FONT_HEIGHT + 2) * 2;

  // Get the current time from the RTC
  if (is_data_sampled)
  {
    String formattedAQI = "AQI:" + String(multisensor.get_ens160().get_sampled_AQI());
    String formattedTVOC = "TVOC:" + String(multisensor.get_ens160().get_sampled_TVOC()) + "ppm";
    String formattedeCO2 = "eCO2:" + String(multisensor.get_ens160().get_sampled_eCO2()) + "ppb";

    formattedAQI = string_extend(formattedAQI, 13, STRING_ALIGN_LEFT);
    formattedTVOC = string_extend(formattedTVOC, 13, STRING_ALIGN_LEFT);
    formattedeCO2 = string_extend(formattedeCO2, 13, STRING_ALIGN_LEFT);

    tft.setTextSize(2);

    tft.setCursor(2, (FONT_HEIGHT + 2));
    tft.println(formattedAQI);
    tft.setCursor(2, (FONT_HEIGHT + 2) + (TEXT_BLOCK_HEIGHT * 1) / 3);
    tft.println(formattedTVOC);
    tft.setCursor(2, (FONT_HEIGHT + 2) + (TEXT_BLOCK_HEIGHT * 2) / 3);
    tft.println(formattedeCO2);

    tft.setTextSize(1);
  }
  else
  {
    tft.setCursor(12, (FONT_HEIGHT + 2) * 1);
    tft.println("No data available");
  }
}

void LCD_wrapper ::display_WINDOW_AHT2x()
{
  static bool is_data_sampled_prev = false;

  bool is_data_sampled = multisensor.get_aht2x().get_is_sampled();

  if (is_data_sampled_prev != is_data_sampled)
  {
    display_full_refresh();
  }

  is_data_sampled_prev = is_data_sampled;

  const int TEXT_BLOCK_HEIGHT = SCREEN_HEIGHT - (FONT_HEIGHT + 2) * 2;

  // Get the current time from the RTC
  if (is_data_sampled)
  {
    String formattedTemperature = "T:" + String(multisensor.get_aht2x().get_sampled_temperature()) + "C";
    String formattedHumidity = "H:" + String(multisensor.get_aht2x().get_sampled_humidity()) + "%";

    formattedTemperature = string_extend(formattedTemperature, 7, STRING_ALIGN_LEFT);
    formattedHumidity = string_extend(formattedHumidity, 7, STRING_ALIGN_LEFT);

    tft.setTextSize(3);

    tft.setCursor(2, (FONT_HEIGHT + 2));
    tft.println(formattedTemperature);
    tft.setCursor(2, (FONT_HEIGHT + 2) + (TEXT_BLOCK_HEIGHT * 1) / 2);
    tft.println(formattedHumidity);

    tft.setTextSize(1);
  }
  else
  {
    tft.setCursor(12, (FONT_HEIGHT + 2) * 1);
    tft.println("No data available");
  }
}

void LCD_wrapper ::display_WINDOW_VEML7700()
{
}

void LCD_wrapper ::display_WINDOW_DHT11()
{
  static bool is_data_sampled_prev = false;

  bool is_data_sampled = multisensor.get_dht11().get_is_sampled();

  if (is_data_sampled_prev != is_data_sampled)
  {
    display_full_refresh();
  }

  is_data_sampled_prev = is_data_sampled;

  const int TEXT_BLOCK_HEIGHT = SCREEN_HEIGHT - (FONT_HEIGHT + 2) * 2;

  // Get the current time from the RTC
  if (is_data_sampled)
  {
    String formattedTemperature = "T:" + String(multisensor.get_dht11().get_sampled_temperature()) + "C";
    String formattedHumidity = "H:" + String(multisensor.get_dht11().get_sampled_humidity()) + "%";

    formattedTemperature = string_extend(formattedTemperature, 7, STRING_ALIGN_LEFT);
    formattedHumidity = string_extend(formattedHumidity, 7, STRING_ALIGN_LEFT);

    tft.setTextSize(3);

    tft.setCursor(2, (FONT_HEIGHT + 2));
    tft.println(formattedTemperature);
    tft.setCursor(2, (FONT_HEIGHT + 2) + (TEXT_BLOCK_HEIGHT * 1) / 2);
    tft.println(formattedHumidity);

    tft.setTextSize(1);
  }
  else
  {
    tft.setCursor(12, (FONT_HEIGHT + 2) * 1);
    tft.println("No data available");
  }
}

void LCD_wrapper ::display_WINDOW_LM35()
{
  static bool is_data_sampled_prev = false;

  bool is_data_sampled = multisensor.get_lm35().get_is_sampled();

  if (is_data_sampled_prev != is_data_sampled)
  {
    display_full_refresh();
  }

  is_data_sampled_prev = is_data_sampled;

  const int TEXT_BLOCK_HEIGHT = SCREEN_HEIGHT - (FONT_HEIGHT + 2) * 2;

  // Get the current time from the RTC
  if (is_data_sampled)
  {
    String formattedTemperature = "T:" + String(multisensor.get_lm35().get_sampled_temperature()) + "C";

    formattedTemperature = string_extend(formattedTemperature, 7, STRING_ALIGN_LEFT);

    tft.setTextSize(3);

    tft.setCursor(2, (FONT_HEIGHT + 2) + (TEXT_BLOCK_HEIGHT * 1) / 3);
    tft.println(formattedTemperature);

    tft.setTextSize(1);
  }
  else
  {
    tft.setCursor(12, (FONT_HEIGHT + 2) * 1);
    tft.println("No data available");
  }
}

void LCD_wrapper ::display_WINDOW_LDR()
{
  static bool is_data_sampled_prev = false;

  bool is_data_sampled = multisensor.get_ldr().get_is_sampled();

  if (is_data_sampled_prev != is_data_sampled)
  {
    display_full_refresh();
  }

  is_data_sampled_prev = is_data_sampled;

  const int TEXT_BLOCK_HEIGHT = SCREEN_HEIGHT - (FONT_HEIGHT + 2) * 2;

  // Get the current time from the RTC
  if (is_data_sampled)
  {
    String formatted = String(multisensor.get_ldr().get_sampled_ldr());
    formatted = string_extend(formatted, 7, STRING_ALIGN_LEFT);

    tft.setTextSize(3);

    tft.setCursor(2, (FONT_HEIGHT + 2));
    tft.println(formatted);
    tft.setCursor(2, (FONT_HEIGHT + 2) + (TEXT_BLOCK_HEIGHT * 1) / 2);
    tft.println("mV");

    tft.setTextSize(1);
  }
  else
  {
    tft.setCursor(12, (FONT_HEIGHT + 2) * 1);
    tft.println("No data available");
  }
}

void LCD_wrapper ::display_full_refresh()
{
  display_background();
  display_header_footer();
}

void LCD_wrapper ::display_background()
{
  tft.fillScreen(ST77XX_BLACK);

  tft.fillRect(0, 0, SCREEN_WIDTH, FONT_HEIGHT + 2, ST77XX_ORANGE);
  tft.fillRect(0, FONT_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - FONT_HEIGHT - FONT_HEIGHT, ST77XX_BLACK);
  tft.fillRect(0, SCREEN_HEIGHT - FONT_HEIGHT, SCREEN_WIDTH, FONT_HEIGHT + 2, ST77XX_ORANGE);
}

void LCD_wrapper ::display_header_footer()
{
  String caption_s = LCD_Window_e_ToString(cur_window);
  String footer_s = "<--" + LCD_Window_e_ToString(get_prev_window(cur_window)) + " | " + LCD_Window_e_ToString(get_next_window(cur_window)) + " -->";

  tft.setTextColor(ST77XX_BLACK, ST77XX_ORANGE);
  tft.setCursor(SCREEN_WIDTH / 2 - (caption_s.length() / 2) * FONT_WIDTH, (FONT_HEIGHT + 2) * 0 + 2);
  tft.println(caption_s);

  tft.setCursor(SCREEN_WIDTH / 2 - (footer_s.length() / 2) * FONT_WIDTH, SCREEN_HEIGHT - FONT_HEIGHT + 2);
  tft.println(footer_s);

  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
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