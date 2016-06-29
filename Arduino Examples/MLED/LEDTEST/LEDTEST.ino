#include <MLED.h>
int LEDBrightness = 100;
int LedEnable = 3;
int blue = 11;
int green =12;
int red = 10;
int delayTime = 500;


MLED myLED(red, blue, green, LedEnable);  //Upon creating the object the proper pins are setup for you.

void setup() {

myLED.SetBrightness(LEDBrightness); // set to ~ half power. 255 is max. Must be passed as a variable.
                        //Pins for LED are the only ones setup, so nothing needs to be placed here for now...
}

void loop() { //Test all colors of the LED. Colors much be cleared before applying another color.

myLED.Red();
delay(delayTime);


myLED.Blue();
delay(delayTime);


myLED.Green();
delay(delayTime);


myLED.Purple();
delay(delayTime);


myLED.Yellow();
delay(delayTime);


myLED.LightBlue();
delay(delayTime);


myLED.White();
delay(delayTime);


}





