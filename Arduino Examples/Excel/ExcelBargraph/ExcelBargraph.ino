#include <Excel.h>

int SensorCount = 2;   // Number of sensors to setup Excel object
int BaudRate = 19200; // Max communication speed 

Excel myObject(SensorCount); //Create Object for interacting with Excel plug-in

void setup() {
 
  Serial.begin(BaudRate);  //set communication speed

  delay(10);                          //Wait for serial communications to initialize
  
   myObject.ClearData();              //Clear Excel sheet on reboot
   myObject.ClearLabels();            //Clear old labels from excel document
   
   myObject.Labels[0] = "Hi";        //Sample setup of table enteries in an object. Notice enteries are zero indexed.
   myObject.Labels[1] = "There";     
   myObject.PushLabels();            // Update table labels in Excel 
   
   myObject.EnableRowWrite();        //Enable singular row writing


}

void loop()    //Begin Arduino loop
{

double i = 0;  // Created to increment value passed to sin and cos.

while(true)
{
   myObject.Data[0] = sin(i);  //Update Object's data array position 0
   myObject.Data[1] = cos(i);  // Update Object's data array position 1
   
   myObject.PushData();      //Update excel sheet with new array
 
  
i += 0.01;
}

}
