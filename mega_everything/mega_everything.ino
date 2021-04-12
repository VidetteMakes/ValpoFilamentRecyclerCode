#define ALPHA_LIGHT 0.001
#define BLACK_LINES_PER_REVOLUTION 3.0
#define ALPHA_RPM 0.35
#define VERBOSE 1


#include "Wire.h"
#include <PID_v1.h>

const int potPin = A3;    // select the input pin for the left potentiometer
#define motorPin 5

float potVal = 0;
int motorVal = 0;

// Define pin locations and delay time between each query
#define EncoderPinA  2
#define EncoderPinB  3
 
// Create a variable in memory that the interrupt code can access
// Also create the change value globally
volatile unsigned int EncoderPos = 0;
unsigned long lastmillis = 0; 
double rpm = 0;     // rpm reading

volatile int lastEncoded = 0;

double rollerSet, rollerIn, rollerOut;
PID rollerPID(&rollerIn, &rollerOut, &rollerSet,.6,4,0, DIRECT);

double screwSet, screwIn, screwOut;
PID screwPID(&screwIn, &screwOut, &screwSet,1,1,0, DIRECT);

void EncoderEvent();


#define ENCODER_READ_TIME 50
#define MAX_RPM 270
#define pulsesperturn 980
const double ENCODER_POS_TO_RPM  = 60000/((double) pulsesperturn*ENCODER_READ_TIME);


byte motorSpeedUI;
byte uiControl = 0;


const int PhotoIn = A1;
const int LED = 13;
int State = 0;

int led_pin = 4; 
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
    return (int) (60000.0/(average_period * BLACK_LINES_PER_REVOLUTION));
  }
}



void setup() {
  pinMode(13, OUTPUT);    // sets the digital pin 13 as output
  
  pinMode(led_pin, OUTPUT);
  pinMode(PhotoIn, INPUT);


  

  // Set input pin state for the line sensors
  pinMode(EncoderPinA, INPUT);
  pinMode(EncoderPinB, INPUT);
  digitalWrite(EncoderPinA, HIGH); //turn pullup resistor on
  digitalWrite(EncoderPinB, HIGH); //turn pullup resistor on
  // Register a hardware interupt function for pin 2 (which is indexed by value 0)
  // This function sets the EncoderEvent() function to be called whenever a line detector detects a change
  attachInterrupt(0, EncoderEvent, CHANGE);
  attachInterrupt(1, EncoderEvent, CHANGE);

  //turn the PID on
  rollerPID.SetMode(AUTOMATIC);



  // I2C to receive motor speed from UI
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output



  
  Serial.begin(9600);
  Serial.println(ENCODER_POS_TO_RPM);
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
    if (new_state == WHITE_LINE){
      Serial.print(" White Line");
    }else{
      record_edge();
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

  if ( millis() - lastmillis >= ENCODER_READ_TIME ){  // Uptade every one second, this will be equal to reading frecuency (Hz).
 //Don't process interrupts during calculations
     detachInterrupt(0);
     detachInterrupt(1);

     rpm = (double) EncoderPos  * ENCODER_POS_TO_RPM  ;  
     
     EncoderPos = 0; // Restart the RPM counter
     
     lastmillis = millis(); // Uptade lasmillis
     
     //Restart the interrupt processing
     attachInterrupt(0, EncoderEvent, CHANGE);
     attachInterrupt(1, EncoderEvent, CHANGE);
     

  
     potVal = analogRead(potPin);
     rollerSet = map(potVal, 0, 1023, 0, MAX_RPM);
     rollerIn = rpm ;

     
     rollerPID.Compute();
     /*Serial.print("RPM: ");
     Serial.print(rpm);
     
     Serial.print(" rollerIn: ");
     Serial.print(rollerIn);
     Serial.print(" rollerSet: ");
     Serial.print(rollerSet);
      Serial.print(" rollerOut: ");
      Serial.println(rollerOut);
     
      */
  

    if (rollerSet < 0.1){
      rollerOut = 0;
    }
    
    analogWrite(motorPin, rollerOut);
 

  }

  if(uiControl == 1){
    screwSet = motorSpeedUI;
    //Serial.println(screwSet);
    screwIn = get_rpm();

    // screwPID.Compute();

    if(screwSet < 1){
      screwOut = 0;
    }

    // analogWrite(led_pin, screwOut);
  }


}


// Encoder event for the interrupt call
void EncoderEvent()
{
  int MSB = digitalRead(EncoderPinA); //MSB = most significant bit
  int LSB = digitalRead(EncoderPinB); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

//clockwise movement
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) EncoderPos --;
//counter-clockwise movement  
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) EncoderPos ++;

  lastEncoded = encoded; //store this value for next time

  
}



void receiveEvent(int howMany){
  motorSpeedUI = Wire.read();
  Serial.println(motorSpeedUI);
  //uiControl = 1;
}
