#include "transfert.h"
#include "Arduino.h"

Transfert::Transfert()
{
  
}

void Transfert::get_data_6( double *prm0, double *prm1, double *prm2, double *prm3, double *prm4, double *prm5 )
{
byte buffer[BUFF_SIZE];
  
while( Serial.available() != BUFF_SIZE );
Serial.readBytes(buffer,BUFF_SIZE);

*prm0  = byteToDouble( buffer    )/10;
*prm1  = byteToDouble( buffer+ 2 )/10;
*prm2  = byteToDouble( buffer+ 4 )/10;
*prm3  = byteToDouble( buffer+ 6 )/10;
*prm4  = byteToDouble( buffer+ 8 )/10;
*prm5  = byteToDouble( buffer+10 )/10; 
}

void Transfert::send_data_6( double prm0, double prm1, double prm2, double prm3, double prm4, double prm5 )
{
int param[PARAM_SIZE];
byte buff[BUFF_SIZE];
  
param[0] = (int)(prm0*10);
param[1] = (int)(prm1*10);
param[2] = (int)(prm2*10);
param[3] = (int)(prm3*10);
param[4] = (int)(prm4*10);
param[5] = (int)(prm5*10);
memcpy( buff, param, BUFF_SIZE );
Serial.write( buff, BUFF_SIZE );  
}

void Transfert::send_data_2( double prm0, double prm1 )
{
byte val[4];
  
doubleToByte( (prm0*10), val );        
doubleToByte( (prm1*10), val+2 );     
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
