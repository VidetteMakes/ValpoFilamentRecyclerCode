#ifndef SLACK_SENSOR_H
#define SLACK_SENSOR_H

#include "IR_slot_interrupt.h"

#define DEFAULT_BUFFER 1
#define DEFAULT_CONCSECUTIVE_HITS 3

class slack_sensor
{
private:
    /* data */
    IR_slot_interrupt *top_interrupt;
    IR_slot_interrupt *bottom_interrupt;
    int consecutive_top_hits, consecutive_bottom_hits;
public:
    slack_sensor();
    int setup(int analogPinTop,int analogPinBottom);
    const int NOTHING_HAPPENED = 0;
    const int HIT_TOP = 1;
    const int HIT_BOTTOM = 2;
    const int ERROR= 3;
    int loop();
    void printData();
};




#endif 
