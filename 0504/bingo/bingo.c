#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>


#define CANT_CARTON 		5
#define BOLILLA_DESDE 		1
#define BOLILLA_HASTA 		99
#define TIEMPO_COMPRUEBA	10
#define TIEMPO_BOLILLAS		500

/* Variables globales */
pthread_mutex_t mutex;
int g_carton_lleno = 0;
int g_control = 0;

/* Estructura tipo jugador */
typedef struct tipo_jugador tjugador;
struct tipo_jugador
{
	int 	nro_jugador;	
	int*	bolillas;						
	int  	cantidad_aciertos;
	
};

/* funcion de thread jugador */
void *ThreadJugadores (void *parametro)
{

	/* variables locales de jugador*/

	int cantidad_aciertos = 0;
	int carton[CANT_CARTON]={0};
	int* bolillas;
	int nro_jugador;
	int bingo = 0;
	int bolilla = 0;
	int	i, j;
	int encontrado = 0;
	tjugador *datos_thread 	= (tjugador*) parametro;

	/* asignacion de variables locales de jugador
	   a las variables del parametro */
	nro_jugador = datos_thread->nro_jugador;
	bolillas = datos_thread->bolillas;
	
	
	printf("\nJugador %d\n", nro_jugador+1);

	/* inicializacion del carton */
	for(i = 0; i < CANT_CARTON; i++)
	{
		/* 
		si encontrado es 0, sigue buscando una bolilla
		hasta que encuentre una bolilla que no este
		en el carton, una vez que lo encuentra i++
		*/

		encontrado = 0;
		while(encontrado == 0)
		{
			bolilla = rand()%(BOLILLA_HASTA+1-BOLILLA_DESDE)+BOLILLA_DESDE;
			encontrado = 1;
			for(j = 0; j < CANT_CARTON; j++)
			{
				if(bolilla == carton[j])
					encontrado = 0;	
			}
		}
		carton[i] = bolilla; 
	}
	/* 
	mientras que algun jugador no saque el bingo, sigue
	 */
	while(bingo == 0)
	{
		pthread_mutex_lock (&mutex);
			if(g_control != 0)
			{
				g_control--;				
				if(g_carton_lleno == 0)
				{
					for(i = 0; i<CANT_CARTON; i++)
					{	
						if(carton[i] != 0)
						{
							for(j = 0; j < BOLILLA_HASTA; j++)
							{			
								if(carton[i] == bolillas[j] && carton[i] != 0)
								{	
									carton[i] = 0;
									cantidad_aciertos++;
								}
							}
						}
					}
					printf("JUGADOR %d ", nro_jugador+1);				
					for(j = 0; j < CANT_CARTON; j++)
					{			
						printf(" %02d ", carton[j]);
					}
					if(cantidad_aciertos == CANT_CARTON)
					{
						g_carton_lleno = nro_jugador+1;
					}
					printf(" Aciertos:%d - g_control=%d - g_carton_lleno=%d\n", cantidad_aciertos, g_control, g_carton_lleno);				
				}
				else
				{
					/* todos los jugadores completaron el carton */
					bingo = 1;
				}
			}
		pthread_mutex_unlock (&mutex);

		usleep(TIEMPO_COMPRUEBA*1000);		

	};

	datos_thread->cantidad_aciertos = cantidad_aciertos;
	pthread_exit ((void *)"Listo");
}
		
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
				/* si g_control es 0, es porque todos los jugadores
				   pasaron, entonces se cambia el valor
				   de g_control a la cantidad de jugadores
				   y se saca otra bolilla
				*/

				g_control = cantidad;	

				/* si el carton no esta lleno
				se van pasando bombillas sino 
				sacaron el bingo
				*/

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



