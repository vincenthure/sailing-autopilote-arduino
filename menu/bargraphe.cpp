#include "bargraphe.h"
#include "Arduino.h"
 
#include <LiquidCrystal_I2C.h>

byte Bargraphe::_level1[8] = { B10000, B10000, B10000, B10000, B10000, B10000, B10000, B10000 };
byte Bargraphe::_level2[8] = { B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000 };
byte Bargraphe::_level3[8] = { B11100, B11100, B11100, B11100, B11100, B11100, B11100, B11100 };
byte Bargraphe::_level4[8] = { B11110, B11110, B11110, B11110, B11110, B11110, B11110, B11110 };
byte Bargraphe::_back1[8]  = { B00001, B00001, B00001, B00001, B00001, B00001, B00001, B00001 };
byte Bargraphe::_back2[8]  = { B00011, B00011, B00011, B00011, B00011, B00011, B00011, B00011 };
byte Bargraphe::_back3[8]  = { B00111, B00111, B00111, B00111, B00111, B00111, B00111, B00111 };
byte Bargraphe::_back4[8]  = { B01111, B01111, B01111, B01111, B01111, B01111, B01111, B01111 };

Bargraphe::Bargraphe( LiquidCrystal_I2C* Lcd )
{
_Lcd = Lcd;
}

void Bargraphe::init( )
{
_Lcd->createChar(0, _level1);
_Lcd->createChar(1, _level2);
_Lcd->createChar(2, _level3);
_Lcd->createChar(3, _level4);
_Lcd->createChar(4, _back4);
_Lcd->createChar(5, _back3);
_Lcd->createChar(6, _back2);
_Lcd->createChar(7, _back1); 
}

void Bargraphe::draw( int val )
{
val += 50;
if(val>100) val=100;
if(val<0)   val=0;
char a = (byte)val/5;      // block entier
char b = (byte)val%5;      // block variable

if ( a<10 )           // deviation à gauche
    {
    for( char i=0 ; i<a ; i++)        Bargraphe::_Lcd->print(' ');
    if( b== 0 )                       Bargraphe::_Lcd->write(255);  
    else                              Bargraphe::_Lcd->write(b+3);
    for( char i= a+1 ; i<10 ; i++)    Bargraphe::_Lcd->write(byte(255));
    for( char i=10 ; i<20 ; i++)      Bargraphe::_Lcd->print(' ');
    }
else                 // deviation à droite
    {
    for( char i=0 ; i<10 ; i++)       Bargraphe::_Lcd->print(' ');  
    for( char i= 10 ; i<a ; i++)      Bargraphe::_Lcd->write(byte(255));
    if( b== 0 )                       Bargraphe::_Lcd->print(' ');
    else                              Bargraphe::_Lcd->write(b-1);
    for( char i=a+1 ; i<20 ; i++)     Bargraphe::_Lcd->print(' ');
    }  
}

