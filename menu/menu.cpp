#include "menu.h"
#include "bouton.h"
#include "Arduino.h"

char Menu::_titre[MENU_SIZE][21] =  
  {  
  "Cap :", "Kp :",  "Ki :",  "Kd :" , "I Max :", "V min :",
  "Load Preset 1" , "load Preset 2" , "Load Preset 3" , "Load Default", 
  "Save Preset 1" , "Save Preset 2" , "Save Preset 3" ,  
  "Info", "Calibration"
   };
   
Menu::Menu( Data* data, LiquidCrystal_I2C* lcd )
{
_Item = MENU_CAP;
_data = data;
_lcd  = lcd;
}

void Menu::select()
{
int delta = 1;

int c = _bouton.check_all();
if( c==false ) return;   // pas de bouton préssé
int btn   = c%10;
if( c/10 ) delta = 10;    // appui long

switch(btn)
  {
  case DOWN   : next();      break;
  case UP     : previous();  break;
  
  case LEFT   : 
    switch( _Item )
        {
        case MENU_CAP  :  _data->change_cap  (     -delta);       break;
        case MENU_KP   :  _data->change_param(KP,  -delta);       break;
        case MENU_KI   :  _data->change_param(KI,  -delta);       break;
        case MENU_KD   :  _data->change_param(KD,  -delta);       break;
        case MENU_IMAX :  _data->change_param(IMAX,-delta);       break;
        case MENU_VMIN :  _data->change_param(VMIN,-delta);       break;
        }
     break;
     
  case RIGHT  : 
    switch( _Item )
        {
        case MENU_CAP  :  _data->change_cap  ( delta*10 );     break;
        case MENU_KP   :  _data->change_param(KP,  delta);     break;
        case MENU_KI   :  _data->change_param(KI,  delta);     break;
        case MENU_KD   :  _data->change_param(KD,  delta);     break;
        case MENU_IMAX :  _data->change_param(IMAX,delta);     break;
        case MENU_VMIN :  _data->change_param(VMIN,delta);     break;
        }
    break;
    
  case VALIDE : switch( _Item )
    {
    case MENU_CAP           :  _data->change_actif();                     break;
    case MENU_LOAD_PRESET_1 :  _data->load(1);                            break;
    case MENU_LOAD_PRESET_2 :  _data->load(2);                            break;
    case MENU_LOAD_PRESET_3 :  _data->load(3);                            break;
    case MENU_LOAD_DEFAULT  :  _data->load_default();                     break;
    case MENU_SAVE_PRESET_1 :  if( confirme() ) _data->save(1);           break;
    case MENU_SAVE_PRESET_2 :  if( confirme() ) _data->save(2);           break;
    case MENU_SAVE_PRESET_3 :  if( confirme() ) _data->save(3);           break;
    case MENU_INFO          :  show_info();                               break;
    case MENU_CALIBRATION   :  if( confirme() ) 
                                  {
                                  _data->calibration();
                                  processing();
                                  }                                       break; 
    } 
    _Item = MENU_CAP; 
    break;  
  }
show_item();
}

void Menu::show_item()
{
efface_line(0);
efface_line(1);
_lcd->setCursor(2, 0);
_lcd->print(_titre[_Item]); 
 
switch( _Item )
  {
  case MENU_CAP :  _lcd->setCursor(9, 0); 
                   _lcd->print( _data->get_param(CAP), 0 );
                   _lcd->setCursor(9, 1);
                   if( _data->get_actif() )     _lcd->print( "ACTIF " );
                   else                         _lcd->print( "PAUSE " ); 
                   break;
              
  case MENU_KP :   _lcd->setCursor(10, 0); 
                   _lcd->print( _data->get_param(KP), 1 );
                   break;
              
  case MENU_KI :   _lcd->setCursor(10, 0); 
                   _lcd->print( _data->get_param(KI), 1 );
                   break;
              
  case MENU_KD :   _lcd->setCursor(10, 0); 
                   _lcd->print( _data->get_param(KD), 1 );
                   break;
              
  case MENU_IMAX : _lcd->setCursor(10, 0); 
                   _lcd->print( _data->get_param(IMAX), 1 );
                   break;
              
  case MENU_VMIN : _lcd->setCursor(10, 0); 
                   _lcd->print( _data->get_param(VMIN), 1 );
                   break; 
  }     
}

void Menu::show_info()
{
int  val[PARAM_SIZE];
_data->pull_param( val );

efface_line(1);
_lcd->print( "Cap :");
_lcd->print( (int)val[0]/10 );
_lcd->setCursor(10, 1);
_lcd->print( "Kp  :");
_lcd->print( (float)(val[1])/10,1 );

efface_line(2);
_lcd->print( "Ki  :");
_lcd->print( (float)(val[2])/10,1 );
_lcd->setCursor(10, 2);
_lcd->print( "Kd  :");
_lcd->print( (float)(val[3])/10,1 );

efface_line(3);
_lcd->print( "Imax:");
_lcd->print( (float)(val[4])/10,1 );
_lcd->setCursor(10, 3);
_lcd->print( "Vmin:");
_lcd->print( (float)(val[5])/10,1 );
while( _bouton.check_all()==false );
}

void Menu::next()
{
_Item++;
if(_Item>(MENU_SIZE-1)) _Item=0;   
}

void Menu::previous()
{
_Item--;
if(_Item<0) _Item=(MENU_SIZE-1);  
}

void Menu::efface_line( short line )
{
_lcd->setCursor(0, line);
_lcd->print( "                    " );
_lcd->setCursor(0, line);  
}

bool Menu::confirme()
{
int c;

_lcd->clear();
_lcd->setCursor(6, 1);
_lcd->print("Validez");
_lcd->setCursor(3, 2);
_lcd->print("pour confirmer");
while(1)
    {
    c =_bouton.check_all();
    if( c==VALIDE )  
        {
        _lcd->clear();
        _lcd->setCursor(6, 1);
        _lcd->print("Merci"); 
        delay( 1000 );
        return true;
        }
    if( c!=0 )
        {
        _lcd->clear();
        _lcd->setCursor(6, 1);
        _lcd->print("Abandon"); 
        delay( 1000 );
        return false;
        }
    }  
}

void Menu::processing()
{
_lcd->clear();
_lcd->setCursor(5, 1);
_lcd->print("Processing");
while(Serial.read() != '!')
    {
    efface_line(3);
    for( int i=0; i<20; i++ ) 
      {
      _lcd->write(255);
      delay(500);
      }
    }  
}

