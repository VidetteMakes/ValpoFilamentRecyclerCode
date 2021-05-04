/*
 * This is the slack sensor code. Detects whether there is too much
 * slack in the system
 */

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

    // Setup with the pin of the top and bottom
    int setup(int analogPinTop,int analogPinBottom);

    const int NOTHING_HAPPENED = 0;
    const int HIT_TOP = 1;
    const int HIT_BOTTOM = 2;
    const int ERROR= 3;

    //loop returns an integer that corresponds to the constants above
    int loop();

    //print data about the slack sensor
    void printData();
};

/*
An example use case of the slack sensor.

#include "slack_sensor.h"

const int top_slack_pin = A0;
const int bottom_slack_pin = A2; 

slack_sensor my_slack_sensor;


void setup() {
  my_slack_sensor.setup(top_slack_pin,bottom_slack_pin);

}


void loop() {


  int slack_sensor_output = my_slack_sensor.loop();

  if (slack_sensor_output == my_slack_sensor.HIT_BOTTOM){
    //Hit bottom
  }
  else if (slack_sensor_output == my_slack_sensor.HIT_TOP){
    //Hit Top
  }

}
 */


#endif 
