#define ALPHA_LIGHT 0.001
#define EDGES_PER_REVOLUTION 6.0
#define ALPHA_RPM 0.35
#define VERBOSE 1

#define  DIAL_CLK_PIN    4 //yellow
#define  DIAL_DATA_PIN   5 //white


#include "data_process.h"
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"

const int PhotoIn = A1;
const int LED = 13;
int State = 0;

int led_pin = 3; 
int pot_pin = A0;
int output;
int led_value;

float average_light = 0;

void set_average_light(int new_value){
  average_light = (ALPHA_LIGHT * new_value) + (1.0 - ALPHA_LIGHT) * average_light;
  
}

int get_average_light(){
  return (int) average_light;
}


float average_period = 0;
unsigned long last_edge_found = -1;

void record_edge(){
  if (last_edge_found == -1){
    last_edge_found = millis();
  }else{
    unsigned long current_ms = millis();
    unsigned long period = current_ms - last_edge_found;
    Serial.print(" Period ");
    Serial.print(period);
    last_edge_found = current_ms;
    average_period = (ALPHA_RPM * (float)period) + (1.0 - ALPHA_RPM) * average_period;
  }
  
}

int get_rpm(){
  if (average_period == 0) {
    return 0;
  }else{
    return (int) (60000.0/(average_period * EDGES_PER_REVOLUTION));
  }
}

// Connect via i2c, default address #0 (A0-A2 not jumpered)
Adafruit_LiquidCrystal lcd(0);

void setup() {
  pinMode(13, OUTPUT);    // sets the digital pin 13 as output
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  lcd.setBacklight(HIGH);
   
   // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);
  // print the number of seconds since reset:
  lcd.print("RPM: ");

  
  CaliperSetup(DIAL_CLK_PIN,DIAL_DATA_PIN);

  
  pinMode(led_pin, OUTPUT);
  pinMode(PhotoIn, INPUT);
  Serial.begin(9600);
}

#define WHITE_LINE 0
#define BLACK_LINE 1
#define UNKNOWN_LINE 2
#define DEADZONE 20
short last_reading = UNKNOWN_LINE;
void update_taciometer(){
  State = analogRead(PhotoIn);
     // Serial.println(State);
  set_average_light(State);
  int thres = get_average_light();
  short new_state;
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
    record_edge();
    if (new_state == WHITE_LINE){
      Serial.print(" White Line");
    }else{
      Serial.print(" Black Line");      
    }
    Serial.print(" RPM: ");  
    Serial.println(get_rpm());
  }
  last_reading = new_state;
}

void loop() {
  update_taciometer();

  // speed controller

  //Reading from potentiometer
  output = analogRead(pot_pin);
  //Serial.println(output);


  //Mapping the Values between 0 to 255 because we can give output
  //from 0 -255 using the analogwrite funtion
  led_value = map(output, 0, 1023, 0, 255);
  analogWrite(led_pin, led_value);
  if (led_value ==0){
    digitalWrite(13, LOW);
  }else{
    digitalWrite(13, HIGH);
  }


  long CaliperValueMain = CaliperLoop();
  if (CaliperValueMain != -1){
    Serial.println(CaliperValueMain);
  }

   // set the cursor to column 0, line 1
  lcd.setCursor(5, 0);
  lcd.print("    ");
  
   // set the cursor to column 0, line 1
  lcd.setCursor(5, 0);
  lcd.print(get_rpm());
}
