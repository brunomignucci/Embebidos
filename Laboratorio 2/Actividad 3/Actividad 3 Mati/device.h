#ifndef DEVICE_H
#define DEVICE_H
#define CANTIDAD_TECLAS 5 	// Cantidad teclas

struct callbackstr
{
    void (*callbackUP)();
    void (*callbackDOWN)();
};

//Rutinas que asocian rutinas keyup y keydown a cada boton
void key_down_callback(void (*handler)(),int tecla);
void key_up_callback(void (*handler)(), int tecla);

// Inicia el conversor analogico digital
void teclado_setup();

//Defino una estrcutura para el concepto de teclado_loop
struct teclado{
	volatile int16_t ultima_tecla; // guardará la ultima tecla presionada
	void (*handlers_up[CANTIDAD_TECLAS])(); //arreglo de handlers para keys up
	void (*handlers_down[CANTIDAD_TECLAS])(); // arreglo de handlers para key down
};
#endif
