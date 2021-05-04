#include "I2cController.h"
#include "Arduino.h"
#include "Wire.h"


I2C_Controller::I2C_Controller(){
  motorSpeedScrew = 0;
  motorSpeedSpooler = 0;
}

void I2C_Controller::setup(){
  // I2C to receive motor speed from UI
  Wire.begin(MY_ADDRESS);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
}



static int I2C_Controller::motorSpeedScrew = 0;
static int I2C_Controller::motorSpeedSpooler = 0;

byte c1 = 0;

static void I2C_Controller::receiveEvent(int howMany){

  for(int j = 0; j< howMany; j++){
    byte data = Wire.read();

    if (j == 0){
      
       c1 = data;
    }    

    if (j == 1){
      
       motorSpeedScrew = (int)(c1 << 8) + (int) data;
    }   
    
    if (j == 2){
      
       c1 = data;
    }

    
    if (j == 3){
      
       motorSpeedSpooler = (int)(c1 << 8) + (int) data;
    }
    
  }
}
