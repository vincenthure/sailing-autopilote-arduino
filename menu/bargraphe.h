#ifndef bargraphe_h
#define bargraphe_h

#include "Arduino.h"
#include "LiquidCrystal_I2C.h"

class Bargraphe
{
public: Bargraphe( LiquidCrystal_I2C* Lcd );
        void init();
        void draw( int val );
  
private:  LiquidCrystal_I2C* _Lcd;
          static byte        _level1[8],
                             _level2[8],
                             _level3[8],
                             _level4[8],
                             _back1[8],
                             _back2[8],
                             _back3[8],
                             _back4[8];
};

#endif

