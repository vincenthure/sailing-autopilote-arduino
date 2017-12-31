#ifndef data_h
#define data_h

#include "Arduino.h"

#define CAP         0
#define KP          1
#define KI          2
#define KD          3
#define IMAX        4
#define VMIN        5

#define BUFF_SIZE  12
#define PARAM_SIZE  6

#define Kp_init    20
#define Ki_init    50
#define Kd_init    10
#define Imax_init  60
#define Vmin_init  70

class Data
{
public :  Data( );
          void  load( char i );
          void  load_default();
          void  save( char i );
          
          void  push_param();
          void  pull_param( int* val );
          void  pull_bargraph( int* val );
          
          void  change_param( int param, int val );
          void  change_cap( int val );
          void  change_actif();
          
          float get_param( int param );
          int   get_cap();
          bool  get_actif();

          void calibration();
          
private : int   _param[PARAM_SIZE];
          bool  _actif;
          int   byteToInt( byte* buf );
};

#endif
