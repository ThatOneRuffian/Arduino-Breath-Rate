#include "BreathModule.h"

int WindMod::RV, WindMod::TMP, WindMod::OUT;

WindMod::WindMod(int _rv, int _tmp, int _out, int EnablePin){

this -> RV = _rv;
this -> TMP = _tmp;
this -> OUT = _out;


pinMode(this -> RV, INPUT);
pinMode(this -> TMP, INPUT);
pinMode(this -> OUT, INPUT);

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


