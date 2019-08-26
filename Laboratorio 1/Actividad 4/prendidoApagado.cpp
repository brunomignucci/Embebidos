#include <avr/io.h>
#include <util/delay.h>
// variables
int buttonState;// variable para leer el estado del pulsador
int modoOperacion = 0;//variable para cambiar los modos de operacion

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
        //Si cambio el estado del pulsador
        if(buttonState)
        {
            //Se espera un tiempo para evitar leer el ruido del rebote del pulsador
            _delay_ms(85);
            //Se vuelve a leer el estado del pulsador
            buttonState= PIND & (1<<PD2);
            //Si el cambio se mantiene, se interpreta como evento keydown/keyup valido
            if(buttonState)
            {
                if(!modoOperacion)
                {   //Si esta apagado lo prendo
                    modoOperacion=1;
                    //Encender el led
                    PORTB |= (1<<PB5);
                }
                else
                {   //Si esta prendido lo apago
                    modoOperacion=0;
                    //Apagar el led
                    PORTB &= ~(1<<PB5);
          
                }
         
            } 
           
    

        }
    }
}