// https://learn.adafruit.com/i2c-spi-lcd-backpack
#include "Adafruit_LiquidCrystal.h"

// https://learn.adafruit.com/matrix-keypad/
#include "Adafruit_Keypad.h"

#define KEYPAD_PID1332					// membrane 1x4 keypad

const int R1 = 13;						// pin for keypad row 1
const int C2 = 12;						// pin for keypad column 2
const int C1 = 11;						// pin for keypad column 1
const int C4 = 10;						// pin for keypad column 4
const int C3 = 9;						// pin for keypad column 3

#include "keypad_config.h"				// leave after pin definitions

static int speedFans = 0;				// desired fan speed
static int speedSpool = 0;				// desired spooler speed

// Initialize display class, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

// Initialize instance of class keypad.
Adafruit_Keypad keypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// At startup...
void setup() {
	// Set up serial port (with baud rate).
	Serial.begin(115200);

	// Set up keypad.
	keypad.begin();
	
	// Set up display (with number of columns and rows).
	lcd.begin(16, 2);

	// Print placeholding text on the display.
	lcd.setCursor(0, 0);
	lcd.print("FANS       SPOOL");

	// Print settings.
	PrintSetting(speedFans, 0, 1);
	PrintSetting(speedSpool, 10, 1);
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
				// Increase fan speed.
				AdjustSetting(&speedFans, 1, 0, 1);
				break;
			// If button 2 is pressed...
			case '2':
				// Decrease fan speed.
				AdjustSetting(&speedFans, -1, 0, 1);
				break;
			case '3':
				// Increase spool speed.
				AdjustSetting(&speedSpool, 1, 10, 1);
				break;
			case '4':
				// Decrease spool speed.
				AdjustSetting(&speedSpool, -1, 10, 1);
				break;
			}
		}
	}
}

void AdjustSetting(int *setting, int adjustment, int column, int row) {
	// Change speed.
	*setting += adjustment;
	
	// Print setting.
	Serial.println(*setting);
	
	// Print setting.
	PrintSetting(*setting, column, row);
}

void PrintSetting(int setting, int column, int row) {
	// Clear the display
	lcd.setCursor(column, row);
	lcd.print("    ");

	lcd.setCursor(column, row);
	lcd.print(setting);
}
