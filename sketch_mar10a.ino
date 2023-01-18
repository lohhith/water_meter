#include <LiquidCrystal.h>
int rsPin = 6;
int enPin = 7;
int d4Pin = 8;
int d5Pin = 9;
int d6Pin = 10;
int d7Pin = 11;
LiquidCrystal myLcd(rsPin,enPin,d4Pin,d5Pin,d6Pin,d7Pin);

//Basically it is sahil's project here we get introduced to the concept of the interupt 

float volume = 0.0;
float flow_Rate_m;
float flow_Rate_s;


// It is called frequency because every time fan takes one rotation one pulse is passed 
//and the below variable is incrimented and since we are doing the calculation for every one second it is 
//called as the frequency you will get to know about it in next lines of the code

volatile int flow_Frequency; 
int sensor_Pin = 2;
unsigned long int current_Time;
unsigned long int taime_Came;

//this func take place with highest priority in arduino when the interupt pulse is received
//irrespective of what microcontroller is doing 

void intrpt_fun(){
  flow_Frequency = flow_Frequency + 1;
}

void setup() {
  pinMode(sensor_Pin,INPUT);
  Serial.begin(9600);
  myLcd.begin(16,2);
  myLcd.clear();
  myLcd.setCursor(0,0);
  myLcd.print("water flow meter");
  delay(500);
  myLcd.setCursor(0,1);
  myLcd.print("By SAHIL G");
  delay(3000);
  

  // the below line of the code add an interupt list and we will also pass the triggering thing
  //for this interupt to happen and what function should run when this interupt activates
  //this interupt list are given the highest priority and this gets activated and call their func
  //irrespective of whatever going on

  attachInterrupt(digitalPinToInterrupt(sensor_Pin),intrpt_fun,RISING);
  current_Time = millis();
  taime_Came = current_Time;
}


void loop() {
  current_Time = millis();
  if(current_Time >= (taime_Came + 1000)){
    taime_Came = current_Time;
    if(flow_Frequency != 0){

      //this is formula derived by hall effect sensor given in data sheet of the sensor
      //pulse freq = 7.5*(flow_rate in L/min)

      flow_Rate_m = (flow_Frequency/7.5);
      flow_Rate_s = (flow_Rate_m/60);
      volume = (volume + flow_Rate_s);

      Serial.print("Rate= ");
      Serial.print(flow_Rate_m);
      Serial.print("L/min");

      myLcd.clear();
      myLcd.setCursor(0,0);
      myLcd.print("Rate= ");
      myLcd.print(flow_Rate_m);
      myLcd.print("L/min");
      myLcd.setCursor(0,1);
      myLcd.print("Rate= ");
      myLcd.print(flow_Rate_s);
      myLcd.print("L/sec");
      
      
      flow_Frequency = 0;
    }

    else{
      Serial.println("please pass the water");
      
      delay(1000);
      myLcd.clear();
      myLcd.setCursor(0,0);
      myLcd.print("vol flown= ");
      myLcd.print(volume);
      myLcd.print("L");
      myLcd.setCursor(0,1);
      myLcd.println("pass the water");
      
    }
  }
}
