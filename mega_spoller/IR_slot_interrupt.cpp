#include "IR_slot_interrupt.h"
#include "Arduino.h"

IR_slot_interrupt::IR_slot_interrupt()
{
  alpha =  0.0001;
  average_light = 0;
}

int IR_slot_interrupt::setup(int setAnalogPin, int setbuffer){
  analogPin = setAnalogPin;
  IR_buffer = setbuffer;
  average_light = (float) analogRead(analogPin);  // read the input pin
  return 0;
}

int IR_slot_interrupt::loop(){
  float temp = (float) analogRead(analogPin);  // read the input pin
  set_average_light(temp);
  val = temp - get_average_light() - IR_buffer;
  return 0;
}

void IR_slot_interrupt::set_average_light(int new_value){
  average_light = (alpha * new_value) + (1.0 - alpha) * average_light;
}

int IR_slot_interrupt::get_average_light(){
  return (int) average_light;
}

int IR_slot_interrupt::get_value(){
  return val;
}
