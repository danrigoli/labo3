#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "funciones.h"
#include "definiciones.h"
#include "caballo.h"

#define RECORRE_DESDE 10
#define RECORRE_HASTA 50
#define ESPERA_DESDE 100
#define ESPERA_HASTA 200

int send_metros(int id_cola_mensajes, int cantidad, int nro_caballo) {
    char aux[4];
    int random;
    /* numero random entre 10 y 50 */
    random = rand() % (RECORRE_HASTA - RECORRE_DESDE + 1) + RECORRE_DESDE;
    printf("Soy el caballo %d y voy a recorrer %d metros\n", nro_caballo, random);
    sprintf(aux, "%d", cantidad);
    enviar_mensaje(id_cola_mensajes, MSG_HIPODROMO, MSG_CABALLO+nro_caballo, EVT_METROS, aux);
    return random;
}

void *ThreadCaballo (void *parametro)
{
    int random;
    int nro_caballo;
    int done = 0;
	int id_cola_mensajes;
	mensaje msg;
    int metros_al_disco;
    int metros_recorridos = 0;
	tcaballo *datos_thread = (tcaballo*) parametro;
    nro_caballo = datos_thread->nro_caballo;
	id_cola_mensajes = datos_thread->id_cola;

    /* mientras que nadie haya ganado la carrera */
    while(done == 0)
    {

        /* recibe mensaje y se bloquea hasta que le llegue algo */
        recibir_mensaje(id_cola_mensajes, MSG_CABALLO+nro_caballo, &msg);

        switch (msg.int_evento)
        {
            /* si el mensaje es el de inicio */
            case EVT_INICIO:
                /* guarda la cantidad de metros a correr en variable local */
                metros_al_disco = atoi(msg.char_mensaje);
                metros_recorridos += send_metros(id_cola_mensajes, metros_recorridos, nro_caballo);
            break;
            /* si alguien ya cruzo el disco */
            case EVT_CRUZARON_EL_DISCO:
                printf("Soy el caballo %d y cruzo el disco el caballo %d\n", nro_caballo, atoi(msg.char_mensaje));
                datos_thread->ganador = atoi(msg.char_mensaje);
                done = 1;
            break;	    	
            case EVT_SIGA:
                /* numero random de espera */
                random = rand() % (ESPERA_HASTA - ESPERA_DESDE + 1) + ESPERA_DESDE;
                usleep(random * 1000);
                metros_recorridos += send_metros(id_cola_mensajes, metros_recorridos, nro_caballo);
            break;		    	
            default:
                printf("Evento sin definir\n");
            break;
        }

    };
	pthread_exit ((void *)"Listo");
}

