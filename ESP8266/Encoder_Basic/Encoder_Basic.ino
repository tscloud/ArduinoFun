/* Encoder Library - Basic Example
 * http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * This example code is in the public domain.
 */
#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(13, 14);
//   avoid using pins with LEDs attached

long oldPosition  = -999;

long bigPos = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Basic Encoder Test:");
}

void loop() {
  long newPosition = myEnc.read();
  if (newPosition != oldPosition) {
    //Serial.println(newPosition);

    if ((newPosition % 4) == 0) {
      if (newPosition > oldPosition) {
          bigPos += 1;
      }
      else {
          bigPos -= 1;
      }
      Serial.print("bigPos: ");
      Serial.println(bigPos);
    }

    oldPosition = newPosition;
  }
}
