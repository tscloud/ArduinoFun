/*
 *  ESP8266 NTP Clock Program
 *
 *  This hardware/software combination implements a digital clock that
 *  never needs setting as it gets the current time and date by polling
 *  Network Time Protocol (NTP) servers on the Internet. The clock's time
 *  is synchronized to NTP time every 5 minutes.
 *
 *  The hardware consists of the following parts:
 *    NodeMCU Amica R2 from Electrodragon
 *    1.8" SPI LCD Color Display from Adafruit - Product ID: 358
 *    Pushbutton switch
 *
 *  The connections between the NodeMCU board the the display are
 *  as follows:
 *
 *  ESP8266 Pin  LCD Display Pin       Function
 *  --------------------------------------------------------------------
 *      D3            Lite       Backlight light LED
 *                    MISO       Master In Slave Out (unused)
 *      D5            CLK        SPI Clock
 *      D7            MOSI       Master Out Slave In (data from Esp8266->display)
 *      D8            TFT_CS     Display chip select
 *                    SD_CS      SD card chip select (unused)
 *      D4            D/C        Data/Command toggle
 *                    Reset      Display controller reset (unused)
 *      3V3           Vcc        Power for the display
 *      GND           Gnd        Ground for the display
 *
 *  The Daylight Savings Time (DST) pushbutton switch is connected between
 *  GND and D1 (GPIO5). Every press of this push button toggles clock
 *  between DST and non DST modes. DST status is saved in EEPROM so
 *  it survives power outages.
 *
 *  Concept, Design and Implementation by: Craig A. Lindley
 *  Last Update: 10/20/2015
 */

#include <Adafruit_GFX.h>
#include <EEPROM.h>
#include <SPI.h>
#include <Time.h>
#include <ESP8266WiFi.h>

#include "ESP8266_ST7735.h"
#include "TextGraphicsFunctions.h"
#include "Misc.h"
#include "NTP.h"
#include "mqtt.h"

// ***************************************************************
// Start of user configuration items
// ***************************************************************

// Set your WiFi login credentials
#define WIFI_SSID "gosox"
#define WIFI_PASS "15courthouselane"

#define TIMEZONE_OFFSET -5    // Set your timezone offset (-7 is mountain time)
#define USE_DST         true  // Set to false to disable DST mode
#define HOUR_FORMAT_12  false  // Set to false for 24 hour time mode

// ***************************************************************
// End of user configuration items
// ***************************************************************

#define TITLE_STRING "Esp8266 NTP Clock"
#define DST_STRING   "DST"

// Connections between NodeMCU module and Adafruit 1.8" LCD display
#define TFT_CS   5
#define TFT_DC   4
#define TFT_LITE -1

// Create LCD driver instance
ESP8266_ST7735 lcd = ESP8266_ST7735(TFT_CS, TFT_DC);

// Connection for DST pushbutton
#define DST_SW   5

// Various variables
time_t previousMinute = 0;

#if (USE_DST == true)
bool dstEnabled;
#endif

/*******************************************************************/
/***                     Support Functions                       ***/
/*******************************************************************/

#if (USE_DST == true)
// Toggle the DST enabled flag
void toggleDSTEnabled() {
  // Toggle the enabled indicator
  dstEnabled = (dstEnabled == true) ? false : true;

  // Store the enabled indicator into the EEPROM
  // so it survives a power outage
  setDSTIndicator(dstEnabled);

  // Inform the NTP code
  dstChanged(dstEnabled);
}

// Get the DST string
const char *getDSTStr() {
  if (dstEnabled) {
    return DST_STRING;
  } else {
    return "";
  }
}
#else
// DST string empty
const char *getDSTStr() {
  return "";
}
#endif

// Get the day of the week string
const char *getDWStr() {
  static char buffer[20];
  strcpy(buffer, dayShortStr(weekday()));
  return buffer;
}

// Build and return the date string
const char *getDateStr() {
  static char buffer[20];
  sprintf(buffer, "%s %d, %d", monthShortStr(month()), day(), year());
  return buffer;
}

// Build and return the time string
const char *getTimeStr() {
  int hours, minutes;
  char ampmBuffer[5];
  static char buffer[20];

  minutes = minute();

#if (HOUR_FORMAT_12 == true)
  hours = hourFormat12();
  if (isPM()) {
    strcpy(ampmBuffer, "PM");
  } else  {
    strcpy(ampmBuffer, "AM");
  }
  sprintf(buffer, "%d:%02d %s", hours, minutes, ampmBuffer);
#else
  hours = hour();
  sprintf(buffer, "%d:%02d", hours, minutes);
#endif
  return buffer;
}

// Determine day time or night time for proper icon selection
// Do comparison in 24 hour format only
// I define daytime roughly as the hours between:
//   12 hour time: 6:00AM till 8:00PM
//   24 hour time: 6 till 20
bool isDayTime() {
  int hours = hour();
  if ((hours >= 6) && (hours < 20)) {
    return true;
  } else  {
    return false;
  }
}

/*******************************************************************/
/***                     Main Program Setup                     ***/
/*******************************************************************/

void setup() {

  // Backlight off
  pinMode(TFT_LITE, OUTPUT);
  digitalWrite(TFT_LITE, LOW);

  Serial.begin(115200);
  delay(1000);

#if (USE_DST == true)
  // Setup DST pushbutton switch
  pinMode(DST_SW, INPUT_PULLUP);

  dstEnabled = getDSTIndicator();
#endif

  // Initialize the LCD (blacktab) display
  lcd.init();

  // Set display orientation
  lcd.setRotation(3);

  // Clear the display
  clearDisplay();

  // Draw the frame around the display
  drawDisplayFrame();

  // Draw credits on display
  drawCenteredText(10, 1, TITLE_STRING, COLOR_BLACK);
  drawCenteredText(25, 1, "Craig A. Lindley",  COLOR_BLACK);

  // Display the WiFi logo icon
  drawWiFiIcon(COLOR_BLACK);

  // Backlight on
  digitalWrite(TFT_LITE, HIGH);

  // Attempt logon to WiFi network and set timezone
  initNTP(WIFI_SSID, WIFI_PASS, TIMEZONE_OFFSET);

  // Do MQTT setup
  mqtt_setup();

#if (USE_DST == true)
  // Inform the NTP code of DST status
  dstChanged(dstEnabled);
  if (dstEnabled) {
    // DST is enabled so adjust time for immediate display
    adjustTime(SECS_PER_HOUR);
  }
#endif

  // Clear the display
  clearDisplay();
}

/*******************************************************************/
/***                      Main Program Loop                      ***/
/*******************************************************************/

void loop() {

  // Do MQTT stuff we need to do in the loop()
  mqtt_loop();

  if (timeStatus() != timeNotSet) {
    if (minute() != previousMinute) { //Update the display only if time has changed
      previousMinute = minute();

      // Draw the updated clock face
      //drawClockFace(getDWStr(), getDSTStr(), getDateStr(), getTimeStr(), TITLE_STRING, isDayTime());
      drawClockFace(getTemp(), getDSTStr(), getDateStr(), getTimeStr(), TITLE_STRING, isDayTime());
    }
  }
  // Has the DST switch been pressed ?
  if (digitalRead(DST_SW) == LOW) {
    // Yes it has
    if (dstEnabled) {
      // DST was enabled and will now be disabled
      // Adjust time for immediate display
      adjustTime(-SECS_PER_HOUR);

    } else  {
      // DST was disabled and will now be enabled
      // Adjust time for immediate display
      adjustTime(+SECS_PER_HOUR);
    }
    // Toggle the DST enabled bit and inform NTP code for next time sync
    toggleDSTEnabled();

    // Draw the updated clock face
    drawClockFace(getDWStr(), getDSTStr(), getDateStr(), getTimeStr(), TITLE_STRING, isDayTime());
  }
  delay(1000);
}

