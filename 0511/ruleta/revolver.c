#include "definiciones.h"
#include "funciones.h"

#define CANTIDAD 6

int done = 0;

/* funcion para enviar a todos los jugadores un mensaje */
void send_massive(int id_cola_mensajes, int evento, char mensaje[1]) {
    int i;
    for (i = 0; i < CANTIDAD; i++)
    {
        enviar_mensaje(id_cola_mensajes, MSG_JUGADOR+i, MSG_REVOLVER, evento, mensaje);
    }
    
}

/* funcion para procesar el mensaje recibido */
void procesar_evento(int id_cola_mensajes, mensaje msg, int random) {
    /* convierto el codigo de destino jugador a char para poder enviarlo */
    char aux[1];
    sprintf(aux, "%d", msg.int_rte - MSG_JUGADOR);

    /* switch con los distintos eventos que puede recibir */
	switch (msg.int_evento)
	{
        /* si alguien disparo */
		case EVT_DISPARO:
            /* si el jugador que disparo estaba en la posicion elegida, pierde */
            if(atoi(msg.char_mensaje) == random) {
                printf("Volo el cerebro del jugador %d\n", msg.int_rte - MSG_JUGADOR);
                done = 1;
                /* borra todos los mensajes que se encuentran en la cola, sino los jugadores van a intentar disparar igual */
                borrar_mensajes(id_cola_mensajes);
                /* envio un mensaje a todos los jugadores que se termino el juego y quien es el jugador que perdio */
                send_massive(id_cola_mensajes, EVT_FIN, aux);
            } else {
                /* si el jugador que disparo no estaba en la posicion elegida, se salvo */
                printf("Se salvo el jugador %d\n", msg.int_rte - MSG_JUGADOR);
                enviar_mensaje(id_cola_mensajes, msg.int_rte, MSG_REVOLVER, EVT_SALVADO, "");
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
    int random;
	int id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);
	mensaje msg;

    /* genero un numero aleatorio entre 0 y 5 */
    srand(time(NULL));
    random = rand() % CANTIDAD;
    /* borra todos los mensajes que se encuentran en la cola */
    borrar_mensajes(id_cola_mensajes);

    /* print DEBUG para saber que posicion eligio */
    printf("Inicia el juego, elegi la posicion %d\n", random);
    /* envio un mensaje a todos los jugadores que se inicio el juego */
    send_massive(id_cola_mensajes, EVT_INICIO, "");
    /* mientras nadie haya perdido, sigue */
	while(done == 0)
	{
		recibir_mensaje(id_cola_mensajes, MSG_REVOLVER, &msg);
		procesar_evento(id_cola_mensajes, msg, random);
	};
	
	return 0;
}

