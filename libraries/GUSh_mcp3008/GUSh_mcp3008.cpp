/*
  GUSh_mcp3008.cpp - Grand Unified Sensor - MCP3008.
  Created by Tom S. Cloud, October 20, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUSh_mcp3008.h"

// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
#define THERMISTORPIN 2

GUSh_mcp3008::GUSh_mcp3008() {
}

void GUSh_mcp3008::setup() {
  // default settings
  // Hardware SPI (specify CS, use any available digital)
  // Can use defaults if available, ex: UNO (SS=10) or Huzzah (SS=15)
  bool status = adc.begin(15);
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

  Serial.println("*INITIAL READ*");
  Serial.print("read MCP3008 pin: ");
  Serial.println(THERMISTORPIN);

  uint16_t sample = adc.readADC(THERMISTORPIN);

  Serial.print("sample value: ");
  Serial.println(sample);
}

float GUSh_mcp3008::analogRead(uint8_t pin) {
  /*
  uint8_t i;
  float average;
  uint16_t samplesT[NUMSAMPLES];

  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samplesT[i] = adc->readADC(pin);
   delay(10);
  }

  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) {
     average += samplesT[i];
  }
  average /= NUMSAMPLES;

  Serial.print("Average analog reading ");
  Serial.println(average);

  return average;
  */

  Serial.print("read MCP3008 pin: ");
  Serial.println(THERMISTORPIN);

  uint16_t sample = adc.readADC(THERMISTORPIN);

  Serial.print("sample value: ");
  Serial.println(sample);

  return sample;
}