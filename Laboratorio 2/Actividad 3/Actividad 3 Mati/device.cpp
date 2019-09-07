#include "fnqueue.h"
#include "critical.h"
#include "device.h"
#include <stdint.h>
#include <avr/interrupt.h>

#define NUMERO_TECLAS 5

static uint16_t adc_key_val[5] ={30, 150, 360, 535, 760 };
static uint16_t adc_key_in;
static uint16_t key=255;
static uint16_t contador=0;//contador para debouncing de interrupciones



// Iniciar variables. 
//volatile boolean readFlag = false; 
// Valor leído volatile 
//int analogVal; 
//void enviardato();

volatile struct teclado var_teclado;

//Rutinas que asocian rutinas keyup y keydown a cada boton
void key_down_callback(void (*handler)(),int tecla)
{	
    if(tecla>=0 || tecla<5) 
		var_teclado.handlers_down[tecla]=handler;
}
void key_up_callback(void (*handler)(), int tecla)
{
	if(tecla>=0 || tecla<5) 
		var_teclado.handlers_up[tecla]=handler;
}

//Inicia el conversor analogico digital
void teclado_setup()
{	
	/*
	1 Si esta "high" la ultima tecla apretada -> no se apreto ninguna tecla. inicializarla en 255
		
	*/
  var_teclado.ultima_tecla = 255;  // no se apretó ninguna tecla
  //for ( int i = 0; i < 5; i++ )
  //{
    //var_teclado.handlers_up[i] = NULL;
   // var_teclado.handlers_down[i] = NULL;
  //}
  
	
    // 1. Suspender interrupciones. 
    cli();
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);//  prescaler al máximo. para modular velocidad de muestreo
	ADMUX |= (1 << REFS0);      // voltaje referencia.
	ADMUX &= ~(1 << ADLAR);     // left aligned (sheet: 24.9.3.1/2).
	ADCSRA |= (1 << ADATE);     // habilitar auto trigger.
	ADCSRA |= (1 << ADEN);      // enable ADC.
	ADCSRA |= (1 << ADIE);      // habilitar interrupciones.
	ADCSRB &= ~(1 << ADTS2) & ~(1 << ADTS1) & ~(1 << ADTS0); // habilitar auto-
                               // trigger.
	// habilito las interrupciones
	sei(); 
	ADCSRA |= (1 << ADSC); 
	//pinMode(13, OUTPUT); 
	// Configurar puerto serial para recibir datos a la tasa de muestreo. 
	//Serial.begin(2000000); /*velocidad maxima para que no interfiera con otros procesos*/ 
	/*
	while (!Serial) 
	{ ; // Esperar a que el puerto inicie 
	} 
	*/
}

/*
void enviardato() 
{
	Serial.println(analogVal); //Serial.println("1"); 
}
*/

/*
uint16_t get_key(uint16_t k)
{
	// este es el que habia puesto brunito
    return 0;
}*/

/*int get_key(int input)
{
	// a mi entender esto no cambia
    int k;
    for (k = 0; k < NUMERO_TECLAS; k++)
        if (input < adc_key_val[k])
            return k;

    if (k >= NUMERO_TECLAS)
        k = -1;     // No valid key pressed

    return k;
}
*/
 
// Convert ADC value to key number
int get_key(unsigned int input)
{
    int k;
    for (k = 0; k < NUMERO_TECLAS; k++)
        if (input < adc_key_val[k])
            return k;

    if (k >= NUMERO_TECLAS)
        k = 255;     // No valid key pressed

    return k;
}

void procesar_adc()
{
            key=get_key(adc_key_in);
            if ( 0 <= key && key <= 4 )
            {
                var_teclado.handlers_down[key]();
                var_teclado.ultima_tecla = key;
            }
            else
            {
                if ( var_teclado.ultima_tecla != 255 )
                    var_teclado.handlers_up[var_teclado.ultima_tecla]();
                var_teclado.ultima_tecla = 255;
            }

}

ISR(ADC_vect)
{
  contador++;
  if(contador==40){
     adc_key_in=(ADCL) | (ADCH << 8);
  }
  else
  {
      if(contador==100)
      {
        fnqueue_add(procesar_adc); // si pongo aca el cola_add el read flag no tiene sentido
        
      }
      if(contador==120)contador=0;
  }
}




/*
void ISR()
{
	// ESTE ISR es el que escribio brunito
	key=adc_key_in;
	fnqueue_add(procesar_adc);
}*/

/*

ISR(ADC_vect) 
{
	TIFR0 |= (1 << OCF0A);
	// Bajar la bandera del disparo por timer0. 
	readFlag = true;
	// Leer dato 
	analogVal = ADCL | (ADCH << 8); 
} 

ISR(TIMER0_COMPA_vect) { }
*/
