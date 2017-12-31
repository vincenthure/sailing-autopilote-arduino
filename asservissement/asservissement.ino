#include "calibration.h"
#include "transfert.h"
#include <PID_v1.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#define PIN_PWM       10
#define PIN_REVERSE   11
#define PIN_AMP        0

double  Setpoint=0,
        Cap,
        Heading, 
        Kp, 
        Ki, 
        Kd,
        Imax,
        Vmin,
        Input,
        Output;

bool    actif = true;

Adafruit_BNO055    bno  = Adafruit_BNO055(55);
PID                pilote(&Input, &Output, &Setpoint, Kp, Ki, Kd, P_ON_M, REVERSE);
Transfert          transfert;
Calibration        calibration( &bno );

void setup()
  {
  Serial.begin(9600);            // initialise le port serie

  pinMode( PIN_PWM, OUTPUT);
  pinMode( PIN_REVERSE, OUTPUT);
  
  pilote.SetMode(AUTOMATIC);    // initialise l'asservissement PID
  pilote.SetOutputLimits(-127, 127);
  
  bno.begin( Adafruit_BNO055::OPERATION_MODE_NDOF );   // mode gyro + compas + accelerometre
  calibration.load();
  delay(1000);
  bno.setExtCrystalUse(true);

  Serial.print("#");           // envoie un message pour dire qu'il est pret
  }

void loop()
  {
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  Heading = euler.x();

  Input = substract_heading( Cap , Heading );

  if( actif )
      {
      pilote.Compute();
  
      analogWrite (PIN_PWM,     abs( Output ) );             // PWM
      digitalWrite(PIN_REVERSE, is_negatif( Output ));      // REVERSE MODE
      }
      
  if( Serial.available() )        // check Serial port
    switch( Serial.read() )
      {
      case '#' :  transfert.send_data_2( Input, Output );
                  break;
      case '@' :  transfert.get_data_6( &Cap, &Kp, &Ki, &Kd, &Imax, &Vmin );
                  pilote.SetTunings( Kp, Ki, Kd) ; 
                  break;
      case '&' :  transfert.send_data_6( Cap, Kp, Ki, Kd, Imax, Vmin);
                  break;
      case '$' :  actif = Serial.read();
                  break;
      case '!' :  calibration.make();
                  Serial.write('!');
                  break;
      }  

  }
  
bool is_negatif( float val )
{
if( val<0 ) return HIGH;
return LOW; 
}


float substract_heading( float a, float b )
{
if( a<180 && b<180 )   return a-b;
if( a>180 && b>180 )   return a-b;
if( a<180 && b>180 )   return 360+a-b;
if( a>180 && b<180 )   return 360-b+a;
}
