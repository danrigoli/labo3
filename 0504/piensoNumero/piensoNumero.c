#include "thread.h"
	
pthread_mutex_t mutex;


int main(int argc, char *argv[])
{
	/* variables locales */
	int i = 0;
	int cantidad = 1;
	int pense = 0;
	tjugador *datos_thread;
	int alguien_acerto = 0;
	pthread_t* idHilo;
    pthread_attr_t 	atributos;
    
	/* si hay +1 argumento, el segundo es la cantidad de jugadores */
	if (argc > 1)
		cantidad = atoi(argv[1]);
	
	/* semilla rand */
	srand(time(NULL));
    
	/* numero random pensado */
    pense = rand()%(CANTIDAD_NUMEROS+1-DESDE)+DESDE;
    printf("Pense en: %d\n", pense);

	/* se guarda memoria dinamica para un array de ids de hilo segun la cantidad de jugadores */
	idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantidad);

	/* inicia atributos de threads con la variable atributos*/
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);

	/* inicializacion de mutex */
	pthread_mutex_init (&mutex, NULL);

	/* se guarda memoria dinamica para los datos a pasar al thread (parametro) x cantidad de jugadores*/
	datos_thread = (tjugador*) malloc(sizeof(tjugador)*cantidad);

	/* inicializacion de los datos de los jugadores en thread */
	for(i = 0; i < cantidad; i++)
	{
		datos_thread[i].nro_jugador = i + 1;
		datos_thread[i].alguien_acerto = &alguien_acerto;
		datos_thread[i].penso = pense;
        datos_thread[i].cantidad_aciertos = 0;
		pthread_create (&idHilo[i], &atributos, ThreadJugadores, &datos_thread[i]);
	}

	/* espera a que alguien acierte */
	while(alguien_acerto == 0) {}

	/* une los hilos a main */
	for(i=0; i< cantidad; i++)
	{
		pthread_join (idHilo[i], NULL);
	}	

    printf("Adivino el jugador %d con %d aciertos\n", alguien_acerto, datos_thread[alguien_acerto - 1].cantidad_aciertos);
	
    return 0;
}



