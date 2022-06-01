#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "funciones.h"
#include "definiciones.h"
#include "jugador.h"


void *ThreadJugador (void *parametro)
{
	int random_number;
    int nro_jugador;
	int	i;
    int done = 0;
	int id_cola_mensajes;
	mensaje msg;
    char aux[1];
	tjugador *datos_thread = (tjugador*) parametro;
	int *vector_tambor = datos_thread->vector_tambor;
    nro_jugador = datos_thread->nro_jugador;
	id_cola_mensajes = datos_thread->id_cola;

    /* mientras que no haya disparado o terminado el juego */
    while(done == 0)
    {


        /* pone el semaforo en espera */
        pthread_mutex_lock(&mutex);


        /* recibe mensaje y se bloquea hasta que le llegue algo */
        recibir_mensaje(id_cola_mensajes, MSG_JUGADOR+nro_jugador, &msg);

        /* si recibimos el mensaje y despues ponemos en espera el semaforo, los threads siguientes van a leer un mensaje que aun no tienen que leer */

        switch (msg.int_evento)
        {
            /* si el mensaje es el de inicio */
            case EVT_INICIO:
                for (i = 0; i < CANTIDAD; i++)
                {
                    /* selecciona la posicion que ningun jugador uso */
                    if (vector_tambor[i] == 0) {
                        vector_tambor[i] = 1;
                        break;
                    }
                }

                /* Imprime nro_jugador y posicion a disparar */
                printf("Soy el jugador %d y estoy a punto de disparar el barril: %d\n", nro_jugador, i);
                sprintf(aux, "%d", i);
                /* Envia el mensaje a revolver con la posicion a disparar */
                enviar_mensaje(id_cola_mensajes, MSG_REVOLVER, MSG_JUGADOR+nro_jugador, EVT_DISPARO, aux);
            break;
            case EVT_SALVADO:
                /* Si no era la posicion, se salva y termina de jugar */
                printf("Soy el jugador %d y me salve\n", nro_jugador);
                done = 1;
            break;	    	
            case EVT_FIN:
                /* 
                si es el jugador que disparo, pierde 
                si es un jugador que no disparo aun, avisa y termina
                */

                if (atoi(msg.char_mensaje) == nro_jugador) printf("Soy el jugador %d y perdi la ruleta \n", nro_jugador);
                else printf("Soy el jugador %d y no pude jugar, se murio el jugador %d\n", nro_jugador, atoi(msg.char_mensaje));
                done = 1;
            break;		    	
            default:
                printf("Evento sin definir\n");
            break;
        }

        pthread_mutex_unlock(&mutex);

    };
	pthread_exit ((void *)"Listo");
}

