#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define CANTIDAD_PARTIDA 	10
#define INTERVALO_PRODUCTO 	500
#define INTERVALO_PARTIDA 	1000
#define ROJO  			0
#define VERDE 			1
#define LARGO 			100

key_t creo_clave()
{
	// Igual que en cualquier recurso compartido (memoria compartida, semaforos
	// o colas) se obtien una clave a partir de un fichero existente cualquiera
	// y de un entero cualquiera. Todos los procesos que quieran compartir este
	// semaforo, deben usar el mismo fichero y el mismo entero.
	key_t clave;
	clave = ftok ("/bin/ls", 33);
	if (clave == (key_t)-1)
	{
		printf("No puedo conseguir clave de semáforo\n");
		exit(0);
	}
	return clave;
}

//funcion que crea el semaforo
int creo_semaforo()
{
  key_t clave = creo_clave();
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
void inicio_semaforo(int id_semaforo, int valor)
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
	char cadena[LARGO]; /* Un array lo suficientemente grande como para guardar la línea más larga del fichero */
	int cant_producto=0;
	FILE *consumidor; 
	
	int id_semaforo;
	id_semaforo =  creo_semaforo();
	
	while(1)
	{
		espera_semaforo(id_semaforo);
			
			consumidor = fopen("producto.txt", "r");
			if (consumidor!=NULL)
			{
				printf("\nCONSUMIMOS\n");
				while (!feof(consumidor))
				{
					fscanf(consumidor,"%s",cadena);
					printf("%s\n", cadena);
					usleep(INTERVALO_PRODUCTO*1000);
					cant_producto++;
				}
				fclose(consumidor);
				if(cant_producto>0)
				{
					printf("\nBORRAMOS\n");
					remove("producto.txt");
					cant_producto=0;
				}
			}
			else
				perror ("Error al abrir producto.txt");
			printf("\nESPERAMOS\n");
						
		levanta_semaforo(id_semaforo);
		usleep(INTERVALO_PARTIDA*1000);
	};
	return 0;
}
