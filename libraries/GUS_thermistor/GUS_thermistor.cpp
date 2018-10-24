/*
  GUS_thermistor.cpp - Grand Unified Sensor - thermistor.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUS_thermistor.h"

GUS_thermistor::GUS_thermistor() {
}

GUS_thermistor::GUS_thermistor(GUSh_mcp3008 *aMcp, uint8_t aPin) {
  mcp = aMcp;
  pin = aPin;
}

void GUS_thermistor::setup() {
}

float GUS_thermistor::readTemperature(void) {
  return getTMPTemperature();
}

float GUS_thermistor::readFTemperature(void) {
  return tempToF(getTMPTemperature());
}

float GUS_thermistor::getTMPTemperature() {
  uint8_t i;
  float average;
  uint16_t samplesT[NUMSAMPLES];

  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
    if (mcp != NULL) {
      samplesT[i] = mcp->analogRead(pin);
    }
    else{
      samplesT[i] = analogRead(THERMISTORPIN);
    }
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

  return thermistorTemp(average);
}

float GUS_thermistor::thermistorTemp(float analogValue) {
  // convert the value to resistance
  analogValue = 1023 / analogValue - 1;
  analogValue = SERIESRESISTOR / analogValue;
  Serial.print("Thermistor resistance ");
  Serial.println(analogValue);

  float steinhart;
  steinhart = analogValue / THERMISTORNOMINAL; // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C

  Serial.print("Temperature ");
  Serial.print(steinhart);
  Serial.println(" *C");
  Serial.print("Temperature ");
  Serial.print(tempToF(steinhart));
  Serial.println(" *F");

  return steinhart;
}
