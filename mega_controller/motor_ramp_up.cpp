#include "motor_ramp_up.h"

motor_ramp_up::motor_ramp_up()
{
    timestamp = 0;
    speedCurrent = 0;
    speedGoal = 0;
}

void motor_ramp_up::setup(int pin, int rampTime, int rampStep) {
	// Remember settings.
    motorPin = pin;
    motorRampTime = rampTime;
    motorRampStep = rampStep;
	
	// Set motor pin as output.
    pinMode(motorPin, OUTPUT);
}

void motor_ramp_up::loop() {
	// Calculate elapsed time.
    unsigned long timeCurrent = millis();
    unsigned long timeElapsed = timeCurrent - timestamp;
	
	// If elapsed time is greater than ramp time...
	if (timeElapsed > motorRampTime) {
		// Remember the current time.
		timestamp = timeCurrent;

		// If the motor's current speed is less than a single step away from
		// the desired speed...
		if ((speedGoal < (speedCurrent + motorRampStep)) && 
			(speedGoal > (speedCurrent - motorRampStep))) {
			// Update the motor's speed.
			speedCurrent = speedGoal;
		}
		// If the motor still needs to speed up.
		else if (speedGoal < speedCurrent) {
			// Increase speed by the step size.
			speedCurrent -= motorRampStep;
		}
		// If the motor still needs to slow down...
		else if (speedGoal > speedCurrent) {
			// Decrease speed by the step size.
			speedCurrent += motorRampStep;
		}

		// Update the motor's PWM pin (analogWrite values go from 0 to 255).
		analogWrite(motorPin, speedCurrent);
	}
}

void motor_ramp_up::set_speed(byte speed) {
    speedGoal = speed;
}

byte motor_ramp_up::get_speed() {
	return speedCurrent;
}

