#include "fnqueue.h"
#include "critical.h"
#include "device.h"
#include <stdint.h>

#define NUMERO_TECLAS 5

uint16_t adc_key_in;
uint16_t key=-1;
uint16_t oldkey=-1;
uint16_t keyTeclas=-1;



struct callbackstr teclas_callback[NUMERO_TECLAS];
//Rutinas que asocian rutinas keyup y keydown a cada boton
void key_down_callback(void (*handler)(),int tecla)
{
    if(tecla>=0 || tecla<5) teclas_callback[tecla].callbackDOWN=handler;
}
void key_up_callback(void (*handler)(), int tecla)
{
	if(tecla>=0 || tecla<5) teclas_callback[tecla].callbackUP=handler;
}

//Inicia el conversor analogico digital
void teclado_setup()
{

}
void teclado_loop()
{


}
uint16_t get_key(uint16_t k)
{

    return 0;
}
void procesar_adc()
{
    keyTeclas=get_key(key);
    if(key!=oldkey)
    {
        if(keyTeclas>=0)teclas_callback[key].callbackDOWN();
        else teclas_callback[key].callbackUP();
    }


}
void ISR()
{
	key=adc_key_in;
	fnqueue.add(procesar_adc());
}

