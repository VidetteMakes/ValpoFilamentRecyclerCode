/*
 * This code will slowly increase the speed of the motors 
 * so that these is not too high a current throught the system
 */

#ifndef MOTOR_RAMP_UP_H
#define MOTOR_RAMP_UP_H

#include "Arduino.h"

class motor_ramp_up
{
private:
	int motorPin = 0;
    unsigned long timestamp;
    unsigned long motorRampTime;
    unsigned int motorRampStep;
    byte speedCurrent;
    byte speedGoal;
public:
    motor_ramp_up();
	
    // Define motor pins and ramp speed. The motor's output will change by
	// 'rampStep' counts every `rampTime' ms.
    void setup(int pin, int rampTime, int rampStep);
	
	// Process motor speed.  Call in main loop.
    void loop();
	
	// Get the current speed.
    byte get_speed();
	
	// Set the desired speed.
    void set_speed(byte speed);
};

/* Example

#include "motor_ramp_up.h"

const int PIN_MOTOR = 11;

motor_ramp_up motor;

void setup() {
  motor.setup(MOTOR_PIN, 3, 5);
}

void loop() {
  motor.loop();

  if (TURN MOTOR ON){
    motor.set_speed(1023);
  }
  else if (TURN MOTOR OFF){
    motor.set_speed(0);
  }
}
  
*/

#endif
