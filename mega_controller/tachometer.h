/*
 *
 * This code caluclates the rpm of the barrel
 * 
 */

#ifndef TACHOMETER_H
#define TACHOMETER

#define ALPHA_LIGHT 0.01
#define BLACK_LINES_PER_REVOLUTION 3.0
#define ALPHA_RPM 0.35
#define DEADZONE 10

const int PhotoIn = A1;

float average_light = 0;

void set_average_light(int new_value){
  average_light = (ALPHA_LIGHT * new_value) + (1.0 - ALPHA_LIGHT) * average_light;  
}

int get_average_light(){
  return (int) average_light;
}

float average_period = 0;
unsigned long last_edge_found = -1;
int State = 0;

void record_edge(){
  if (last_edge_found == -1){
    last_edge_found = millis();
  }else{
    unsigned long current_ms = millis();
    unsigned long period = current_ms - last_edge_found;
    last_edge_found = current_ms;
    average_period = (ALPHA_RPM * (float)period) + (1.0 - ALPHA_RPM) * average_period;
  }
  
}

// use this function to get rpm
int get_rpm(){
  if (average_period == 0) {
    return 0;
  }else{
    return (int) (60000.0/(average_period * BLACK_LINES_PER_REVOLUTION));
  }
}

// setup the pins
void setup_tachometer(){
  
  pinMode(PhotoIn, INPUT);
  average_light = analogRead(PhotoIn);
}

// run an update and return the status
#define WHITE_LINE 0
#define BLACK_LINE 1
#define UNKNOWN_LINE 2
short last_reading = UNKNOWN_LINE;
void update_taciometer(){
  
  State = analogRead(PhotoIn);
     // Serial.println(State);
  set_average_light(State);
  int thres = get_average_light();
  short new_state;




  #if (VERBOSE == 6)
      Serial.print("Current Reading: ");  
      Serial.print(State);
      Serial.print(" Average Value: ");  
      Serial.print(thres);
      Serial.print(" RPM: ");  
      Serial.println(get_rpm());
  #endif



  
  if (State > (thres + DEADZONE)){
    //found a black Line
    new_state = BLACK_LINE;
  }else if (State < (thres - DEADZONE)){
    //White Line
    new_state = WHITE_LINE;
  }else{
    return;
  }

  
  if ((new_state != last_reading) && (last_reading != UNKNOWN_LINE)){
    if (new_state == WHITE_LINE){
      
   #if (VERBOSE == 5)
      Serial.print(" White Line");  
   #endif
         
   #if (VERBOSE == 6)
      Serial.print(" White Line");  
   #endif
    }else{
      record_edge();
      
   #if (VERBOSE == 5)
      Serial.print(" Black Line");  
   #endif           
   #if (VERBOSE == 6)
      Serial.print(" Black Line");  
   #endif
    }
  #if (VERBOSE == 5)
      Serial.print(" RPM: ");  
      Serial.println(get_rpm());
  #endif
  }
  last_reading = new_state;



}

#endif
