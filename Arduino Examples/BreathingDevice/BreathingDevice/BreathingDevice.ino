#include <Excel.h>
#include <BreathModule.h>
//#include <math.H>


int RVpin = A1;
int TMPpin = A2;
int OUTpin = A3;


int SensorCount = 1;   // Number of sensors to setup Excel object
int BaudRate = 19200; // Max communication speed 

Excel myObject(SensorCount); //Create Object for interacting with Excel plug-in
WindMod myModule(RVpin,TMPpin, OUTpin);


void setup() {
  
     Serial.begin(BaudRate); 
     
     delay(10);  

      myObject.Labels[0] = "Sense"; 
      
      myObject.PushLabels(); 
     
      myObject.EnableRowWrite();

}

void loop(){

//myObject.Data[0] = myModule.getCurrentMPH();

myObject.Data[0] = myModule.getTempF();

myObject.PushData();

}

