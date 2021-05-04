/*
 * This controlls receiving messages from the UI
 * 
 */

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
    static int motorSpeedScrew;// the value of the desired motor speed
    static int motorSpeedSpooler;// The value of the desired spooler 
    static void receiveEvent(int howMany);//read the incoming data
    static int get_desired_screw_speed(){return motorSpeedScrew;}//get the desired screw speed
    static int get_desired_spooler_speed(){return motorSpeedSpooler;}//get the desired spooler speed

};

#endif 
