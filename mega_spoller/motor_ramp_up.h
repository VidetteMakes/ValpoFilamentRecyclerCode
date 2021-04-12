#ifndef MOTOR_RAMP_UP_H
#define MOTOR_RAMP_UP_H

#include "Arduino.h"

class motor_ramp_up
{
private:
    /* data */
    unsigned long last_loop_time;
    unsigned long ramp_up_ms;
    int goal_speed;
    int speed;
    int change;
    int motor_pin = 0;
public:
    motor_ramp_up();
    void setup(int motor_pin, int new_ramp_up_ms);
    void loop();
    int get_speed(){return speed;}
    void set_speed(int new_speed);

};

motor_ramp_up::motor_ramp_up()
{
    last_loop_time = 0;
    speed = 0;
    goal_speed = 0;
    change = 0;
}

void motor_ramp_up::setup(int new_motor_pin, int new_ramp_up_ms){
    motor_pin = new_motor_pin;
    ramp_up_ms = new_ramp_up_ms;
    pinMode(motor_pin, OUTPUT);  // sets the pin as output
}

void motor_ramp_up::loop(){
    /*Serial.print("Goal: ");
    Serial.print(goal_speed);
    Serial.print("Change: ");
    Serial.println(change);*/
    //change_time
    unsigned long curr_time = millis();
    unsigned long change_time = curr_time - last_loop_time;
    if (change_time>5){
      last_loop_time = curr_time;
  
      
      if (goal_speed == speed){
          return;
      }
      else if (goal_speed<speed){
          speed -= 7;
      }
      else if (goal_speed>speed){
          speed += 7;
      }
  
  
      if (speed < 0){
          speed = 0;
      }
      else if (speed > 1023){
          speed = 1023;
      }
  
      analogWrite(motor_pin, speed); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
    }
    
}


void motor_ramp_up::set_speed(int new_speed){
    if (new_speed < 0){
        new_speed = 0;
    }
    else if (new_speed > 1023){
        new_speed = 1023;
    }
    goal_speed = new_speed;
    change = goal_speed - speed;
}


#endif
