#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "funciones.h"
#include "definiciones.h"
#include "jugador.h"

/* mutex para que no tiren multiples jugadores al mismo tiempo */
pthread_mutex_t mutex;

int main(int argc, char *argv[])
{
	int id_cola_mensajes;
	int cantidad_jugadores;
	int i;
	int max_puntos;
	int posicion_ganador;
	int id_memoria;
	int *memoria;
	tjugador *datos_thread;
	pthread_t* idHilo;
	pthread_attr_t 	atributos;

	/* tomo memoria compartida para tener la misma cantidad de jugadores entre procesos */
    id_memoria = creo_id_memoria(sizeof(int), CLAVE_BASE);
    memoria = (int*)creo_memoria(id_memoria);

	/* consigue el id de la cola con la clave base */
	id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);

	/* seed del random con el tiempo actual para que no sea siempre lo mismo */
	srand(time(NULL));
	
	/* tomo la cantidad_jugadores de la memoria, si esta en 0, quiere decir que no inicializo el proceso bolera la cantidad */
	cantidad_jugadores = memoria[0];
	if (cantidad_jugadores == 0 || cantidad_jugadores > 3)
	{
		printf("No inicializo primero la bolera, por ende no sabemos la cantidad de jugadores\n");
		return -1;
	}
	

	/* guarda el espacio en memoria de idHilo con la cantidad de threads */
	idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*CANTIDAD);

	/* inicializo los atributos de los threads */
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
	
	/* guarda el espacio en memoria de datos_thread con la cantidad de threads */
	datos_thread = (tjugador*) malloc(sizeof(tjugador)*CANTIDAD);
	
	/* creo tantos threads como jugadores haya */
	for(i = 0; i < cantidad_jugadores; i++)
	{
		/* guarda los parametros a pasar al thread*/
		datos_thread[i].nro_jugador = i;
		datos_thread[i].cantidad_intentos = CANTIDAD_INTENTOS;
		datos_thread[i].id_cola = id_cola_mensajes;
		datos_thread[i].puntaje = 0;
		/* crea el thread */
		pthread_create (&idHilo[i], &atributos, ThreadJugador, &datos_thread[i]);
	}

	
	/* espera a que todos los threads terminen y los une */
	for(i = 0; i < cantidad_jugadores; i++)
	{
		pthread_join (idHilo[i], NULL);
	}	

	/* imprime el ganador */
	max_puntos = 0;
	for(i = 0; i < cantidad_jugadores; i++)
	{
		printf("El jugador %d tiene %d puntos\n", i, datos_thread[i].puntaje);
		if(datos_thread[i].puntaje > max_puntos)
		{
			max_puntos = datos_thread[i].puntaje;
			posicion_ganador = i;
		}
	}	
	printf("El ganador es el jugador %d con %d puntos!\n", posicion_ganador, max_puntos);
			
	return 0;
}



