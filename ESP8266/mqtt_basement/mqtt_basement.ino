#include <Wire.h>

#include <GUS_base.h>
#include <GUS_bme680.h>
#include <GUS_htu21d.h>

#define CONFIG_FILE  "/config.json"
#define SENSOR1  "BME680"
#define SENSOR2  "HTU21D"
#define SEALEVELPRESSURE_HPA (1013.25)

// declare GUS object refs
GUS_bme680 sensor1;
GUS_htu21d sensor2;
GUS_base gus;

void setup() {
    Serial.begin(115200);

    // GUS & sensor setup
    gus.setup();
    sensor1.setup();
    sensor2.setup();
}

void loop() {
    // do GUS loop stuff
    gus.loop();

    // Publish data
    pubSensorData(&sensor1, SENSOR1);
    pubSensorData(&sensor2, SENSOR2);
}

// Helper function
void pubSensorData(GUS_sensor_super *sensor, char *sensorName) {
    gus.pubData(sensorName,
      sensor -> readFTemperature(),
      sensor -> readHumidity(),
      sensor -> readPressure()/100.0F);
}
