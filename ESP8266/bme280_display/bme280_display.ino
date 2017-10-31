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
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// --display
#include <ESP_SSD1306.h>    // Modification of Adafruit_SSD1306 for ESP8266 compatibility
#include <Adafruit_GFX.h>   // Needs a little change in original Adafruit library (See README.txt file)

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)


// --display
// Pin definitions
#define OLED_RESET  16  // Pin 15 -RESET digital signal

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

// --display
ESP_SSD1306 display(OLED_RESET); // FOR I2C

void setup() {
    Serial.begin(115200);
    Serial.println(F("BME280 test"));

    bool status;
    
    // default settings
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();

    // --display
    // SSD1306 Init
    display.begin(SSD1306_SWITCHCAPVCC);  // Switch OLED

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Hello, world!");
    display.setTextColor(BLACK, WHITE); // 'inverted' text
    display.println(3.141592);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print("0x"); display.println(0xDEADBEEF, HEX);
    display.display();
    delay(2000);
    display.clearDisplay();
    display.display();

    /*
    display.display();
    delay(2000);
    // Clear the buffer.
    display.clearDisplay();
    */

    /*
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    //display.clearDisplay();
    display.println("for real?");
    display.display();
    delay(delayTime);
    display.clearDisplay();
    display.display();
    */
}


void loop() { 
    printValues();
    delay(delayTime);
}


void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();

    // -- display
    //display.clearDisplay();
    //display.display();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("Temperature");
    display.println(bme.readTemperature());
    display.display();
    display.clearDisplay();
    
}
