/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// --display
#include <ESP_SSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <Adafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)
//#include <Fonts/FreeMono9pt7b.h>

// --EEPROM
#include <EEPROM.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// --display
// Pin definitions
#define OLED_RESET  16  // Pin 15 -RESET digital signal

// digital pin
#define LED_PIN 15
#define DISPLAY_TOGGLE_PIN 12
#define SET_BUTTON_UP_PIN 13
#define SET_BUTTON_DOWN_PIN 14

// sensor
Adafruit_BME280 bme; // I2C

unsigned const long delayTime = 1000;

// used for blanking out screen after waiting for so long
int blankoutCounter = 0;
const int blankoutThresh = 3;

// used to determine which type of sensor data to display
volatile int interruptDisplayInd = 0;

// used for conversion of sensor value to string for centering
char convBuffer[64];

// used for debounce
const long debouncing_time = 300; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;

// used for wifi
const char* ssid     = "gopats";
const char* password = "15courthouselane";

// value (temp) & min/max <- use byte  - we're going to store trackedValue in EEPROM
const byte valueMin = 50;
const byte valueMax = 80;
const byte valueThreth = 2;
byte trackedValue = 65; //arbitrary default
bool displayTrackedValue = false;

ESP8266WebServer server(80);

String webString = "";    // String to display

// --TEST
int16_t xx1, yy1;
uint16_t ww, hh;


// --display
ESP_SSD1306 display(OLED_RESET); // FOR I2C

// --EEPROM
int addr = 0;

void setup() {

    EEPROM.begin(1);
    byte readValue = EEPROM.read(addr);

    //if something weird read from EEPROM -> set trackedValue to something sane
    if ((readValue >= valueMin) && (readValue <= valueMax)) {
      trackedValue = readValue;
    }

    Serial.begin(115200);
    Serial.println(F("BME280 test"));

    // do our wifi thing inc. network connect, set hostname, disp html
    wifiSetup();

    // setup button interrupt
    pinMode(DISPLAY_TOGGLE_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(DISPLAY_TOGGLE_PIN), handleInterruptDisplay, FALLING);
    pinMode(SET_BUTTON_UP_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SET_BUTTON_UP_PIN), handleInterruptValUp, FALLING);
    pinMode(SET_BUTTON_DOWN_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SET_BUTTON_DOWN_PIN), handleInterruptValDown, FALLING);

    // setup LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    bool status;

    // default settings
    status = bme.begin();
    if (!status) {
        Serial.println(F("Could not find a valid BME280 sensor, check wiring!"));
        while (1);
    }

    Serial.println(F("-- Default Test --"));
    //delayTime = 1000;

    // --display
    display.begin(SSD1306_SWITCHCAPVCC);  // Switch OLED

    // SSD1306 Init - this has to be done here or display will not output
    display.clearDisplay();
    //display.setFont(&FreeMono9pt7b);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(F("Hello, world!"));
    display.display();
    delay(2000);
    display.clearDisplay();
    display.display();
}

void loop() {

    //--TEST LED
    //digitalWrite(LED_PIN, !digitalRead(LED_PIN));

    // flip LED ON/OFF determined by temp & trackedValue
    // if the heat is a on -> don't turn off until we get to threthhold above temp
    if (digitalRead(LED_PIN) == HIGH) {
      if (tempToF(bme.readTemperature()) > float(trackedValue + valueThreth)) {
        Serial.println(F("--TURN OFF--"));
        digitalWrite(LED_PIN, LOW);
      }
    }
    else {
      if (tempToF(bme.readTemperature()) < float(trackedValue)) {
        Serial.println(F("--TURN ON--"));
        digitalWrite(LED_PIN, HIGH);
      }
    }

    // for webserver
    server.handleClient();

    //maybe display tracked value (Temp)
    if (displayTrackedValue) {
      displayTrackedValue = false;
      displayData("Termo Set", trackedValue);
    }
    //maybe blankout the screen
    else if (blankoutCounter >= blankoutThresh){
      //reset the blankoutCounter so we don't exceed int capacity
      blankoutCounter = blankoutThresh;
      displayData("", -1);
    }
    //display some data (or maybe blank out screen)
    else {
      switch (interruptDisplayInd) {
        case 0:
          displayData("Temperature", tempToF(bme.readTemperature()));
          break;
        case 1:
          displayData("Pressure", pressToMBar(bme.readPressure()));
          break;
        case 2:
          displayData("Humidity", bme.readHumidity());
          break;
        case 3:
          displayData("Termo Set", trackedValue);
          break;
        case 4:
          displayData("", -1);
          break;
        default:
          Serial.print("Unhandled interrupt value: ");
          Serial.println(interruptDisplayInd);
          break;
      }
    }

    blankoutCounter++;

    delay(delayTime);
}

// convert temperature
float tempToF(float tempC) {
  return (tempC * 9.0F)/5.0F + 32.0F;
}

// convert pressure
float pressToMBar(float pressure) {
  return (pressure / 100.0F);
}

void handleInterruptDisplay() {
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    // if we pass debounce -> do our thing unless we're coming from a blanked screen
    if (blankoutCounter < blankoutThresh) {
      if(interruptDisplayInd >= 4) {
        interruptDisplayInd = 0;
      }
      else {
        interruptDisplayInd++;
      }
      Serial.print("handleInterruptDisplay received - value now: ");
      Serial.println(interruptDisplayInd);
    }

    last_micros = micros();

    // reset blankoutCounter
    blankoutCounter = 0;
  }
}

void handleInterruptValDown() {
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    // if we pass debounce -> do our thing
    Serial.println("handleInterruptValDown received");
    if (trackedValue <= valueMin) {
      Serial.println("min value already reached");
    }
    else {
      trackedValue--;
      EEPROM.write(addr, trackedValue);
      EEPROM.commit();
    }
    Serial.print("trackedValue: ");
    Serial.println(trackedValue);

    last_micros = micros();

    // display termo set on next loop iteration
    displayTrackedValue = true;
    // reset blankoutCounter
    blankoutCounter = 0;
  }
}

void handleInterruptValUp() {
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    // if we pass debounce -> do our thing
    Serial.println("handleInterruptValUp received");
    if (trackedValue >= valueMax) {
      Serial.println("max value already reached");
    }
    else {
      trackedValue++;
      EEPROM.write(addr, trackedValue);
      EEPROM.commit();
    }
    Serial.print("trackedValue: ");
    Serial.println(trackedValue);

    last_micros = micros();

    // display termo set on next loop iteration
    displayTrackedValue = true;
    // reset blankoutCounter
    blankoutCounter = 0;
  }
}

/*
void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}
*/

void displayData(char *title, float value) {
    // -- display
    // 1st determine if we simply want to blank screen
    if (value < 0) {
      display.clearDisplay();
      display.display();
    }
    else {
      // Get the size of value to be displayed
      //snprintf(convBuffer, 64, "%f", value);
      itoa(value, convBuffer, 10);

      // -title
      display.clearDisplay();
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.setTextSize(1);
      display.println(title);

      // -value
      display.setTextSize(2);
      display.getTextBounds(convBuffer, 0, 0, &xx1, &yy1, &ww, &hh);

      // -- TEST
      //Serial.print("Value :");  Serial.println(value);
      //Serial.print("xx1:");  Serial.println(xx1);
      //Serial.print("yy1:");  Serial.println(yy1);
      //Serial.print("ww :");  Serial.println(ww);
      //Serial.print("hh :");  Serial.println(hh);
      //Serial.print("pad :");  Serial.println((display.width()-ww)/2);
      //Serial.print("screen width :");  Serial.println(display.width());

      // don't know why I need to sub an extra 35 to center text!!
      display.setCursor((display.width()-ww-35)/2,16);
      display.println(value);
      display.display();
      //display.clearDisplay();
    }
}

void wifiSetup() {

  // set hostname
  WiFi.hostname(F("giniger"));

  // Setup as softAP
  /*
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");
  */

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print(F("\n\r \n\rWorking to connect"));

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(F("BME280 Environmental Display Server"));
  Serial.print(F("Connected to "));
  Serial.println(ssid);
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());

  // Set up mDNS responder:
  //if (!MDNS.begin("giniger")) {
  //  Serial.println("Error setting up MDNS responder!");
  //  while (1) {
  //    delay(1000);
  //  }
  //}
  //Serial.println("mDNS responder started");

  server.on("/", []() {
    webString = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\r\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\r\
<head>\r\
<title>BME280 Environmental Display Server</title>\r\
<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\r\
<meta http-equiv=\"Cache-Control\" content=\"no-cache, no-store, must-revalidate\" />\r\
<meta http-equiv=\"refresh\" content=\"20\" />\r\
<meta name=\"viewport\" content=\"user-scalable=no, initial-scale=1.0, maximum-scale=1.0, width=device-width\" />\r\
<style type=\"text/css\">\r\
body { margin-top:75px; background-color:#ffffff; width:100%; }\r\
h1 { text-align:center; font:normal 24px/0px helveticaneue-bold,sans-serif; color:#666666; text-transform:uppercase; letter-spacing:0.4em; }\r\
h2 { text-align:center; font:normal 48px/0px helveticaneue-ultralight,sans-serif; color:#000000; }\r\
</style>\r\
</head>\r\
<body>\r\
<h1>Temperature</h1>\r<h2>" + String(tempToF(bme.readTemperature()), 2) + "&deg;F</h2><br />" + "\r\
<h1>Humidity</h1>\r<h2>" + String(bme.readHumidity(), 2) + "%</h2><br />" + "\r\
<h1>Pressure</h1>\r<h2>" + String(pressToMBar(bme.readPressure()), 2) + " mBar</h2>\r\
</body>\r\
</html>";
    server.send(200, "text/html", webString); // send to someone's browser when asked
  });

  server.begin();
  Serial.println(F("HTTP server started"));

  // Add service to MDNS-SD
  //MDNS.addService("http", "tcp", 80);

}
