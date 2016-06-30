/*
    Purpose: Creates object for interacting with the Multicolored LED.

    @author Marcus Mesta
*/

#include "MLED.h"
int MLED::RED, MLED::BLUE, MLED::GREEN, MLED::ENABLE;

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

void MLED::SetBrightness(int& Val){

if(Val >= 0 && Val <= 255)
{ analogWrite( this -> ENABLE, Val);}

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

void MLED::Fade(double& FadeCycleTime)
{
    /*
    const double MaxVal = 255; //Max PWM speed for arduino.

    double i; // while loop to delay fade

    if (FadeCycleTime > 0)
    {
        const double PeakTime = (FadeCycleTime/2.0); //time required to reach MaxVal


        while()
        {

       SetBrightness(i);
        delay()
        }
    }
*/

}

void MLED::ClearColors(void){
digitalWrite(this -> RED,HIGH);
digitalWrite(this -> BLUE,HIGH);
digitalWrite(this -> GREEN,HIGH);
}
