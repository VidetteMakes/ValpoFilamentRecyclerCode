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
    int setup(int setAnalogPin, int setbuffer);
    int loop();
    int get_value();
    int get_average_light();
};





#endif 
