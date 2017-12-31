
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "bargraphe.h"
#include "data.h" 
#include "menu.h" 

#define RESET 4

LiquidCrystal_I2C lcd(0x20,20,4);
Bargraphe         bargraphe(&lcd);
Data              data;
Menu              menu(&data,&lcd);

void setup()
{
pinMode( RESET, OUTPUT);
digitalWrite( RESET, LOW );     // Reset l'arduino asservisssement
delay( 10 );
digitalWrite( RESET , HIGH ); 

Serial.begin(9600); 

lcd.begin();                  //initialise l'ecran LCD en I2C
bargraphe.init();
lcd.backlight();
lcd.clear();
lcd.setCursor(5, 0);
lcd.print("pilote V2.1");
lcd.setCursor(3, 2);
lcd.print("Initialisation");
    
while( Serial.read() != '#' );     // wait for response

lcd.clear();    

data.push_param();
menu.show_item();

delay( 100 );
}


void loop()
{
short c = 0;
int   request[2];

data.pull_bargraph( request );
lcd.setCursor(0, 2);
bargraphe.draw( request[0]/2 );       // ecart de cap
lcd.setCursor(0, 3);
bargraphe.draw( request[1]/2.56 );    //  compensation

menu.select(); 

delay(50);
}


