#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaforo.h"

#define CANTIDAD_VUELOS 	11 // del 0 al 10
#define INTERVALO_PARTIDA 	300 // cantidad de ms que espera el consumidor antes de esperar el semaforo
#define LARGO 			100 // largo maximo de strings

struct flight
{
    int vuelo;
    char destino[LARGO];
    int pasajeros;
};

// defino el tipo de dato Flight
typedef struct flight Flight;

int main(int argc, char *argv[])
{

    // declaro variables
	FILE *consumidor; 
	int lote_id = 0;
    Flight vuelos[CANTIDAD_VUELOS];
    int position = 0;
	int id_semaforo =  creo_semaforo();
    int nro_vuelo;
    int i = 0;
    char destino[LARGO];
    char nombre_pasajero[LARGO];

	while(1)
	{
        // espero el semaforo
		espera_semaforo(id_semaforo);
			
            // leo el archivo de vuelos si existe
			consumidor = fopen("lote.dat", "r");
			if (consumidor!=NULL)
			{
				while (!feof(consumidor))
				{
                    // leo la linea de vuelo
					fscanf(consumidor,"%d %s %s", &nro_vuelo, destino, nombre_pasajero);
                    position = nro_vuelo % 100; // obtengo el ultimo digito del nro de vuelo
                    if (vuelos[position].vuelo != nro_vuelo && destino != NULL) { // si el vuelo no esta en la lista y el destino no es NULL (arreglo por si es una linea en blanco) lo agrego
                        vuelos[position].vuelo = nro_vuelo;
                        strcpy(vuelos[position].destino, destino);
                        vuelos[position].pasajeros = 1;
                    } else vuelos[position].pasajeros++; // sino incremento la cantidad de pasajeros
				}
				fclose(consumidor); // cierro el puntero del archivo

                // imprimo los vuelos
                printf("VUELO    DESTINO    PASAJEROS\n");
                for(i = 0; i < CANTIDAD_VUELOS; i++) {
                    if (vuelos[i].vuelo <= 1010 && vuelos[i].vuelo >= 1000) {
                        printf("%d     %s     %d\n", vuelos[i].vuelo, vuelos[i].destino, vuelos[i].pasajeros);
                    }
                }
                // renombro el archivo .dat para que no se vuelva a leer
                char *file_name = malloc(sizeof(char) * 15);
                sprintf(file_name, "lote.%d.dat", lote_id);
                rename("lote.dat", file_name);
				lote_id++; // aumento el lote para saber el total de lotes recorridos
			}
			else perror ("Error al cargar lote");
						
        // libero el semaforo
		levanta_semaforo(id_semaforo);
        // espero un intervalo antes de volver a leer el archivo
		usleep(INTERVALO_PARTIDA*1000);
	};
	return 0;
}
