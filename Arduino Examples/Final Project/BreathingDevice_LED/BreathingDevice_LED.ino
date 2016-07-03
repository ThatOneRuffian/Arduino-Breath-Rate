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


/**************Excel variables - start *****************/

int SensorCount = 1;   // Number of sensors to setup Excel object
int BaudRate = 19200; // Max communication speed 

Excel myExcelObject(SensorCount); //Create Object for interacting with Excel plug-in

/**************Excel variables - end *****************/


void setup() {
  
     Serial.begin(BaudRate); //start Serial bus
     
     delay(10);    //wait till serial is ready

      myExcelObject.Labels[0] = "Sense"; 
      
      myExcelObject.PushLabels(); 
     
      //myExcelObject.EnableRowWrite();

      myModule.calibrate(myLED);
     
}

void loop(){

myExcelObject.Data[0] = myModule.getCurrentMPH();
 delay(20);
myExcelObject.PushData();
delay(20);
}

