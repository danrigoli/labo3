#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CANTIDAD 10

pthread_mutex_t mutex;


void *funcionThread (void *parametro)
{
	int i;
	printf ("Hijo\n");
	for(i=0; i<CANTIDAD; i++)
	{
		pthread_mutex_lock (&mutex);
			printf("Soy el hijo y tengo el mutex\n");
			sleep (1);
		pthread_mutex_unlock (&mutex);
		sleep(1);
	}
	printf("Soy el hijo y espero 10seg\n");
	sleep (10);
	printf ("Hijo  : Termino\n");
	pthread_exit ((void *)"Listo");
}

int main()
{
	
	pthread_t 		idHilo;
	pthread_attr_t 	atributos;
	int 			error;
	int 			i;
	char*			valorDevuelto = NULL;

	pthread_mutex_init (&mutex, NULL);
	
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
	if (pthread_create (&idHilo, &atributos, funcionThread, NULL)!= 0)
	{
		perror ("No puedo crear thread");
		exit (-1);
	}
	
	for(i=0; i<CANTIDAD; i++)
	{
		pthread_mutex_lock (&mutex);
			printf("Soy el padre y tengo el mutex\n");
			sleep (1);
		pthread_mutex_unlock (&mutex);
		sleep(1);
	}
	
	printf ("Padre : Espero al thread\n");
	pthread_join (idHilo, (void **)&valorDevuelto);
	printf ("Padre : Ya ha terminado el thread\n");
	printf ("Padre : devuelve \"%s\"\n", valorDevuelto);
	
	return 0;
}
