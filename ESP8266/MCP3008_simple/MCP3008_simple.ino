/***************************************************
Simple example of reading the MCP3008 analog input channels and printing
them all out.

Author: Carter Nelson
License: Public Domain
****************************************************/

#include <Adafruit_MCP3008.h>

//Thermistor stuff
// which analog pin to connect
#define THERMISTORPIN 2
// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25
// how many samples to take and average, more takes longer
// but is more 'smooth'
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 10000

Adafruit_MCP3008 adc;

int count = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("MCP3008 simple test.");

  // Hardware SPI (specify CS, use any available digital)
  // Can use defaults if available, ex: UNO (SS=10) or Huzzah (SS=15)
  adc.begin();
  // Feather 32u4 (SS=17) or M0 (SS=16), defaults SS not broken out, must specify
  //adc.begin(10);

  // Software SPI (specify all, use any available digital)
  // (sck, mosi, miso, cs);
  //adc.begin(13, 11, 12, 10);
}

void loop() {
  /*
  for (int chan=0; chan<8; chan++) {
    Serial.print(adc.readADC(chan)); Serial.print("\t");
  }

  Serial.print("["); Serial.print(count); Serial.println("]");
  count++;
  */
  uint8_t i;
  float average;
  uint16_t samplesT[NUMSAMPLES];

  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samplesT[i] = adc.readADC(THERMISTORPIN);
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

  delay(1000);
}

float tempToF(float tempC) {
  return (tempC * 9.0F)/5.0F + 32.0F;
}