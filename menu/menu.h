#ifndef menu_h
#define menu_h

#include "LiquidCrystal_I2C.h"
#include "Arduino.h"
#include "data.h"
#include "bouton.h"

#define MENU_CAP             0
#define MENU_KP              1
#define MENU_KI              2
#define MENU_KD              3
#define MENU_IMAX            4
#define MENU_VMIN            5
#define MENU_LOAD_PRESET_1   6
#define MENU_LOAD_PRESET_2   7
#define MENU_LOAD_PRESET_3   8
#define MENU_LOAD_DEFAULT    9
#define MENU_SAVE_PRESET_1  10 
#define MENU_SAVE_PRESET_2  11
#define MENU_SAVE_PRESET_3  12
#define MENU_INFO           13
#define MENU_CALIBRATION    14
#define MENU_CAPTEUR        15

#define MENU_SIZE           16

class Menu
{
public  : Menu( Data* data, LiquidCrystal_I2C* Lcd );
          void select( );
          void show_item();
                  
private : char               _Item;
          Data*              _data;
          class Bouton       _bouton;             
          LiquidCrystal_I2C* _lcd;
          static char        _titre[MENU_SIZE][21];
          void next();
          void previous();
          void show_info();
          void show_capteur();
          void efface_line( short line );
          bool confirme();
          void processing_calibration();
};

#endif
