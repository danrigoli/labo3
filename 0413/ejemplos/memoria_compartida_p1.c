/*
2016.04.11 Se unifico funcion de creo_memoria
*/

#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>

#define CANTIDAD 10
#define CANTIDAD_TOTAL 10
#define DESDE 0
#define HASTA 25

#define CLAVE_BASE 33

typedef struct tipo_dato dato;

struct tipo_dato
{
	int numero;
    char letra;
};

key_t creo_clave(int r_clave)
{
	// Igual que en cualquier recurso compartido (memoria compartida, semaforos
	// o colas) se obtien una clave a partir de un fichero existente cualquiera
	// y de un entero cualquiera. Todos los procesos que quieran compartir este
	// memoria, deben usar el mismo fichero y el mismo entero.
	key_t clave;
	clave = ftok ("/bin/ls", r_clave);	
	if (clave == (key_t)-1)
	{
		printf("No puedo conseguir clave para memoria compartida\n");
		exit(0);
	}
	return clave;
}

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
int main()
{

	int id_memoria;
	dato *memoria = NULL;
	int i, aleatorio;

	memoria = (dato*)creo_memoria(sizeof(dato)*CANTIDAD, &id_memoria, CLAVE_BASE);

	srand(time(NULL));

	while(1)
	{
		printf("Escribimos en la memoria\n");
		for (i=0; i<CANTIDAD; i++)
		{
			aleatorio = rand()%(HASTA-DESDE)+DESDE;
			memoria[i].numero = aleatorio ;
			memoria[i].letra  = 0x41 + aleatorio ;
			printf("Escrito %d %c\n", memoria[i].numero, memoria[i].letra);
			sleep (1);
		}
		printf("Dejamos de escribir en la memoria\n");
		sleep (10);
	}
	//
	//	Terminada de usar la memoria compartida, la liberamos.
	//
	shmdt ((char *)memoria);
	shmctl (id_memoria, IPC_RMID, (struct shmid_ds *)NULL);
	return 0;
}
