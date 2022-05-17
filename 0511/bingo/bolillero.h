#ifndef _THREAD_H
#define _THREAD_H


#include <pthread.h>
#include "funciones.h"
#include "definiciones.h"

/* definicion de constantes */
#define N_DESDE 		        1
#define N_HASTA                 99
#define TIEMPO_COMPRUEBA_DESDE	500
#define TIEMPO_COMPRUEBA_HASTA  5000

/* estructura tipo bolillero */
struct tipo_bolillero
{
    int id_cola;
    int* carton_lleno;	
    int cantidad_jugadores;
};

/* definicion de tipo tbolillero */
typedef struct tipo_bolillero tbolillero;

/* declaro que mutex va a estar declarada en algun .c */
extern pthread_mutex_t mutex;

/* declaro funcion de thread jugador */
void *ThreadBolillero (void *parametro);

#endif