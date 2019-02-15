/*
  Arduino TFT character set printout
 */

#include <Adafruit_GFX.h>  // Arduino LCD library
#include <SPI.h>

// pin definition for the Uno
#define cs   5
#define dc   4
#define rst  -1

// pin definition for the Leonardo
// #define cs   7
// #define dc   0
// #define rst  1

TFT TFTscreen = TFT(cs, dc, rst);

void setup() {
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.background(255, 255, 255);
  TFTscreen.setTextSize(1);
  int x = 18;
  int y = 0;
  boolean zebra = false;
  for (int i = 0; i <= 255; i++) {
    if (zebra) TFTscreen.stroke(0, 0, 0); // black
    else TFTscreen.stroke(255, 0, 0); // blue
    // row number:
    char z[] = " 0 ";
    if (i < 0xA0) z[0] = '0' + i / 16;
    else z[0] = '7' + i / 16;
    TFTscreen.text(z, 1, y);
    // the characters:
    char s[] = "  ";
    s[0] = i;
    TFTscreen.text(s, x, y);
    x = x + 9;
    if (x > 158) {
      y = y + 8;
      x = 18;
      zebra = !zebra;
    }
  }
}
void loop() { }