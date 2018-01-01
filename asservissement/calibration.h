#ifndef calibration_h
#define calibration_h

#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "utility/imumaths.h"

#include <EEPROM.h>

#include "PID_v1.h"
#include "Arduino.h"

#define CALIBRATION_SIZE 22

class Calibration
{
public :  Calibration( Adafruit_BNO055* bno );
          void load();
          void make();
           
private : Adafruit_BNO055* _bno;
          byte c_data[CALIBRATION_SIZE];
};

#endif
