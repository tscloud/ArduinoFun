/*
 *  TextGraphicsFunctions.h - Misc helper functions
 *
 *  Concept, Design and Implementation by: Craig A. Lindley
 *  Last Update: 10/20/2015
 */

#ifndef TEXTGRAPHICSFUNCTIONS_H
#define TEXTGRAPHICSFUNCTIONS_H

extern ESP8266_ST7735 lcd;

#include "Icons.h"

// Clear the display
void clearDisplay() {
  // Set all pixels to white
  lcd.fillScreen(COLOR_WHITE);
}

// Get the width in pixels of a text string of specified size
int getTextWidth(const char *text, int size) {

  int len = strlen(text);

  // Chars are 5 px wide with 1 px space = 6
  return 6 * len * size;
}

// Calculate the x offset necessary to center a text string of specified size
int xOffsetForCenteredText(const char *text, int size) {
  return (lcd.width() - getTextWidth(text, size)) / 2;
}

// Draw a text string at specified location with specified size and color
void drawText(int x, int y, int size, const char *text, uint16_t color) {
  lcd.setCursor(x, y);
  lcd.setTextSize(size);
  lcd.setTextColor(color);
  lcd.print(text);
}

// Draw a text string of specified size and color centered on display with y position
void drawCenteredText(int y, int size, const char *text, uint16_t color) {
  drawText(xOffsetForCenteredText(text, size), y, size, text, color);
}

// Draw WiFi Icon centered on display
void drawWiFiIcon(uint16_t color) {
  // Calculate position of icon
  int xOffset = (lcd.width()  - WIFIICON_WIDTH)  / 2;
  int yOffset = (lcd.height() - WIFIICON_HEIGHT) / 2;

  lcd.drawXBitmap(xOffset, yOffset, WiFiIcon, WIFIICON_WIDTH, WIFIICON_HEIGHT, color);
}

// Draw display frame
void drawDisplayFrame() {
  // Draw frame around display
  lcd.drawRect(0, 0, lcd.width(),      lcd.height(),     COLOR_BLUE);
  lcd.drawRect(1, 1, lcd.width() - 2 , lcd.height() - 2, COLOR_RED);
  lcd.drawRect(2, 2, lcd.width() - 4 , lcd.height() - 4, COLOR_BLUE);
}

// Draw clock face
void drawClockFace(const char *dwStr,   const char *dstStr,   const char *dateStr,
                   const char *timeStr, const char *titleStr, bool isDaytime) {
  // Clear display
  clearDisplay();

  // Draw display frame
  drawDisplayFrame();

  // Draw centered day of week string
  drawCenteredText(18, 2, dwStr, COLOR_RED);

  // Conditionally display daylight saving time indicator
  if ((dstStr != NULL) && (strlen(dstStr) != 0)) {
    drawText(117, 23, 1, dstStr, COLOR_BLACK);
  }
  // Draw centered date string
  //drawCenteredText(54, 2, dateStr, COLOR_RED);

  // Draw centered time string
  drawCenteredText(65, 5, timeStr, COLOR_RED);

  // Draw centered title string
  drawCenteredText(116, 1, titleStr, COLOR_BLUE);

  // Draw an Icon
  /*
  if (isDaytime) {
    lcd.drawXBitmap(0, 0, sunIcon,  ICON_WIDTH, ICON_HEIGHT, COLOR_BLACK);
  } else  {
    lcd.drawXBitmap(0, 0, moonIcon, ICON_WIDTH, ICON_HEIGHT, COLOR_BLACK);
  }
  */
}

#endif

