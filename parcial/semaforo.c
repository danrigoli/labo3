#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaforo.h"
#include "creo_clave.h"

#define CLAVE_BASE 33

//funcion que crea el semaforo
int creo_semaforo()
{
	key_t clave = creo_clave(CLAVE_BASE);
	int id_semaforo = semget(clave, 1, 0600|IPC_CREAT); 
	//PRIMER PARAMETRO ES LA CLAVE, EL SEGUNDO CANT SEMAFORO, EL TERCERO 0600 LO UTILIZA CUALQUIERA, IPC ES CONSTANTE (VEA SEMAFORO)
	if(id_semaforo == -1)
	{
	printf("Error: no puedo crear semaforo\n");
	exit(0);
	}
	return id_semaforo;
}

//inicia el semaforo
void inicia_semaforo(int id_semaforo, int valor)
{
	semctl(id_semaforo, 0, SETVAL, valor);
}

// libera el semaforo - lo incrementa en 1
void levanta_semaforo(int id_semaforo)
{
	struct sembuf operacion;
	printf("Libero semaforo\n");
	operacion.sem_num = 0;
	operacion.sem_op = 1; //incrementa el semaforo en 1
	operacion.sem_flg = 0;
	semop(id_semaforo,&operacion,1);
}

// bloquea semaforo - lo decrementa -1
void espera_semaforo(int id_semaforo)
{
	struct sembuf operacion;
	printf("Bloqueo semaforo\n");
	operacion.sem_num = 0;
	operacion.sem_op = -1; //decrementa el semaforo en 1
	operacion.sem_flg = 0;
	semop(id_semaforo,&operacion,1);

}