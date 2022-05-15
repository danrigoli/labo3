#ifndef _THREAD_H
#define _THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>


/* definicion de constantes */
#define DESDE 		            1
#define CANTIDAD_NUMEROS        99
#define TIEMPO_COMPRUEBA_DESDE	500
#define TIEMPO_COMPRUEBA_HASTA  5000

/* estructura tipo jugador */
struct tipo_jugador
{
    int 	nro_jugador;
	int* 	alguien_acerto;	
	int  	penso;
	int     cantidad_aciertos;
};

/* definicion de tipo tjugador */
typedef struct tipo_jugador tjugador;

/* declaro que mutex va a estar declarada en algun .c */
extern pthread_mutex_t mutex;

/* declaro funcion de thread jugador */
void *ThreadJugadores (void *parametro);

#endif
