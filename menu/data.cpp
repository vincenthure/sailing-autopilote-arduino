#include "data.h"
#include "Arduino.h"

#include <EEPROM.h>

Data::Data()
{
_actif = true;
load(0); 
}

void Data::load( char nb )
{
short offset = BUFF_SIZE * nb;
char  buffer[BUFF_SIZE];
 
for( int i=0; i<BUFF_SIZE; i++ )    buffer[i] = EEPROM.read( offset+i );  
memcpy( _param, buffer, BUFF_SIZE );       // converti de byte en int 
save( 0 );
push_param();
}

void Data::load_default( )
{
_param[KP]    = Kp_init;
_param[KI]    = Ki_init;  
_param[KD]    = Kd_init;
_param[IMAX]  = Imax_init;
_param[VMIN]  = Vmin_init;
push_param();
}

void Data::save( char nb )
{
short first = BUFF_SIZE * nb;
char  buffer[BUFF_SIZE];
  
memcpy( buffer, _param, BUFF_SIZE );
for( int i=0; i<BUFF_SIZE; i++ )    EEPROM.write( first+i, buffer[i] );   
}

void Data::calibration()
{
while( Serial.available())   { Serial.read(); }   // vide les buffers
Serial.write("!");                                // envoie une requete
}

void Data::push_param()
{
byte buffer[BUFF_SIZE];

Serial.write( '@' );  
memcpy( buffer, _param, BUFF_SIZE );
Serial.write( buffer, BUFF_SIZE );  
}

void Data::change_actif( )
{
_actif = !_actif;
Serial.write('$');
Serial.write(_actif);     
}

void Data::pull_param( int* val )
{
byte info[BUFF_SIZE];

while( Serial.available())   { Serial.read(); }    // flush buffer
Serial.write('&');                                 // send request
while( Serial.available() != BUFF_SIZE );          // wait response completed
Serial.readBytes(info,BUFF_SIZE);                  // get response
memcpy( val , info, BUFF_SIZE );  
}

void Data::pull_bargraph( int* result )
{
byte val[4];

while( Serial.available())   { Serial.read(); }   // vide les buffers
Serial.write("#");                                // envoie une requete
while( Serial.available() != 4 );                 // attends la reponse complete
Serial.readBytes( val , 4 );
result[0]  = byteToInt( val );
result[1]  = byteToInt( val+2 );  
}

void Data::change_param( int index, int val)
{
if(index < 0)               return;
if(index > (PARAM_SIZE-1) ) return;

Serial.println( _param[index] );

_param[index] += val;
save(0); 
push_param(); 
}

void Data::change_cap( int val)
{
_param[CAP] /= 10;
_param[CAP] *= 10;
_param[CAP] += val;

if (_param[CAP] > 359) _param[CAP] -= 360;  
if (_param[CAP] < 0)   _param[CAP] += 360;  
save(0); 
push_param(); 
}

float Data::get_param( int index )
{
if(index < 0)               return 0;
if(index > (PARAM_SIZE-1) ) return 0;

return (float)(_param[index])/10; 
}


bool Data::get_actif( )
{
return _actif;   
}

int Data::byteToInt( byte* buf )
{
return (int)((buf[1]<<8) | buf[0]);   
}

