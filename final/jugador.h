#ifndef _THREAD_H
#define _THREAD_H


#include <pthread.h>

/* definicion de constantes */
#define CANTIDAD 		        6
#define CANTIDAD_INTENTOS       3
#define TIEMPO_COMPRUEBA_DESDE	500
#define TIEMPO_COMPRUEBA_HASTA  5000

/* estructura tipo jugador */
struct tipo_jugador
{
    int 	nro_jugador;
	int 	cantidad_intentos;
    int     id_cola;	
    int     puntaje;
};

/* definicion de tipo tjugador */
typedef struct tipo_jugador tjugador;

/* declaro que mutex va a estar declarada en algun .c */
extern pthread_mutex_t mutex;

/* declaro funcion de thread jugador */
void *ThreadJugador (void *parametro);

#endif