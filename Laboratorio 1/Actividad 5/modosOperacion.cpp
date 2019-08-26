#include "HardwareSerial.h"
#include "Arduino.h"


// variables
int buttonState = 0;// variable para leer el estado del pulsador
int modoOperacion = 0;//variable para cambiar los modos de operacion
int prendido = 0;//variable para indicar si el led esta prendido o no
int contadorModo1 = 4;
int contadorModo2 = 2;
int contadorModo3 = 1;
int contadorModo4Prendido = 2;
int contadorModo4Apagado = 4;
int modoSerial =- 1;

//Constantes
const int buttonPin = 2;     // el pin del pulsador
const int ledPin =  13;      // el pin de led 

//modo 0= prendido
//modo 1=titilando 0.5hz
//modo 2=titilando a 1hz
//modo 3=titilando a 2hz 
//modo 4= medio segundo encendido y un segundo apagado
//modo 5= apagado
void setup()
{   //Inicializacion
    pinMode(ledPin,OUTPUT);
    pinMode(buttonPin,INPUT);
    //Inicializacion del puerto serial
    Serial.begin(9600);
}
void loop()
{
    //Se espera un tiempo para evitar leer el ruido del rebote del pulsador
    delay(250); //Además va a ser nuestra unidad de tiempo
    //Leer el estado del pulsador
    buttonState = digitalRead(buttonPin);
    //Leer entrada serial si esta disponible
    
    if (Serial.available()>0)
    {
        modoSerial=Serial.read()-48;
        Serial.read();//Leo el fin de linea y lo descarto
    }       
    if (modoSerial>-1)
    {
        modoOperacion=modoSerial;
        //Tambien vamos a reiniciar los contadores
        contadorModo1 = 4; 
        contadorModo2 = 2;
        contadorModo3 = 1;
        contadorModo4Prendido = 2;
        contadorModo4Apagado = 4;
        modoSerial=-1;
        //switch case modoOperacion para enviar data serial
        switch(modoOperacion)
        {
            case 0:
                Serial.println("Esta encendido");
                break;
            case 1:
                Serial.println("Frecuencia de 0.5hz");
                break;
            case 2:
                Serial.println("Frecuencia es 1hz");
                break;
            case 3:
                Serial.println("Frecuencia de 2hz");
                break;
            case 4:
                Serial.println("Medio segundo encendido y un segundo apagado");
                break;
            case 5:
                Serial.println("Esta apagado");
                break;
        }
    }
    if(buttonState)
    {
        delay(85);
        //Se vuelve a leer el estado del pulsador
        buttonState = digitalRead(buttonPin);
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
                    digitalWrite(ledPin, HIGH);//Encender el led
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
                            digitalWrite(ledPin, LOW);//Apagar el led
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
                            digitalWrite(ledPin, HIGH);//Encender el led
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
                            digitalWrite(ledPin, LOW);//Apagar el led
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
                            digitalWrite(ledPin, HIGH);//Encender el led
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
                            digitalWrite(ledPin, LOW);//Apagar el led
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
                            digitalWrite(ledPin, HIGH);//Encender el led
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
                            digitalWrite(ledPin, LOW);//Apagar el led
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
                            digitalWrite(ledPin, HIGH);//Encender el led
                            prendido=1;
                            contadorModo4Prendido=2;
                        }
                    }
                    break;
                }
            case 5:
                if(prendido)
                {
                    digitalWrite(ledPin, LOW);//Apagar el led
                    prendido = 0;
                }
                break;
        }

    }

}