#include <stdbool.h>
#include <stdint.h>
#include "Arduino.h"
// include the library code:
#include <LiquidCrystal.h>
#include "fnqueue.h"
#include "device.h"

// these constants won't change.  But you can change the size of
// your LCD using them:
const uint8_t numRows = 2;
const uint8_t numCols = 16;

//Constantes de los botones del display
const int TECLA_UP = 0;
const int TECLA_DOWN = 1;
const int TECLA_LEFT = 2;
const int TECLA_RIGHT = 3;
const int TECLA_SELECT = 4;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void up_keyup()
{
    lcd.print("UP keyUP");  
}
void down_keyup()
{
    lcd.print("DOWN keyUP"); 
}
void up_keydown()
{
    lcd.print("UP keyDOWN");  
}
void down_keydown()
{
    lcd.print("DOWN keyDOWN"); 
}
void up_keyleft()
{
    lcd.print("UP keyLEFT");  
}
void down_keyleft()
{
    lcd.print("DOWN keyLEFT"); 
}
void up_keyright()
{
    lcd.print("UP keyRIGHT");  
}
void down_keyright()
{
    lcd.print("DOWN keyRIGHT"); 
}
void up_keyselect()
{
    lcd.print("UP keySELECT");  
}
void down_keyselect()
{
    lcd.print("DOWN keySELECT"); 
}

void setup()
{   
    pinMode(10, OUTPUT);
    // set up the LCD's number of columns and rows: 
    lcd.begin(numCols,numRows);
    analogWrite(10, 100); //Controla intensidad backlight
    lcd.setCursor(0, 0);
    lcd.print("Laboratorio 2");
    lcd.setCursor(0, 1);
    lcd.print("Interrupciones");

    //Inicializacion de la cola de funciones
    fnqueue_init();
    //Inicializacion del driver de teclado
    teclado_setup();
    //Asocio funciones de callback a cada tecla
    key_up_callback(up_keyup,TECLA_UP);
    key_down_callback(down_keyup,TECLA_UP);

    key_up_callback(up_keydown,TECLA_DOWN);
    key_down_callback(down_keydown,TECLA_DOWN);

    key_up_callback(up_keyleft,TECLA_LEFT);
    key_down_callback(down_keyleft,TECLA_LEFT);

    key_up_callback(up_keyright,TECLA_RIGHT);
    key_down_callback(down_keyright,TECLA_RIGHT);

    key_up_callback(up_keyselect,TECLA_SELECT);
    key_down_callback(down_keyselect,TECLA_SELECT);
  
}
void loop()
{
    fnqueue_run();
}
