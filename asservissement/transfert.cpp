#include "transfert.h"
#include "Arduino.h"

Transfert::Transfert( PID* pilote, double* Cap, double* Kp, double* Ki, double* Kd, double* Imax, double* Vmin, double* Input, double* Output )
{
_pilote = pilote;
_Cap    = Cap;
_Kp     = Kp;
_Ki     = Ki;
_Kd     = Kd;
_Imax   = Imax;
_Vmin   = Vmin;
_Input  = Input;
_Output = Output;
}

void Transfert::get_data()
{
byte buffer[BUFF_SIZE];
  
while( Serial.available() != BUFF_SIZE );
Serial.readBytes(buffer,BUFF_SIZE);

  
*_Cap    = byteToDouble( buffer    );
*_Kp     = byteToDouble( buffer+ 2 )/10;
*_Ki     = byteToDouble( buffer+ 4 )/10;
*_Kd     = byteToDouble( buffer+ 6 )/10;
*_Imax   = byteToDouble( buffer+ 8 )/10;
*_Vmin   = byteToDouble( buffer+10 )/10;
_pilote->SetTunings(*_Kp, *_Ki, *_Kd);  
}

void Transfert::send_data()
{
int param[PARAM_SIZE];
byte buff[BUFF_SIZE];
  
param[0] = (int)(*_Cap);
param[1] = (int)(*_Kp*10);
param[2] = (int)(*_Ki*10);
param[3] = (int)(*_Kd*10);
param[4] = (int)(*_Imax*10);
param[5] = (int)(*_Vmin*10);
memcpy( buff, param, BUFF_SIZE );
Serial.write( buff, BUFF_SIZE );  
}

void Transfert::send_baregraphe()
{
byte val[4];
  
doubleToByte( *_Input*10,  val );     // envoie le cap en dixieme de degré         
doubleToByte( *_Output, val+2 );      // envoie la correction de barre en +/- 127
Serial.write( val, 4 ); 
}

void Transfert::doubleToByte( float val, byte buf[2])
{
int data = (int)val;
buf[0] = data & 255;
buf[1] = (data >> 8)  & 255;  
}

double Transfert::byteToDouble( byte buf[2] )
{
return (double)((buf[1]<<8) | buf[0]);  
}
