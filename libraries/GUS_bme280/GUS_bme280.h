/*
  GUS_bme280.h - Grand Unified Sensor - HTU21D.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

class GUS_bme280
{
public:
  GUS_bme280();
  //~GUS_bme280();
  float readTemperature(void);
  float readHumidity(void);
  // ** put in super
  float tempToF(float tempC);
  void printValues(void);

private:
  Adafruit_BME280 bme; // I2C
};