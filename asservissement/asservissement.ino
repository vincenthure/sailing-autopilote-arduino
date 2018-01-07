#include "calibration.h"
#include "transfert.h"
#include "PID_v1.h"

#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "utility/imumaths.h"

#define PIN_PWM       10
#define PIN_REVERSE   11
#define PIN_AMP        0
#define DC_POWER      12

#define INFO_REQUEST        '&'
#define BAREGRAPHE_REQUEST  '#'
#define CHANGE_MODE         '$'
#define MAKE_CALIBRATION    '!'
#define SAVE_CALIBRATION    '%'
#define REFRESH_PARAMETER   '@' 
#define CURRENT_HEADING     '*'  
#define CAPTEUR_REQUEST     '/' 

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
  double Pwm;
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  Heading = euler.x();

  Input = substract_heading( Cap , Heading );

  if( actif )
      {
      pilote.Compute();
      
      Pwm = adjust_output( Output );
      analogWrite (PIN_PWM, abs(Pwm) );                  // PWM
      digitalWrite(PIN_REVERSE, is_negatif( Pwm ));      // REVERSE MODE
      }
      
  if( Serial.available() )        // check Serial port
    switch( Serial.read() )
      {
      case BAREGRAPHE_REQUEST : transfert.send_data_2( Input, Pwm );                      // send data to baregraphe
                                break;
                                
      case REFRESH_PARAMETER :  transfert.get_data_6( &Cap, &Kp, &Ki, &Kd, &Imax, &Vmin );   // get parametre
                                pilote.SetTunings( Kp, Ki, Kd) ;                             // adjust PID
                                break;
                  
      case INFO_REQUEST :       transfert.send_data_6( Cap, Kp, Ki, Kd, Imax, Vmin);         //send info
                                break;
                                 
      case CAPTEUR_REQUEST :    transfert.send_data_2( Heading, Input);         //send info
                                break;

      case CHANGE_MODE :        actif = Serial.read();                                       // change mode
                                break;
                                
      case MAKE_CALIBRATION :   calibration.make();                                          // processing calibration
                                break;

      case SAVE_CALIBRATION :   calibration.save();                                          // SAVING calibration
                                break;
                                
      case CURRENT_HEADING :    Cap = Heading;
                                transfert.send_data_1( Heading );  
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

double adjust_output( double x )
{
double y;
double abs_x = abs(x);

// double c = Vmin / DC_POWER;
// y =  abs(x) * ( 1 - c ) + ( 127 * c );    // courbe de transfert lineaire

double b = 127 - ( 127 / Vmin );

y = (abs_x/Vmin) + b*( 1 - exp( -abs_x/10 ));

if( is_negatif(x) ) return -y;
else                return  y;
}

