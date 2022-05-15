#ifndef _THREAD_H
#define _THREAD_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>


/* definicion de constantes */
#define CANT_CARTON 		5
#define BOLILLA_DESDE 		1
#define BOLILLA_HASTA 		99
#define TIEMPO_COMPRUEBA	10
#define TIEMPO_BOLILLAS		500

/* Estructura tipo jugador */

struct tipo_jugador
{
	int 	nro_jugador;	
	int*	bolillas;						
	int  	cantidad_aciertos;
	
};

typedef struct tipo_jugador tjugador;

/* declaro que mutex va a estar declarada en algun .c */
extern pthread_mutex_t mutex;
extern int g_carton_lleno;
extern int g_control;

/* declaro funcion de thread jugador */
void *ThreadJugadores (void *parametro);

#endif
