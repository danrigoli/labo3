#include "default.h"


#define MARCAS {"Piero", "Suavestar", "Cannon", "Simmons", "Belmo"}
#define ESPERA 0.5

int main(int argc, char *argv[])
{
	// declaro variables
	dato *memoria = NULL;
	int id_memoria, id_semaforo;
	int i = 0;
	char *marcas[CANTIDAD] = MARCAS;

	srand(time(NULL));//cambia la semilla para random,  usa el time como semilla inicial

	id_semaforo = creo_semaforo();
	// inicio memoria
	inicia_semaforo(id_semaforo, 1);
	memoria = (dato*)creo_memoria(sizeof(dato)*CANTIDAD, &id_memoria, CLAVE_BASE);
	// el jugador ingresa su nombre y se guarda en memoria

	for (i = 0; i < CANTIDAD; i++) // guardo las marcas con sus codigos y cantidad = 10 en memoria compartida
	{
		strcpy(memoria[i].nombre, marcas[i]);
		memoria[i].codigo = i + 1;
		memoria[i].cantidad = 10;
	}
	

	while(1)
	{
		// pongo en espera el semaforo
		espera_semaforo(id_semaforo);
		printf("Revisa stock \n");
		for (i = 0; i < CANTIDAD; i++) // va uno por uno y si tiene menos de 10, agrega 1 al stock
		{
			if (memoria[i].cantidad < 10) {
				memoria[i].cantidad++;
				printf("Se sumo %d colchon de la marca %s, total: %d\n", 1, memoria[i].nombre, memoria[i].cantidad);
			}
		}

		levanta_semaforo(id_semaforo); // levanto el semaforo
		sleep(ESPERA); // espera 500ms
	}

	cierro_memoria(memoria, id_memoria);
	return 0;
}
