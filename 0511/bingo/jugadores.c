#include "jugador.h"
		
pthread_mutex_t mutex;


int main(int argc, char *argv[])
{
	int cantidad = 1;
	int i = 0;
	tjugador *datos_thread;
	pthread_t* idHilo;
	pthread_attr_t 	atributos;
	int id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);

	srand(time(NULL));
	
	/* toma la cantidad de jugadores del parametro del proceso si existe, sino es 1 por default */
    if (argc > 1) cantidad = atoi(argv[1]);

	/* inicializa el vector de ids de hilos y inicializa los attr_t */
	idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantidad);
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
	
	datos_thread = (tjugador*) malloc(sizeof(tjugador)*cantidad);
	
	/* crea los hilos con sus parametros */
	for(i = 0; i < cantidad; i++)
	{
		datos_thread[i].nro_jugador = i;
		datos_thread[i].cantidad_jugadores = cantidad;
		datos_thread[i].id_cola = id_cola_mensajes;
		
		pthread_create (&idHilo[i], &atributos, ThreadJugador, &datos_thread[i]);
	}

	/* espera a que todos los hilos terminen y los une */
	for(i = 0; i< cantidad; i++)
	{
		pthread_join (idHilo[i], NULL);
	}	
			
	return 0;
}



