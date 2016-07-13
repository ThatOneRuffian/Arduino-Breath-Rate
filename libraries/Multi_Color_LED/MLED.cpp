/*
    Purpose: Creates object for interacting with the Multicolored LED.

    @author Marcus Mesta
*/

#include "MLED.h"
int MLED::RED, MLED::BLUE, MLED::GREEN, MLED::ENABLE, MLED::UserBrightness = 255;


MLED::MLED(int& Red, int& Blue, int& Green, int& Enable){

this -> ENABLE = Enable;
this -> RED = Red; //Upon creating the object the proper pins are setup for you.
this -> BLUE = Blue;
this -> GREEN = Green;

pinMode(ENABLE, OUTPUT);
pinMode(RED, OUTPUT);                //sets up pins to be output pins.
pinMode(BLUE, OUTPUT);
pinMode(GREEN, OUTPUT);

pinMode(ENABLE, HIGH);                 // Output enabled
analogWrite(ENABLE, 255);           //default full Brightness
digitalWrite(RED, HIGH);            //Default mode off. HIGH is off for this particular LED
digitalWrite(BLUE, HIGH);
digitalWrite(GREEN, HIGH);

}

void MLED::Fade(double fadeTime, int fadeCycles){
  const int MaxRes = 255; //Max resolution on the PWM pin.
  const int delayTime = 1;    //delay 1 ms every loop
  int tempEnable = this -> ENABLE;
  double HalfTime = fadeTime/2.0;  // Time to reach peak of the user's input.

  double fadeamount = MaxRes/HalfTime;


  while( fadeCycles > 0 )
  {



   for ( double CurrentPWM = 255; CurrentPWM > 0; CurrentPWM -= fadeamount)

    {
         analogWrite(tempEnable, CurrentPWM);
         delay(delayTime);
    }

        for ( double CurrentPWM = 0; CurrentPWM < 255; CurrentPWM += fadeamount)
    {

      analogWrite(tempEnable, CurrentPWM);
      delay(delayTime);
    }


    fadeCycles = fadeCycles -1;


}

   analogWrite(this ->ENABLE, this -> UserBrightness);

}

void MLED::SetBrightness(int Val){  //Not compatible with fade function.



if(Val >= 0 && Val <= 255)
{
    this -> UserBrightness = Val;
    analogWrite( this -> ENABLE,  this -> UserBrightness);}

}

void MLED::Purple(void){
   this -> ClearColors();

   digitalWrite(this -> BLUE, LOW);
   digitalWrite(this -> RED, LOW);
}

void MLED::Yellow(void){
   this -> ClearColors();
   digitalWrite(this -> GREEN, LOW);
   digitalWrite(this -> RED, LOW);
}

void MLED::LightBlue(void){
   this -> ClearColors();
   digitalWrite(this -> GREEN, LOW);
   digitalWrite(this -> BLUE, LOW);
}

void MLED::White(void){
   this -> ClearColors();
   digitalWrite(this -> GREEN, LOW);
   digitalWrite(this -> BLUE, LOW);
   digitalWrite(this -> RED, LOW);
}

void MLED::Green(void){
  this -> ClearColors();
  digitalWrite(this -> GREEN, LOW);
}

void MLED::Blue(void){
  this -> ClearColors();
  digitalWrite(this -> BLUE, LOW);
}

void MLED::Red(void){
  this -> ClearColors();
  digitalWrite(this -> RED, LOW);
}

void MLED::ClearColors(void){
digitalWrite(this -> RED,HIGH);
digitalWrite(this -> BLUE,HIGH);
digitalWrite(this -> GREEN,HIGH);
}
