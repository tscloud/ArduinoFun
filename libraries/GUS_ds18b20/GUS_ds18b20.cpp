/*
  GUS_ds18b20.cpp - Grand Unified Sensor - BME280.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUS_ds18b20.h"

GUS_ds18b20::GUS_ds18b20() {
}

void GUS_ds18b20::setup() {
  // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
  oneWire = new OneWire(ONE_WIRE_BUS);

  // Pass our oneWire reference to Dallas Temperature.
  sensors = new DallasTemperature(oneWire);

  // Start up the library
  sensors->begin();

  // Grab a count of devices on the wire
  int numberOfDevices = sensors->getDeviceCount();

  // locate devices on the bus
  Serial.println("Locating devices...");

  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors->isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  // find address of device 0 <- should be the only device
  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++) {
    // Search the wire for address
    if(sensors->getAddress(tempDeviceAddress, i)) {
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();

      Serial.print("Setting resolution to ");
      Serial.println(TEMPERATURE_PRECISION, DEC);

      // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
      sensors->setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);

      Serial.print("Resolution actually set to: ");
      Serial.print(sensors->getResolution(tempDeviceAddress), DEC);
      Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}


float GUS_ds18b20::readTemperature(void) {
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors->requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  // report connection
  Serial.print("isConnected: ");
  if (sensors->isConnected(tempDeviceAddress)) Serial.println("TRUE");
  else Serial.println("FALSE");

  float reply = 0;
  //reply = sensors.getTempCByIndex(0);
  reply = sensors->getTempC(tempDeviceAddress);

  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(reply);

  return reply;
}

float GUS_ds18b20::readFTemperature(void) {
  return tempToF(readTemperature());
}

float GUS_ds18b20::readHumidity(void) {
  return 0;
}

float GUS_ds18b20::readPressure(void) {
  return 0;
}

// function to print a device address
void GUS_ds18b20::printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
