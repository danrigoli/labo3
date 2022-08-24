#ifndef __semaforo_H
#define __semaforo_H
#endif

int creo_semaforo();

void inicia_semaforo(int id_semaforo, int valor);

void levanta_semaforo(int id_semaforo);

void espera_semaforo(int id_semaforo);