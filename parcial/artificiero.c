#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "memoria_compartida.h"
#include "semaforo.h"

#define DESDE 1
#define HASTA 99
#define CLAVE_BASE 33
#define ESPERA_MIN 200
#define ESPERA_MAX 800


    // declaro variables
	dato *memoria = 0;
	int id_memoria, id_semaforo;
	int i, posicion_artificiero, cantidad_cables, cantidad_artificieros, numero_artificiero, cable_desarmar = 0;

int main(int argc, char *argv[])
{
    if (argv[1] == 0) { // si el primer argumento es nulo, termina el programa
        printf("No ingreso la cantidad de cables \n");
        exit(0);
    }
    if (argv[2] == 0) { // si el segundo argumento es nulo, termina el programa
        printf("No ingreso la cantidad de artificieros \n");
        exit(0);
    }
    if (argv[3] == 0) { // si el tercer argumento es nulo, termina el programa
        printf("No ingreso el nro de artificiero \n");
        exit(0);
    }
    // *habia hecho una validacion de que el nro de artificiero no fuera mayor que la cantidad de cables, pero no lo hice porque la idea era que sea como nro legajo (ej. 403221)

	srand(time(0)); // se cambia la semilla para que sean numeros no repetitivos utilizando el time


    // tomo la cantidad de cables y de artificieros, y el nro de artificiero por argumentos y los convierto a enteros con atoi()
    cantidad_cables = atoi(argv[1]);
    cantidad_artificieros = atoi(argv[2]);
    numero_artificiero = atoi(argv[3]);

    if (cantidad_cables == 0 || cantidad_artificieros == 0 || numero_artificiero == 0) { // si se paso caracteres no numericos, termina el programa
        printf("No ingreso un numero valido en los parametros\n");
        exit(0);
    }

    id_semaforo = creo_semaforo();
	// inicio semaforo y memoria
	memoria = (dato*)creo_memoria(sizeof(dato) * cantidad_artificieros, &id_memoria, CLAVE_BASE);
    
    for(i = 0; i < cantidad_artificieros; i++) { // recorro la memoria
        if (memoria[i].numero_artificiero == 0) { // si el numero de artificiero es el que quiero, guardo la posicion
            memoria[i].numero_artificiero = numero_artificiero;
            posicion_artificiero = i;
            break;
        }
    }
    
	while(memoria[0].estado_bomba == 0 || memoria[0].estado_bomba == 2) // mientras que el estado_bomba sea 0 o 2 (intentos disponibles) seguira intentando desarmar la bomba
	{
        // pongo en espera el semaforo
		espera_semaforo(id_semaforo);
       
        if (memoria[posicion_artificiero].cableSeleccionado == 0) { // si el cable seleccionado es 0, es porque no selecciono ningun cable
            cable_desarmar = rand() % (cantidad_cables - 1) + 1; // random entre 1 y cantidad de cables
            printf("Estoy cortando el cable %d\n", cable_desarmar); // imprimo el cable que esta cortando el artificiero
            memoria[posicion_artificiero].cableSeleccionado = cable_desarmar; // guardo el cable que esta cortando el artificiero en memoria
        }

		// levanto el semaforo
		levanta_semaforo(id_semaforo);
        usleep((rand() % (ESPERA_MAX - ESPERA_MIN) + ESPERA_MIN) * 1000); // espera random entre entre 200 y 800 ms
	}

    // termino de usar memoria y la cierro
    printf("Cierro memoria compartida\n");
	cierro_memoria(memoria, id_memoria);
	return 0;
}
