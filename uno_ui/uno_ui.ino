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
static int speedSpool = 0;				// desired spooler motor speed

// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

void setup() {
	// Set up the LCD's number of rows and columns.
	lcd.begin(16, 2);

  // Print placeholding text on the display.
  lcd.setCursor(0, 0);
  lcd.print("SCREW:       RPM");
  lcd.setCursor(0, 1);
  lcd.print("SPOOL:       RPM");

	// Initialize the pushbutton pins as inputs with pullup resistors.
	pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
	pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);

	// Set up I2C (as master).
	Wire.begin();

	// Set up serial port.
	Serial.begin(9600);
}

void loop() {
	// Read potentiometer values.
	speedScrew = analogRead(PIN_POT_LEFT);
	speedSpool = analogRead(PIN_POT_RIGHT);

	// Print settings on LCD display (converting to RPM when able).
  UpdateDisplay(0, map(speedScrew, 0, 1023, 0, 14));
  UpdateDisplay(1, speedSpool);

	checkButton();

  // Send speed data over serial port.
  Serial.print(speedScrew);
	Serial.print(" ");
	Serial.println(speedSpool);
	
  // Send speed data over I2C (to controller module).
	sendI2C();

  // Pause for effect.
  delay(100);
}

// Print settings on LCD display.
void UpdateDisplay(int row, int number) {
  lcd.setCursor(7, row);
	if (number < 10) {
    lcd.print("   ");
  }
  else if (number < 100) {
    lcd.print("  ");
  }
  else if (number < 1000) {
    lcd.print(" ");
  }

	lcd.print(number);
}

// Send speed data over I2C (to controller module).
void sendI2C() {
	Wire.beginTransmission(I2C_ADDR_CONTROLLER);
	
	// Send the desired screw speed.
	int data = speedScrew;
	char c1 = data >> 8;
	char c2 = data & 0x00ff;
	Wire.write(c1);              // sends one byte
	Wire.write(c2);              // sends one byte

	// Send the desired spooler speed.
	data = speedSpool;
	c1 = data >> 8;
	c2 = data & 0x00ff;
	Wire.write(c1);              // sends one byte
	Wire.write(c2);              // sends one byte
	
	// TODO:  Send the desired fan speed.

	Wire.endTransmission();            // stop transmitting 
}
