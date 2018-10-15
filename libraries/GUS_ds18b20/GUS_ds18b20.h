/*
  GUS_ds18b20.h - Grand Unified Sensor - DS18B20.
  Created by Tom S. Cloud, October 15, 2018.
  See LICENSE.txt.
*/

#ifndef GUS_ds18b20_H
#define GUS_ds18b20_H

#include "Arduino.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "GUS_sensor_super.h"

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

class GUS_ds18b20: public GUS_sensor_super {
  public:
    GUS_ds18b20();
    //~GUS_ds18b20();
    void setup();
    float readTemperature(void);
    float readFTemperature(void);
    float readHumidity(void);
    float readPressure(void);

  private:
    DallasTemperature sensors;
};
#endif