/*
  GUS_bme680.cpp - Grand Unified Sensor - BME680.
  Created by Tom S. Cloud, October 17, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUS_bme680.h"

GUS_bme680::GUS_bme680() {
}

void GUS_bme680::setup() {
  // default settings
  bme = new Adafruit_BME680();
  bool status = bme->begin();
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
  // Set up oversampling and filter initialization
  bme->setTemperatureOversampling(BME680_OS_8X);
  bme->setHumidityOversampling(BME680_OS_2X);
  bme->setPressureOversampling(BME680_OS_4X);
  bme->setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme->setGasHeater(320, 150); // 320*C for 150 ms
}

// -- look into performReading(void) and beginReading(void) --
float GUS_bme680::readTemperature(void) {
  return (useForcedRead) ? bme->temperature : bme->readTemperature();
}

float GUS_bme680::readFTemperature(void) {
  return (useForcedRead) ? tempToF(bme->temperature) : tempToF(bme->readTemperature());
}

float GUS_bme680::readHumidity(void) {
  return (useForcedRead) ? bme->humidity : bme->readHumidity();
}

float GUS_bme680::readPressure(void) {
  return (useForcedRead) ? bme->pressure : bme->readPressure();
}

bool GUS_bme680::forcedRead(void) {
  if (!bme->performReading()) {
    Serial.println("Failed to perform reading :(");
    useForcedRead = false;
  }
  else {
    useForcedRead = true;
  }

  return useForcedRead;
}
