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

            MLED(int&, int&, int&, int&);     // Constructor for the object. Determines which pins are connected to which LED

            /*******Colors*******/
            void Purple(void);
            void Yellow(void);
            void LightBlue(void);
            void White(void);
            void Green(void);
            void Blue(void);
            void Red(void);

            /*********Color - Functions*********/
            void ClearColors(void);


            /*********Misc - Functions*********/
            void SetBrightness(int&); //Sets the brightness of the LED from 0 - 255
            void Fade(double, int);

       private:

            static int RED;      //Holds reserved pin numbers
            static int BLUE;
            static int GREEN;
            static int ENABLE;
            static int UserBrightness;

};


#endif
