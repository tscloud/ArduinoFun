/*
  GUS_ds18b20.cpp - Grand Unified Sensor - BME280.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUS_ds18b20.h"

GUS_ds18b20::GUS_ds18b20() {
  /*
  // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
  OneWire oneWire(ONE_WIRE_BUS);

  // Pass our oneWire reference to Dallas Temperature.
  sensors(&oneWire);

  // Start up the library
  sensors.begin();
  */
}

void GUS_ds18b20::setup() {
  // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
  OneWire oneWire(ONE_WIRE_BUS);

  // Pass our oneWire reference to Dallas Temperature.
  sensors = DallasTemperature(&oneWire);

  // Start up the library
  sensors.begin();
}


float GUS_ds18b20::readTemperature(void) {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));
}

float GUS_ds18b20::readFTemperature(void) {
  return tempToF(readTemperature());
}

float GUS_ds18b20::readHumidity(void) {
  return 0;
}

float GUS_ds18b20::readPressure(void) {
  return 0;
}
