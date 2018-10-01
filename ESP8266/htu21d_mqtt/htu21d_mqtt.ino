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
#include <Adafruit_HTU21DF.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_HTU21DF htu = Adafruit_HTU21DF(); // I2C

// used for wifi & MQTT
const char* ssid     = "gopats";
const char* password = "15courthouselane";
const char* myhostname = "mallory";
const char* mqtt_server = "bigasspi";
const char* mqtt_channel = "/test/htu21d";

char result[19]; // T[+/-]xx.x,Hyy.y,Pzz.z <-- greatest length: 19
char loctemp char[6] // [+/-]xx.x
char lochum char[5] // xx.x

//webserver
ESP8266WebServer server(80);
//MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

String webString = "";    // String to display

unsigned long delayTime;

void setup() {
    Serial.begin(115200);
    Serial.println(F("HTU21D test"));

    // do our wifi thing inc. network connect, set hostname, disp html
    wifiSetup();

    bool status;

    // default settings
    status = htu.begin();
    if (!status) {
        Serial.println("Could not find a valid HTU21D sensor, check wiring!");
        while (1);
    }

    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();

    // setup MQTT
    client.setServer(mqtt_server, 1883);

}

void loop() {
    // for webserver
    server.handleClient();

    printValues();
    delay(delayTime);

    // MQTT publish
    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    // Publish data
    pubData(tempToF(htu.readTemperature()), htu.readHumidity(), 0);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //client.publish("test_channel","hello world");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void pubData(float temp, float humidity, float pressure) {
  // publish real data
  //Serial.print("MQTT client state:");
  //Serial.println(client.state());

  String result_s =

  // convert temp to string
  dtostrf(temp, 5, 1, loctemp);
  // convert humidity to string
  dtostrf(humidity, 4, 1, lochum);

  client.publish(mqtt_channel, result);
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
    Serial.print(tempToF(htu.readTemperature()));
    Serial.println(" *F");

    Serial.print("Humidity = ");
    Serial.print(htu.readHumidity());
    Serial.println(" %");

    Serial.println();
}

void wifiSetup() {

  // set hostname
  WiFi.hostname(myhostname);

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

  Serial.println(F("HTU21D Environmental Display Server"));
  Serial.print(F("Connected to "));
  Serial.println(ssid);
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("hostname: "));
  Serial.println(WiFi.hostname());

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
<title>HTU21D Environmental Display Server</title>\r\
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
<h1>Temperature</h1>\r<h2>" + String(tempToF(htu.readTemperature()), 2) + "&deg;F</h2><br />" + "\r\
<h1>Humidity</h1>\r<h2>" + String(htu.readHumidity(), 2) + "%</h2><br />" + "\r\
</body>\r\
</html>";
    server.send(200, "text/html", webString); // send to someone's browser when asked
  });

  server.begin();
  Serial.println(F("HTTP server started"));

  // Add service to MDNS-SD
  //MDNS.addService("http", "tcp", 80);

}