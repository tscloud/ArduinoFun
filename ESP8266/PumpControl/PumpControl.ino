#include "ESP8266WiFi.h"

// digital pin
#define BUTTON_PIN 12

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop()
{
  Serial.print("GPIO=");
  Serial.println(digitalRead(BUTTON_PIN));
  delay(1000);
}
