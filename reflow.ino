#include <math.h>
#include <LiquidCrystal.h>

// manufacturer data for episco k164 10k thermistor
// simply delete this if you don't need it
// or use this idea to define your own thermistors
//#define EPISCO_K164_10k 4300.0f,298.15f,10000.0f  // B,T0,R0
#define EPISCO_G540_100k 4036.0f,298.15f,100000l// B,T0,R0

// Hotbed
//#define HOTBED 3
const int HOTBED =  3;
const int thermistance = 8;

// Temparature que l'on veux obtenir
float consigne=37;

// Pin de l'écran
LiquidCrystal lcd( 8, 9, 4, 5, 6, 7 );

int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 return btnNONE;  // when all others fail, return this...
}

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

// Température par rapport à la consigne (en °C) à partir de laquelle ont controle le hotbed en PWN
int temp_pwm = 5;

// Variation entre la consigne et le tempréature captée
int delta = (consigne - Temp)*100;

int signal_pwm = map(delta, 0, 500, 255, 0); 

 if(delta <= 0) {
   signal_pwm = 255;
 }
 else if(delta >= 500) {
   signal_pwm = 0;
 }
 analogWrite(HOTBED,signal_pwm);

// Sortie pour le graph en python
Serial.println(Temp);
// Sortie lisible sur le terminal
//Serial.print("Temprature : ");
//Serial.print(Temp);
//Serial.print(" | PWM : ");
//Serial.println(signal_pwm);

lcd_key = read_LCD_buttons();
// Action des bouttons
 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
/*   case btnRIGHT:
     {
     lcd.print("RIGHT ");
     break;
     }
   case btnLEFT:
     {
     lcd.print("LEFT   ");
     break;
     }*/
   case btnUP:
     {
     consigne+=0.5;
     break;
     }
   case btnDOWN:
     {
     consigne-=0.5;
     break;
     }
/*   case btnSELECT:
     {
     lcd.print("SELECT");
     break;
     }
     case btnNONE:
     {
     lcd.print("NONE  ");
     break;
     }*/
 }
 
// Affichage sur l'écran
lcd.begin(16, 2);
lcd.print(Temp);
lcd.print(" C");
lcd.setCursor(0, 1);
lcd.print("Consigne : ");
lcd.print(consigne);

delay(1000);
}


