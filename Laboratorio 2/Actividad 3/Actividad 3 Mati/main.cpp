#include <stdbool.h>
#include <stdint.h>
#include <queue>
#include "Arduino.h"
// include the library code:
#include <LiquidCrystal.h>
#include "fnqueue.h"
#include "device.h"
#include "HardwareSerial.h"


/*

COMENTARIOS

No se como importar el String de arduino

falta implementar la parte qye duce que el sistema pasa de MAD A MP al no registrarse ninguna intervencion del usuario sobre los pulsadores durante mas de 5 segundos
estrategia: tener una variable que cuenta los segundos y la actualizo por cada segundo que pasa independientemente del cronometro y setearla a 0 cada vez que se presiona un boton y que si llega a 300 segundos lance una interrupcion que cambie el modo de operacion a MP


*/ 

// these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 16;

//Constantes de los botones del display
const int TECLA_UP = 1;
const int TECLA_DOWN = 2;
const int TECLA_LEFT = 3;
const int TECLA_RIGHT = 0;
const int TECLA_SELECT = 4;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);



/*
* Esta variable tiene el siguiente proposito: suponiendo que el temporizador a usar lanza interrupciones cada 1ms y que la interrucpion 
* haría milisegundos++;
* 

ISR (temp2){
	diezmilisegundos++;
}

*/
static uint16_t diezmilisegundos; 

// minutos y segundos 
static uint16_t minutos;
static uint16_t segundos;

// minutos y segundos del select
static uint16_t minutosDownSelect;
static uint16_t segundosDownSelect;

// brillo del display
static uint16_t brilloDisplay;

// defino el string para el cronometro
static string lcd_texto;

// cola que guarda los tiempos 
static queue<String> tiempos_guardados;

// entero para la referencia de que tiempo voy mostrando
static uint16_t referencia;

//temporizador para el cronometro
//static temporizador temp;

// modos de operacion
/*
0: MCA	: el cronometro cuenta en sentido ascendente el tiempo transcurrido el display muestra de manera continuada dicho tiempo
1: MP	: el cronometro se pausa y el display muestra el tiempo trascurrido desde el ultimo reseteo hasta que se pausa
2: MVT	: el cronometro muestra hasta 10 tiempos previamente guardados
3: MAD	: en este modo es posible ajustar la intensidad luminosa del blacklight del display LCD el cual se mantendra mostrando el 
tiempo pausado, el modo y el brillo actual usando los pulsadores up y down: 20 40 60 80 y 100.
*/
static uint16_t modoOperacion;

void mensajeBienvenida()
{
	/*
		metodo para el mensaje de bienvenida que pide el enunciado
		Laboratorio nº2  
		Sist. embebidos  
		2do cuat 2019    
		Mignucci-Rios    
	*/
	
    lcd.setCursor(0, 0);
    lcd.print("Laboratorio nº2  ");
    lcd.setCursor(0, 1);
    lcd.print("Sist. embebidos  ");
	delay(1000);
	lcd.setCursor(0, 0);
    lcd.print("2do cuat 2019    ");
    lcd.setCursor(0, 1);
    lcd.print("Mignucci-Rios    ");
}

boolean masDeTres()
{
	// funcion para devolver si la diferencia en segundos entre los minutos y segundos del down select y el actual es mayor o menor a 3
	uint16_t min_actual=minutos;
	uint16_t seg_actual=segundos;
	boolean ret=false;
	if (seg_actual-segundosDownSelect>3)
	{
		ret=true;
	}
	else
	{	
		uint16_t aux=min_actual-minutosDownSelect;
		if (aux)
		{
			if (seg_actual+60-segundosDownSelect>3)
			{
				ret=true;
			}
		}
	}
	return ret;
}

void pausarTemp()
{
	// funcion para desactivar el temporizador
	
}

void iniciarTemp()
{
	//funcion para activar el temporizador
}

void reiniciarTemp()
{
	//funcion para reiniciar el temporizador
	minutos=0;
	segundos=0;
	diezmilisegundos=0;
}

void up_keyup()
{
	// creo que no hace falta
}

void down_keyup()
{
	if (modoOperacion==0)
	{
		// estoy en MCA, tengo que pausar el contador
		/*
		* Se me ocurre deshabilitar las interrupciones del temporizador 2 o el que sea que usemos
		*
		*/
		lcd.setCursor(0, 0);
		lcd.print("Pausa           ");
		lcd.setCursor(0, 1);
		pausarTemp();		
	}
	else
	{
		if (modoOperacion==1)
		{
			// estoy en MP, tengo que reanudar el conteo temporal ascendente
			/*
			* Se me ocurre habilitar las interrupciones del contador
			*			
			*/
			lcd.setCursor(0, 0);
			lcd.print("MP              ");
			lcd.setCursor(0, 1);
			while (modoOperacion==1)
			{
				//mientras este en este modo actualizo el lcd con el valor del cronometro
				/*
				* Supongo que si ocurre una interrupcion del SELECT me va a cambiar el modo de operacion
				* las interrupciones pueden modificar las variables minutos y segundos o lo puedo hacer acá
				*/
				if (diezmilisegundos==100)// asumo que la interrupcion del temporizador se lanza cada 10ms
				{
					segundos++;
					if (segundos==60)
					{
						segundos=0;
						minutos++;						
					}
					lcd_texto= String(minutos) + String(" : ") + String(segundos); 
					//alternativa para quitar el ruido del lcd
					// creo que es mejor que el while de mas abajo
					if (minutos >9 & segundos>9)
					{
						lcd_texto.concat("          ");//10 espacios
					}
					else{
						if (minutos>9 | segundos>9)
						{
							lcd_texto.concat("           ");//11 espacios
						}
						else
						{
							lcd_texto.concat("            ");//12 espacios
						}
					}
					lcd.print(lcd_texto);
				}				
			}
		}
		else
		{
			if (modoOperacion==2)
			{
				// estoy en MVT, tengo que mostrar el siguiente tiempo guardado
				/*
				* a que se refiere con siguiente, cual es la referencia?
				* asumo que la referencia la hacemos nostoros con un entero de 0 a 9
				* muestra el tiempo en la posicion en el que esta la referencia
				*	
				*/
				lcd.setCursor(0, 0);
				lcd.print("MVT             ");
				lcd.setCursor(0, 1);				
				queue<String> colaAuxiliar=tiempos_guardados;
				if (!colaAuxiliar.empty())
				{
					for (int i=0;i<=referencia;i++)
					{					
						String tiempo=colaAuxiliar.front();
						colaAuxiliar.pop();					
					}
					referencia++;
					if (referencia==10){
						referencia=0; 
					}
					lcd.print(tiempo); 
				}
				else
				{
					lcd.print(String("No times saved   "));
				}
				 
			}
			else
			{
				// estoy en MAD, tengo que incrementar un 20% el brillo del display
				lcd.setCursor(0, 0);
				lcd.print("MAD             ");
				lcd.setCursor(0, 1);
				if (brilloDisplay!=255)
				{
					// no está al 100, puedo aumentarle un 20%, sumarle 51 (255/cantidad niveles brillo)					
					brilloDisplay+=51;
					analogWrite(10, brilloDisplay);
				}
			}
		}
	}
    //lcd.print("DOWN keyUP      "); 
}
void up_keydown()
{
    // creo que no lo necesitamos
}
void down_keydown()
{
	if (modoOperacion==0)
	{
		// estoy en MCA, tengo que guardar el tiempo actual en la cola
		lcd.setCursor(0, 0);
		lcd.print("MCA             ");
		lcd.setCursor(0, 1);
		lcd_texto=String(minutos) + String(" : ") + String(segundos);
		while (lcd_texto.lenght()!=17){ // para que no muestre ruido el led
			lcd_texto.concat(" ");
		}
		tiempos_guardados.push(lcd_texto);		
	}
	else
	{
		if (modoOperacion==1)
		{
			// estoy en MP, tengo que guardar el tiempo actual y reiniciar el cronometro
			lcd.setCursor(0, 0);
			lcd.print("MP              ");
			lcd.setCursor(0, 1);
			if (tiempos_guardados.size()==10){
				tiempos_guardados.pop();// el primero que guarde lo tiro para darle espacio al nuevo				
			}
			lcd_texto=String(minutos) + String(" : ") + String(segundos);
			while (lcd_texto.lenght()!=17){// para que no muestre ruido el led
				lcd_texto.concat(" ");
			}
			tiempos_guardados.push(lcd_texto);
			reiniciarTemp();
		}
		else
		{
			if (modoOperacion==2)
			{
				/*estoy en MVT, tengo que mostrar el anterior tiempo guardado
				* asumo que la referencia la hacemos nostoros con un entero de 0 a 9
				* muestra el tiempo en la posicion en el que esta la referencia-1
				*	
				*/	
				lcd.setCursor(0, 0);
				lcd.print("MVT             ");
				lcd.setCursor(0, 1);
				queue<String> colaAuxiliar=tiempos_guardados;
				referencia--;
				if (referencia==0){
					referencia=tiempos_guardados.size(); 
				}
				for (int i=0;i<=referencia ;i++)
				{					
					String tiempo=colaAuxiliar.front();
					colaAuxiliar.pop();					
				}				
				lcd.print(tiempo); 
			}
			else
			{
				// estoy en MAD, tengo que decremental un 20% el brillo del display
				lcd.setCursor(0, 0);
				lcd.print("MAD             ");
				lcd.setCursor(0, 1);
				if (brilloDisplay!=0)
				{
					// no está al 0, puedo dismunuirle un 20%, restarle 51 (255/cantidad niveles brillo)					
					brilloDisplay-=51;
					analogWrite(10, brilloDisplay);
				}
			}
		}
	}
}

void up_keyselect()
{
	if (modoOperacion==1)
	{
			// estoy en MP, tengo que pasar a MVT si se mantiene menos de 3 segundos o a MAD si se mantiene mas de 3 segundos
			/*
			* 1 registrar el tiempo en que se detecto que se soltó
			* 2 si es menor, entrar en MVT, sino a MAD
			*/
			// if ((tiempo_select_up-tiempo_select_down)<3)
			
			if (masDeTres()){
				modoOperacion=3;
			}
			else{
				modoOperacion=2;
			}
	}
	else
	{
		if (modoOperacion==2)
		{
			// estoy en MVT, tengo que pasar a MP
			modoOperacion=1;
		}
		else
		{
			// estoy en MAD, tengo que volver a MP
			modoOperacion=1;
		}
	}		
}
void down_keyselect()
{
	if (modoOperacion==1)
	{
		// estoy en MP, tengo que pasar a MVT si se mantiene menos de 3 segundos
		/*
		* Supongo que no tiene que ir implementado aca, que para el select es que estan los key up, para comparar el tiempo que pasó
		* desde que se presiono hasta que se levanto
		* 1 registrar el tiempo en que se presiono
		*
		*/
		minutosDownSelect=minutos;
		segundosDownSelect=segundos;
	}
	else
	{
		if (modoOperacion==2)
		{
			// estoy en MVT, tengo que pasar a MP
			modoOperacion=1;
		}
		else
		{
			// estoy en MAD, tengo que volver a MP
			modoOperacion=1;
		}
	}
}

int main()
{
	referencia=0;
	mostrar=false;
	//inicializo el modo de operacion en MP
	modoOperacion=1;
	//inicializo el cronometro en 00:00
	minutos=0;
	segundos=0;
	
    init();
    
    // inicializar pin 10 como salida
    pinMode(10, OUTPUT);
  
    //Serial.begin(9600);
  
    // set up the LCD's number of columns and rows:
    lcd.begin(numCols,numRows);
	brilloDisplay=204; 
    analogWrite(10, brilloDisplay); // lo inicializo en 80% como pide el enunciado
	

    fnqueue_init();
	
    //Serial.println("Inicio Cola");
    teclado_setup();
	
    //Asocio funciones de callback a cada tecla
	
	key_up_callback(&up_keyup,TECLA_UP);
    key_down_callback(&down_keyup,TECLA_UP);
	
	key_up_callback(&up_keydown,TECLA_DOWN);
    key_down_callback(&down_keydown,TECLA_DOWN);	
	
    key_up_callback(&up_keyselect,TECLA_SELECT);
    key_down_callback(&down_keyselect,TECLA_SELECT)
	
    mensajeBienvenida();   

    while (1)
        fnqueue_run();
    return 0;
}

