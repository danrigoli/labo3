#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "memoria_compartida.h"
#include "semaforo.h"

#define CANTIDAD_PANELES 3
#define CLAVE_BASE 33
#define ESPERA 100


int main(int argc, char *argv[])
{

    /* variables */

    dato *memoria = 0;
	int id_memoria, id_semaforo;
	int i; 
    int opcion;
    int invalid;
    int nro_panel;

    opcion = 0;
    invalid = 1; 

    /* creo e inicio semaforo */
    id_semaforo = creo_semaforo();
	inicia_semaforo(id_semaforo, 1);

    /* creo memoria compartida */
	memoria = (dato*)creo_memoria(sizeof(dato) * CANTIDAD_PANELES, &id_memoria, CLAVE_BASE);

    while (1 == 1) {
        
        while (invalid == 1) /* mientras no ingrese una opcion valida */
        {
            printf("Ingresa la opcion a elegir: \n");
            printf("1. Leer Panel \n");
            printf("2. Escribir Panel \n");
            printf("3. Salir \n");
            /* leo opcion */
            scanf("%d", &opcion);
            if (opcion > 3 && opcion != 0) printf("Ingreso una opcion invalida \n");
            else invalid = 0;
        }
        
        /* si ingreso salir */
        if (opcion==3) break;
        
        
        while (invalid == 0) { /* mientras no ingrese un nro panel valido */
            printf("Ingresa el numero de panel: ");
            /* leo nro panel */
            scanf("%d", &nro_panel);
            if (nro_panel > CANTIDAD_PANELES) printf("El numero de panel es invalido \n");
            else invalid = 1;
        }

    
        for (i = 0; i < CANTIDAD_PANELES; i++) /* recorro memoria compartida para encontrar nro_panel pedido*/
        {
            if (memoria[i].nro_panel == nro_panel) /* si encuentra el pedido */
            {
                espera_semaforo(id_semaforo); /* pongo en espera el semaforo */
                if (opcion == 1) /* si es opcion 1, leo mensaje */
                {
                    printf("Mensaje del panel NRO%d: %s\n", nro_panel, memoria[i].mensaje);
                }
                else if (opcion == 2) /* si es opcion 2, guardo mensaje */
                {
                    printf("Ingresa el mensaje:");
                    scanf("%s", memoria[i].mensaje);
                    printf("Mensaje escrito correctamente \n");
                }
                levanta_semaforo(id_semaforo); /* levanto el semaforo */
            }
            
        }
            usleep(ESPERA * 1000); /* espero 100ms */
    }

    printf("Cierro memoria compartida\n"); /* cierro memoria compartida */
	cierro_memoria(memoria, id_memoria);
	return 0;
}
