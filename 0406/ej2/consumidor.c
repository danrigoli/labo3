#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaforo.h"

#define CANTIDAD_CAJEROS	3
#define INTERVALO_PARTIDA 	500 // cantidad de ms que espera el productor antes de esperar el semaforo
#define LARGO 			100 // largo maximo de string

struct cajero
{
    int cant_efvo;
    int total_efvo;
    int cant_cheq;
    int total_cheq;
};

// declaro el tipo cajero
typedef struct cajero Cajero;

int main(int argc, char *argv[])
{
    // declaro variables
	FILE *consumidor; 
	int lote_id = 0;
    Cajero cajeros[CANTIDAD_CAJEROS];
    int position = 0;
	int id_semaforo =  creo_semaforo();
    inicia_semaforo(id_semaforo, 1);
    int deposito;
    int is_efvo;
    int i = 0;
    char nombre_pasajero[LARGO];
	while(1)
	{
        // espero el semaforo
		espera_semaforo(id_semaforo);
            // voy cajero x cajero leyendo los .dat correspondientes
            for(i = 0; i < CANTIDAD_CAJEROS; i++) {
                    char *file_name = malloc(sizeof(char) * 15);
                sprintf(file_name, "cajero%d.dat", i + 1);
			    consumidor = fopen(file_name, "r");
			    if (consumidor!=NULL) // si el cajero llego a cargar datos
			    {
				    while (!feof(consumidor))
				    {
                        // leo deposito y aumento total y cantidad dependiendo si es efvo o cheq
				    	fscanf(consumidor,"%d %d", &deposito, &is_efvo);
                        if (is_efvo == 0) cajeros[i].cant_efvo++;
                        else cajeros[i].cant_cheq++;
                        cajeros[i].total_efvo += deposito;
				    }
				    fclose(consumidor); // cierro el puntero del archivo del cajero

                    // imprimo datos del cajero
                    printf("TOTAL \n");
                    printf("CAJERO%d", i + 1);
                    printf("TC-EFEC  T$-EFEC  TC-CHEQ  T$-CHEQ\n");
                    printf("%d       %d     %d      %d\n", cajeros[i].cant_efvo, cajeros[i].total_efvo, cajeros[i].cant_cheq, cajeros[i].total_cheq);
                } else printf ("Error al cargar el cajero %d \n", i + 1);

                // renombro el archivo para que no se repita
                char *new_file_name = malloc(sizeof(char) * 15);
                sprintf(new_file_name, "cajero%d.%d.dat", i + 1, lote_id);
                rename(file_name, new_file_name);
                // termina y va al siguiente cajero o levanto semaforo
            }
				lote_id++;	// aumento lote_id para saber cuantos lotes se corrieron					
		levanta_semaforo(id_semaforo);
		usleep(INTERVALO_PARTIDA*10000);
	};
	return 0;
}
