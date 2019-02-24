#include <NTPClient.h>
// change next line to use with another board/shield
//#include <ESP8266WiFi.h>
#include <ESP8266WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include "Free_Fonts.h" // Include the header file attached to this sketch
// MQTT header to get weather data
#include "mqtt.h"


const char *ssid     = "gosox";
const char *password = "15courthouselane";

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "us.pool.ntp.org", -18000, 60000);

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

int checkMin = -1;

void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();

  // TFT setup
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  // Do MQTT setup
  mqtt_setup();
}

void loop() {
  timeClient.update();

  //Serial.println(timeClient.getFormattedTime());

  // check NTP and potentially update display
  getTime();

  // Do MQTT stuff we need to do in the loop()
  mqtt_loop();

  delay(1000);
}

// return time to display
void getTime() {
  int ntpMinutes = timeClient.getMinutes();

  if (ntpMinutes != checkMin) {
    checkMin = ntpMinutes;

    int ntpHours = timeClient.getHours();
    String strHours = String(ntpHours);
    String strMinutes = String(ntpMinutes);

    if (ntpHours < 10) {
      strHours = "0"+strHours;
    }
    if (ntpMinutes < 10) {
      strMinutes = "0"+strMinutes;
    }

    String timeString = strHours + ":" + strMinutes;

    // --draw time--
    //tft.drawCentreString(timeClient.getFormattedTime(), 80, 54, 4); // Draw text centre at position 80, 24 using font 6
    tft.setTextSize(1);;
    tft.setTextDatum(BC_DATUM); // Centre text on x,y position
    tft.drawString(timeString, 160, 54, 8);  // Draw the text string in the selected GFX free font

    // --draw weather--
    tft.setFreeFont(FSS18);                              // Select the font
    tft.setTextSize(1);
    tft.setTextDatum(TC_DATUM); // Centre text on x,y position
    tft.drawString(getTemp(), 160, 150, GFXFF);  // Draw the text string in the selected GFX free font
    //tft.drawString(getTemp(), 160, 150, 4);  // Draw the text string in the selected GFX free font

  }

 }
