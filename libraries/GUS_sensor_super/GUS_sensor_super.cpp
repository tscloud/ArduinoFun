/*
  GUS_sensor_super.cpp - Grand Unified Sensor - Super class for sensor subclasses.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
//#include "Arduino.h"
#include "GUS_sensor_super.h"

GUS_sensor_super::GUS_sensor_super() {
}

// virtual functions that may be overridden
float GUS_sensor_super::readTemperature(void) {
  return 0;
}

float GUS_sensor_super::readFTemperature(void) {
  return 0;
}

float GUS_sensor_super::readHumidity(void) {
  return 0;
}

float GUS_sensor_super::readPressure(void) {
  return 0;
}

// may or may not be supported by sunclass
bool GUS_sensor_super::forcedRead() {
  return false;
}

// static helper functions
float GUS_sensor_super::tempToF(float tempC) {
  return (tempC * 9.0F)/5.0F + 32.0F;
}

float GUS_sensor_super::pressToMBar(float pressure) {
  return (pressure / 100.0F);
}

// object helper function
void GUS_sensor_super::printValues(void) {
  Serial.print("Temperature = ");
  Serial.print(readTemperature());
  Serial.println(" *C");

  Serial.print("Temperature = ");
  Serial.print(tempToF(readTemperature()));
  Serial.println(" *F");

  Serial.print("Pressure = ");
  Serial.print(readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Humidity = ");
  Serial.print(readHumidity());
  Serial.println(" %");

  Serial.println();
}

