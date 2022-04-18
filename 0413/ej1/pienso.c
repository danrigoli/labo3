#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<time.h>
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

	srand(time(NULL)); //cambia la semilla para random,  usa el time como semilla inicial

	id_semaforo = creo_semaforo();
	// inicio semaforo y memoria
	inicia_semaforo(id_semaforo, 1);
	memoria = (dato*)creo_memoria(sizeof(dato)*CANTIDAD, &id_memoria, CLAVE_BASE);
    sleep(1); // sleep de 1s

	while(memoria[i].estado_acierto == 0)
	{
        // pongo en espera el semaforo
		espera_semaforo(id_semaforo);		
        printf("Numero en memoria: %d\n", memoria[i].numero_pensado); // imprimo el numero que esta en memoria
        local_pienso_un_numero = rand() % (HASTA - DESDE + 1) + DESDE; // pienso un numero con DESDE y HASTA
        printf("Pense: %d\n", local_pienso_un_numero); // imprimo el numero que pienso

        // si lo acierto cambio a memoria 1 y el loop terminara
        if (memoria[i].numero_pensado == local_pienso_un_numero) {
            memoria[i].estado_acierto = 1;
            printf("%s", memoria[0].nombre_jugador);
        }
        // sino reseteo el numero del jugador y el loop continua
        else {
            memoria[i].numero_pensado = 0;
            i++; // avanzo en memoria
        }
		// levanto el semaforo
		levanta_semaforo(id_semaforo);
		sleep(2); // sleep de 2s
	}
    // termino de usar memoria y la cierro
	cierro_memoria(memoria, id_memoria);
	return 0;
}
