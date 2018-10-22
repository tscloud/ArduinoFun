/*
  GUS sensor platform sketch template.
  --Change/add proper entries at each '+++' section--
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include <Wire.h>

#include <GUS_base.h>
// +++ include proper sensor (& maybe GUSh) headers
#include <GUSh_mcp3008.h>
#include <GUS_thermistor.h>

// +++ needed for MQTT topic: 1 for each sensor
#define SENSOR1  "THERM"
// pins for ADC
#define MCABOARDPIN 15 // digital pin on board ADC connects to
#define MCPAPIN1 2 // analog pin on ADC sensor connects to

// declare GUS base & sensors
GUS_base *gus;
// +++ references to sensors
GUSh_mcp3008 *aReader; // GUSh: MCP3008 ADC: needed for multiple anlog sensors
GUS_thermistor *sensor1; // sensor: thermistor

void setup() {
    Serial.begin(115200);

    // GUS & sensor setup
    gus = new GUS_base();
    gus->setup();

    // +++ instantiate & setup sensors
    aReader = new GUSh_mcp3008();
    aReader->setup(MCABOARDPIN);
    sensor1 = new GUS_thermistor(aReader, MCPAPIN1);
    sensor1->setup(); // empty setup?
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
