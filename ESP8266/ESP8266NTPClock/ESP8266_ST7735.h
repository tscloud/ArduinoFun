/*
 ********************************************************************
  This is a library for the Adafruit 1.8" SPI display.

This library works with the Adafruit 1.8" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/358
The 1.8" TFT shield
  ----> https://www.adafruit.com/product/802
The 1.44" TFT breakout
  ----> https://www.adafruit.com/product/2088
as well as Adafruit raw 1.8" TFT display
  ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution

  Modified by Craig A. Lindley to support only the 1.8" LCD display
  running on the ESP8266 NodeMCU Amica.
  ********************************************************************
 */

#ifndef ESP8266_ST7735_H
#define ESP8266_ST7735_H

#include "Adafruit_GFX.h"

// LCD display dimensions
#define TFT_WIDTH  128
#define TFT_HEIGHT 160

// Color definitions
#define COLOR_BLACK   0x0000
#define COLOR_BLUE    0x001F
#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_CYAN    0x07FF
#define COLOR_MAGENTA 0xF81F
#define COLOR_YELLOW  0xFFE0
#define COLOR_WHITE   0xFFFF

class ESP8266_ST7735 : public Adafruit_GFX {

  public:
    ESP8266_ST7735(int8_t CS, int8_t RS, int8_t RST = 0);

    void init(void);
    void setAddrWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
    void fillScreen(uint16_t color);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void setRotation(uint8_t r);
    void invertDisplay(boolean i);
    uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);

  private:
    void writecommand(uint8_t c);
    void writedata(uint8_t d);
    void writedata16(uint16_t d);
    void writeColor(uint16_t color, uint16_t count);
    void setCS(bool level);
    void setRS(bool level);
    void commandList(const uint8_t *addr);

    uint8_t _cs, _rs, _rst;
    uint8_t colstart, rowstart;
};

#endif

