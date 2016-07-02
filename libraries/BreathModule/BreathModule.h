#ifndef BMOD
#define BMOD

#include "Arduino.h"
#include <math.H>

class WindMod{

    public:

        static int OUT;
        static int RV;
        static int TMP;

        WindMod(int, int, int);

        /*******kool Kid Functions******/
        double getCurrentKPH(void);

        double getCurrentMPH(void);

        double getTempC(void);

        double getTempF(void);

        void calibrate(void);


    private:

        bool TempGood(void);


};


#endif BMOD
