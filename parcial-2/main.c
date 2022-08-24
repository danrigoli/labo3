#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "funciones.h"
#include "definiciones.h"
#include "caballo.h"


int main(int argc, char *argv[])
{
	int cantidad_caballos;
	int id_cola_mensajes;
	int i = 0;
	tcaballo *datos_thread;
	pthread_t* idHilo;
	pthread_attr_t 	atributos;

	srand(time(NULL));

	    /* toma la cantidad de jugadores del parametro del proceso si existe, sino es 1 por default, si pasa un valor que no es un numero, termina el proceso */
    if (argc > 1) {
            cantidad_caballos = atoi(argv[1]);
        if (cantidad_caballos < 1) {
            printf("Error: cantidad de jugadores debe ser mayor a 0\n");
            exit(0);
        }
    }


	/* consigue el id de la cola con la clave base */
	id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);

	/* guarda el espacio en memoria de idHilo con la cantidad de threads */
	idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantidad_caballos);

	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
	
	/* guarda el espacio en memoria de datos_thread con la cantidad de threads */
	datos_thread = (tcaballo*) malloc(sizeof(tcaballo)*cantidad_caballos);
	
	for(i = 0; i < cantidad_caballos; i++)
	{
		/* guarda los parametros a pasar al thread*/
		datos_thread[i].nro_caballo = i;
		datos_thread[i].id_cola = id_cola_mensajes;
		/* crea el thread */
		pthread_create (&idHilo[i], &atributos, ThreadCaballo, &datos_thread[i]);
	}

	/* espera a que todos los threads terminen y los une */
	for(i = 0; i< cantidad_caballos; i++)
	{
		pthread_join (idHilo[i], NULL);
	}

	printf("El caballo ganador es el caballo %d\n", datos_thread[0].nro_caballo);
			
	return 0;
}