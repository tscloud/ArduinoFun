/*
  GUS_bme280.h - Grand Unified Sensor - BME280.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

class GUS_bme280: public GUS_sensor_super {
  public:
    GUS_bme280();
    //~GUS_bme280();
    float readTemperature(void);
    float readHumidity(void);
    float readPressure(void);

  private:
    Adafruit_BME280 bme; // I2C
};