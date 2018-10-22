/*
  GUS_sensor_super.cpp - Grand Unified Sensor - Super class for sensor subclasses.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUS_sensor_super.h"

GUS_sensor_super::GUS_sensor_super() {
}

bool GUS_sensor_super::forcedRead() {
  return false;
}

float GUS_sensor_super::tempToF(float tempC) {
  return (tempC * 9.0F)/5.0F + 32.0F;
}

float GUS_sensor_super::pressToMBar(float pressure) {
  return (pressure / 100.0F);
}

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

