#ifndef I2C_CONTROLLER_H
#define I2C_CONTROLLER_H

#include "Arduino.h"
#include "Wire.h"
#define MY_ADDRESS 8 

class I2C_Controller
{
public:
    I2C_Controller();
    void setup();
    static int motorSpeedScrew;
    static int motorSpeedSpoiler;
    static void receiveEvent(int howMany);
    static int get_desired_screw_speed(){return motorSpeedScrew;}
    static int get_desired_spoiler_speed(){return motorSpeedSpoiler;}

};

#endif 
