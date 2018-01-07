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
Serial.write( _bno->getCalibState(Adafruit_BNO055::CALIB_STAT_MAG) );
Serial.write( _bno->getCalibState(Adafruit_BNO055::CALIB_STAT_GYR) );
Serial.write( _bno->getCalibState(Adafruit_BNO055::CALIB_STAT_ACC) );
Serial.write( _bno->getCalibState(Adafruit_BNO055::CALIB_STAT_SYS) );
}

void Calibration::save()
{
byte calibration[CALIBRATION_SIZE];
  
for( int i=0; i<CALIBRATION_SIZE; i++ )  calibration[i]=0;
_bno->getCalibData(calibration);   

for( int i=0; i<CALIBRATION_SIZE; i++ )  
      {
      EEPROM.write( i, calibration[i] );
      Serial.println(calibration[i]);
      }   
}

