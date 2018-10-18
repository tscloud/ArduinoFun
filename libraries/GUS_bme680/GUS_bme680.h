/*
  GUS_bme680.h - Grand Unified Sensor - BME680.
  Created by Tom S. Cloud, October 17, 2018.
  See LICENSE.txt.
*/

#ifndef GUS_BME680_H
#define GUS_BME680_H

#include "Arduino.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME680.h"
#include "GUS_sensor_super.h"

class GUS_bme680: public GUS_sensor_super {
  public:
    GUS_bme680();
    //~GUS_bme680();
    void setup();
    float readTemperature(void);
    float readFTemperature(void);
    float readHumidity(void);
    float readPressure(void);
    bool performReading(void);

  private:
    Adafruit_BME680 *bme; // I2C
};
#endif