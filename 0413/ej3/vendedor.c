#include "default.h"


#define ESPERA 0.2

int main(int argc, char *argv[])
{
	// declaro variables
	dato *memoria = NULL;
	int id_memoria, id_semaforo;
	int i, codigo, cantidad = 0;

	srand(time(NULL));//cambia la semilla para random,  usa el time como semilla inicial

	id_semaforo = creo_semaforo();
	// inicio memoria
	memoria = (dato*)creo_memoria(sizeof(dato)*CANTIDAD, &id_memoria, CLAVE_BASE);
	// el jugador ingresa su nombre y se guarda en memoria

	while(1)
	{
		// pongo en espera el semaforo
		espera_semaforo(id_semaforo);
		
		printf("Ingrese el codigo de marca: ");
		scanf("%d", &codigo);
		printf("\nIngrese la cantidad de colchones (max. 5): ");
		scanf("%d", &cantidad);
		for (i = 0; i < CANTIDAD; i++)
		{
			if (memoria[i].codigo == codigo){
				if (cantidad > 5) { // si la cantidad es mayor a 5, no se puede vender
					printf("No se puede vender mas de 5 colchones\n");
				}
				else if (memoria[i].cantidad < cantidad) { // si la cantidad es mayor a la que hay, no se puede vender
					printf("No hay suficientes colchones, cantidad disponible: %d\n", memoria[i].cantidad);
				}
				else { // se disminuye el stock por la cantidad pedida
					memoria[i].cantidad -= cantidad;
					printf("Se vendieron %d colchones de la marca %s\n", cantidad, memoria[i].nombre);
				}
				break;
			};
		}

		levanta_semaforo(id_semaforo); // levanto el semaforo
		sleep(ESPERA); // espera 200ms
	}

	cierro_memoria(memoria, id_memoria);
	return 0;
}
