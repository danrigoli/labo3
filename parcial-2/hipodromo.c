#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include "funciones.h"
#include "definiciones.h"

#define METROS_DESDE 800
#define METROS_HASTA 1000

/* funcion para enviar a todos los jugadores un mensaje */
void send_massive(int cantidad, int id_cola_mensajes, int evento, char mensaje[1]) {
    int i;
    for (i = 0; i < cantidad; i++)
    {
        enviar_mensaje(id_cola_mensajes, MSG_CABALLO+i, MSG_HIPODROMO, evento, mensaje);
    }
    
}

/* funcion para procesar el mensaje recibido */
void procesar_evento(int id_cola_mensajes, mensaje msg, int distancia, int cantidad) {
    /* convierto el codigo de destino jugador a char para poder enviarlo */
    char aux_caballo[1];
    sprintf(aux_caballo, "%d", msg.int_rte - MSG_CABALLO);

    /* switch con los distintos eventos que puede recibir */
	switch (msg.int_evento)
	{
        /* si algun caballo corrio metros */
		case EVT_METROS:
            /* si el caballo cruzo el disco, gana */
            if(atoi(msg.char_mensaje) >= distancia) {
                printf("Gano el caballo %d\n", msg.int_rte - MSG_CABALLO);
                /* borra todos los mensajes que se encuentran en la cola, sino los jugadores van a intentar disparar igual */
                borrar_mensajes(id_cola_mensajes);
                /* envio un mensaje a todos los jugadores que se termino el juego y quien es el jugador que perdio */
                send_massive(cantidad, id_cola_mensajes, EVT_CRUZARON_EL_DISCO, aux_caballo);
                /* manda señal para terminar el programa */
                kill(getpid(), SIGSTOP);
            } else {
                /* si el jugador que disparo no estaba en la posicion elegida, se salvo */
                printf("El caballo %d no llego a la meta\n", msg.int_rte - MSG_CABALLO);
                enviar_mensaje(id_cola_mensajes, msg.int_rte, MSG_HIPODROMO, EVT_SIGA, "");
            }
		break;
		default:
			printf("\nEvento sin definir\n");
		break;
	}

	printf("------------------------------\n");
}

int main(int argc, char *argv[])
{
	int cantidad_caballos = 1;
	int i = 0;
    mensaje msg;
    int distancia;
    char distancia_aux[1];
    int id_cola_mensajes;
    id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);
	
    srand(time(NULL));

    /* toma la cantidad de caballos del parametro del proceso si existe, sino es 1 por default, si pasa un valor que no es un numero, termina el proceso */
    if (argc > 1) {
            cantidad_caballos = atoi(argv[1]);
        if (cantidad_caballos < 1) {
            printf("Error: cantidad de caballos debe ser mayor a 0\n");
            exit(0);
        }
    }
    /* distancia random a correr */
    distancia = rand() % (METROS_HASTA - METROS_DESDE + 1) + METROS_DESDE;
    printf("Distancia: %d\n", distancia);
    sprintf(distancia_aux, "%d", distancia);

    /* borra los mensajes en la cola de mensajes para empezar de 0 */
    borrar_mensajes(id_cola_mensajes);
    
    /* envia a todos los caballos que inicio la carrera con la cantidad a correr */
    send_massive(cantidad_caballos, id_cola_mensajes, EVT_INICIO, distancia_aux);
    
    /* recibe y procesa los eventos de los caballos hasta que si gana, para el programa con señal */
    while (1 == 1) {
        recibir_mensaje(id_cola_mensajes, MSG_HIPODROMO, &msg);
        procesar_evento(id_cola_mensajes, msg, distancia, cantidad_caballos);
    }
			
	return 0;
}