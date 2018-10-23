/*
  GUS_bme280.cpp - Grand Unified Sensor - BME280.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUS_bme280.h"

GUS_bme280::GUS_bme280() {
}

void GUS_bme280::setup() {
  // default settings
  bme = new Adafruit_BME280();
  bool status = bme->begin();
  if (!status) {
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      while (1);
  }
  /*
  // --- weather monitoring ---
  Serial.println("-- Weather Station Scenario --");
  Serial.println("forced mode, 1x temperature / 1x humidity / 1x pressure oversampling,");
  Serial.println("filter off");
  bme -> setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF   );

  // suggested rate is 1/60Hz (1m)
  //  set delayTime to 60000 in config.json
  */
}


float GUS_bme280::readTemperature(void) {
  // Only needed in forced mode! In normal mode, you can remove the next line.
  //bme->takeForcedMeasurement(); // has no effect in normal mode
  return bme->readTemperature();
}

float GUS_bme280::readFTemperature(void) {
  return tempToF(bme->readTemperature());
}

float GUS_bme280::readHumidity(void) {
  return bme->readHumidity();
}

float GUS_bme280::readPressure(void) {
  return bme->readPressure();
}

bool GUS_bme280::forcedRead() {
  bme->takeForcedMeasurement(); // has no effect in normal mode
  useForcedRead = true; // set instance var

  return useForcedRead;
}

