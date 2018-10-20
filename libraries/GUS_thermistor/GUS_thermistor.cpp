/*
  GUS_thermistor.cpp - Grand Unified Sensor - thermistor.
  Created by Tom S. Cloud, October 10, 2018.
  See LICENSE.txt.
*/
#include "Arduino.h"
#include "GUS_thermistor.h"

GUS_thermistor::GUS_thermistor() {
}

void GUS_thermistor::setup() {
}


float GUS_thermistor::readTemperature(void) {
  return getTMPTemperature();
}

float GUS_thermistor::readFTemperature(void) {
  return tempToF(getTMPTemperature());
}

float GUS_thermistor::readHumidity(void) {
  return 0;
}

float GUS_thermistor::readPressure(void) {
  return 0;
}

float GUS_thermistor::getTMPTemperature() {
  uint8_t i;
  float average;

  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samplesT[i] = analogRead(THERMISTORPIN);
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

  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  Serial.print("Thermistor resistance ");
  Serial.println(average);

  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
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
