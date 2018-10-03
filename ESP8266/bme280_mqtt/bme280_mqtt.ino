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
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

// used for wifi
const char* ssid     = "gopats";
const char* password = "15courthouselane";
const char* myhostname = "dutchess";
// used for MQTT
const char* mqtt_server = "bigasspi";
const char* mqtt_clientid = "bme280";
const char* mqtt_channel = "/test/bme280";

// this will be passed as the published data
// NOTE on data structure: "-" sign not currently accounted for
char result[19] = "T"; // T[+/-]xx.x,Hyy.y,Pzz.z <-- greatest length: 19
// used to build published data - char[]
char loctemp [6]; // [+/-]xx.x
char lochum [5]; // xx.x
char locpress [7]; // xxxx.x
// used to build published data - String
String result_s = "T";

//MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long delayTime;

void setup() {
    Serial.begin(115200);
    Serial.println(F("BME280 test"));

    // do our wifi thing inc. network connect, set hostname
    wifiSetup();

    // default settings
    bool status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

    // DEBUG
    Serial.println("-- Default Test --");
    Serial.print("result: ");
    Serial.println(result);
    Serial.print("result strlen: ");
    Serial.println(strlen(result));
    Serial.print("result sizeof: ");
    Serial.println(sizeof(result));

    delayTime = 1000;

    Serial.println();

    // setup MQTT
    client.setServer(mqtt_server, 1883);
}

void loop() {
    printValues();
    delay(delayTime);

    // MQTT publish
    if (!client.connected()) {
      reconnect();
    }
    client.loop();

    // Publish data
    pubData(tempToF(bme.readTemperature()), bme.readHumidity(), bme.readPressure()/100.0F);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_clientid)) {
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

  // convert temp to string
  dtostrf(temp, 4, 1, loctemp);
  // convert humidity to string
  dtostrf(humidity, 4, 1, lochum);
  // convert humidity to string
  dtostrf(pressure, 6, 1, locpress);

  // DEBUG
  Serial.print("loctemp: ");
  Serial.println(loctemp);
  Serial.print("lochum: ");
  Serial.println(lochum);
  Serial.print("locpress: ");
  Serial.println(locpress);

  // -- char[] way
  // T[+/-]xx.x,Hyy.y,Pzz.z <-- greatest length: 19

  // (re)init array
  result[1] = '\0';

  strcat(result, loctemp);
  strcat(result, ",H");
  strcat(result, lochum);
  strcat(result, ",P");
  strcat(result, locpress);
  // Don't bother w/ pressure --> we don't have it

  // DEBUG
  Serial.print("result: ");
  Serial.println(result);

  // -- Arduino String way

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

  // set hostname
  WiFi.hostname(myhostname);

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
  Serial.print(F("hostname: "));
  Serial.println(WiFi.hostname());
}