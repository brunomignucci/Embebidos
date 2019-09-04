#include "fnqueue.h"
#include "critical.h"
#include "device.h"

#define NUMERO_TECLAS 5


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

