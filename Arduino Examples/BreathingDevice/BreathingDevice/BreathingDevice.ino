#include <Excel.h>
#include <BreathModule.h>



int RVpin = A1;
int TMPpin = A2;
int OUTpin = A3;
int EnablePin = 7;

int SensorCount = 2;   // Number of sensors to setup Excel object
int BaudRate = 19200; // Max communication speed 

Excel myExcelObject(SensorCount); //Create Object for interacting with Excel plug-in
WindMod myModule(RVpin,TMPpin, OUTpin, EnablePin);


void setup() {
  
     Serial.begin(BaudRate); 
     
     delay(10);  

     myModule.EnableMod();

      myExcelObject.Labels[0] = "Sense"; 
      
      myExcelObject.PushLabels(); 
     
      myExcelObject.EnableRowWrite();

}

void loop(){

myExcelObject.Data[0] = myModule.getCurrentMPH();


myExcelObject.PushData();

}

