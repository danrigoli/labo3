#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaforo.h"

#define CANTIDAD_MAXIMA 	10 // cantidad maxima de carga x lote
#define INTERVALO_PARTIDA 	300 // cantidad de ms que espera el productor antes de levantar el semaforo
#define LARGO 			100 // largo maximo de strings

struct flight
{
    int vuelo;
    char destino[LARGO];
    char nombre[LARGO];
};

typedef struct flight Flight;

void check_nro_vuelo(int *nro_vuelo) {
    // hasta que no mande un nro de vuelo valido seguira pediendo
    while(*nro_vuelo > 1010 && *nro_vuelo < 1000)
    {
        printf("Nro del vuelo (1000 a 1010), 0 para terminar: ");
        scanf("%d", nro_vuelo);
    }
}

int main(int argc, char *argv[])
{

    // declaro variables
    Flight vuelos[CANTIDAD_MAXIMA];
    int nro_vuelo;
    int counter = 0;
	FILE *productor; 
    int i = 0;
    // crea el semaforo y lo inicializa en 1
	int id_semaforo =  creo_semaforo();
	inicia_semaforo(id_semaforo, 1);
	
	while(1)
	{
        // espero el semaforo
		espera_semaforo(id_semaforo);
        
        // pide el primer vuelo
        printf("Nro del vuelo (1000 a 1010), 0 para terminar: ");
        scanf("%d", &nro_vuelo);
        while (nro_vuelo != 0 && counter < CANTIDAD_MAXIMA) {
            vuelos[counter].vuelo = nro_vuelo;
            printf("\nDestino: ");
            scanf("%s", vuelos[counter].destino);
            printf("\nNombre del pasajero: ");
            scanf("%s", vuelos[counter].nombre);
            printf("\nNro del vuelo (1000 a 1010), 0 para terminar: ");
            scanf("%d", &nro_vuelo);
            counter++;
            // guarda el vuelo en el array de la struct Flight "vuelos" y sigue pidiendo hasta que sea 0
        }
        // una vez que termina la carga de vuelos, lo guarda en un archivo
        productor = fopen("lote.dat", "a");
        if(productor != NULL) {
            for (i = 0; i < counter; i++) {
                fprintf(productor, "%d %s %s \n", vuelos[i].vuelo, vuelos[i].destino, vuelos[i].nombre); // guarda el pasajero con los datos del vuelo
            }
            counter = 0;            
            fclose(productor);
        } else {
            perror ("Error al abrir lote.dat");
        }

        printf("\nESPERAMOS\n");
        // levanta el semaforo
        levanta_semaforo(id_semaforo);
        // espera 300ms antes de esperar semaforo de vuelta
        usleep(INTERVALO_PARTIDA*1000);
        }

	    return 0;
}
