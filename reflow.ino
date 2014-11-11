#include <math.h>

// manufacturer data for episco k164 10k thermistor
// simply delete this if you don't need it
// or use this idea to define your own thermistors
//#define EPISCO_K164_10k 4300.0f,298.15f,10000.0f  // B,T0,R0
#define EPISCO_G540_100k 4036.0f,298.15f,100000l// B,T0,R0

// Hotbed
//#define HOTBED 3
const int HOTBED =  15;
const int thermistance = 8;

// Temparature que l'on veux obtenir
float consigne=37;

// Temperature function outputs float , the actual 
// temperature
// Temperature function inputs
// 1.AnalogInputNumber - analog input to read from 
// 3.Thermistor B parameter - found in datasheet 
// 4.Manufacturer T0 parameter - found in datasheet (kelvin)
// 5. Manufacturer R0 parameter - found in datasheet (ohms)
// 6. Your balance resistor resistance in ohms  

float Temperature(int AnalogInputNumber,float B,float T0,long R0,float R_Balance)
{
//  for(int i= 0; i < 5; ++) {
    float R,T;

//  R=1024.0f*R_Balance/float(analogRead(AnalogInputNumber)))-R_Balance;
    R=R_Balance*(1024.00f/float(analogRead(AnalogInputNumber))-1);

    T=1.00f/(1.00f/T0+(1.00f/B)*log(R/R0));
    T-=273.15f;
    //T=+T;
  //   }
//T=T/5;

  return T;
}

// example of use #1
// reading from analog input 1, using episco k164 definition
// and 10k balance, getting result in celsius

void setup() {
 Serial.begin(9600);
}

void loop() {
// Temperature actuelle
float Temp=Temperature(thermistance,EPISCO_G540_100k,96300.0f);

int delta = (consigne - Temp)*100;
int signal_pwm = map(delta, 0, 500, 255, 0); 

 if(delta <= 0) {
    analogWrite(HOTBED,255);
 }
 else if(delta >= 500) {
    analogWrite(HOTBED,0);
 }
 else {
   analogWrite(HOTBED,signal_pwm);
 }
 
//Serial.print("Temprature : ");
Serial.println(Temp);
//Serial.print(" | PWM : ");
//Serial.println(signal_pwm);

 delay(100);
}


