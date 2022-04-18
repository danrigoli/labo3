/*
2016.04.11 Se unifico funcion de creo_memoria
*/

#include <sys/shm.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>

#include <sys/sem.h>
#define ROJO 0
#define VERDE 1

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

//levanta el semaforo
void levanta_semaforo(int id_semaforo)
{
	struct sembuf operacion;
	printf("Levanta SEMAFORO \n");
	operacion.sem_num = 0;
	operacion.sem_op = 1; //incrementa el semaforo en 1
	operacion.sem_flg = 0;
	semop(id_semaforo,&operacion,1);
}

//espera semaforo
void espera_semaforo(int id_semaforo)
{
	struct sembuf operacion;
	printf("Espera SEMAFORO \n");
	operacion.sem_num = 0;
	operacion.sem_op = -1; //decrementa el semaforo en 1
	operacion.sem_flg = 0;
	semop(id_semaforo,&operacion,1);

}


int main()
{
	dato *memoria = NULL;
	int id_memoria;
	int i,j;
	int id_semaforo;

	id_semaforo = creo_semaforo();

	memoria = (dato*)creo_memoria(sizeof(dato)*CANTIDAD, &id_memoria, CLAVE_BASE);
	

	//	Vamos leyendo el valor de la memoria con esperas de un segundo
	//	y mostramos en pantalla dicho valor. Deber�a ir cambiando seg�n
	//	p1 lo va modificando.
	//
	while(1)
	{
		espera_semaforo(id_semaforo);			
			printf("leemos la memoria\n");
			for (i=0; i<CANTIDAD; i++)
			{
					printf("Leido %d %c\n", memoria[i].numero, memoria[i].letra);
			}
			printf("Dejamos de leer la memoria\n");
		levanta_semaforo(id_semaforo);
		sleep (1);
	}

	//
	//	Terminada de usar la memoria compartida, la liberamos.
	//
	shmdt ((char *)memoria);
	shmctl (id_memoria, IPC_RMID, (struct shmid_ds *)NULL);
	return 0;
}
