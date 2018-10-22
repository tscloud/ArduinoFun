/*
  GUS_sensor_super.h - Grand Unified Sensor - Super class for sensor subclasses.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/

#ifndef GUS_SENSOR_SUPER_H
#define GUS_SENSOR_SUPER_H

#include "Arduino.h"

class GUS_sensor_super {
  public:
    GUS_sensor_super();
    //~GUS_sensor_super();
    virtual void setup() = 0;
    // environmental data each sensor must report - please return 0
    //  if sensor doeas not report data
    virtual float readTemperature(void) = 0;
    virtual float readFTemperature(void) = 0;
    virtual float readHumidity(void) = 0;
    virtual float readPressure(void) = 0;
    // if sensor supports forced reading -> impliment here, otherwise ->
    //  let it return false indicating that function is not supported
    virtual bool forcedRead();
    // helper functions
    static float tempToF(float tempC);
    static float pressToMBar(float pressure);
    void printValues(void);
};
#endif