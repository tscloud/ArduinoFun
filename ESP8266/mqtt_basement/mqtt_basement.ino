/*
  GUS sensor platform sketch template.
  --Change/add proper entries at each '+++' section--
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include <Wire.h>

#include <GUS_base.h>
// +++ include proper sensor (& maybe GUSh) headers
#include <GUS_bme280.h>
#include <GUS_htu21d.h>

// +++ needed for MQTT topic: 1 for each sensor
#define SENSOR1  "BME280"
#define SENSOR2  "HTU21D"

// declare GUS base & sensors
GUS_base *gus;
// +++ references to sensors
GUS_bme280 *sensor1; // sensor: BME280
GUS_htu21d *sensor2; // sensor: HTU21D

void setup() {
    Serial.begin(115200);

    // GUS & sensor setup
    gus = new GUS_base();
    gus->setup();

    // +++ instantiate & setup sensors
    sensor1 = new GUS_bme280();
    sensor1->setup();
    sensor2 = new GUS_htu21d();
    sensor2->setup();
}

void loop() {
    // do GUS loop stuff
    gus->loop();

    // +++ Publish data for each sensor
    pubSensorData(sensor1, SENSOR1);
    pubSensorData(sensor2, SENSOR2);
}

// Helper function
void pubSensorData(GUS_sensor_super *sensor, char *sensorName) {
    gus->pubData(sensorName,
      sensor->readFTemperature(),
      sensor->readHumidity(),
      sensor->readPressure()/100.0F);
}
