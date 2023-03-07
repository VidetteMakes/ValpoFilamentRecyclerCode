#ifndef BUTTONS
#define BUTTONS

const unsigned long DEBOUNCE_MS = 50;	// button debounce time

/** @brief  Read a button that's on a pullup resistor.
 *  @return true if button has been pressed; false otherwise
 */
bool ButtonPressed(int pin, bool *lastState, unsigned long *lastTime) {
	// return value
	bool buttonPressed = false;
	
	// Read button.
	int state = digitalRead(pin);
	
	// If button has changed...
	if (state != *lastState) {
		// Remember the time.
		*lastTime = millis();
	}

	// If the button has been pressed long enough...
	if ((millis() - *lastTime) > DEBOUNCE_MS) {
		// If the button has changed...
		if (state != lastState) {
			// If the button is LOW...
			if (state == LOW) {
				// Button press detected!
				buttonPressed = true;
			}
		}
	}

	// Save the reading.
	lastState = state;
	
	return buttonPressed;
}

#endif
