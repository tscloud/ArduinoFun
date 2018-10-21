#include <Wire.h>

#include <GUS_base.h>
#include <GUSh_mcp3008.h>
#include <GUS_thermistor.h>

#define CONFIG_FILE  "/config.json"
#define SENSOR1  "THERM"
#define MCPAPIN 2
#define SEALEVELPRESSURE_HPA (1013.25)

// declare
GUS_base *gus;
GUSh_mcp3008 *aReader;
GUS_thermistor *sensor1;

void setup() {
    Serial.begin(115200);

    // GUS & sensor setup
    gus = new GUS_base();
    gus->setup();

    aReader = new GUSh_mcp3008();
    aReader->setup();
    sensor1 = new GUS_thermistor(aReader, MCPAPIN);
    sensor1->setup(); // empty setup?
}

void loop() {
    // do GUS loop stuff
    gus->loop();

    // Publish data
    pubSensorData(sensor1, SENSOR1);
}

// Helper function
void pubSensorData(GUS_sensor_super *sensor, char *sensorName) {
    gus->pubData(sensorName,
      sensor->readFTemperature(),
      sensor->readHumidity(),
      sensor->readPressure()/100.0F);
}
