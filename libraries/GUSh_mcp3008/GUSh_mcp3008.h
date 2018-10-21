/*
  GUS_mcp3008.h - Grand Unified Sensor - MCP3008.
  Created by Tom S. Cloud, October 20, 2018.
  See LICENSE.txt.
*/

#ifndef GUS_MCP3008_H
#define GUS_MCP3008_H

#include "Arduino.h"
#include "Adafruit_MCP3008.h"

class GUSh_mcp3008 {
  public:
    GUSh_mcp3008();
    //~GUS_mcp3008();
    void setup();
    float analogRead(uint8_t pin);

  private:
    Adafruit_MCP3008 adc;
};
#endif