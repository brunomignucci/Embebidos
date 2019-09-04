#include <stdbool.h>
#include <stdint.h>
#include "fnqueue.h"
#include "critical.h"

/*DEVICE.H*/
/*
 * 
 * strcut callbackstr{
 *    void (*callbackup)()
 *    voice(*callbackdown)();
 * }
 * keyupcallback(handler,tecla);
 * keydowncallback(hanlder,tecla);
 * teclado.setup();
 * teclado.loop();
 * 
 * 
 * key up callback y la key down callback (handler,tecla)
 * setup(){
 *  inicializar el adc
 * }
 * loop(){
 * que puede ir o no
 * }
 * 
 * DEVICE.CPP
 * 
 * struct callbackstr teclascallback[5]((((num teclas)))))
 * keyupcallback(handler,tecla)
 * 
 * 
 * key downcallback(handler, tecla)
 * 
 * ISR(adc_vect){
 *    fnqueue.add(procesar_adc())
 * }
 * 
 * procesar_adc(){
 *  keyteclas=setkey(key)
 *  if (key!=oldkey){
 *    if (keyteclas>=0){
 *      teclascallback[key].callbackkeydown():
 *    }
 *    else{
 *      teclascallback[oldkey].callbackup();
 *    }
 *  }
 * }
 * 
 *  y especificar las funciones declaradas en el .h
 * 
 * 
 * 
 */





/*
void ISR_boton_up()
{
    fnqueue_add(void (*function)(void)) 
}

void rutina_boton_up()
{
    
}
*/
