#include <Excel.h>
#include <MLED.h>
#include <BreathModule.h>

/**************LED object variables - start *****************/
int redPin = 12;
int bluePin = 11;
int greenPin = 10;
int LEDenablepin = 9;

MLED myLED(redPin, bluePin, greenPin, LEDenablepin);  // Create LED object
/**************LED object variables - end *****************/


/**************Module variables - start *****************/
int RVpin = A1;
int TMPpin = A2;
int OUTpin = A3;
int EnablePin = 7;

WindMod myModule(RVpin,TMPpin, OUTpin, EnablePin);  //Create Module object

/**************Module variables - end  *****************/


/**************Excel Object variables - start *****************/

int SensorCount = 2;   // Number of sensors to setup Excel object
int BaudRate = 19200; // Max communication speed 

Excel myExcelObject(SensorCount); //Create Object for interacting with Excel plug-in

/**************Excel Object variables - end *****************/

void setup() {
  
     Serial.begin(BaudRate); //start Serial bus
     
     delay(10);    //wait till serial is ready

      myExcelObject.Labels[0] = "Wind Speed MPH";
      
      myExcelObject.Labels[1] = "Standard Dev."; 
      
      myExcelObject.PushLabels(); 
     
      //myExcelObject.EnableRowWrite();  //disable comments to switch excel to plot graph mode.

      myModule.calibrate(myLED);         //passes led object to the calibrate function can perform a series of functions.
     
}

void loop(){
  
myExcelObject.Data[0] = myModule.getCurrentMPH();
delay(20);
myExcelObject.PushData();
delay(20);

}

