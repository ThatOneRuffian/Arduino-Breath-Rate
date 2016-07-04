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

        static double DataBuffer[];

        WindMod(int, int, int, int);

        /*******kool Kid Functions******/
        double getCurrentKPH(void);

        double getCurrentMPH(void);

        double getTempC(void);

        double getTempF(void);

        /*******advanced functions******/
        double getStandardDev(void);

        void EnableMod(void);

        void DisableMod(void);

        void calibrate(MLED&);


    private:

        static double defaultFadeTime;
        static double lowThreshold;
        bool TempGood(void);





};


#endif BMOD
