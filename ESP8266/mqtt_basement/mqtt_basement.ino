/*
  GUS sensor platform sketch template.
  --Change/add proper entries at each '+++' section--
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include <Wire.h>

#include <GUS_base.h>
// +++ include proper sensor (& maybe GUSh) headers
#include <GUS_bme680.h>

// +++ needed for MQTT topic: 1 for each sensor
#define SENSOR1  "BME680"

// declare GUS base & sensors
GUS_base *gus;
// +++ references to sensors
GUS_bme680 *sensor1;

void setup() {
    Serial.begin(115200);

    // GUS & sensor setup
    gus = new GUS_base();
    gus->setup();

    // +++ instantiate & setup sensors
    sensor1 = new GUS_bme680();
    sensor1->setup();
}

void loop() {
    // do GUS loop stuff
    gus->loop();

    // +++ Publish data for each sensor
    pubSensorData(sensor1, SENSOR1);
}

// Helper function
void pubSensorData(GUS_sensor_super *sensor, char *sensorName) {
    gus->pubData(sensorName,
      sensor->readFTemperature(),
      sensor->readHumidity(),
      sensor->readPressure()/100.0F);
}
