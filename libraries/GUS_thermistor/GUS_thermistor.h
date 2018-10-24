/*
  GUS_thermistor.h - Grand Unified Sensor - Thermistor.
  Created by Tom S. Cloud, October 19, 2018.
  See LICENSE.txt.
*/

#ifndef GUS_thermistor_H
#define GUS_thermistor_H

#include "Arduino.h"
#include "GUS_sensor_super.h"
#include "GUSh_mcp3008.h"

//Thermistor stuff
// which analog pin to connect
#define THERMISTORPIN A0
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000

class GUS_thermistor: public GUS_sensor_super {
  public:
    GUS_thermistor();
    GUS_thermistor(GUSh_mcp3008 *aMcp, uint8_t aPin);
    //~GUS_thermistor();
    void setup();
    float readTemperature(void);
    float readFTemperature(void);
    static float thermistorTemp(float analogValue);

  private:
    float getTMPTemperature(void);
    uint8_t pin;
    GUSh_mcp3008 *mcp;
};
#endif