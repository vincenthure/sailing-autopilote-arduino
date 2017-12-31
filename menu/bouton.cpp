#include "bouton.h"
#include "Arduino.h"

Bouton::Bouton()
{
pinMode( PIN_VALIDE,   INPUT_PULLUP); 
pinMode( PIN_LEFT,     INPUT_PULLUP);
pinMode( PIN_RIGHT,    INPUT_PULLUP);
pinMode( PIN_UP,       INPUT_PULLUP);
pinMode( PIN_DOWN,     INPUT_PULLUP); 

_btn[VALIDE] = PIN_VALIDE;
_btn[UP]     = PIN_UP;
_btn[DOWN]   = PIN_DOWN;
_btn[LEFT]   = PIN_LEFT;
_btn[RIGHT]  = PIN_RIGHT;
}

char Bouton::check_all()
{
char x;
x = check( VALIDE );      if(x)   return x;
x = check( UP     );      if(x)   return x;
x = check( DOWN   );      if(x)   return x;
x = check( LEFT   );      if(x)   return x;
x = check( RIGHT  );      if(x)   return x;

return false; 
}

char Bouton::check( char b )   // retourne true ou false si le bouton demandé à été appuyé
{
char x = false;
static char memBouton[10];
static long push;

boolean state  = digitalRead(_btn[b]); 
if((state != memBouton[b]) && (state == LOW))  // changement d'etat du bouton vers le bas
    {
    push  = millis();  
    x=b;
    }

if((  millis() > (push+PUSH) ) && (state == LOW))   // appui long
    {
    push  = millis(); 
    x=b+10;
    }

memBouton[b] = state; //on enregistre l'état du bouton pour le tour suivant
return x; 
}
