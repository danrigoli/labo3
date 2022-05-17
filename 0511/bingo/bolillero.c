#include "bolillero.h"

/* funcion para enviar a todos los jugadores un mensaje */
void send_massive(int cantidad, int id_cola_mensajes, int evento, char mensaje[1]) {
    int i;
    for (i = 0; i < cantidad; i++)
    {
        enviar_mensaje(id_cola_mensajes, MSG_JUGADOR+i, MSG_BINGO, evento, mensaje);
    }
    
}

/* funcion del threadBolillero */
void *ThreadBolillero (void *parametro)
{
	int not_found, bolilla, i;
    int posicion_ultima_bolilla = 0;
    int bolillas[N_HASTA] = {0};
	mensaje msg;
    char aux[1];
	tbolillero *datos_thread = (tbolillero*) parametro;
	int id_cola_mensajes = datos_thread->id_cola;
    int cantidad_jugadores = datos_thread->cantidad_jugadores;
    int *carton_lleno = datos_thread->carton_lleno;

    /* mientras que nadie haya hecho bingo */
    while(*carton_lleno == 0)
    {
        /* flag para saber si la bolilla ya habia salido */
        not_found = 0;
        /* hasta que no encuentre una bolilla nueva, seguira generando */
        while(not_found == 0) {
            bolilla = rand() % (N_DESDE + 1 - N_HASTA) + N_DESDE;
            not_found = 1;
            /* si la bolilla ya habia salido, se pone el flag en 0 para que siga buscando */
            for(i = 0; i < N_HASTA; i++) {
                if(bolilla == bolillas[i]) not_found = 0;	
            }
        }
        /* guarda la bolilla nueva en el carton */
        bolillas[posicion_ultima_bolilla++] = bolilla;
        printf("Bolilla: %d\n", bolilla);
        sprintf(aux, "%d", bolilla);
        /* envia la bolilla a todos los jugadores */
        send_massive(cantidad_jugadores, id_cola_mensajes, EVT_BOLILLA, aux);
        /* espera random entre 500 y 5000 para enviar la siguiente bolilla */
        usleep(rand() % (TIEMPO_COMPRUEBA_DESDE + 1 - TIEMPO_COMPRUEBA_HASTA) + TIEMPO_COMPRUEBA_DESDE * 1000);
    };

	pthread_exit ((void *)"Listo");
}

