#include "BreathModule.h"

int WindMod::RV,
    WindMod::TMP,
    WindMod::OUT,
    WindMod::Enable;

double WindMod::defaultFadeTime = 1000.0,
       WindMod::lowThreshold;//Default fade time.

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

double WindMod::getCurrentKPH(){

return this -> getCurrentMPH()*1.60934;

}

double WindMod::getCurrentMPH(){

const double zeroWindAdjustment =  .2;
double RV_Wind_Volts;
double WindSpeed_MPH;
double  zeroWind_ADunits;
double zeroWind_volts;

double TMP_Therm_ADunits;

double incoming = 0;
double pinADC = 0;

     incoming = analogRead(this -> RV);
     TMP_Therm_ADunits = analogRead(this -> TMP);

     RV_Wind_Volts = (incoming *  0.0048828125);
     zeroWind_ADunits = -0.0006*((double)TMP_Therm_ADunits * (double)TMP_Therm_ADunits) + 1.0727 * (double)TMP_Therm_ADunits + 47.172;
     zeroWind_volts = (zeroWind_ADunits * 0.0048828125) - zeroWindAdjustment;
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
    DataBuffer[i] = this -> getCurrentMPH();
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

noInterrupts();          //disable Arduino interrupts

const int delayTime = 200; // delay two seconds after disabling mod to turn back on.
const int analogThresholdLow = 100; // threshold for warm up OK ~0.3v
const int analogThresholdHIGH = 600; // High threshold for unit ~2v
const int analogThresholdSuperLow = 100;
int analogData = 0;

int dummyRead = 2000;  //compares to ready value to ensure device pin has been read enough to read correct value

LEDobject.Red();      //Change LED to red color.

this -> DisableMod();  // Turn off module

delay(delayTime);       // wait for module to power down

this -> EnableMod();    // Turn mod back on.

delay(delayTime);       // wait for spike on output pin.

analogData = analogRead(this -> OUT); //read analog value.

while(analogData <= analogThresholdLow || analogData >= analogThresholdHIGH ){ //while the mod is warming up then do cool show.

LEDobject.Fade(this -> defaultFadeTime, 1);  //fade 1 cycle.

analogData = analogRead(this -> OUT);  //refresh value

}

interrupts();         // Enable Interrupts

LEDobject.Yellow();   //Change LED to a solid yellow color

while(dummyRead > 0) //bleed sensor to get it ready
{
       analogRead(this->OUT);
       delay(2);
       dummyRead --;
}

LEDobject.Purple();    //set LED purple until the calculation for the module has been completed

this -> lowThreshold = (this -> getStandardDev())*5;  // sets lower threshold 5 standard deviations away from base reading.

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
