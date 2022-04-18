#ifndef __semaforo_H
#define __semaforo_H
#endif

//funcion que crea el semaforo
int creo_semaforo();

//inicia el semaforo
void inicia_semaforo(int id_semaforo, int valor);

//levanta el semaforo
void levanta_semaforo(int id_semaforo);

//espera semaforo
void espera_semaforo(int id_semaforo);