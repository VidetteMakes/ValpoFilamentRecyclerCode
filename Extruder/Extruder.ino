// TODO:  Button press resets speed to zero.

// https://learn.adafruit.com/i2c-spi-lcd-backpack
#include "Adafruit_LiquidCrystal.h"

#include "G2Motor.h"

// Pin definitions
const int PIN_ENCODER_CLK = 2;			// rotary encoder pin
const int PIN_ENCODER_DT = 3;			// rotary encoder pin
const int PIN_nSLEEP = 4;				// motor driver enable
const int PIN_nFAULT = 6;				// motor fault detection
const int PIN_DIR = 7;					// motor direction control
const int PIN_PWM = 9;					// motor speed control
const int PIN_CS = A0;					// motor current measurement

const int LCD_COL_SPEED = 1;			// LCD motor speed location
const int LCD_COL_CURRENT = 9;			// LCD current location
const int SPEED_MIN = -12;				// min allowed speed (reverse)
const int SPEED_MAX = 12;				// max allowed speed (forward)

static int speed = 0;					// desired motor speed
volatile int change = 0;				// rotary encoder change
unsigned long timestamp = 0;			// used for scheduling

// Initialize display class, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

// Initialize motor driver class (dependent on driver version).
G2Motor18v18 md = G2Motor18v18(PIN_nSLEEP, PIN_DIR, PIN_PWM, PIN_nFAULT, PIN_CS);

// At startup...
void setup() {
	// Initialize motor pins,  pause for calibration.
	md.init();
	
	// Calibrate current measurement and pause for completion.
	md.calibrateCurrentOffset();
	delay(10);
	
	// Disable sleep mode and give the driver time to respond.
	md.enableDriver();
	delay(1);
	
	// Set up serial port (with baud rate).
	Serial.begin(115200);

	// Set up display (with number of columns and rows).
	lcd.begin(16, 2);

	// Print placeholding text on the display.
	lcd.setCursor(0, 0);
	lcd.print("SPEED    Current");

	// Print settings.
	AdjustSetting(&speed, 0);
	
	// Set encoder pins as inputs with pull up resistors.
	pinMode(PIN_ENCODER_CLK, INPUT_PULLUP);
	pinMode(PIN_ENCODER_DT, INPUT_PULLUP);
	
	// Call UpdateEncoder when any high/low changed seen on pins 2 or 3.
	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_CLK), UpdateEncoder, CHANGE);
	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_DT), UpdateEncoder, CHANGE);
}

// Over and over...
void loop() {
	// If the encoder has turned...
	if (change != 0) {
		// Adjust motor speed.
		AdjustSetting(&speed, change);
		
		// Reset flag.
		change = 0;
	}

	// Every so often...
	if (millis() >= timestamp + 1000) {
        timestamp += 1000;
        
		// If there is a motor fault...
		if (md.getFault()) {
			// Disable the motor and give the driver time to respond.
			md.disableDriver();
			delay(1);
			
			// Alert the user.
			lcd.setCursor(0, 0);
			lcd.print("Motor fault");
			Serial.println("Motor fault");
	
			// Wait for reset.
			while (1);
		}
	
		// Measure motor current.
		int current = md.getCurrentMilliamps();
		Serial.print("current = ");
		Serial.println(current);
		
		// Clear the display
		lcd.setCursor(LCD_COL_CURRENT, 1);
		lcd.print("    ");
		
		// Print the current.
		lcd.setCursor(LCD_COL_CURRENT, 1);
		lcd.print(current);
	}
}

void AdjustSetting(int *setting, int adjustment) {
	// Change speed.
	*setting += adjustment;

	// Bound at min and max.
	if (*setting > SPEED_MAX) { *setting = SPEED_MAX; }
	else if (*setting < SPEED_MIN) { *setting = SPEED_MIN; }

	// Scale appropriately.
	int pwm = map(*setting, 0, SPEED_MAX, 0, 400);

	// Update PWM.
	md.setSpeed(pwm);

	// Print setting to serial port and LCD.
	Serial.print("PWM = ");
	Serial.println(pwm);

	// Clear the display
	lcd.setCursor(LCD_COL_SPEED, 1);
	lcd.print("    ");

	// Print number.
	lcd.setCursor(LCD_COL_SPEED, 1);
	lcd.print(*setting);
}

// When the rotary encoder is turned...
void UpdateEncoder() {
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
