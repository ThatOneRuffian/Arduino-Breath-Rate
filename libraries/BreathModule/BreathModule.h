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

        double getCurrentMS(void);

        double getTempC(void);

        double getTempF(void);

        /*******advanced functions******/
        double getStandardDev(void);

        void EnableMod(void);

        void DisableMod(void);

        void calibrate(MLED&);

        int sampleBreathRate(MLED&);



        static double defaultFadeTime;
        static double lowThreshold;
        static double DataBuffer[];





};


#endif
