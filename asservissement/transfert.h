#ifndef transfert_h
#define transfert_h

#include <PID_v1.h>
#include "Arduino.h"

#define BUFF_SIZE    12
#define PARAM_SIZE    6

class Transfert
{
public  :  Transfert(PID* _pilote, double* Cap, double* Kp, double* Ki, double* Kd, double* Imax, double* Vmin, double* Input, double* Output );
           void get_data();
           void send_data();
           void send_baregraphe();
           
private :  double  *_Cap,
                   *_Kp,
                   *_Ki,
                   *_Kd,
                   *_Imax,
                   *_Vmin,
                   *_Input,
                   *_Output;
            PID*   _pilote;
            double byteToDouble( byte buf[2] );
            void   doubleToByte( float val, byte buf[2]);
};
#endif
