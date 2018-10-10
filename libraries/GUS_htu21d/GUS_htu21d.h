/*
  GUS_htu21d.h - Grand Unified Sensor - HTU21D.
  Created by Tom S. Cloud, November 2, 2007.
  See LICENSE.txt.
*/
#ifndef GUS_htu21d
#define GUS_htu21d

#include "Arduino.h"

class GUS_htu21d
{
public:
    GUS_htu21d();
    ~GUS_htu21d();
    float readTemperature(void);
    float readHumidity(void);
    // ** put in super
    float tempToF(float tempC);
    void printValues(void);

private:

};