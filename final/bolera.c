#include "definiciones.h"
#include "funciones.h"

#define CANTIDAD 3
#define CANTIDAD_CHANCES 3

int done = 0;

/* funcion para enviar a todos los jugadores un mensaje */
void send_massive(int id_cola_mensajes, int evento, char mensaje[1]) {
    int i;
    for (i = 0; i < CANTIDAD; i++)
    {
        enviar_mensaje(id_cola_mensajes, MSG_JUGADOR+i, MSG_BOLERA, evento, mensaje);
    }
    
}

/* funcion para procesar el mensaje recibido */
void procesar_evento(int id_cola_mensajes, mensaje msg, int* lineas, int* intentos) {

    char aux[2];
    int cantidad_de_pinos;
    int puntos_obtenidos;
    int nro_jugador;

    nro_jugador = msg.int_rte - MSG_JUGADOR;
    /* switch con los distintos eventos que puede recibir */
	switch (msg.int_evento)
	{
        /* si alguien tiro */
		case EVT_TIRO:
            cantidad_de_pinos = rand() % lineas[nro_jugador] + 1;
            if (cantidad_de_pinos == 10) {
                printf("CHUZA \n");
                puntos_obtenidos = 20;
            } else puntos_obtenidos = cantidad_de_pinos;
            printf("El jugador %d tiro %d pinos\n", nro_jugador, cantidad_de_pinos);
            lineas[nro_jugador] -= cantidad_de_pinos;
            intentos[nro_jugador]--;
            sprintf(aux, "%d", cantidad_de_pinos);
            if(lineas[nro_jugador] != 0 && intentos[nro_jugador] != 0)  enviar_mensaje(id_cola_mensajes,msg.int_rte, MSG_BOLERA, EVT_RESULTADO, aux);
            else enviar_mensaje(id_cola_mensajes,msg.int_rte, MSG_BOLERA, EVT_FIN, aux);
		break;

		default:
			printf("\nEvento sin definir\n");
		break;
	}

    memset(aux,0x00,sizeof(aux));
	printf("------------------------------\n");
}

int main(int argc, char *argv[])
{
    int cantidad_jugadores;
    int id_memoria;
	int id_cola_mensajes;
    int *memoria;
	mensaje msg;
    int i;
    int* lineas_jugadores;
    int* cantidad_de_chances;

    srand(time(NULL));

	/* tomo memoria compartida para tener la misma cantidad de jugadores entre procesos */
    id_memoria = creo_id_memoria(sizeof(int), CLAVE_BASE);
    memoria = (int*)creo_memoria(id_memoria);

	/* consigue el id de la cola con la clave base */
	id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);

    /* borra todos los mensajes que se encuentran en la cola */
    borrar_mensajes(id_cola_mensajes);

    /* genero un numero aleatorio entre 1 y 3 y lo guardo en memoria compartida para los jugadores*/
    cantidad_jugadores = rand() % CANTIDAD + 1;
    memoria[0] = cantidad_jugadores;
    printf("Cantidad de jugadores: %d\n", cantidad_jugadores);
    
    /* reservo memoria para guardar la cantidad de chances de cada jugador y las lineas*/
    lineas_jugadores = malloc(sizeof(int) * cantidad_jugadores);
    cantidad_de_chances = malloc(sizeof(int) * cantidad_jugadores);

    /* inicializo las lineas y chances de cada jugador */
    for(i = 0; i < cantidad_jugadores; i++) {
        lineas_jugadores[i] = 10;
        cantidad_de_chances[i] = CANTIDAD_CHANCES;
    }

    /* envio un mensaje a todos los jugadores que se inicio el juego */
    send_massive(id_cola_mensajes, EVT_INICIO, "");
    
    /* mientras que alguno siga con chances y con bolos, sigue */
	while(done == 0)
	{
        /* espero a recibir un mensaje que llegue con receptor MSG_BOLERA */
		recibir_mensaje(id_cola_mensajes, MSG_BOLERA, &msg);

        /* proceso el mensaje */
		procesar_evento(id_cola_mensajes, msg, lineas_jugadores, cantidad_de_chances);

        /* chequeo que no haya terminado el juego */
        for(i = 0; i < cantidad_jugadores; i++) {
            if(lineas_jugadores[i] != 0 && cantidad_de_chances[i] != 0) {
                done = 0;
                break;
            } else done = 1;
        }
	};

    /* limpio memoria para proximo juego */
    memoria[0] = 0;
    
    /* libero memoria */
    free(lineas_jugadores);
    free(cantidad_de_chances);
	return 0;
}

