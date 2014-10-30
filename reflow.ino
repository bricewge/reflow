#include <math.h>

// manufacturer data for episco k164 10k thermistor
// simply delete this if you don't need it
// or use this idea to define your own thermistors
#define EPISCO_K164_10k 4300.0f,298.15f,10000.0f  // B,T0,R0
#define EPISCO_G540_100k 4036.0f,298.15f,100000.0f  // B,T0,R0

// Hotbed
//#define HOTBED 3
const int HOTBED =  3;

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

float Temperature(int AnalogInputNumber,float B,float T0,float R0,float R_Balance)
{
//  for(int i= 0; i < 5; ++) {
    float R,T;

//  R=1024.0f*R_Balance/float(analogRead(AnalogInputNumber)))-R_Balance;
    R=R_Balance*(1024.0f/float(analogRead(AnalogInputNumber))-1);

    T=1.0f/(1.0f/T0+(1.0f/B)*log(R/R0));
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
float Temp=Temperature(0,EPISCO_G540_100k,100000.0f);
Serial.println(Temp);

if(consigne >= Temp) {
   digitalWrite(HOTBED,LOW);
}
else {
   digitalWrite(HOTBED,HIGH);
}

   
 delay(500);
}


