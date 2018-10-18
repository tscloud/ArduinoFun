#include <Wire.h>

#include <GUS_base.h>
#include <GUS_ds18b20.h>

#define CONFIG_FILE  "/config.json"
#define SENSOR1  "DS18B20"
#define SEALEVELPRESSURE_HPA (1013.25)

// declare GUS object refs
GUS_ds18b20 sensor1;
GUS_base gus;

void setup() {
    Serial.begin(115200);

    // GUS & sensor setup
    gus.setup();
    sensor1.setup();
}

void loop() {
    // do GUS loop stuff
    gus.loop();

    // Publish data
    pubSensorData(&sensor1, SENSOR1);
}

// Helper function
void pubSensorData(GUS_sensor_super *sensor, char *sensorName) {
    gus.pubData(sensorName,
      sensor->readFTemperature(),
      sensor->readHumidity(),
      sensor->readPressure()/100.0F);
}
