#define VERBOSE 0
// 0 is verbose is off
// 3 is verbose i2c verbose
// 4 is spooler motor
// 5 is tachometer
// 6 is tachometer everything



#define SPEED 850
#define PIN_SCREW_MOTOR 4
#define PIN_SPOOLER_MOTOR 5
#define PIN_FAN_MOTOR 6

#define FAN_SPEED 255

#include "I2cController.h"
#include "motor_ramp_up.h" 
#include "tachometer.h" 

// Define the motor controllers.
motor_ramp_up motor_spooler;
motor_ramp_up motor_screw;
motor_ramp_up motor_fan;

bool has_been_reset = false;

// I2C bus to communicate with User Interface Module.
I2C_Controller i2c_controller;

void setup() {
  // Set up serial.
  Serial.begin(9600);
  Serial.println("Begin");

  // Set up motor drivers.
  motor_screw.setup(PIN_SCREW_MOTOR);
  motor_spooler.setup(PIN_SPOOLER_MOTOR);
  
  // Set up fans.
  motor_fan.setup(PIN_FAN_MOTOR);
  motor_fan.set_speed(FAN_SPEED);

  // Set up tachometer.
  setup_tachometer();
  
  // Set up I2C bus (communicates with User Interface Module).
  i2c_controller.setup();
  
  // Initialize global variables.
  has_been_reset = false;
}


void loop() {
  // Process motor control.
  motor_screw.loop();
  motor_spooler.loop();
  motor_fan.loop();

  // Process tachometer.
  update_taciometer();

  // Get desired screw speed.
  int desired_screw_speed = map(i2c_controller.get_desired_screw_speed(), 0, 1023, 0, 255);
  
  if (has_been_reset){
    motor_screw.set_speed(desired_screw_speed); 
  }
  else{
    motor_screw.set_speed(0); 
  }
  
  if (desired_screw_speed < 4){
    //ensure motor is off on start up
    has_been_reset = true;
    
  }

  int desired_spooler_speed = map(i2c_controller.get_desired_spooler_speed(), 0, 1023, 0, 255);
  motor_spooler.set_speed(desired_spooler_speed);

  
  #if VERBOSE == 3
    Serial.print("I2C: ");
    Serial.print(i2c_controller.get_desired_screw_speed());
    Serial.print(" ");
    Serial.println(i2c_controller.get_desired_spooler_speed());
  #endif
}
