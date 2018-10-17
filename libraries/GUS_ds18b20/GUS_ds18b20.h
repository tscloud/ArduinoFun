/*
  GUS_ds18b20.h - Grand Unified Sensor - DS18B20.
  Created by Tom S. Cloud, October 15, 2018.
  See LICENSE.txt.
*/

#ifndef GUS_ds18b20_H
#define GUS_ds18b20_H

#define TEMPERATURE_PRECISION 9 // Lower resolution
#define ONE_WIRE_BUS 2 // Data wire is plugged into port 2 on the Arduino

#include "Arduino.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "GUS_sensor_super.h"

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
    OneWire *oneWire;
    DallasTemperature *sensors;
    DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address
    int numberOfDevices; // Number of temperature devices found
    // helper function
    void printAddress(DeviceAddress deviceAddress);
};
#endif