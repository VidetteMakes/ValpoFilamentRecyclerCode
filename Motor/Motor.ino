#include "G2Motor.h"

const int PIN_nSLEEP = 2;
const int PIN_nFAULT = 6;
const int PIN_DIR = 7;
const int PIN_PWM = 9;
const int PIN_CS = A0;

// Initialize motor driver class (dependent on driver version).
G2Motor18v18 md = G2Motor18v18(PIN_nSLEEP, PIN_DIR, PIN_PWM, PIN_nFAULT, PIN_CS);

void stopIfFault() {
	if (md.getFault()) {
		md.disableDriver();
		delay(1);
		Serial.println("Motor fault");
		while (1);
	}
}

void setup() {
  Serial.begin(115200);
  Serial.println("G2 High Power Motor Driver 18v17");
  md.init();
  md.calibrateCurrentOffset();

  delay(10);

  // Uncomment to flip motor's direction:
  //md.flip(true);
}

void loop()
{
	int i = 0;
	md.enableDriver();
	delay(1);  // The driver requires a maximum of 1ms to elapse when brought out of sleep mode.

	Serial.println("Starting");
	for (i = 0; i <= 400; i++)
	{
		md.setSpeed(i);
		stopIfFault();
		if (i % 50 == 0)
		{
			Serial.print("Motor: speed = ");
			Serial.print(i);
			Serial.print(" current = ");
			Serial.println(md.getCurrentMilliamps());
		}
		delay(2);
	}

	for (i = 400; i >= -400; i--)
	{
		md.setSpeed(i);
		stopIfFault();
		if (i % 50 == 0)
		{
			Serial.print("Motor: speed = ");
			Serial.print(i);
			Serial.print(" current = ");
			Serial.println(md.getCurrentMilliamps());
		}
		delay(2);
	}

	for (i = -400; i <= 0; i++)
	{
		md.setSpeed(i);
		stopIfFault();
		if (i % 50 == 0)
		{
			Serial.print("Motor: speed = ");
			Serial.print(i);
			Serial.print(" current = ");
			Serial.println(md.getCurrentMilliamps());
		}
		delay(2);
	}
	
	md.disableDriver(); // Put the MOSFET drivers into sleep mode.
	delay(500);
}
