#include <MLED.h>

int LedEnable = 9;
int blue = 11;
int green =12;
int red = 10;
int delayTime = 500;
int brightness1 = 25;
int brightness2 = 255;
double FadeTime = 500.0;
int NumbOfCycles = 3;

MLED myLED(red, blue, green, LedEnable);  //Upon creating the object the proper pins are setup for you.

void setup() {
                        //Pins for LED are the only ones setup, so nothing needs to be placed here for now...
}

void loop() { //Test all colors of the LED. LED ON -> Delay for viewing -> run fade function on current color -> next color -> repeat.

myLED.Red();
delay(delayTime);

myLED.Fade(FadeTime,NumbOfCycles);

myLED.Blue();
delay(delayTime);

myLED.Fade(FadeTime,NumbOfCycles);

myLED.Green();
delay(delayTime);

myLED.Fade(FadeTime,NumbOfCycles);

myLED.Purple();
delay(delayTime);

myLED.Fade(FadeTime,NumbOfCycles);

myLED.Yellow();
delay(delayTime);

myLED.Fade(FadeTime,NumbOfCycles);

myLED.LightBlue();
delay(delayTime);

myLED.Fade(FadeTime,NumbOfCycles);

myLED.White();
delay(delayTime);

myLED.Fade(FadeTime,NumbOfCycles);


}





