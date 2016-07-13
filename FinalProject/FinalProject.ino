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
const int ModEnablePin = 7;         //Attached to transistor power supply switch

WindMod myModule(RVpin,TMPpin, OUTpin, ModEnablePin);  //Create Module object

/**************Module variables - end  *****************/

/**************Excel Object variables - start *****************/

const int LabelCount = 3;   // Number of sensors to setup Excel object. Wind speed, Breath per min, calculated threshold, and current temperature.
const int BaudRate = 19200; // Max communication speed 

/****Used for reference - position of label array for excel****/
const int WindSpeed = 0;
const int BreathPerMin = 1;
const int Threshold = 2;

Excel myExcelObject(LabelCount); //Create Object for interacting with Excel plug-in

/**************Excel Object variables - end *****************/

/**************Arduino variables - start *****************/
const int BreathPhases = 3;  //Total amount of cycles to compare before making decision
const int ArraySize = 220;  //Array length is calculated     
const int SampleDelay = 100;   //Idle sample rate
const int SampleRate = 50;     // Once activated sample every x ms
const int SamplesNeeded = 1;   //Samples needed
const int MaxGraphLength = 650;  //As seen from excel plugin.lol...

bool ThresholdBroken = false; //Used to determine if threshold was broken in data
bool CycleLogic[3] = { false, false, false }; //Three parts to phase needed to be satisfied inhale, exhale, inhale back to trigger low threshold for complete cycle.
bool BreathingCycleInProgress = true; //Run while cycle is in progress

int BreathIntensity = 0;   //Variable used to store current breath rate
int i = 0;                 //Indexers

int GraphUpdateCount = 0; //Stores how many times the excel graph has been updated. Used to reset the graph when it goes out of bounds.
int breathCounter = 0;    //Counts the number of complete breaths

float BPMtemp = 0;         //Storage for temp BPM before getting pushed to excel
float SampleArray[220];    //Create array to fit specifications. Max mem.    

/**************Arduino - end *****************/
void setup() {
  
     Serial.begin(BaudRate); //start Serial bus at the desired speed
     
     /******Create and update labels for Excel object******/
     myExcelObject.Labels[WindSpeed] =      "Wind Speed (M/S)";
     myExcelObject.Labels[BreathPerMin] =   "Breaths Per Minute" ;
     myExcelObject.Labels[Threshold] =      "Breath Threshold" ;

     myExcelObject.PushLabels();            //Update excel with new labels
     
     //myExcelObject.EnableRowWrite();      //disable comments to switch excel to bar graph mode.

     myModule.calibrate(myLED);             //Function to calibrate the breathing module. Updates object threshold. Takes the LED object as input for proper coloring.     
     myLED.White();                         //Set first color
     myLED.SetBrightness(100);
     myExcelObject.Reset();
}

void loop(){

  while( BreathIntensity < myModule.lowThreshold )  //While the breath rate is below the threshold do nothing.
  {
    i = 0;                                     //Reset length of sample array.
    BreathIntensity = myModule.getCurrentMS(); //Update current BreathIntensity.
    delay(SampleDelay);                        //wait for another sample.
    UpdateGraph();                             //Update excel sheet.
  }

  if(BreathIntensity >= myModule.lowThreshold)
  {
    SampleData();     //Fill Array with sample data at preset sample rate.
    checkRate();      //Check for which color should be assigned to the current breath count.
    UpdateGraph();    //Update graph with new BPM 
  }
  
   BreathIntensity = 0; //Reset Breath for next round of loop
}

/**************Below are functions I defined to run in the main loop**************/

void UpdateGraph() //Updates excel graph with breath per minute and new intensities
{
  if ( GraphUpdateCount <= MaxGraphLength ){
    
   GraphUpdateCount ++;
   
   myExcelObject.Data[BreathPerMin] =   BPMtemp;
   myExcelObject.Data[WindSpeed] =      myModule.getCurrentMS();
   myExcelObject.Data[Threshold] =      myModule.lowThreshold;

   myExcelObject.PushData();            // Update Excel data
}

else{
  myExcelObject.Reset();
  GraphUpdateCount = 0; 
}
}

void SampleData(){  //Load array with data at defined rate. Checks for breath cycle.
  
  //***********Reset values*************//
  i = 0;
  ThresholdBroken = false;
  CycleLogic[0] = false;
  CycleLogic[1] = false;
  CycleLogic[2] = false;
  BreathingCycleInProgress = true;
  
  while( (BreathingCycleInProgress) && ( i <= ArraySize - 1) ) // Fill array with breath data and do this untill required breath cycle is calculated or array is at max length
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
        ThresholdBroken = true;
      }
      
   if( (SampleArray[i] >= myModule.lowThreshold) && ThresholdBroken ) //If next sample is at or above threshold. Last phase detected.
      {
        CycleLogic[2] = true;
      }
    
   if( CycleLogic[0] && CycleLogic[1] && CycleLogic[2])  //if all 3 phases are detected, then breath sample is complete.
     {
       int MinLength = 4;  //filter incomplete data
       BreathingCycleInProgress = false; // Full breath detected. Exit loop.
       breathCounter ++; // Count successful breath.
       if ( i >= MinLength){
        BPMtemp = getBPM();      
       }
       i--;  //compensate for i decrement. Less steps. Retains correct postion.
     }
    i ++;
   
    delay(SampleRate); 
  }


}

void checkRate()
{

  if (BreathingCycleInProgress) //if buffer overflow aka still think there is a breath in progress.
  {
    myLED.Red();  //set color to red

    /**reste values global values**/
    breathCounter = 0; 
    BPMtemp = 0;
  }
  
  else if( (breathCounter % SamplesNeeded) == 0 ) //used if we want a specific amount of breaths before the calculation is performed
  {
    selectColor();     //select color  based on breathrate.
     
     /**reste values global values**/
     breathCounter = 0;  
  }  

}

void selectColor()
{
  
  int range1[2] = { 0 , 6},  // low - red
      range2[2] = {7 , 11},  // medium - yellow
      range3[2] = {12, 20},  //normal - green
      range4[2] = { 21, 30}; // high - blue
  
   if (BPMtemp >= range1[0] && BPMtemp <= range1[1] ) 
   {
    myLED.Red(); 
   }

   else if (BPMtemp >= range2[0] && BPMtemp <= range2[1] ) 
   {
    myLED.Yellow(); 
   }

   else if (BPMtemp >= range3[0] && BPMtemp <= range3[1] ) 
   {
    myLED.Green(); 
   }

   else if (BPMtemp >= range4[0] && BPMtemp <= range4[1] ) 
   {
    myLED.Blue(); 
   }
  
}

/**************Below are functions I defined to clean up code**************/

float getBPM()
{
  const float msPerMin = 60000;  //ms per minute
  return msPerMin/(i*SampleRate);   //returns the breath rate in breaths per min
}
