#ifndef bouton_h
#define bouton_h

#include "Arduino.h"

#define VALIDE     1
#define DOWN       2
#define RIGHT      3
#define UP         4
#define LEFT       5

// pin de l'arduino

#define PIN_VALIDE    5
#define PIN_DOWN      6
#define PIN_RIGHT     7
#define PIN_UP        8
#define PIN_LEFT      9

#define PUSH      1500      // Temps d'appui long sur les boutons en millisecondes

class Bouton
{
public:   Bouton();
          char check_all();
          
private:  int  _btn[6]; 
          char check( char b );
};

#endif
