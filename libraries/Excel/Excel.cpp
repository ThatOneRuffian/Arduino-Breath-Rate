#include "Excel.h"

//String must be defined here because they are a pare of the Arduino libraries and will not work inside the class.
String InvaildLen = "Invalid Message Length";
String InvalidData = "Invalid Number of data sources entered.";
String RowWriteCommand = "ROW,SET,2";  //When row write is enabled then sets which row is written to.

const unsigned int Excel::MaxMSGLength = 36;  //Max length of the message that can be displayed in the application plug-in
const unsigned int Excel::DataLength = 26;    // Max number of information that can be sent to excel A-Z cells.

double Excel::Data[DataLength];   // create the array that handles

int  Excel::UserLength = 0;         //default length of variables if user input is invalid
int  Excel::UserInput;

bool Excel::RowWrite = false;       //holds RowWrite status
bool Excel::Flag = false;           //flag for table clear

String Excel::Labels[DataLength];  // Create label array with width of the max number of displayable values.



Excel::Excel(int UserVal){ //Constructor for class. User input determines the amount of sensors to be setup

this ->UserInput = UserVal;

if (UserInput > this->DataLength)    // Check to see if user's input is within bounds of the excel plug-in's requirements
{
    this->PostMSG(InvalidData);   //Post error
}

else
{
  this -> UserLength = UserInput; //   // If value entered is within range update the class variable.
}

}

void Excel::EnableRowWrite(void){
    this -> RowWrite = true;
} //Enablee writing to a singular row

void Excel::DisableRowWrite(void){
    this -> RowWrite = false;
}

void Excel::PostMSG(String UserString){ //Function Test of User's string is within length requirements. Need to integrate comma checks.

    if (UserString.length() <= MaxMSGLength)
    {
        Serial.print("MSG, ");
        Serial.println(UserString);
    }
    else
    {
        Serial.print("MSG, ");
        Serial.println(InvaildLen);
    }
}

void Excel::ClearMSG(void){  //Clears message in application box
Serial.println("MSG, ");

}

void Excel::ClearLabels(void){

    for(int i = 0; i < this -> DataLength; i++)
    {
        this->Labels[i] = "";
    }

    this -> Flag = true;

    PushLabels();

}

void Excel::PushData(void){  //Pushes data stored in Data array to Excel

        int i = 0;
        int z = 1;

        if( this -> RowWrite == true)
        {
            Serial.println(RowWriteCommand); //sets up operation for next data push.
        }

        Serial.print("DATA,");   //Commence data sending.
        Serial.print("TIMER,");   //Posts current system time of PC. User "TIMER" for Arduino time.

        while ( i < this -> UserLength ){ //Iterate through all Data elements.

            Serial.print(this -> Data[i]);

            if (z != this -> UserLength)
            {
                Serial.print(",");
            }

            i++;
            z++;
        }

        Serial.println(); //Ends message with a carriage return

}

void Excel::PushLabels(void){

        int i = 0;
        int z = 1;

        Serial.print("LABEL,");
        Serial.print("Timer,");


        if (Flag == true)
        {
            this -> UserLength = DataLength;
        }
        else{
           this -> UserLength = this -> UserInput;
        }

        while ( i < this -> UserLength )
        {
            Serial.print(this ->Labels[i]);


                if ( z != (this -> UserLength))
            {
                Serial.print(",");
            }


        i++;
        z++;
        }

        Serial.print("\r\n");
        this -> Flag = false;
}

void Excel::ClearData(void){//Clears data in excel sheet

Serial.println("CLEARDATA");
}
