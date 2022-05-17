#include "jugador.h"

/* mutex para que no disparen multiples jugadores al mismo tiempo */
pthread_mutex_t mutex;


int main(int argc, char *argv[])
{

	int id_cola_mensajes;
	int i = 0;
	int* vector_tambor = malloc(sizeof(int)*CANTIDAD);
	tjugador *datos_thread;
	pthread_t* idHilo;
	pthread_attr_t 	atributos;

	srand(time(NULL));
	
	/* consigue el id de la cola con la clave base */
	id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);

	/* guarda el espacio en memoria de idHilo con la cantidad de threads */
	idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*CANTIDAD);

	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
	
	/* guarda el espacio en memoria de datos_thread con la cantidad de threads */
	datos_thread = (tjugador*) malloc(sizeof(tjugador)*CANTIDAD);
	
	for(i = 0; i < CANTIDAD; i++)
	{
		/* guarda los parametros a pasar al thread*/
		datos_thread[i].nro_jugador = i;
		datos_thread[i].vector_tambor = vector_tambor;
		datos_thread[i].id_cola = id_cola_mensajes;
		/* crea el thread */
		pthread_create (&idHilo[i], &atributos, ThreadJugador, &datos_thread[i]);
	}

	/* espera a que todos los threads terminen y los une */
	for(i = 0; i< CANTIDAD; i++)
	{
		pthread_join (idHilo[i], NULL);
	}	
			
	return 0;
}



