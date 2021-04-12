
// include the library code:
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include "buttons.h"


float potVal = 0;
int motorVal = 0;


void setup() {
  buttonSetup();
}


void loop() {
  UILoop();  
}
