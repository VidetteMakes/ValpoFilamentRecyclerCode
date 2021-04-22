#include "slack_sensor.h"

slack_sensor::slack_sensor()
{
    top_interrupt = new IR_slot_interrupt();
    bottom_interrupt = new IR_slot_interrupt();
}


int slack_sensor::setup(int analogPinTop,int analogPinBottom){
    top_interrupt->setup(analogPinTop,DEFAULT_BUFFER);
    bottom_interrupt->setup(analogPinBottom,DEFAULT_BUFFER);
    consecutive_top_hits= 0;
    consecutive_bottom_hits = 0;
}

int slack_sensor::loop(){
    top_interrupt->loop();
    bottom_interrupt->loop();
    if (top_interrupt->get_value()>0){
        consecutive_top_hits ++;
    }else{
        consecutive_top_hits = 0;
    }
    
    if (bottom_interrupt->get_value()>0){
        consecutive_bottom_hits ++;
    }else{
        consecutive_bottom_hits = 0;
    }

    if (consecutive_top_hits >= DEFAULT_CONCSECUTIVE_HITS){
        return HIT_TOP;
    } else if (consecutive_bottom_hits >= DEFAULT_CONCSECUTIVE_HITS){
        return HIT_BOTTOM;
    }

    return NOTHING_HAPPENED;
    
}

void slack_sensor::printData(){
  Serial.print("Top Value: ");
  Serial.print(top_interrupt->get_value());
  Serial.print(" Top Average: ");
  Serial.print(top_interrupt->get_average_light());
  Serial.print(" Bottom Value: ");
  Serial.print(bottom_interrupt->get_value());
  Serial.print(" Bottom Average: ");
  Serial.print(bottom_interrupt->get_average_light());
  Serial.print("\n");
}

/*
An example use case of the slack sensor.

#include "slack_sensor.h"

const int top_slack_pin = A0;
const int bottom_slack_pin = A2; 

slack_sensor my_slack_sensor;


void setup() {
  my_slack_sensor.setup(top_slack_pin,bottom_slack_pin);

}


void loop() {


  int slack_sensor_output = my_slack_sensor.loop();

  if (slack_sensor_output == my_slack_sensor.HIT_BOTTOM){
    //Hit bottom
  }
  else if (slack_sensor_output == my_slack_sensor.HIT_TOP){
    //Hit Top
  }

}
 */
