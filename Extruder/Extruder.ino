// https://learn.adafruit.com/i2c-spi-lcd-backpack
#include "Adafruit_LiquidCrystal.h"

// https://learn.adafruit.com/matrix-keypad/
#include "Adafruit_Keypad.h"

#define KEYPAD_PID1332					// membrane 1x4 keypad

const int PIN_MOTOR_SCREW = 5;			// pin for screw motor

const int R1 = 12;						// pin for keypad row 1
const int C2 = 11;						// pin for keypad column 2
const int C1 = 10;						// pin for keypad column 1
const int C4 = 9;						// pin for keypad column 4
const int C3 = 8;						// pin for keypad column 3

#include "keypad_config.h"				// leave after pin definitions

const int LCD_COL_SCREW = 1;			// location of screw speed

const int SPEED_MAX = 12;				// max allowed speed

static int speedScrew = 0;				// desired screw speed

// Initialize display class, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

// Initialize instance of class keypad.
Adafruit_Keypad keypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// At startup...
void setup() {
	// Set motor pins as output.
	pinMode(PIN_MOTOR_SCREW, OUTPUT);
	
	// Set up serial port (with baud rate).
	Serial.begin(9600);

	// Set up keypad.
	keypad.begin();
	
	// Set up display (with number of columns and rows).
	lcd.begin(16, 2);

	// Print placeholding text on the display.
	lcd.setCursor(0, 0);
	lcd.print("SCREW SPEED");

	// Print settings.
	AdjustSetting(PIN_MOTOR_SCREW, &speedScrew, 0, LCD_COL_SCREW, 1);
}

// Over and over...
void loop() {
	// Process keypad events.
	keypad.tick();

	// While keypad events are available...
	while (keypad.available()) {
		// Read the event.
		keypadEvent e = keypad.read();

		// If the event is a keypress...
		if (e.bit.EVENT == KEY_JUST_PRESSED) {
			switch (e.bit.KEY) {
			// If button 1 is pressed...
			case '1':
				// Increase screw speed.
				Serial.print("Screw = ");
				AdjustSetting(PIN_MOTOR_SCREW, &speedScrew, 1, LCD_COL_SCREW, 1);
				break;
			// If button 2 is pressed...
			case '2':
				// Decrease screw speed.
				Serial.print("Screw = ");
				AdjustSetting(PIN_MOTOR_SCREW, &speedScrew, -1, LCD_COL_SCREW, 1);
				break;
			// If button 3 is pressed...
			case '3':
				// Do nothing.
				Serial.println("Button 3");
				break;
			// If button 4 is pressed...
			case '4':
				// Do nothing.
				Serial.println("Button 4");
				break;
			}
		}
	}
}

void AdjustSetting(int pin, int *setting, int adjustment, int column, int row) {
	// Change speed.
	*setting += adjustment;

	// Wrap between 0 and max.
	if (*setting > SPEED_MAX) { *setting = 0; }
	else if (*setting < 0) { *setting = SPEED_MAX; }

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
