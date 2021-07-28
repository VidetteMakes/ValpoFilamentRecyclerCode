#ifndef BUTTONS
#define BUTTONS

const int PIN_BUTTON_UP = 11;			// pin for "up" button
const int PIN_BUTTON_LEFT = 6;			// pin for "left" button
const int PIN_BUTTON_DOWN = 4;			// pin for "down" button
const int PIN_BUTTON_RIGHT = 8;			// pin for "right" button

// button debouncing variables
int buttonStateLeft;					// the current reading from the input pin
int lastButtonStateLeft = LOW;			// the previous reading from the input pin
int buttonStateBot;						// the current reading from the input pin
int lastButtonStateBot = LOW;			// the previous reading from the input pin
unsigned long lastDebounceTimeLeft = 0;	// the last time the output pin was toggled
unsigned long lastDebounceTimeBot = 0;	// the last time the output pin was toggled
unsigned long debounceDelay = 50;		// the debounce time; increase if the output flickers

// TODO:  Change this into something that can be called for multiple buttons.
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
				// TODO:  Toggle fans on and off.
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
				// TODO:  Something???
			}
		}
	}

	// Save the reading. Next time through the loop, it'll be the lastButtonState:
	lastButtonStateBot = readingBot;
}

#endif
