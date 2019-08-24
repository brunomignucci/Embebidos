/*
  Button
*/ 
 
#include "Arduino.h"
// Constantes

const int buttonPin = 2;     // el pin del pulsador
const int ledPin =  13;      // el pin de led 

int buttonState = 0;         // variable que lee el estado del pulsador

void setup() {
 // Inicializacion
  pinMode(ledPin, OUTPUT);      
  pinMode(buttonPin, INPUT);     
}

void loop(){

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {     

    digitalWrite(ledPin, HIGH);  
  } 
  else {
    digitalWrite(ledPin, LOW); 
  }
}
