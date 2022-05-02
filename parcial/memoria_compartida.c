#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "creo_clave.h"

#define LARGO 100 

typedef struct tipo_dato dato;


struct tipo_dato
{
    int numero_artificiero;
	int estado_bomba;
    int cableSeleccionado;
};

// crea la memoria compartida
void* creo_memoria(int size, int* r_id_memoria, int clave_base)
{
	void* ptr_memoria;
	int id_memoria;
	id_memoria = shmget (creo_clave(clave_base), size, 0777 | IPC_CREAT); 

	if (id_memoria == -1)
	{

		printf("No consigo id para memoria compartida\n");
		exit (0);
	}

	ptr_memoria = (void *)shmat (id_memoria, (char *)0, 0);

	if (ptr_memoria == NULL)
	{
		printf("No consigo memoria compartida\n");

		exit (0);
	}
	*r_id_memoria = id_memoria;
	return ptr_memoria;
}

// cierra la memoria compartida
void cierro_memoria(dato *memoria, int id_memoria)
{
    shmdt ((char *)memoria);
    shmctl (id_memoria, IPC_RMID, (struct shmid_ds *)NULL);

}