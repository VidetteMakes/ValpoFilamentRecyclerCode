#define VERBOSE 0
// 0 is verbose is off
// 3 is verbose i2c verbose
// 4 is spooler motor
// 5 is tachometer
// 6 is tachometer everything



#define SPEED 850
#define SCREW_MOTOR_PIN 4
#define spooler_MOTOR_PIN 5
#define FAN_PIN 6

#define FAN_SPEED 255

#include "I2cController.h"
#include "motor_ramp_up.h" 
#include "tachometer.h" 

motor_ramp_up spooler_motor;
motor_ramp_up screw_motor;
motor_ramp_up fan;

bool has_been_reset = false;

I2C_Controller i2c_controller;

void setup() {
  Serial.begin(9600);           //  setup serial
  Serial.println("Begin");           //  setup serial


  screw_motor.setup(SCREW_MOTOR_PIN);
  spooler_motor.setup(spooler_MOTOR_PIN);
  
  fan.setup(FAN_PIN);
  fan.set_speed(FAN_SPEED);

  setup_tachometer();
  
  i2c_controller.setup();
  has_been_reset = false;
}


void loop() {

  screw_motor.loop();
  spooler_motor.loop();
  fan.loop();

  update_taciometer();

  int desired_screw_speed = map(i2c_controller.get_desired_screw_speed(),0,1023,0,255);
  if (has_been_reset){
    screw_motor.set_speed(desired_screw_speed); 
  }else{
    screw_motor.set_speed(0); 
    
  }
  if (desired_screw_speed<4){
    //ensure motor is off on start up
    has_been_reset = true;
    
  }


  int desired_spooler_speed = map(i2c_controller.get_desired_spooler_speed(),0,1023,0,255);
  spooler_motor.set_speed(desired_spooler_speed);

  
  #if VERBOSE == 3
    Serial.print("I2C: ");
    Serial.print(i2c_controller.get_desired_screw_speed());
    Serial.print(" ");
    Serial.println(i2c_controller.get_desired_spooler_speed());
  #endif
}
