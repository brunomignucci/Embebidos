#include "fnqueue.h"
#include "critical.h"
#include "device.h"
#include <stdint.h>

#define NUMERO_TECLAS 5

uint16_t adc_key_in;
uint16_t key=-1;
uint16_t oldkey=-1;
uint16_t keyTeclas=-1;

// Iniciar variables. 
volatile boolean readFlag = false; 
// Valor leído volatile 
int analogVal; 
void enviardato();

struct callbackstr teclas_callback[NUMERO_TECLAS];

//Rutinas que asocian rutinas keyup y keydown a cada boton
void key_down_callback(void (*handler)(),int tecla)
{
    if(tecla>=0 || tecla<5) 
		teclas_callback[tecla].callbackDOWN=handler;
}
void key_up_callback(void (*handler)(), int tecla)
{
	if(tecla>=0 || tecla<5) 
		teclas_callback[tecla].callbackUP=handler;
}

//Inicia el conversor analogico digital
void setup()
{	
    // 1. Suspender interrupciones. 
    cli();
	// 2. Programar temporizador 0 para producir interrupciones a la tasa de muestreo de 4 kHz. 
	TCCR0A &= 0x00;
	TCCR0B &= 0x00; 
	TCCR0A |= (0 << WGM00) | (1 << WGM01); 
	TCCR0B |= (0 << WGM02);
	TCNT0 &= 0x00;
	TCCR0B |= (1 << CS00) | (1 << CS01) | (0 << CS02);
	OCR0A |= 62 ; 
	TIMSK0 |= (1 << OCIE0A);
	// 3. Programar conversor A/D para convertir a la máxima velocidad de reloj, pero empleando el temporizador 0 como fuente de disparo (Timer/Counter0 Compare Match A). 
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); 
	ADMUX |= (1 << REFS0) | (0 << REFS1); 
	ADMUX |= (0 << MUX0) | (0 << MUX1) | (0 << MUX2) | (0 << MUX3); 
	ADMUX |= (0 << ADLAR); 
	ADCSRB |= (1 << ADTS0) | (1 << ADTS1) | (0 << ADTS2); 
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	ADCSRA |= (1 << ADIE); 
	// habilito las interrupciones
	sei(); 
	ADCSRA |= (1 << ADSC); 
	pinMode(13, OUTPUT); 
	// Configurar puerto serial para recibir datos a la tasa de muestreo. 
	//Serial.begin(2000000); /*velocidad maxima para que no interfiera con otros procesos*/ 
	/*
	while (!Serial) 
	{ ; // Esperar a que el puerto inicie 
	} 
	*/
}

 void loop() 
{
	/*
	// Si hay un dato lo voy a procesar
	if (readFlag) 
	{
		readFlag = false;		
		fnqueue_add(procesar_adc);
	} 
	*/
	//esto lo puedo descomentar si quiero y sacar el cola_add de la ISR y trabajar con flags
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

uint16_t get_key(unsigned int input)
{
	// a mi entender esto no cambia
    uint16_t k;
    for (k = 0; k < NUM_KEYS; k++)
        if (input < adc_key_val[k])
            return k;

    if (k >= NUM_KEYS)
        k = -1;     // No valid key pressed

    return k;
}

void procesar_adc()
{
    keyTeclas=get_key(key);
    if(key!=oldkey)
    {
        if(keyTeclas>=0)
			teclas_callback[key].callbackDOWN();
        else
			teclas_callback[key].callbackUP();
    }


}

void ISR(){
	TIFR0 |= (1 << OCF0A);
	// Bajar la bandera del disparo por timer0. 
	//readFlag = true;
	// Leer dato 
	key = ADCL | (ADCH << 8);
	fnqueue_add(procesar_adc); // si pongo aca el cola_add el read flag no tiene sentido
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