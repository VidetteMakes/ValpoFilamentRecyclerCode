#pragma once

#include <Arduino.h>

class G2Motor
{
  public:
    // CONSTRUCTORS
    G2Motor();
    G2Motor(unsigned char nSLEEP,
            unsigned char DIR,
            unsigned char PWM,
            unsigned char nFAULT,
            unsigned char CS);

    // PUBLIC METHODS
    void init();
    void setSpeed(int speed); // Set speed.
    unsigned char getFault(); // Get fault reading.
    void flip(boolean flip); // Flip the direction of the speed.
    void enableDriver(); // Enables the MOSFET driver.
    void disableDriver(); // Puts the MOSFET driver into sleep mode.
    unsigned int getCurrentReading();
    void calibrateCurrentOffset();
    unsigned int getCurrentMilliamps(int gain);

  protected:
    unsigned int _offset;

  private:
    unsigned char _PWM;
    static const unsigned char _PWM_TIMER1_PIN = 9;	// can also use 10
    unsigned char _nSLEEP;
    unsigned char _DIR;
    unsigned char _nFAULT;
    unsigned char _CS;
    static boolean _flip;

};

class G2Motor24v14 : public G2Motor
{
  public:
    using G2Motor::G2Motor;
    unsigned int getCurrentMilliamps(); // Get current reading.
};

class G2Motor18v18 : public G2Motor
{
  public:
    using G2Motor::G2Motor;
    unsigned int getCurrentMilliamps(); // Get current reading.
};

class G2Motor24v18 : public G2Motor
{
  public:
    using G2Motor::G2Motor;
    unsigned int getCurrentMilliamps(); // Get current reading.
};

class G2Motor18v22 : public G2Motor
{
  public:
    using G2Motor::G2Motor;
    unsigned int getCurrentMilliamps(); // Get current reading.
};
