/*
  GUS_bme280.cpp - Grand Unified Sensor - BME280.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUS_bme280.h"

GUS_bme280::GUS_bme280() {
  // default settings
  bool status = bme.begin();
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
}

float GUS_bme280::readTemperature(void) {
  return bme.readTemperature();
}

float GUS_bme280::readHumidity(void) {
  return bme.readHumidity();
}

float GUS_bme280::readPressure(void) {
  return bme.readPressure();
}
