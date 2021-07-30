#include "Wire.h"
#include "motor_ramp_up.h" 

// Pin definitions
static int PIN_SCREW_MOTOR = 4;
static int PIN_SPOOLER_MOTOR = 5;
static int PIN_FAN_MOTOR = 6;

// I2C Slave Address
static int I2C_ADDRESS = 8;

// Define the motor controllers.
motor_ramp_up motorSpooler;
motor_ramp_up motorScrew;
motor_ramp_up motorFan;

int motorSpeedScrew;		// desired screw motor speed
int motorSpeedSpooler;		// desired spooler motor speed
int motorSpeedFan;			// desired fan motor speed

void setup() {
  // Initialize global variables.
  motorSpeedScrew = 0;
  motorSpeedSpooler = 0;
  motorSpeedFan = 1023;
  
  // Set up serial.
  Serial.begin(9600);

  // Set motor control pins as output.
  pinMode(PIN_SCREW_MOTOR, OUTPUT);
  pinMode(PIN_SPOOLER_MOTOR, OUTPUT);
  pinMode(PIN_FAN_MOTOR, OUTPUT);
  
  // Set up motor drivers.
//  motorScrew.setup(PIN_SCREW_MOTOR, 3, 5);
//  motorSpooler.setup(PIN_SPOOLER_MOTOR, 3, 5);
//  motorFan.setup(PIN_FAN_MOTOR, 3, 5);
  
  // Set up I2C bus (as slave).
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);
}


void loop() {
  // Process motor control.
  motorScrew.loop();
  motorSpooler.loop();
  motorFan.loop();

  // Update screw speed.
  int desired_screw_speed = map(motorSpeedScrew, 0, 1023, 0, 255);
//motorScrew.set_speed(desired_screw_speed);
	analogWrite(PIN_SCREW_MOTOR, desired_screw_speed);

  // Update spooler speed.
  int desired_spooler_speed = map(motorSpeedSpooler, 0, 1023, 0, 255);
//motorSpooler.set_speed(desired_spooler_speed);
  analogWrite(PIN_SPOOLER_MOTOR, desired_spooler_speed);

  // Update fan speed.
  int desiredFanSpeed = map(motorSpeedFan, 0, 1023, 0, 255);
//motorFan.set_speed(desiredFanSpeed);
  analogWrite(PIN_FAN_MOTOR, desiredFanSpeed);

  Serial.print(motorSpeedScrew);
  Serial.print(" ");
  Serial.print(motorSpeedSpooler);
  Serial.print(" ");
  Serial.println(motorSpeedFan);

  delay(200);
}

// Read incoming I2C data from UI Module.
void receiveEvent(int numBytes) {
  byte c1 = 0;	// received byte

  // For each byte received...
  for(int j = 0; j < numBytes; j++) {
	  // Read the byte.
    byte data = Wire.read();

	  // The first two bytes are the desired screw motor speed.
    if (j == 0) {
       c1 = data;
    }

    if (j == 1) {
       motorSpeedScrew = (int)(c1 << 8) + (int) data;
    }

	  // The 2nd two bytes are the desired spooler motor speed.
    if (j == 2) {
       c1 = data;
    }

    if (j == 3) {
       motorSpeedSpooler = (int)(c1 << 8) + (int) data;
    }
	
    // The 3rd two bytes are the desired fan speed.
    if (j == 4) {
      c1 = data;
    }
    
    if (j == 5) {
      motorSpeedFan = (int)(c1 << 8) + (int) data;
    }
  }
}
