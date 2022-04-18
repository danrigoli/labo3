#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <unistd.h>
#include "memoria_compartida.h"
#include "semaforo.h"

#define ESPERA_DESDE 100
#define ESPERA_HASTA 5000
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
	int i, local_pienso_un_numero = 0;
	int cantidad_vias = atoi(argv[1]);
	int espera;
	srand(time(NULL));//cambia la semilla para random,  usa el time como semilla inicial

	id_semaforo = creo_semaforo();
	
	// inicio semaforo y memoria
	inicia_semaforo(id_semaforo, 1);
	memoria = (int*)creo_memoria(sizeof(int)*cantidad_vias, &id_memoria, CLAVE_BASE); // cantidad de vias definido por primer argumento
	sleep(2); // sleep de 2s

	while(1)
	{
		for (i = 0; i < cantidad_vias; i++) // recorro vias en fila
		{
			espera_semaforo(id_semaforo);	// pongo en espera el semaforo
			if (memoria[i] > 0) memoria[i] = memoria[i] - 1;
			if (memoria[i] >= 10) memoria[i] = 0;
			printf("AUTOS EN LA VIA %d: %d \n", i, memoria[i]);
			levanta_semaforo(id_semaforo); // levanto el semaforo
			espera = rand() % (ESPERA_HASTA - ESPERA_DESDE + 1) + ESPERA_DESDE;
			usleep(espera*1000); // espera random entre 100ms y 5000ms x cada via
		}
	}

	cierro_memoria(memoria, id_memoria); // termino de usar memoria y la cierro
	return 0;
}
