#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "funciones.h"
#include "definiciones.h"
#include "jugador.h"

#define ESPERA 300

void *ThreadJugador (void *parametro)
{
    int nro_jugador;
    int cantidad_intentos;
    int puntaje;
    int done = 0;
	int id_cola_mensajes;
	mensaje msg;
    tjugador *datos_thread;



	datos_thread = (tjugador*) parametro;
    cantidad_intentos = datos_thread->cantidad_intentos;
    nro_jugador = datos_thread->nro_jugador;
    puntaje = datos_thread->puntaje;
	id_cola_mensajes = datos_thread->id_cola;

    /* mientras que haya llegado el evento_fin */
    while(done == 0)
    {

        /* recibe mensaje y se bloquea hasta que le llegue algo */
        recibir_mensaje(id_cola_mensajes, MSG_JUGADOR+nro_jugador, &msg);

        /* si recibimos el mensaje ponemos en espera el semaforo */
        pthread_mutex_lock(&mutex);

        switch (msg.int_evento)
        {
            /* si el mensaje es el de inicio */
            case EVT_INICIO:
                printf("Soy el jugador %d y voy a tirar\n", nro_jugador);
                datos_thread->cantidad_intentos--;
                enviar_mensaje(id_cola_mensajes, MSG_BOLERA, MSG_JUGADOR+nro_jugador, EVT_TIRO, "");
            break;
            case EVT_RESULTADO:
                /* Sumo el puntaje que llego */
                datos_thread->puntaje += atoi(msg.char_mensaje);
                /* Resto intentos */
                datos_thread->cantidad_intentos--;
                printf("Soy el jugador %d y tire %d bolos \n", nro_jugador, atoi(msg.char_mensaje));
                /* Aviso a la bolera que vuelvo a tirar */
                enviar_mensaje(id_cola_mensajes, MSG_BOLERA, MSG_JUGADOR+nro_jugador, EVT_TIRO, "");
            break;	    	
            case EVT_FIN:
                /* Termine de jugar, aviso cuantos tire en el ultimo turno y termino */
                printf("Soy el jugador %d y tire %d bolos, termine de jugar. ", nro_jugador, atoi(msg.char_mensaje));
                datos_thread->puntaje += atoi(msg.char_mensaje);
                printf("En total tire: %d bolos en %d intentos\n", datos_thread->puntaje, cantidad_intentos);
                done = 1;
            break;		    	
            default:
                printf("Evento sin definir\n");
            break;
        }
        pthread_mutex_unlock(&mutex);
        usleep(ESPERA * 1000);


    };
	pthread_exit ((void *)"Listo");
}

