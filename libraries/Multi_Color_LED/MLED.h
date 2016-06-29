/*
    Purpose: Allows the Arduino to interface with the PLX-DAQ plug-in for excel.

    @author Marcus Mesta
*/

#ifndef MultiLED
#define MultiLED

#include "Arduino.h"

//#include "WString.h"  //library to allow the use of Arduino string objects.

class MLED{

       public:

            MLED(int, int, int, int);     // Constructor for the object. Determines which pins are connected to which LED

            void SetBrightness(int); //Sets the brightness of the LED from 0 - 255
            void ClearColors(void);


            /*******Colors*******/
            void Purple(void);
            void Yellow(void);
            void LightBlue(void);
            void White(void);
            void Green(void);
            void Blue(void);
            void Red(void);

       private:

            static int RED;      //Holds reserved pin numbers
            static int BLUE;
            static int GREEN;
            static int BRIGHTNESS;

};


#endif MultiLED
