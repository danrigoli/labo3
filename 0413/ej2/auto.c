#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <unistd.h>
#include "memoria_compartida.h"
#include "semaforo.h"

#define ESPERA_DESDE 400
#define ESPERA_HASTA 600
#define CLAVE_BASE 33

int main(int argc, char *argv[])
{
    if(argv[1] == NULL)
    {
        printf("Error: no se ingreso cantidad de vias\n");
        exit(0);
    }

	// declaro variables
	int *memoria = NULL;
	int id_memoria, id_semaforo;
	int i, min, min_i = 0;
	int cantidad_vias = atoi(argv[1]);
	int espera;
	srand(time(NULL));//cambia la semilla para random,  usa el time como semilla inicial

	id_semaforo = creo_semaforo();
	
	// inicio semaforo y memoria
	memoria = (int*)creo_memoria(sizeof(int)*cantidad_vias, &id_memoria, CLAVE_BASE); // cantidad de vias definido por primer argumento
	sleep(2); // sleep de 2s

	while(1)
	{
        espera_semaforo(id_semaforo);	// pongo en espera el semaforo
        min = memoria[0];
		for (i = 0; i < cantidad_vias; i++) // recorro todas las vias y guardo cual es la que menos tiene
		{
            if (memoria[i] < min)
            {
                min = memoria[i];
                min_i = i;
            }		
        }

        memoria[min_i] = memoria[min_i] + 1; // le agrego un auto al que menos tiene
		printf("SE AGREGO UN AUTO EN LA VIA %d. TOTAL: %d \n", min_i, memoria[min_i]);

        levanta_semaforo(id_semaforo); // levanto el semaforo

        espera = rand() % (ESPERA_HASTA - ESPERA_DESDE + 1) + ESPERA_DESDE;
        usleep(espera*1000);

	}

	cierro_memoria(memoria, id_memoria); // termino de usar memoria y la cierro
	return 0;
}
