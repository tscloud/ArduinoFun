/*
 *  Misc.h - Misc Support Functions
 *
 *  Concept, Design and Implementation by: Craig A. Lindley
 *  Last Update: 10/20/2015
 */

#ifndef MISC_H
#define MISC_H

#include <EEPROM.h>

// Number of bytes of the EEPROM we are using
#define EEPROM_TAG_SIZE 4

// Get the DST indicator from the EEPROM.
// If EEPROM has been marked valid, retrieve the
// indicator. If EEPROM has not been marked valid,
// just return false;
bool getDSTIndicator() {
  byte b0, b1, b2, b3;
  bool result = false;

  EEPROM.begin(EEPROM_TAG_SIZE);

  // Read first four bytes from the EEPROM
  b0 = EEPROM.read(0);
  b1 = EEPROM.read(1);
  b2 = EEPROM.read(2);
  b3 = EEPROM.read(3);

  // The EEPROM's is valid if first three bytes are C, A, L
  if ((b0 == 'C') && (b1 == 'A') && (b2 == 'L')) {
    // Yes the EEPROM has been initialized so retrieve the indicator
    result = (b3 == 1) ? true : false;
  }
  EEPROM.end();
  return result;
}

// Set the DST indicator
void setDSTIndicator(bool enabled) {

  EEPROM.begin(EEPROM_TAG_SIZE);

  // First write the tag to indicate EEPROM is initialized
  EEPROM.write(0, 'C');
  EEPROM.write(1, 'A');
  EEPROM.write(2, 'L');

  // Then write the indicator
  EEPROM.write(3, (enabled) ? 1 : 0);

  // Commit the write
  EEPROM.end();
}

#endif



