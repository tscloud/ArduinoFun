#include <Wire.h>

#include <GUS_base.h>
#include <GUS_bme280.h>
#include <GUS_htu21d.h>

#define CONFIG_FILE  "/config.json"
#define SENSOR1  "BME280"
#define SENSOR2  "HTU21D"
#define SEALEVELPRESSURE_HPA (1013.25)

// declare GUS object refs
GUS_bme280 *sensor1;
GUS_htu21d *sensor2;
GUS_base *gus;

void setup() {
    Serial.begin(115200);

    // GUS & sensor setup
    gus = new GUS_base();
    gus->setup();

    sensor1 = new GUS_bme280();
    sensor2 = new GUS_htu21d();
    sensor1->setup();
    sensor2->setup();
}

void loop() {
    // do GUS loop stuff
    gus->loop();

    // Publish data
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
