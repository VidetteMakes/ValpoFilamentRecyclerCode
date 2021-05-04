// include the library code:
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include "buttons.h"

#define CONTROLLER_UI_SLAVE 8

float potVal = 0;
int motorVal = 0;

void sendI2C(){
  Wire.beginTransmission(8);         // transmit to device 8
  int data =  getScrewSpeed();
  char c1 = data >> 8;
  char c2 = data & 0x00ff;
  Wire.write(c1);              // sends one byte 
  Wire.write(c2);              // sends one byte 

  data =  getspoolerSpeed();
  c1 = data >> 8;
  c2 = data & 0x00ff;
  Wire.write(c1);              // sends one byte 
  Wire.write(c2);              // sends one byte 
  
  Wire.endTransmission();            // stop transmitting 
}

void setup() {
  buttonSetup();

  Wire.begin();        // join i2c bus (address optional for master)

  Serial.begin(9600);
}

void loop() {
  Serial.print(getScrewSpeed());
  Serial.print(" ");
  Serial.println(getspoolerSpeed());


  UILoop(); 
  
  sendI2C();
}
