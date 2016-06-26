#include <MLED.h>

int LedEnable = 3;
int blue = 11;
int green =12;
int red = 10;
int delayTime = 500;


MLED myLED(red, blue, green, LedEnable);


void setup() {
 //Upon creating the object the proper pins are setup for you.


pinMode(LedEnable, OUTPUT);

digitalWrite(LedEnable, HIGH);

myLED.SetBrightness(10);
}

void loop() { //Test all colors of the LED. Colors much be cleared before applying another color.

  
myLED.Red();
delay(delayTime);
myLED.ClearColors();

myLED.Blue();
delay(delayTime);
myLED.ClearColors();

myLED.Green();
delay(delayTime);
myLED.ClearColors();

myLED.Purple();
delay(delayTime);
myLED.ClearColors();

myLED.Yellow();
delay(delayTime);
myLED.ClearColors();

myLED.LightBlue();
delay(delayTime);
myLED.ClearColors();

myLED.White();
delay(delayTime);
myLED.ClearColors();


}





