#ifndef DEVICE_H
#define DEVICE_H

struct callbackstr
{
    void (*callbackUP)();
    void (*callbackDOWN)();
}
//Rutinas que asocian rutinas keyup y keydown a cada boton
void key_down_callback(void (*handler)(),int tecla);
void key_up_callback(void (*handler)(), int tecla);

//Inicia el conversor analogico digital
void teclado_setup();
void teclado_loop();

#endif
