/*
 * This is the code for the IR sensor
 * It detects when there is filament in front of the sensor
 * It uses a rolling average to detect whether is blocked or not
 */

#ifndef IR_SLOT_INTERRUPT_H
#define IR_SLOT_INTERRUPT_H

#include "Arduino.h"

class IR_slot_interrupt
{
private:
    /* data */
    int analogPin = A0;
    int val = 0;
    float average_light = 0;
    float alpha =  0.0001;
    void set_average_light(int new_value);
    int IR_buffer;
    
public:
    IR_slot_interrupt();
    
    //set up the analog pin and buffer. The buffer is an positve offset to prevent false positives
    int setup(int setAnalogPin, int setbuffer);

    
    int loop();//called every loop
    int get_value();//return the current reading
    
    // return the threshold value for light. If it is greater than 0 there is filament in front of the sensor.
    int get_average_light();
};





#endif 
