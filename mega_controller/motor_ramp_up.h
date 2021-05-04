/*
 * 
 * This code will slowly increase the speed of the motors 
 * so that these is not too high a current throught the system
 * 
 */
#ifndef MOTOR_RAMP_UP_H
#define MOTOR_RAMP_UP_H

#include "Arduino.h"

#define STEP_VALUE 3

class motor_ramp_up
{
private:
    /* data */
    unsigned long last_loop_time;
    unsigned long ramp_up_ms;
    unsigned int ramp_step;
    int goal_speed;
    int speed;
    int change;
    int motor_pin = 0;
public:
    motor_ramp_up();
    // define the pins. The ramp function will move `new_ramp_step` every `new_ramp_up_ms`
    void motor_ramp_up::setup(int new_motor_pin, int new_ramp_up_ms, int new_ramp_step);
    void motor_ramp_up::setup(int new_motor_pin);
    void loop();//call every loop
    int get_speed(){return speed;}//get the current speed
    void set_speed(int new_speed);//set the desired speed

};



/*
 Example

#define MOTOR_PIN 11

#include "motor_ramp_up.h"

motor_ramp_up spooler_motor;



void setup() {

  spooler_motor.setup(MOTOR_PIN,1000);
}


void loop() {

  spooler_motor.loop();

  if (TURN MOTOR ON){
    spooler_motor.set_speed(1023);
  }
  else if (TURN MOTOR OFF){
    spooler_motor.set_speed(0);
  }


}
  
 */

#endif
