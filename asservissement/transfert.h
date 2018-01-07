#ifndef transfert_h
#define transfert_h

#include "Arduino.h"

#define BUFF_SIZE    12
#define PARAM_SIZE    6

class Transfert
{
public  :  Transfert();
           void get_data_6( double* prm0, double* prm1, double* prm2, double* prm3, double* prm4, double* prm5  );
           void send_data_6( double prm0, double prm1, double prm2, double prm3, double prm4, double prm5 );
           void send_data_2( double prm0, double prm1 );
           void send_data_1( double prm );
           
private :  double byteToDouble( byte buf[2] );
           void   doubleToByte( float val, byte buf[2]);
};
#endif
