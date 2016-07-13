#include "BreathModule.h"

int WindMod::RV,
    WindMod::TMP,
    WindMod::OUT,
    WindMod::Enable;

double WindMod::defaultFadeTime = 1000.0,    //MLED libraries used.
       WindMod::lowThreshold;  // This value is set during the calibration function.

WindMod::WindMod(int _rv, int _tmp, int _out, int EnablePin){

this -> RV = _rv;
this -> TMP = _tmp;
this -> OUT = _out;
this -> Enable = EnablePin;

pinMode(this -> RV, INPUT);
pinMode(this -> TMP, INPUT);
pinMode(this -> OUT, INPUT);
pinMode(this -> Enable, OUTPUT);

this -> EnableMod();

}

float WindMod::getCurrentKPH(){

return this -> getCurrentMPH()*1.60934;

}

float WindMod::getCurrentMS(){
float KMperMS = 0.27777;
return (this -> getCurrentKPH()*KMperMS);
}

float WindMod::getCurrentMPH(){

const float zeroWindAdjustment =  .2;
const double constant = 0.0048828125;
float RV_Wind_Volts;
float WindSpeed_MPH;
float zeroWind_ADunits;
float zeroWind_volts;

float TMP_Therm_ADunits;

float incoming = 0;
//double pinADC = 0;

     incoming = analogRead(this -> RV);
     TMP_Therm_ADunits = analogRead(this -> TMP);

     RV_Wind_Volts = (incoming *  constant);
     zeroWind_ADunits = -0.0006*((float)TMP_Therm_ADunits * (float)TMP_Therm_ADunits) + 1.0727 * (float)TMP_Therm_ADunits + 47.172;
     zeroWind_volts = (zeroWind_ADunits * constant) - zeroWindAdjustment;
     WindSpeed_MPH =  pow(((RV_Wind_Volts - zeroWind_volts) /.2300) , 2.7265);


     return WindSpeed_MPH;

}

double WindMod::getTempC(){

    double TMP_Therm_ADunits = analogRead(this -> TMP);

    double TempCtimes100 = (0.005 *((double)TMP_Therm_ADunits * (double)TMP_Therm_ADunits))
                  - (16.862 * (double)TMP_Therm_ADunits) + 9075.4;

return TempCtimes100/100;


}

double WindMod::getTempF(){

return  (this -> getTempC()* 9.0/5.0) + 32;


}

double WindMod::getStandardDev(){

int BufferArraySize = 100; //100 samples to calculate standard deviation. Or a proper threshold for triggering.
int DelayPerSample = 40; //wait  ms per sample to get good range of values. should take about 4 seconds.
double DataBuffer[BufferArraySize];

double mean = 0.0;
double sumDeviation = 0.0;

for (int i = 0; i < BufferArraySize; i++)

{
    DataBuffer[i] = this -> getCurrentMS();
    delay(BufferArraySize);
}


for (int i = BufferArraySize; i > 0; i--)

{
    mean += DataBuffer[i];
}

mean = mean/BufferArraySize;


for (int z = 0; z < BufferArraySize; z++)
{
    sumDeviation +=(DataBuffer[z] - mean) * (DataBuffer[z] - mean);
}

return sqrt(sumDeviation/BufferArraySize);




}

void WindMod::calibrate(MLED& LEDobject){





    const int delayTime = 200; // delay two seconds after disabling mod to turn back on.
    const int standardDevs = 5; //threshold set to be x times the standard deviation taken from calibration
    const int lowOffset = 2;
    const int analogThresholdLow = 100; // threshold for warm up OK ~0.3v
    const int analogThresholdHIGH = 600; // High threshold for unit ~2v
    const int analogThresholdSuperLow = 100;

    const double ReadToAvg = 100;

    int dummyRead = 2000;  //compares to ready value to ensure device pin has been read enough to read correct value
    int IncrementCount = 0;
    const int MaxAnalogWrite = 255;
    const int halfValue = dummyRead/2;
    const int stepSize = halfValue/MaxAnalogWrite;
    bool fadeToggle = true;

    int analogData = 0;

    double AverageReadValue = 0;

    LEDobject.Red();      //Warm up indication color

    this -> DisableMod();  // Turn off module

    delay(delayTime);       // wait for module to power down

    this -> EnableMod();    // Turn mod back on.

    delay(delayTime);       // wait for voltage spike on output pin.

    analogData = analogRead(this -> OUT); //read analog value.

    while(analogData <= analogThresholdLow || analogData >= analogThresholdHIGH ){ //while the mod is warming up then do cool show.

        LEDobject.Fade(this -> defaultFadeTime, 1);  //fade 1 cycle.

        analogData = analogRead(this -> OUT);  //refresh value

    }

    LEDobject.Yellow();   //Change LED to a solid yellow color


    while(dummyRead > 0) //bleed sensor to get it ready
    {

        analogRead(this->OUT);


         if (IncrementCount <= halfValue && fadeToggle)
             {
               LEDobject.SetBrightness(stepSize*IncrementCount);
               IncrementCount ++;
               delay(2);

             }

         else
             {
               LEDobject.SetBrightness(stepSize*IncrementCount);
               IncrementCount --;

               fadeToggle = false;
               delay(2);

             }
                 dummyRead --;



    }
    LEDobject.SetBrightness(255); //restore to max brightness

    LEDobject.Purple();
    for(int z = 0; z < ReadToAvg; z++)
    {
        AverageReadValue += this -> getCurrentMS();

    }//find average of the read values.
    AverageReadValue /= ReadToAvg;

    this -> lowThreshold  = ((this -> getStandardDev())*standardDevs) + AverageReadValue + lowOffset;  // sets lower threshold 5 standard deviations away from base reading.

    LEDobject.Green();
    delay(delayTime);
    LEDobject.ClearColors(); // Clear red LED
}

void WindMod::EnableMod(){

digitalWrite(this -> Enable, HIGH);

}

void WindMod::DisableMod(){

digitalWrite(this -> Enable, LOW);

}
