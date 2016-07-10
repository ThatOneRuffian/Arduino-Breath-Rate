#include <Excel.h>         //include functions needed for Arduino to talk to PLX plug-in for excel
#include <MLED.h>          // include functions needed to talk to our LED module
#include <BreathModule.h>  // include functions needed to interact with the breath sensor  

/**************LED object variables - start *****************/
int redPin = 12;           //Assignment of LED pins
int bluePin = 11;
int greenPin = 10;
int LEDenablepin = 9;

MLED myLED(redPin, bluePin, greenPin, LEDenablepin);  // Create LED object
/**************LED object variables - end *****************/


/**************Module variables - start *****************/
int RVpin = A1;            //Assignment of Module pins
int TMPpin = A2;
int OUTpin = A3;
int EnablePin = 7;

WindMod myModule(RVpin,TMPpin, OUTpin, EnablePin);  //Create Module object

/**************Module variables - end  *****************/

/**************Excel Object variables - start *****************/

int LabelCount = 4;   // Number of sensors to setup Excel object. Wind speed, Breath per min, calculated threshold, and current temperature.
int BaudRate = 19200; // Max communication speed 

              /****Used for reference - position of label array****/
               int WindSpeed = 0;
               int BreathPerMin = 1;
               int Threshold = 2;
               int Temp = 3;
              

Excel myExcelObject(LabelCount); //Create Object for interacting with Excel plug-in

/**************Excel Object variables - end *****************/


/**************Arduino variables - start *****************/

int BreathRate;          //Variable used to store current breath rate
int SampleDelay = 100;   //Idle sample rate
int SampleRate = 10;     // Once activated sample every 10 ms
int SamplesNeeded = 3;   //Samples needed
int MaxCycleTime = 6700; //6700 milliseconds given for breath cycle, 6.7 seconds

int BreathPhases = 3;  //Total amount of cycles to compare before making decision


const int ArraySize = MaxCycleTime/SampleRate;  //Array length is calculated     

double* SampleArray = new double[ArraySize];    //Create array to fit specifications     

/**************Arduino - end *****************/

void setup() {
  
     Serial.begin(BaudRate); //start Serial bus 
     
     while (!Serial){ ; } // wait for serial port to connect. Needed for native USB port only
       
     myExcelObject.Labels[WindSpeed] =      "Wind Speed (M/S)";
     myExcelObject.Labels[BreathPerMin] =   "Breaths Per Minute" ;
     myExcelObject.Labels[Threshold] =      "Breath Threshold" ;
     myExcelObject.Labels[Temp] =           "Temperature ºC" ;
     myExcelObject.PushLabels();            //Update excel with new labels
     
     //myExcelObject.EnableRowWrite();   //disable comments to switch excel to bar graph mode.

     myModule.calibrate(myLED);         //Function to calibrate the breathing module.Updates object threshold. Takes the LED object as input for proper coloring.     


     
}

void loop(){


 

while( BreathRate < myModule.lowThreshold )  //While the breath rate is below the threshold do nothing.
{
   myLED.Purple();                       //Monitor Mode is Purple.
   BreathRate = myModule.getCurrentMS(); //Update BreathRate
   delay(SampleDelay);                   //wait for another sample.
}

if(BreathRate >= myModule.lowThreshold)
{
  myLED.White();   //Change LED color to light blue
  int i = 0;    
  bool breakCount = false; //Count how many times the data has crossed the threshold
  bool CycleLogic[3] = { false, false, false }; //Three parts to phase needed to be satisfied inhale, exhale, inhale back to trigger low threshold for complete cycle.
  int BreathingCycleInProgress = true;


  /********* Sample Data - Start *********/ 
  while( (BreathingCycleInProgress) && ( i != ArraySize - 1) ) // Fill array with breath data and do this untill required breath cycle is calculated or array is at max length
  {
  
   SampleArray[i] = myModule.getCurrentMS();   //Read new value into array

    if( SampleArray[i] > myModule.lowThreshold) //If next sample is also above threshold. To reduce noise.
      {
        CycleLogic[0] = true;
      }

   if( SampleArray[i] < myModule.lowThreshold) //If next sample goes below threshold. exhale detected.
      {
        CycleLogic[1] = true;
        breakCount = true;
      }
      
   if( SampleArray[i] >= myModule.lowThreshold && breakCount ) //If next sample is at or above threshold. Last phase detected.
      {
        CycleLogic[2] = true;
      }
    
    if( CycleLogic[0] && CycleLogic[1] && CycleLogic[2])  //if all 3 phases are detected, then breath sample is complete.
    {
      BreathingCycleInProgress = false; 
      i --; //Perserve i value on exit of while loop
    }

    i ++;
  }

  /********* Sample Data - End *********/ 


  
  /********* Calculate Breath length - start  *********/ 


  /********* Calculate Breath length - end  *********/ 

  
}



}

