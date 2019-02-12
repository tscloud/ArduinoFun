/*
  GUS_bme280.h - Grand Unified Sensor - BME280.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/

#ifndef GUS_BME280_H
#define GUS_BME280_H

//#include "Arduino.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"
#include "GUS_sensor_super.h"

class GUS_bme280: public GUS_sensor_super {
  public:
    GUS_bme280();
    //~GUS_bme280();
    void setup();
    float readTemperature(void);
    float readFTemperature(void);
    float readHumidity(void);
    float readPressure(void);
    bool forcedRead(void);
  private:
    Adafruit_BME280 *bme; // I2C
};
#endif