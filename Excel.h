#ifndef EXCEL
#define EXCEL

#include "Arduino.h"
#include "WString.h"  //library to allow the use of Arduino string objects.

//String must be defined here because they are a pare of the Arduino libraries and will not work inside the class.
extern String InvaildLen;
extern String InvalidData;
extern String RowWrite;

class Excel
{


       public:

            Excel(int);     // User Inputs

            static double Data[];    //stores equivalent cell element for excel update. Max 26 characters.
            static String Labels[];   //Holds the table names for excel

            void EnableRowWrite(void); //Enablee writing to a singular row
            void DisableRowWrite(void); //Disable row write.

            void PostMSG(String);   //post message to app
            void ClearMSG(void);     //clear app message

            void PushData(void);    //push data stored in arduino to excel
            void PushLabels(void);  //Update String labels to excel sheet

            void ClearData(void);    //Clears excel sheet
            void ClearLabels(void);

       private:
            static bool RowWrite;
            static bool Flag;
            static int UserLength; //default length for number of Data sources eg. sensors
            static int UserInput;
            static const unsigned int MaxMSGLength;  //Max number of message characters allowed on PLX-DAQ
            static const unsigned int DataLength;    //Max number of data that may be sent.



};




#endif EXCEL
