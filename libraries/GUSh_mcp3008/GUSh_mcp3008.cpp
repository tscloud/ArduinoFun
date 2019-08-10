/*
  GUSh_mcp3008.cpp - Grand Unified Sensor - MCP3008.
  Created by Tom S. Cloud, October 20, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUSh_mcp3008.h"

GUSh_mcp3008::GUSh_mcp3008() {
}

void GUSh_mcp3008::setup(uint8_t digpin) {
  // default settings
  // Hardware SPI (specify CS, use any available digital)
  // Can use defaults if available, ex: UNO (SS=10) or Huzzah (SS=15)
  bool status = adc.begin(digpin);
  // Feather 32u4 (SS=17) or M0 (SS=16), defaults SS not broken out, must specify
  //adc.begin(10);

  // Software SPI (specify all, use any available digital)
  // (sck, mosi, miso, cs);
  //adc.begin(13, 11, 12, 10);
  if (!status) {
    Serial.println("Could not find a valid MCP3008, check wiring!");
    while (1);
  }
  else {
    Serial.println("MCP3008 begin() success!");
  }
}

float GUSh_mcp3008::analogRead(uint8_t pin) {
  return adc.readADC(pin);
}