#ifndef BUTTONS
#define BUTTONS

#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

// 0 = main menu
// 1 = monitor temp and diameter
// 2 = select from presets
// 3 = manual adjustment of screw speed
// 4 = display preset instructions 
int UIState = 0;

// tracks position of "cursor" when selecting menu options
int menuScroll;

// digital pinout for input buttons
const int PIN_BUTTON_UP = 11;
const int PIN_BUTTON_LEFT = 6;
const int PIN_BUTTON_DOWN = 4;
const int PIN_BUTTON_RIGHT = 8;
const int PIN_POT_LEFT = 0;		// left potentiometer
const int PIN_POT_RIGHT = 1;	// right potentiometer

// left potentiometer's value
float valPotL = 0;

// right potentiometer's value
float valPotR = 0;

// button debouncing variables
int buttonStateLeft;						// the current reading from the input pin
int lastButtonStateLeft = LOW;				// the previous reading from the input pin
int buttonStateBot;							// the current reading from the input pin
int lastButtonStateBot = LOW;				// the previous reading from the input pin
unsigned long lastDebounceTimeLeft = 0;		// the last time the output pin was toggled
unsigned long lastDebounceTimeBot = 0;		// the last time the output pin was toggled
unsigned long debounceDelay = 50;			// the debounce time; increase if the output flickers

byte msb;									// diameter most significant byte
byte lsb;									// diameter least significant byte
short tempD;								// diameter reading
float diameterIn;							// diameter reading from dial indicator
static int speedOutScrew = 0;				// desired screw motor speed
static int speedOutspooler = 0;				// desired spooler motor speed

#define TEMP_PLA 200
#define SPEED_PLA 14

// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

void buttonSetup(){
	// Set up the LCD's number of rows and columns.
	lcd.begin(16, 2);

	// Initialize the pushbutton pins as inputs with pullup resistors.
	pinMode(PIN_BUTTON_LEFT, INPUT_PULLUP);
	pinMode(PIN_BUTTON_DOWN, INPUT_PULLUP);
}

void checkButton(){
	/***********************************LEFT BUTTON**************************************/

	// Read the state of the switch into a local variable.
	int readingLeft = digitalRead(PIN_BUTTON_LEFT);
	// Check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH), and you've waited long enough
	// since the last press to ignore any noise.

	// If the switch changed, due to noise or pressing...
	if (readingLeft != lastButtonStateLeft) {
		// Reset the debouncing timer.
		lastDebounceTimeLeft = millis();
	}

	if ((millis() - lastDebounceTimeLeft) > debounceDelay) {
		// Whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state.

		// If the button state has changed...
		if (readingLeft != buttonStateLeft) {
			buttonStateLeft = readingLeft;

			// If the new button state is HIGH...
			if (buttonStateLeft == HIGH) {
				UIState = 0;
				lcd.clear();
			}
		}
	}

	// Save the reading. Next time through the loop, it'll be the lastButtonState.
	lastButtonStateLeft = readingLeft;

	/***********************************BOTTOM BUTTON**************************************/

	// Read the state of the switch into a local variable.
	int readingBot = digitalRead(PIN_BUTTON_DOWN);
	// Check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH), and you've waited long enough
	// since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing...
	if (readingBot != lastButtonStateBot) {
		// Reset the debouncing timer.
		lastDebounceTimeBot = millis();
	}

	if ((millis() - lastDebounceTimeBot) > debounceDelay) {
		// Whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state.

		// If the button state has changed...
		if (readingBot != buttonStateBot) {
			buttonStateBot = readingBot;

			// If the new button state is HIGH...
			if (buttonStateBot == HIGH) {
				switch (UIState){
				// main menu
				case 0:
					switch (menuScroll){
					case 3:
						// lcd.print("[Prst] Mnl  Mon ");
						UIState = 2;
						break;
					case 2:
						// lcd.print(" Prst [Mnl] Mon ");
						UIState = 3;
						break;
					case 1:
						// lcd.print(" Prst  Mnl [Mon]");
						UIState = 1;
						break;
					}
					break;
				// monitor
				case 1:
					break;
				// presets
				case 2:
					switch (menuScroll){
					case 2:
						// lcd.print("[PLA] PETG ");
						UIState = 4;
						break;
					case 1:
						// lcd.print(" PLA [PETG]");
						UIState = 4;
						break;
					}
					break;
				case 3: // manual control
					//Wire.beginTransmission(4);         // transmit to device 8
					//Wire.write(speedOutScrew);         // sends one byte
					//Wire.endTransmission();            // stop transmitting

					delay(100);
					break;
				}
				lcd.clear();
			}
		}
	}

	// Save the reading. Next time through the loop, it'll be the lastButtonState:
	lastButtonStateBot = readingBot;
}

int getScrewSpeed() {
	return speedOutScrew;
}

int getspoolerSpeed() {
	return speedOutspooler;
}

void UILoop() {
	valPotL = analogRead(PIN_POT_LEFT);    // read the value from the sensor
	valPotR = analogRead(PIN_POT_RIGHT);    // read the value from the sensor

	//not sure about this
	speedOutScrew = valPotL;//map(valPotL,0,1023,255,0);
	speedOutspooler = valPotR;//map(valPotR,0,1023,255,0);

	switch (UIState){
	// Main Menu
	case 0:
		lcd.setCursor(0, 0);
		lcd.print("MENU");

		menuScroll = 4*(valPotR/1023);
		lcd.setCursor(0, 1);

		switch (menuScroll){
		case 3:
			lcd.print("[Prst] Mnl  Mon ");
			break;
		case 2:
			lcd.print(" Prst [Mnl] Mon ");
			break;
		case 1:
			lcd.print(" Prst  Mnl [Mon]");
		break;
		default:
			lcd.print(" Prst  Mnl  Mon ");
			break;
		}
		break;
	// Monitor diameter and temperature.
	case 1:

		lcd.setCursor(0, 0);

		// Request 2 bytes from slave device at address 8.
		Wire.requestFrom(8, 2);

		// Slave may send less than requested.
		while (Wire.available()) {
			// Receive a byte as character.
			msb = Wire.read();
			lsb = Wire.read();
			tempD = msb;
			tempD = tempD << 8 | lsb;
			diameterIn = float(tempD) / 1000;
		}
		delay(500);

		//Serial.println(tempD);

		lcd.print("DIAM: ");
		lcd.print(diameterIn,3);
		lcd.print(" mm   ");
		// lcd.print(" RPM");

		break;
	// select presets for PLA and PETG (not implemented)
	case 2:
		lcd.setCursor(0, 0);
		lcd.print("PRESETS");

		menuScroll = 3*(valPotR/1023);
		lcd.setCursor(0, 1);

		switch (menuScroll){
		case 2:
			lcd.print("[PLA] PETG ");
			break;
		case 1:
			lcd.print(" PLA [PETG]");
			break;
		default:
			lcd.print(" PLA  PETG ");
			break;
		}
		break;
	// manually adjust speed and temperature
	case 3:
		lcd.setCursor(0, 0);

		speedOutScrew = valPotL;//map(valPotL,0,1023,255,0);
		speedOutspooler = valPotR;//map(valPotR,0,1023,255,0);
		lcd.print("SPEED: ");
		if(speedOutScrew < 10){
			lcd.print(" ");
		}
		lcd.print(speedOutScrew);
		lcd.print(" RPM");

		break;
	// display preset instructions
	case 4:
		lcd.setCursor(0,0);
		switch (menuScroll){
		case 2:
			// lcd.print("[PLA] PETG ");
			lcd.print("SET TEMP: ");
			lcd.print(TEMP_PLA);
			lcd.print(" C");
			lcd.setCursor(0,1);
			lcd.print("SET SPD: ");
			lcd.print(SPEED_PLA);
			lcd.print(" RPM");
			break;
		case 1:
			// lcd.print(" PLA [PETG]");
			lcd.print("NOT YET");
			lcd.setCursor(0,1);
			lcd.print("IMPLEMENTED");
			break;
		}
		break;
	}

	checkButton();
}

#endif
