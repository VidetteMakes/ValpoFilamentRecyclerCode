// include the library code:
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include "buttons.h"

const int I2C_ADDR_CONTROLLER = 8;		// address of controller Arduino
const int PIN_POT_LEFT = 0;				// left potentiometer
const int PIN_POT_RIGHT = 1;			// right potentiometer

float valPotL = 0;						// left potentiometer's value
float valPotR = 0;						// right potentiometer's value
static int speedScrew = 0;				// desired screw motor speed
static int speedSpooler = 0;			// desired spooler motor speed

// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

void setup() {
	// Set up the LCD's number of rows and columns.
	lcd.begin(16, 2);

	// Initialize the pushbutton pins as inputs with pullup resistors.
	pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
	pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);

	// Set up I2C (as master).
	Wire.begin();

	// Set up serial port.
	Serial.begin(9600);
}

void loop() {
	Serial.print(speedScrew);
	Serial.print(" ");
	Serial.println(speedSpooler);

	// Read potentiometer values.
	valPotL = analogRead(PIN_POT_LEFT);
	valPotR = analogRead(PIN_POT_RIGHT);

	// Convert to PWM values.
	speedScrew = valPotL;//map(valPotL,0,1023,255,0);
	speedSpooler = valPotR;//map(valPotR,0,1023,255,0);

	// Print settings on LCD display.
	lcd.setCursor(0, 0);
	lcd.print("SPEED: ");
	if(speedScrew < 10){
		lcd.print(" ");
	}
	lcd.print(speedScrew);
	lcd.print(" RPM");

	checkButton();
	
	sendI2C();
}

void sendI2C(){
	Wire.beginTransmission(I2C_ADDR_CONTROLLER);
	
	// Send the desired screw speed.
	int data = speedScrew;
	char c1 = data >> 8;
	char c2 = data & 0x00ff;
	Wire.write(c1);              // sends one byte
	Wire.write(c2);              // sends one byte

	// Send the desired spooler speed.
	data = speedSpooler;
	c1 = data >> 8;
	c2 = data & 0x00ff;
	Wire.write(c1);              // sends one byte
	Wire.write(c2);              // sends one byte
	
	// Send the desired fan speed.
	data = 

	Wire.endTransmission();            // stop transmitting 
}
