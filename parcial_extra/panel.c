#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "memoria_compartida.h"
#include "semaforo.h"

#define CLAVE_BASE 33
#define ESPERA 100
#define CANTIDAD_PANELES 3




int main(int argc, char *argv[])
{

    /* variables */
    dato *memoria;
    int id_memoria, id_semaforo;
    int i, nro_panel;
    char mensaje[100];

    /* si el nro panel no es valido termina */
    if (argv[1] == 0) {
        printf("No ingreso el nro de panel \n");
        exit(0);
    }
        if (atoi(argv[1]) > CANTIDAD_PANELES) {
        printf("El nro de panel es invalido \n");
        exit(0);
    }

    
    nro_panel = atoi(argv[1]); /* guardo nro_panel */
    strcpy(mensaje, "default"); /* guardo mensaje default */

    id_semaforo = creo_semaforo();
	memoria = (dato*)creo_memoria(sizeof(dato) * CANTIDAD_PANELES, &id_memoria, CLAVE_BASE);

        for(i = 0; i < CANTIDAD_PANELES; i++) {
        if (memoria[i].nro_panel == 0)  {
            memoria[i].nro_panel = nro_panel;
            break;
        }
    }

    strcpy(memoria[i].mensaje, mensaje);  /* guardo mensaje default */

    while (1 == 1) /* consulta infinitamennte */
    {
        for(i = 0; i < CANTIDAD_PANELES; i++) { /* recorro paneles memoria */
            if (memoria[i].nro_panel == nro_panel) { /* si es el panel, verifica que no sea el mismo string y lo imprime */
                if(strcmp(memoria[i].mensaje, mensaje) != 0) {
                    espera_semaforo(id_semaforo); /* espera a que el semaforo este libre */
                    strcpy(mensaje, memoria[i].mensaje);
                    printf("Panel NRO%d: %s\n", nro_panel, mensaje);
                    levanta_semaforo(id_semaforo); /* levanta el semaforo */
                }
            }
        }
        usleep(ESPERA * 1000); /* espera 100ms */
    }
    
    printf("Cierro memoria compartida\n"); /* cierro memoria compartida */
	cierro_memoria(memoria, id_memoria);
	return 0;
}
