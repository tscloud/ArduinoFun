/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

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
    // TEST
    //sensor1.readTemperature();

    //delay(gus.delaytime);

    // do GUS loop stuff
    gus.loop();

    // Publish data
    pubSensorData(&sensor1, SENSOR1);
}

// Helper function
void pubSensorData(GUS_sensor_super *sensor, char *sensorName) {
    gus.pubData(sensorName,
      sensor -> readFTemperature(),
      sensor -> readHumidity(),
      sensor -> readPressure()/100.0F);
}
