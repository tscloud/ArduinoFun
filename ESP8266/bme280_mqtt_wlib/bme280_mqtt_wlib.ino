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

#include <GUS_base.h>
#include <GUS_bme280.h>

#define CONFIG_FILE  "/config.json"
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C

// this will be passed as the published data
// NOTE on data structure: "-" sign not currently accounted for
char result[19] = "T"; // T[+/-]xx.x,Hyy.y,Pzz.z <-- greatest length: 19
// used to build published data - char[]
char loctemp [6]; // [+/-]xx.x
char lochum [5]; // xx.x
char locpress [7]; // xxxx.x

// declare GUS object refs
GUS_bme280 sensor1;
GUS_base gus;

void setup() {
    Serial.begin(115200);
    Serial.println(F("BME280 test"));

    // GUS setup
    sensor1 = GUS_bme280();
    gus = GUS_base();

    // default settings
    bool status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

    Serial.println();
}

void loop() {
    printValues();

    // do GUS loop stuff
    gus.loop();

    // Publish data
    pubData(tempToF(bme.readTemperature()), bme.readHumidity(), bme.readPressure()/100.0F);
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
  //Serial.print("loctemp: ");
  //Serial.println(loctemp);
  //Serial.print("lochum: ");
  //Serial.println(lochum);
  //Serial.print("locpress: ");
  //Serial.println(locpress);

  // -- char[] way
  // T[+/-]xx.x,Hyy.y,Pzz.z <-- greatest length: 19

  // (re)init array
  result[1] = '\0';

  strcat(result, loctemp);
  strcat(result, ",H");
  strcat(result, lochum);
  strcat(result, ",P");
  strcat(result, locpress);

  // DEBUG
  Serial.print("result: ");
  Serial.println(result);

  gus.client.publish(gus.mqtt_channel, result);
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
