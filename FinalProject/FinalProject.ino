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
const int RVpin = A1;            //Assignment of Module pins
const int TMPpin = A2;
const int OUTpin = A3;
const int EnablePin = 7;

WindMod myModule(RVpin,TMPpin, OUTpin, EnablePin);  //Create Module object

/**************Module variables - end  *****************/

/**************Excel Object variables - start *****************/

const int LabelCount = 3;   // Number of sensors to setup Excel object. Wind speed, Breath per min, calculated threshold, and current temperature.
const int BaudRate = 19200; // Max communication speed 

              /****Used for reference - position of label array****/
               const int WindSpeed = 0;
               const int BreathPerMin = 1;
               const int Threshold = 2;

              

Excel myExcelObject(LabelCount); //Create Object for interacting with Excel plug-in

/**************Excel Object variables - end *****************/


/**************Arduino variables - start *****************/

const int SampleDelay = 100;   //Idle sample rate
const int SampleRate = 35;     // Once activated sample every x ms
const int SamplesNeeded = 3;   //Samples needed
const int MaxCycleTime = 6700; //6700 milliseconds given for breath cycle, 6.7 seconds

int i = 0;  
int BreathIntensity = 0;          //Variable used to store current breath rate
int BreathPerMinTemp = 0;
const int BreathPhases = 3;  //Total amount of cycles to compare before making decision
const int ArraySize = 191;  //Array length is calculated     

float SampleArray[191];    //Create array to fit specifications. array size



  /***Sample Data Variables***/
    bool breakCount = false; //Count how many times the data has crossed the threshold
 
    bool CycleLogic[3] = { false, false, false }; //Three parts to phase needed to be satisfied inhale, exhale, inhale back to trigger low threshold for complete cycle.
 
    bool BreathingCycleInProgress = true;
  

/**************Arduino - end *****************/

void setup() {
  
     Serial.begin(BaudRate); //start Serial bus 
     
     while (!Serial){ ; } // wait for serial port to connect. Needed for native USB port only
       
     myExcelObject.Labels[WindSpeed] =      "Wind Speed (M/S)";
     myExcelObject.Labels[BreathPerMin] =   "Breaths Per Minute" ;
     myExcelObject.Labels[Threshold] =      "Breath Threshold" ;

     myExcelObject.PushLabels();            //Update excel with new labels
     
     //myExcelObject.EnableRowWrite();   //disable comments to switch excel to bar graph mode.

     myModule.calibrate(myLED);         //Function to calibrate the breathing module.Updates object threshold. Takes the LED object as input for proper coloring.     

}

void loop(){

while( BreathIntensity < myModule.lowThreshold )  //While the breath rate is below the threshold do nothing.
{
   i = 0;
   myLED.Purple();                            //Monitor Mode is Purple.
   BreathIntensity = myModule.getCurrentMS(); //Update BreathRate
   delay(SampleDelay);                        //wait for another sample.
   UpdateGraph();
}

if(BreathIntensity >= myModule.lowThreshold)
{
  myLED.Yellow();   //Change LED color
  SampleData();      //Fill Array with sample data

  UpdateGraph();       // Update graph with new BPM
  
}
 
   BreathIntensity = 0;
}

void UpdateGraph()
{
   int minLength = 100;
   
   if( i > minLength)
   {
   myExcelObject.Data[BreathPerMin] =   getBPM(i);
   BreathPerMinTemp =  myExcelObject.Data[BreathPerMin];
   }

   else{
    myExcelObject.Data[BreathPerMin] = BreathPerMinTemp;
   }
   myExcelObject.Data[WindSpeed] =      myModule.getCurrentMS();
   myExcelObject.Data[Threshold] =      myModule.lowThreshold;

   myExcelObject.PushData();      // Update Excel data
}

void SampleData(){
  
  i = 0;
  breakCount = false;
  CycleLogic[0] = false;
  CycleLogic[1] = false;
  CycleLogic[2] = false;
  BreathingCycleInProgress = true;
  

  while( (BreathingCycleInProgress) && ( i != ArraySize - 1) ) // Fill array with breath data and do this untill required breath cycle is calculated or array is at max length
  {
  
   SampleArray[i] = myModule.getCurrentMS();   //Read new value into array
   
   UpdateGraph();

    if( SampleArray[i] > myModule.lowThreshold) //If next sample is also above threshold. To reduce noise.
      {
        CycleLogic[0] = true;
      }

   if( SampleArray[i] < myModule.lowThreshold) //If next sample goes below threshold. exhale detected.
      {
        CycleLogic[1] = true;
        breakCount = true;
      }
      
   if( (SampleArray[i] >= myModule.lowThreshold) && breakCount ) //If next sample is at or above threshold. Last phase detected.
      {
        CycleLogic[2] = true;
      }
    
   if( CycleLogic[0] && CycleLogic[1] && CycleLogic[2])  //if all 3 phases are detected, then breath sample is complete.
     {
       BreathingCycleInProgress = false; 
       i--;
     }

    i ++;
  delay(SampleRate);    
  }
  

}

float getBPM(int indexCount)
{
  const float msPerMin = 60000;  //ms per minute
  return msPerMin/(indexCount*SampleRate); 
}


