#define ALPHA_LIGHT 0.0001
#define EDGES_PER_REVOLUTION 6.0
#define VERBOSE 1

#define BUFFER_TOP 6
#define BUFFER_BOTTOM 2

#define SPEED 1023
#define MOTOR_PIN 11
#define MOROR_RAMP_UP 1000

#include "slack_sensor.h"
#include "motor_ramp_up.h"

const int top_slack_pin = A0;
const int bottom_slack_pin = A2; 

slack_sensor my_slack_sensor;
motor_ramp_up spooler_motor;

void turn_motor_on(){
    #ifdef VERBOSE
    Serial.println("Turn motor On");
    #endif
    
    spooler_motor.set_speed(SPEED);
    
  digitalWrite(13, HIGH);  // sets the pin as output

}

void turn_motor_off(){
    Serial.println("Turn motor Off"); 
    
    spooler_motor.set_speed(0);
    //analogWrite(MOTOR_PIN, 0); // analogRead values go from 0 to 1023, analogWrite values from 0 to 255

  
  digitalWrite(13, LOW);  // sets the pin as output
}

void setup() {
  Serial.begin(9600);           //  setup serial

  my_slack_sensor.setup(top_slack_pin,bottom_slack_pin);

  spooler_motor.setup(MOTOR_PIN,1000);
  pinMode(13, OUTPUT);  // sets the pin as output
}


void loop() {

  spooler_motor.loop();
  //Serial.println(spooler_motor.get_speed());

  int slack_sensor_output = my_slack_sensor.loop();

  if (slack_sensor_output == my_slack_sensor.HIT_BOTTOM){
    turn_motor_on();
  }
  else if (slack_sensor_output == my_slack_sensor.HIT_TOP){
    turn_motor_off() ; 
  }

  #if VERBOSE == 2
    my_slack_sensor.printData();
  #endif
}
