/*
  Arduino TFT character set printout
 */

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
//#include "Free_Fonts.h" // Include the header file attached to this sketch

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

uint16_t bg = TFT_BLACK;
uint16_t fg = TFT_WHITE;

void setup() {
  // TFT setup
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextFont(4);
  //tft.setTextColor(TFT_YELLOW);
  //tft.setCursor(0, 15);    // Set cursor near top left corner of screen
  //tft.setFreeFont(FM9);// Select the font

  for (int i = 0; i < 250; ++i)
  {
    tft.print(char(i));
  }
  //tft.drawChar(127, 80, 64, 2);
  tft.print(char(0x2d));
  //tft.print("Y");
/*
  Serial.begin(115200); // Used for messages and the C array generator

  Serial.println("NodeMCU vlw font test!");

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nInitialisation done.");

  //listFiles(); // Lists the files so you can see what is in the SPIFFS

  tft.init();
  tft.setRotation(2);

  fg = TFT_WHITE;
  bg = TFT_BLACK;

  tft.setTextColor(fg, bg);
  // Load a smooth font from SPIFFS
  tft.loadFont("Latin-Hiragana-24");

  tft.fillScreen(bg);

  tft.setCursor(0,0);
  tft.println(char(36));
  tft.print("hello");

  //for (int i = 400; i < 600; ++i)
  //{
  //  tft.print(char(i));
  //}
*/
}
void loop() {
  // Show all characters on screen with 2 second (2000ms) delay between screens
  //tft.showFont(2000); // Note: This function moves the cursor position!
  //delay(2000);

}