/*
  GUS_htu21d.h - Grand Unified Sensor - HTU21D.
  Created by Tom S. Cloud, October 12, 2018.
  See LICENSE.txt.
*/

#ifndef GUS_HTU21D_H
#define GUS_HTU21D_H

#include "Arduino.h"
#include "Adafruit_HTU21DF.h"
#include "GUS_sensor_super.h"

class GUS_htu21d: public GUS_sensor_super {
  public:
    GUS_htu21d();
    //~GUS_htu21d();
    void setup();
    float readTemperature(void);
    float readFTemperature(void);
    float readHumidity(void);
    float readPressure(void);

  private:
    Adafruit_HTU21DF htu = Adafruit_HTU21DF(); // I2C
};
#endif