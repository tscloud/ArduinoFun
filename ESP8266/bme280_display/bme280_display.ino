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
//#include <Fonts/FreeMono9pt7b.h>

#define SEALEVELPRESSURE_HPA (1013.25)

// --display
// Pin definitions
#define OLED_RESET  16  // Pin 15 -RESET digital signal

// digital pin
#define BUTTON_PIN 12

// sensor
Adafruit_BME280 bme; // I2C

unsigned long delayTime;

// used to determine which type of sensor data to display
volatile int interruptDisplayInd = 0;

// used for conversion of sensor value to string for centering
char convBuffer[64];

// used for debounce
long debouncing_time = 300; //Debouncing Time in Milliseconds
volatile unsigned long last_micros;

// --TEST
int16_t xx1, yy1;
uint16_t ww, hh;


// --display
ESP_SSD1306 display(OLED_RESET); // FOR I2C

void setup() {
    Serial.begin(115200);
    Serial.println(F("BME280 test"));

    // setup button interrupt
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, FALLING);

    bool status;
    
    // default settings
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;

    // --display
    display.begin(SSD1306_SWITCHCAPVCC);  // Switch OLED

    // SSD1306 Init - this has to be done here or display will not output
    display.clearDisplay();
    //display.setFont(&FreeMono9pt7b);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Hello, world!");
    display.display();
    delay(2000);
    display.clearDisplay();
    display.display();
}

void loop() {
    switch (interruptDisplayInd) {
      case 0:
        displayData("Temperature", (bme.readTemperature() * 9.0F)/5.0F + 32.0F);
        break;
      case 1:
        displayData("Pressure", bme.readPressure() / 100.0F);
        break;
      case 2:
        displayData("Humidity", bme.readHumidity());
        break;
      default:
        Serial.print("Unhandled interrupt value: ");
        Serial.println(interruptDisplayInd);
        break;
    }
    
    delay(delayTime);
}

void handleInterrupt() {
  if((long)(micros() - last_micros) >= debouncing_time * 1000) {
    // if we pass debounce -> do our thing
    if(interruptDisplayInd >= 2) {
      interruptDisplayInd = 0;
    }
    else {
      interruptDisplayInd++;
    }
    Serial.print("Interupt received - value now: ");
    Serial.println(interruptDisplayInd);

    last_micros = micros();
  }
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
}

void displayData(char *title, float value) {
    // -- display
    // Get the size of value to be displayed
    //snprintf(convBuffer, 64, "%f", value);
    itoa(value, convBuffer, 10);
    
    // -title
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.setTextSize(1);
    display.println(title);

    // -value
    display.setTextSize(2);
    display.getTextBounds(convBuffer, 0, 0, &xx1, &yy1, &ww, &hh);
    
    // -- TEST
    //Serial.print("Value :");  Serial.println(value);
    //Serial.print("xx1:");  Serial.println(xx1);
    //Serial.print("yy1:");  Serial.println(yy1);
    //Serial.print("ww :");  Serial.println(ww);
    //Serial.print("hh :");  Serial.println(hh);
    //Serial.print("pad :");  Serial.println((display.width()-ww)/2);
    //Serial.print("screen width :");  Serial.println(display.width());

    // don't know why I need to sub an extra 35 to center text!!
    display.setCursor((display.width()-ww-35)/2,16);
    display.println(value);
    display.display();
    //display.clearDisplay();
}
