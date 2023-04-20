#include "G2Motor.h"

boolean G2Motor::_flip = false;

// Constructors

G2Motor::G2Motor()
{
	_nSLEEP = 2;
	_nFAULT = 6;
	_DIR = 7;
	_PWM = 9;
	_CS = A0;
}

G2Motor::G2Motor(unsigned char nSLEEP, unsigned char DIR, unsigned char PWM, unsigned char nFAULT, unsigned char CS)
{
	_nSLEEP = nSLEEP;
	_nFAULT = nFAULT;
	_DIR = DIR;
	_PWM = PWM;
	_CS = CS;
}

// Public Methods
void G2Motor::init()
{
	pinMode(_nSLEEP, OUTPUT);
	pinMode(_PWM, OUTPUT);
	pinMode(_nFAULT, INPUT_PULLUP);
	pinMode(_CS, INPUT);
	pinMode(_DIR, OUTPUT);
}

// Set speed for motor, speed is a number betwenn -400 and 400
void G2Motor::setSpeed(int speed) {
	unsigned char reverse = 0;

	if (speed < 0) {
		speed = -speed;  // Make speed a positive quantity
		reverse = 1;  // Preserve the direction
	}
	if (speed > 400)  // Max PWM dutycycle
		speed = 400;

	analogWrite(_PWM, speed * 51 / 80); // map 400 to 255

	// flip if speed was negative or _flip setting is active, but not both
	if (reverse ^ _flip)
	{
		digitalWrite(_DIR, HIGH);
	}
	else
	{
		digitalWrite(_DIR, LOW);
	}
}

// Return error status for motor
unsigned char G2Motor::getFault()
{
	return !digitalRead(_nFAULT);
}

void G2Motor::flip(boolean flip)
{
	G2Motor::_flip = flip;
}

// Enables the MOSFET driver.
void G2Motor::enableDriver()
{
	digitalWrite(_nSLEEP, HIGH);
}

// Puts the MOSFET driver into sleep mode.
void G2Motor::disableDriver()
{
	digitalWrite(_nSLEEP, LOW);
}

unsigned int G2Motor::getCurrentReading()
{
	return analogRead(_CS);
}

// Set voltage offset of current reading at 0 speed.
void G2Motor::calibrateCurrentOffset()
{
	setSpeed(0);
	enableDriver();
	delay(1);
	G2Motor::_offset = getCurrentReading();
}

// Return motor current in milliamps using the gain value for the specific version.
unsigned int G2Motor::getCurrentMilliamps(int gain)
{
	// 5V / 1024 ADC counts / gain mV per A
	// The 24v14, 18v18 and 24v18 results in 244 mA per count.
	// The 18v22 results in 488 mA per count.
	unsigned int mAPerCount = 5000000/1024/gain;
	int reading = (getCurrentReading() - G2Motor::_offset);
	if (reading > 0)
	{
		return reading * mAPerCount;
	}
	return 0;
}

// Return motor current in milliamps for 24v14 version.
unsigned int G2Motor24v14::getCurrentMilliamps()
{
	return G2Motor::getCurrentMilliamps(20);
}

// Return motor current in milliamps for 18v18 version.
unsigned int G2Motor18v18::getCurrentMilliamps()
{
	return G2Motor::getCurrentMilliamps(20);
}

// Return motor current in milliamps for 24v18 version.
unsigned int G2Motor24v18::getCurrentMilliamps()
{
	return G2Motor::getCurrentMilliamps(20);
}

// Return motor current in milliamps for 18v22 version.
unsigned int G2Motor18v22::getCurrentMilliamps()
{
	return G2Motor::getCurrentMilliamps(10);
}
