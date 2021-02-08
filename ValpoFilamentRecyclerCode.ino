#define ALPHA_LIGHT 0.001
#define EDGES_PER_REVOLUTION 6.0
#define ALPHA_RPM 0.001

const int PhotoIn = A0;
const int LED = 13;
int State = 0;


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

void setup() {
  pinMode(PhotoIn, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  State = analogRead(PhotoIn);
  set_average_light(State);
  int thres = get_average_light();
  if (State > thres){
    Serial.println("Black Line");
  }else{
    Serial.println("White Line");   
  }
  
}
