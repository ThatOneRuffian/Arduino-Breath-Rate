/*
    Purpose: Creates object for interacting with the Multicolored LED.

    @author Marcus Mesta
*/

#include "MLED.h"
int MLED::RED, MLED::BLUE, MLED::GREEN, MLED::BRIGHTNESS;

MLED::MLED(int Red, int Blue, int Green, int Brightness){

this -> BRIGHTNESS = Brightness;
this -> RED = Red;
this -> BLUE = Blue;
this -> GREEN = Green;

pinMode(BRIGHTNESS, OUTPUT);
pinMode(RED, OUTPUT);                //sets up pins to be output pins.
pinMode(BLUE, OUTPUT);
pinMode(GREEN, OUTPUT);

digitalWrite(BRIGHTNESS, HIGH);
digitalWrite(RED, HIGH);            //Default mode off. HIGH is off for this particular LED
digitalWrite(BLUE, HIGH);
digitalWrite(GREEN, HIGH);

}

void MLED::SetBrightness(int Val){

if(Val >= 0 && Val <= 255)
{
   analogWrite( this -> BRIGHTNESS, Val);
}

}

void MLED::Purple(void){
Blue();
Red();
}

void MLED::Yellow(void){

  Green();
  Red();
}

void MLED::LightBlue(void){

  Green();
  Blue();
}

void MLED::White(void){

  Green();
  Blue();
  Red();
}

void MLED::Green(void){

  digitalWrite(GREEN, LOW);
}

void MLED::Blue(void){

  digitalWrite(BLUE, LOW);
}

void MLED::Red(void){

  digitalWrite(RED, LOW);
}

void MLED::ClearColors(void){
digitalWrite(RED,HIGH);
digitalWrite(BLUE,HIGH);
digitalWrite(GREEN,HIGH);
}
