#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include<time.h>
#include "semaforo.h"

#define CANTIDAD_MAXIMA 	10
#define LARGO 			100

struct deposito
{
    int importe;
    int is_efvo;
};

// declaro el tipo Deposito
typedef struct deposito Deposito;


int main(int argc, char *argv[])
{
    srand(time(NULL)); // inicializo la semilla del random para que sea aleatorio

    // declaro variables
	FILE *productor; 
    int importe, is_efvo, random_wait, random_quantity;
	int id_semaforo =  creo_semaforo();
	int i = 0;
	while(1)
	{
        // genero la cantidad y espera random
        random_wait = (rand() % (2500 - 1000 + 1)) + 1000;
        random_quantity = (rand() % (20 - 10 + 1)) + 10;
        Deposito depositos[random_quantity];

        // espero el semaforo
		espera_semaforo(id_semaforo);

        // lleno el array de depositos de manera random
        for (i = 0; i < random_quantity; i++)
        {
            depositos[i].importe = (rand() % (500 - 100 + 1)) + 100;
            depositos[i].is_efvo = (rand() % (1 - 0 + 1)) + 0;
        }
        
        char *file_name = malloc(sizeof(char) * 12);
        sprintf(file_name, "%s.dat", argv[0]); // utilizo el nombre del programa para generar el nombre del archivo
        productor = fopen(file_name, "a"); // abro el archivo en modo append (edita y crea si no existe)
        if(productor != NULL) {
            // escribo los depositos en el archivo
            for (i = 0; i < random_quantity; i++) {
                fprintf(productor, "%d %d \n", depositos[i].importe, depositos[i].is_efvo);
            }
            fclose(productor); // cierro el puntero del archivo del productor
        } else {
            perror ("Error al abrir el archivo");
        }

        // levanto semaforo
        levanta_semaforo(id_semaforo);

        usleep(random_wait*1000);
        }

	    return 0;
}
