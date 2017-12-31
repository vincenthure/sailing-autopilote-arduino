#include "calibration.h"
#include "Arduino.h"

Calibration::Calibration( Adafruit_BNO055* bno )
{
_bno = bno; 
}

void Calibration::load()
{
for( int i=0; i<CALIBRATION_SIZE; i++ )   c_data[i] = EEPROM.read( i ); 
_bno->setCalibData(Calibration::c_data); 
}

void Calibration::make()
{
byte calibration[CALIBRATION_SIZE];
bool bMAG,bGYR,bACC,b;

do  
{
bMAG = _bno->getCalibState(Adafruit_BNO055::CALIB_STAT_MAG);
bGYR = _bno->getCalibState(Adafruit_BNO055::CALIB_STAT_GYR);
bACC = _bno->getCalibState(Adafruit_BNO055::CALIB_STAT_ACC);
b    = _bno->getCalibState(Adafruit_BNO055::CALIB_STAT_SYS);
Serial.print(bMAG);
Serial.print(bGYR);
Serial.print(bACC);
Serial.println(b);
}while( bMAG==false || bGYR==false  || bACC==false || b==false  );

for( int i=0; i<CALIBRATION_SIZE; i++ )  calibration[i]=0;
_bno->getCalibData(calibration);   

for( int i=0; i<CALIBRATION_SIZE; i++ )  {
                                         EEPROM.write( i, calibration[i] );
                                        // Serial.println(calibration[i]);
                                         }   
}


