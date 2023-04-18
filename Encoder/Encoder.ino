// Source:
// https://forum.arduino.cc/t/reading-rotary-encoders-as-a-state-machine/937388

// Rotary Encoder Inputs
const int PIN_ENCODER_CLK = 2;
const int PIN_ENCODER_DT = 3;

// Counts up or down depending which way the encoder is turned.
volatile int counter = 0;

// Flag to indicate that the value of inputDelta should be printed
volatile int encoderFlag = false;

// At startup...
void setup() {
	// Disable interrupts during setup.
	noInterrupts();
	
	// Set encoder pins as inputs with pull up resistors.
	pinMode(PIN_ENCODER_CLK, INPUT_PULLUP);
	pinMode(PIN_ENCODER_DT, INPUT_PULLUP);

	// Setup Serial Monitor
	Serial.begin(115200);
	
	// Call updateEncoder when any high/low changed seen on pins 2 or 3.
	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_CLK), updateEncoder, CHANGE);
	attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_DT), updateEncoder, CHANGE);

	// Enable interrupts.
	interrupts();
}

// Over and over...
void loop() {
	if (encoderFlag == true) {			// If the encoder has turned...
		Serial.println(counter);		// Print the counter.
		encoderFlag = false;			// Reset flag.
	}
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
			++counter;				// Increment the counter.
			encoderFlag = true;		// Set the flag.
		}
		break;
	// Anticlockwise rotation; like clockwise but with PIN_ENCODER_CLK and PIN_ENCODER_DT reversed.
	case 4: if (!CLKstate) { state = 5; } break;
	case 5: if (DTstate)   { state = 6; } break;
	case 6:
		if (CLKstate && DTstate) {
			state = 0;
			--counter;				// Decrement the counter.
			encoderFlag = true;		// Set the flag.
		}
		break; 
	}
}
