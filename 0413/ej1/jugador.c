#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <unistd.h>
#include "memoria_compartida.h"
#include "semaforo.h"

#define CANTIDAD 10
#define DESDE 1
#define HASTA 99
#define CLAVE_BASE 33

int main(int argc, char *argv[])
{
	// declaro variables
	dato *memoria = NULL;
	int id_memoria, id_semaforo;
	int i, local_pienso_un_numero = 0;

	srand(time(NULL));//cambia la semilla para random,  usa el time como semilla inicial

	id_semaforo = creo_semaforo();
	// inicio memoria
	memoria = (dato*)creo_memoria(sizeof(dato), &id_memoria, CLAVE_BASE);
	// el jugador ingresa su nombre y se guarda en memoria
	printf("Ingrese su nombre: ");
	scanf("%s", memoria[i].nombre_jugador);

	while(memoria[0].estado_acierto == 0 && i < CANTIDAD) // mientras que no acierte y el nro se haya reseteado y no supera la cantidad maxima, piensa otro numero
	{
		// pongo en espera el semaforo
		espera_semaforo(id_semaforo);
		if (memoria[0].numero_pensado == 0) {
			local_pienso_un_numero = rand() % (HASTA - DESDE + 1) + DESDE; // pienso un numero con DESDE y HASTA
			memoria[0].numero_pensado = local_pienso_un_numero; // guardo el numero en memoria
        	i++; // avanzo cantidad de intentos
		}
		
		levanta_semaforo(id_semaforo); // levanto el semaforo
		sleep(2); // sleep de 2s
	}

	if (memoria[0].estado_acierto == 1) printf("Felicitaciones %s, acertaste el numero %d\n", memoria[0].nombre_jugador, memoria[0].numero_pensado); // si acerto
	else printf("Lo siento %s, no acertaste el numero\n", memoria[0].nombre_jugador); // si se acabaron los intentos

	// termino de usar memoria y la cierro
	cierro_memoria(memoria, id_memoria);
	return 0;
}
