#include "jugador.h"

/* funcion para enviar a todos los jugadores un mensaje */
void send_massive(int cantidad, int id_cola_mensajes, int nro_jugador, int evento, char mensaje[1]) {
    int i;
    for (i = 0; i < cantidad; i++)
    {
        enviar_mensaje(id_cola_mensajes, MSG_JUGADOR+i, MSG_JUGADOR+nro_jugador, evento, mensaje);
    }
    
}

void *ThreadJugador (void *parametro)
{
	int random_number;
	int	i;
    int done = 0;
    int carton[5] = {0};
	mensaje msg;
    char aux[1];
	tjugador *datos_thread = (tjugador*) parametro;
	int cantidad_aciertos = 0;
    int nro_jugador = datos_thread->nro_jugador;
	int id_cola_mensajes = datos_thread->id_cola;

    /* bloquea el semaforo para imprimir el carton del jugador correctamente, sino se pisan entre ellos */
    pthread_mutex_lock(&mutex);
    printf("Carton del jugador %d - ", nro_jugador);

    for (i = 0; i < 5; i++)
    {
        random_number = rand() % 99 + 1;
        carton[i] = random_number;
        printf("%d ", carton[i]);
    }

    printf("\n");
    pthread_mutex_unlock(&mutex);
    
    while(done == 0)
    {
        /* espera a recibir un mensaje */
        recibir_mensaje(id_cola_mensajes, MSG_JUGADOR+nro_jugador, &msg);
        switch (msg.int_evento)
        {
            case EVT_BOLILLA:
                /* recorre el carton si encuentra la bolilla */
                for (i = 0; i < 5; i++) {
                    if (carton[i] == atoi(msg.char_mensaje)) {
                        carton[i] = 0;
                        cantidad_aciertos++;
                        break;
                    }
                }
                /* si completo el carton */
                if (cantidad_aciertos == 5) {
                    /* imprime que hizo bingo */
                    printf("El jugador %d hizo bingo\n", nro_jugador);
                    sprintf(aux, "%d", nro_jugador);
                    /* bloquea el semaforo */
                    pthread_mutex_lock(&mutex);
                    /* elimina todos los mensajes de la cola, para que no haya otro jugador intentando decir bingo */
                    borrar_mensajes(id_cola_mensajes);
                    /* le envia al bingo que hizo bingo */
                    enviar_mensaje(id_cola_mensajes, MSG_BINGO, MSG_JUGADOR+nro_jugador, EVT_CARTON_LLENO, aux);
                    /* le envia a todos los jugadores que hizo bingo */
                    send_massive(datos_thread->cantidad_jugadores, id_cola_mensajes, nro_jugador, EVT_CARTON_LLENO, aux);
                    pthread_mutex_unlock(&mutex);
                }

            break;
            case EVT_CARTON_LLENO:
                /* imprime la cantidad de aciertos del jugador */
                printf("Cantidad de aciertos del jugador %d: %d\n",nro_jugador, cantidad_aciertos);
                done = 1;
            break;
		    	
            default:
                printf("Evento sin definir\n");
            break;
        }
        /* espera para que el hilo no se quede en un estado infinito */
        usleep(10000);
    };
	pthread_exit ((void *)"Listo");
}

