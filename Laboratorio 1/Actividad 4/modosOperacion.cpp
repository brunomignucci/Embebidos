#include <avr/io.h>
#include <util/delay.h>

// variables
int buttonState = 0;// variable para leer el estado del pulsador
int modoOperacion = 0;//variable para cambiar los modos de operacion
int lastButtonState= 0;//variable para indicar el estado anterior del boton
int prendido = 0;//variable para indicar si el led esta prendido o no
int contadorModo1 = 4;
int contadorModo2 = 2;
int contadorModo3 = 1;
int contadorModo4Prendido = 2;
int contadorModo4Apagado = 4;

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
        //Se espera un tiempo para evitar leer el ruido del rebote del pulsador
        _delay_ms(250); //Además va a ser nuestra unidad de tiempo
        //Leer el estado del pulsador
        buttonState = PIND & (1<<PD2);
        if(buttonState)
        {
            _delay_ms(85);
            //Se vuelve a leer el estado del pulsador
            buttonState= PIND & (1<<PD2);
            if(buttonState)
            {
                //si el estado del botón perdura lo es un evento valido y
                //tenemos que cambiar el modo de operacion
                modoOperacion = (modoOperacion+1)%6;
                //Tambien vamos a reiniciar los contadores
                contadorModo1 = 4;
                contadorModo2 = 2;
                contadorModo3 = 1;
                contadorModo4Prendido = 2;
                contadorModo4Apagado = 4;
                //buttonState=0;//Esto està para que entre en el switch
            }

        }
        if(!buttonState)
        {
            //No hubo evento keyup o keydown valido por lo que resto el contador del modo
            //que esté activo
            switch(modoOperacion)
            {
                case 0:
                    if(!prendido)
                    {
                        PORTB |= (1<<PB5);//Encender el led
                        prendido = 1;
                    }
                    break;
                case 1:
                    if(prendido)
                    {
                        if(contadorModo1>0)
                        {
                            contadorModo1--;
                            if(!contadorModo1)
                            {//Significa que consumi la ultima porcion de tiempo
                                PORTB &= ~(1<<PB5);//Apagar el led
                                prendido=0;
                                contadorModo1=4;
                            } 
                        }
                        break;
                    }   
                    else
                    {
                        if(contadorModo1>0)
                        {
                            contadorModo1--;
                            if(!contadorModo1)
                            {//Significa que consumi la ultima porcion de tiempo
                                PORTB |= (1<<PB5);//Encender el led
                                prendido=1;
                                contadorModo1=4;
                            }
                        }
                        break;
                    }
                case 2:
                    if(prendido)
                    {
                        if(contadorModo2>0)
                        {
                            contadorModo2--;
                            if(!contadorModo2)
                            {//Significa que consumi la ultima porcion de tiempo
                                PORTB &= ~(1<<PB5);//Apagar el led
                                prendido=0;
                                contadorModo2=2;
                            } 
                        }
                        break;
                    }   
                    else
                    {
                        if(contadorModo2>0)
                        {
                            contadorModo2--;
                            if(!contadorModo2)
                            {//Significa que consumi la ultima porcion de tiempo
                                PORTB |= (1<<PB5);//Encender el led
                                prendido=1;
                                contadorModo2=2;
                            }
                        }
                        break;
                    }                
                case 3:
                    if(prendido)
                    {
                        if(contadorModo3>0)
                        {
                            contadorModo3--;
                            if(!contadorModo3)
                            {//Significa que consumi la ultima porcion de tiempo
                                PORTB &= ~(1<<PB5);//Apagar el led
                                prendido=0;
                                contadorModo3=1;
                            } 
                        }
                        break;
                    }   
                    else
                    {
                        if(contadorModo3>0)
                        {
                            contadorModo3--;
                            if(!contadorModo3)
                            {//Significa que consumi la ultima porcion de tiempo
                                PORTB |= (1<<PB5);//Encender el led
                                prendido=1;
                                contadorModo3=1;
                            }
                        }
                        break;
                    }                
                case 4:
                    if(prendido)
                    {
                        if(contadorModo4Prendido>0)
                        {
                            contadorModo4Prendido--;
                            if(!contadorModo4Prendido)
                            {//Significa que consumi la ultima porcion de tiempo
                                PORTB &= ~(1<<PB5);//Apagar el led
                                prendido=0;
                                contadorModo4Apagado=4;
                            } 
                        }
                        break;
                    }   
                    else
                    {
                        if(contadorModo4Apagado>0)
                        {
                            contadorModo4Apagado--;
                            if(!contadorModo4Apagado)
                            {//Significa que consumi la ultima porcion de tiempo
                                PORTB |= (1<<PB5);//Encender el led
                                prendido=1;
                                contadorModo4Prendido=2;
                            }
                        }
                        break;
                    }
                case 5:
                    if(prendido)
                    {
                        PORTB &= ~(1<<PB5);//Apagar el led
                        prendido = 0;
                    }
                    break;
            }

        }    

    
    
    }
  
}
