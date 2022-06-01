#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "bolillero.h"
#include "funciones.h"
#include "definiciones.h"

#define VEHICULOS_DESDE 1
#define VEHICULOS_HASTA 2
#define MAX_LIBERAR 10
		
pthread_mutex_t mutex;


int main(int argc, char *argv[])
{
	int cantidad_jugadores = 1;
	int i = 0;
    int carton_lleno = 0;
	tbolillero datos_thread;
	pthread_t idHilo;
	pthread_attr_t 	atributos;
    mensaje msg;
    int id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);
	srand(time(NULL));
	
    /* toma la cantidad de jugadores del parametro del proceso si existe, sino es 1 por default */
    if (argc > 1) cantidad_jugadores = atoi(argv[1]);
	

    /* inicializa un solo thread para el bolillero con sus parametros */
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
	
    datos_thread.id_cola = id_cola_mensajes;
    datos_thread.carton_lleno = &carton_lleno;
    datos_thread.cantidad_jugadores = cantidad_jugadores;
    pthread_create (&idHilo, &atributos, ThreadBolillero, &datos_thread);

    /* espera a recibir un mensaje de carton_lleno, si no es ese evento, sigue recibiendo mensajes */
    recibir_mensaje(id_cola_mensajes, MSG_BINGO, &msg);
    while(msg.int_evento != EVT_CARTON_LLENO) {
        recibir_mensaje(id_cola_mensajes, MSG_BINGO, &msg);
    }
    /* actualiza carton_lleno para que el bolillero deje de generar numeros */ 
    pthread_mutex_lock(&mutex);
    carton_lleno = 1;
    pthread_mutex_unlock(&mutex);
    printf("Bingo! Gano el jugador %s\n", msg.char_mensaje);
	pthread_join (idHilo, NULL);
	
			
	return 0;
}


