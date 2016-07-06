#include <Excel.h>   //include functions needed for Arduino to talk to PLX plug-in for excel
#include <MLED.h>   // include functions needed to talk to our LED module
#include <BreathModule.h>  // include functions needed to interact with the breath sensor  

/**************LED object variables - start *****************/
int redPin = 12;          //Assignment of LED pins
int bluePin = 11;
int greenPin = 10;
int LEDenablepin = 9;

MLED myLED(redPin, bluePin, greenPin, LEDenablepin);  // Create LED object
/**************LED object variables - end *****************/


/**************Module variables - start *****************/
int RVpin = A1;     //Assignment of Module pins
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
     
     delay(10);    //wait till serial is ready. May require other options

      myExcelObject.Labels[0] = "Wind Speed Meters/Second"; // First Excel Label
      
      myExcelObject.Labels[1] = "Threshold Meters/Second";              // Second Excel Label
       
      myExcelObject.PushLabels();                    //Update excel with new labels
     
      //myExcelObject.EnableRowWrite();   //disable comments to switch excel to bar graph mode.

      myModule.calibrate(myLED);         //Function to calibrate the breathing module. Takes the LED object as input for proper coloring.     
}

void loop(){
  
myExcelObject.Data[0] = myModule.getCurrentMS(); //Set data entry to current windspeed in m\s
myExcelObject.Data[1] = myModule.lowThreshold;   //Output current threshold for debugging purposes.
delay(20);    //wait 20 ms
myExcelObject.PushData(); //update excel data
delay(20);

}

