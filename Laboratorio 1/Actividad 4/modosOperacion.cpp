#include <avr/io.h>
#include <util/delay.h>

// variables
int buttonState = 0;// variable para leer el estado del pulsador
int modoOperacion = 0;//variable para cambiar los modos de operacion
int lastButtonState= 0 ;//variable para indicar el estado anterior del boton
int prendido=0;//variable para indicar si el led esta prendido o no
//modo 0= prendido
//modo 1=titilando 0.5hz
//modo 2=titilando a 1hz
//modo 3=titilando a 2hz 
//modo 4= medio segundo encendido y un segundo apagado
//modo 5= apagado
int main()
{
  //Inicializar el pin del pulsador como entrada
  DDRD &= ~(1<<DDD2);
  //Inicializar el pin del led como salida
  DDRB |= (1<<DDB5);

  while (1)
  {
    //Leer el estado del pulsador
    buttonState = PIND & (1<<PD2);
    
    if(buttonState) { //Si el pulsador está apretado cambio el modo de operacion
      modoOperacion=(modoOperacion+1)%6;
    }
    
    switch(modoOperacion)
    {
      case 0:
            PORTB |= (1<<PB5);//Encender el led
            prendido=1;
            break;
      case 1:
            if(prendido){
                  PORTB &= ~(1<<PB5);//Apagar el led
                  _delay_ms(1000);
                  prendido=0;
                  }
            else{
                  PORTB |= (1<<PB5);//Encender el led
                  _delay_ms(1000);
                  prendido=1;
              }
            break;
      case 2:
            if(prendido){
                  PORTB &= ~(1<<PB5);//Apagar el led
                  _delay_ms(500);
                  prendido=0;
                  }
            else{
                  PORTB |= (1<<PB5);//Encender el led
                  _delay_ms(500);
                  prendido=1;
              }
            break;
      case 3:
            if(prendido){
                  PORTB &= ~(1<<PB5);//Apagar el led
                  _delay_ms(250);
                  prendido=0;
                  }
            else{
                  PORTB |= (1<<PB5);//Encender el led
                  _delay_ms(250);
                  prendido=1;
              }
            break;
      case 4:
            if(prendido){
                  PORTB &= ~(1<<PB5);//Apagar el led
                  _delay_ms(1000);
                  prendido=0;
                  }
            else{
                  PORTB |= (1<<PB5);//Encender el led
                  _delay_ms(500);
                  prendido=1;
              }
            break;
      case 5:
            PORTB &= ~(1<<PB5);//Apagar el led
            prendido=0;
            break;
    }

  }
}