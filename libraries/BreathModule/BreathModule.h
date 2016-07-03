#ifndef BMOD
#define BMOD

#include "Arduino.h"
#include "MLED.h"
#include <math.H>

class WindMod{

    public:

        static int OUT;
        static int RV;
        static int TMP;
        static int Enable;

        WindMod(int, int, int, int);

        /*******kool Kid Functions******/
        double getCurrentKPH(void);

        double getCurrentMPH(void);

        double getTempC(void);

        double getTempF(void);

        /*******advanced functions******/
        void EnableMod(void);

        void DisableMod(void);

        void calibrate(MLED&);


    private:

        bool TempGood(void);

        static double defaultFadeTime;



};


#endif BMOD
