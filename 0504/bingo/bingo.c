#include "thread.h"


/* Variables globales */
pthread_mutex_t mutex;
int g_carton_lleno = 0;
int g_control = 0;

		
int main(int argc, char *argv[])
{
	/* variables locales */
	int i, ctl = 0;
	int cantidad = 1;
	int bolillas[BOLILLA_HASTA] = {0};
	int bolilla = 0;
	tjugador *datos_thread;
	pthread_t* idHilo;
	pthread_attr_t 	atributos;
	int bingo = 0;
	int j, encontrado;
		
	/* si pasan un argumento, es la cantidad de jugadores
	   sino, se queda en 1 por default */

	if (argc > 1) cantidad = atoi(argv[1]);

	printf("%d\n", cantidad);
	
	/* semilla del random */
	srand(time(NULL));

	/* inicializacion de los parametros de los threads*/	
	idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantidad);
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);

	pthread_mutex_init (&mutex, NULL);

	datos_thread = (tjugador*) malloc(sizeof(tjugador)*cantidad);
	/* inicializacion de los datos de los jugadores en thread */
	for(i=0; i<cantidad; i++)
	{
		datos_thread[i].nro_jugador = i;
		datos_thread[i].cantidad_aciertos = 0;
		datos_thread[i].bolillas = bolillas;
		pthread_create (&idHilo[i], &atributos, ThreadJugadores, &datos_thread[i]);
	}
		
	while(bingo == 0)
	{
		
		pthread_mutex_lock (&mutex);
		
			if(g_control == 0) 
			{
				/* si g_control es 0, es porque todos los jugadores pasaron, entonces se cambia el valor
				   de g_control a la cantidad de jugadores y se saca otra bolilla
				*/

				g_control = cantidad;	

				/* si el carton no esta lleno se van pasando bombillas sino sacaron el bingo */

				if(g_carton_lleno == 0)
				{
					encontrado = 0;
					while(encontrado == 0)
					{
						bolilla = rand()%(BOLILLA_HASTA+1-BOLILLA_DESDE)+BOLILLA_DESDE;
						encontrado = 1;
						for(j = 0; j<BOLILLA_HASTA; j++)
						{
							if(bolilla == bolillas[j])
								encontrado=0;	
						}
					}
					bolillas[i++] = bolilla; 
					
					printf("BINGO: bolilla %d\n", bolilla);	
				}
				else
				{
					bingo = 1;
				}
			}

		pthread_mutex_unlock (&mutex);
		usleep(TIEMPO_BOLILLAS*1000);	
	};

	for(i = 0; i < cantidad; i++)
	{
		pthread_join (idHilo[i], NULL);
		printf("TERMINO\n");
		printf("Jugador %d: %d aciertos\n", i+1, datos_thread[i].cantidad_aciertos);
	}			
	return 0;
}



