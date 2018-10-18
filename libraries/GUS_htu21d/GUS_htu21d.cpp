/*
  GUS_htu21d.cpp - Grand Unified Sensor - HTU21D.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUS_htu21d.h"

GUS_htu21d::GUS_htu21d() {
}

void GUS_htu21d::setup() {
  // default settings
  htu = new Adafruit_HTU21DF(); // I2C
  bool status = htu->begin();
  if (!status) {
      Serial.println("Could not find a valid HTU21D sensor, check wiring!");
      while (1);
  }
}


float GUS_htu21d::readTemperature(void) {
  return htu->readTemperature();
}

float GUS_htu21d::readFTemperature(void) {
  return tempToF(htu->readTemperature());
}

float GUS_htu21d::readHumidity(void) {
  return htu->readHumidity();
}

float GUS_htu21d::readPressure(void) {
  return 0;
}
