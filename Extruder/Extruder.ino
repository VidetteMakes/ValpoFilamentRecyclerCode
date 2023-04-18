// TODO:  Button press resets speed to zero.
// TODO:  Implement new motor driver.
// TODO:  Display motor current.

// https://learn.adafruit.com/i2c-spi-lcd-backpack
#include "Adafruit_LiquidCrystal.h"

const int PIN_ENCODER_CLK = 2;			// rotary encoder pin
const int PIN_ENCODER_DT = 3;			// rotary encoder pin
const int PIN_MOTOR_SCREW = 5;			// PWM motor control
const int LCD_COL_SCREW = 1;			// LCD motor speed location
const int SPEED_MIN = -12;				// min allowed speed (reverse)
const int SPEED_MAX = 12;				// max allowed speed (forward)

static int speed = 0;					// desired motor speed
volatile int change = 0;				// rotary encoder change

// Initialize display class, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

// At startup...
void setup() {
	// Set motor pins as output.
	pinMode(PIN_MOTOR_SCREW, OUTPUT);
	
	// Set up serial port (with baud rate).
	Serial.begin(115200);

	// Set up display (with number of columns and rows).
	lcd.begin(16, 2);

	// Print placeholding text on the display.
	lcd.setCursor(0, 0);
	lcd.print("SCREW SPEED");

	// Print settings.
	AdjustSetting(PIN_MOTOR_SCREW, &speed, 0, LCD_COL_SCREW, 1);
	
	// Set encoder pins as inputs with pull up resistors.
	pinMode(PIN_ENCODER_CLK, INPUT_PULLUP);
	pinMode(PIN_ENCODER_DT, INPUT_PULLUP);
	
	// Call updateEncoder when any high/low changed seen on pins 2 or 3.
	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_CLK), updateEncoder, CHANGE);
	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_DT), updateEncoder, CHANGE);
}

// Over and over...
void loop() {
	// If the encoder has turned...
	if (change != 0) {
		// Adjust motor speed.
		Serial.print("Screw = ");
		AdjustSetting(PIN_MOTOR_SCREW, &speed, change, LCD_COL_SCREW, 1);
		
		// Reset flag.
		change = 0;
	}
}

void AdjustSetting(int pin, int *setting, int adjustment, int column, int row) {
	// Change speed.
	*setting += adjustment;

	// Bound at min and max.
	if (*setting > SPEED_MAX) { *setting = SPEED_MAX; }
	else if (*setting < SPEED_MIN) { *setting = SPEED_MIN; }

	// Update PWM (and scale appropriately).
	analogWrite(pin, map(*setting, 0, SPEED_MAX, 0, 255));

	// Print setting to serial port and LCD.
	Serial.println(*setting);

	// Clear the display
	lcd.setCursor(column, row);
	lcd.print("    ");

	// Print number.
	lcd.setCursor(column, row);
	lcd.print(*setting);
}

void updateEncoder() {
	// Remember the state between interrupts.
	static uint8_t state = 0;
	
	// Read encoder pins.
	bool CLKstate = digitalRead(PIN_ENCODER_CLK);
	bool DTstate = digitalRead(PIN_ENCODER_DT);
	
	switch (state) {
	case 0:							// Idle state, encoder not turning
		if (!CLKstate) {			// If PIN_ENCODER_CLK goes low first...
			state = 1;				// We're turning clockwise.
		} else if (!DTstate) {		// If PIN_ENCODER_DT goes low first...
			state = 4;				// We're turning counterclockwise.
		}
		break;
	case 1:							// Clockwise rotation
		if (!DTstate) {				// If PIN_ENCODER_DT goes low...
		state = 2;					// We're continuing clockwise.
		}
		break;
	case 2:							// Continuing clockwise
		if (CLKstate) {				// If PIN_ENCODER_CLK goes high...
			state = 3;				// We're continuing clockwise.
		}
		break;
	case 3:							// Continuing clockwise
		if (CLKstate && DTstate) {  // If both PIN_ENCODER_CLK and PIN_ENCODER_DT now high...
			state = 0;				// One step clockwise is complete.
			change = 1;				// Set the flag.
		}
		break;
	// Anticlockwise rotation; like clockwise but with PIN_ENCODER_CLK and PIN_ENCODER_DT reversed.
	case 4: if (!CLKstate) { state = 5; } break;
	case 5: if (DTstate)   { state = 6; } break;
	case 6:
		if (CLKstate && DTstate) {
			state = 0;
			change = -1;			// Set the flag.
		}
		break; 
	}
}
