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
//#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <WebServer.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// used for wifi
const char* ssid     = "gopats";
const char* password = "15courthouselane";
const char* myhostname = "mallory";

WebServer server(80);

String webString = "";    // String to display

unsigned long delayTime;

void setup() {
    Serial.begin(115200);
    Serial.println(F("BME280 test"));

    // do our wifi thing inc. network connect, set hostname, disp html
    wifiSetup();

    bool status;

    // default settings
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();
}


void loop() {
    // for webserver
    server.handleClient();

    printValues();
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

void wifiSetup() {

  // Setup as softAP
  /*
  Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");
  */

  // what we are
  WiFi.mode(WIFI_STA);
  //WiFi.mode(WIFI_AP_STA);
  //WiFi.softAP(myhostname);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print(F("\n\r \n\rWorking to connect"));

  // set hostname
  //WiFi.setHostname(F("giniger"));
  WiFi.setHostname(myhostname);

  // Wait for connection
  /*
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  */

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // print out some stuff about local network
  Serial.println(F("BME280 Environmental Display Server"));
  Serial.print(F("Connected to "));
  Serial.println(ssid);
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("hostname: "));
  Serial.println(WiFi.getHostname());

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
}