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

pthread_mutex_t mutex;
int g_carton_lleno = 0;
int g_control = 0;

typedef struct tipo_jugador tjugador;
struct tipo_jugador
{
	int 	nro_jugador;	
	int*	bolillas;						
	int  	cantidad_aciertos;
	
};

void *ThreadJugadores (void *parametro)
{
	int 			cantidad_aciertos=0;
	int 			carton[CANT_CARTON]={0};
	int*			bolillas;
	int 			nro_jugador;
	int 			done=0;
	int 			bolilla=0;
	int			i, j;
	tjugador *datos_thread 	= (tjugador*) parametro;
	nro_jugador 		= datos_thread->nro_jugador;
	bolillas 		= datos_thread->bolillas;
	
	int encontrado = 0;
	
	printf("\nSoy el jugador %d\n", nro_jugador+1);

	for(i=0; i<CANT_CARTON; i++)
	{
		encontrado=0;
		while(encontrado==0)
		{
			bolilla = rand()%(BOLILLA_HASTA+1-BOLILLA_DESDE)+BOLILLA_DESDE;
			encontrado = 1;
			for(j=0; j<CANT_CARTON; j++)
			{
				if(bolilla == carton[j])
					encontrado=0;	
			}
		}
		carton[i]=bolilla; 
	}
	

		
	while(done==0)
	{
		pthread_mutex_lock (&mutex);
			if(g_control!=0)
			{
				g_control--;				
				if(g_carton_lleno==0)
				{			
					for(i=0; i<CANT_CARTON; i++)
					{	
						if(carton[i]!=0)
						{
							for(j=0; j<BOLILLA_HASTA; j++)
							{			
								if(carton[i]==bolillas[j] && carton[i]!=0)
								{	
									carton[i]=0;
									cantidad_aciertos++;
								}
							}
						}
					}
					printf("JUGADOR %d ", nro_jugador+1);				
					for(j=0; j<CANT_CARTON; j++)
					{			
						printf(" %02d ", carton[j]);
					}
					if(cantidad_aciertos==CANT_CARTON)
					{
						g_carton_lleno=nro_jugador+1;
					}
					printf(" Aciertos=%d g_control=%d g_carton_lleno=%d\n", cantidad_aciertos, g_control, g_carton_lleno);				
				}
				else
				{
					done=1;
				}
			}
		pthread_mutex_unlock (&mutex);

		usleep(TIEMPO_COMPRUEBA*1000);		

	};

	datos_thread->cantidad_aciertos=cantidad_aciertos;
	pthread_exit ((void *)"Listo");
}
		
int main(int argc, char *argv[])
{
	int i, ctl = 0;
	int cantidad=1;
	int bolillas[BOLILLA_HASTA]={0};
	int bolilla=0;
	tjugador *datos_thread;
	int done=0;
	int j, encontrado;
		
	if (argc>1)
		cantidad = atoi(argv[1]);

	printf("%d\n", cantidad);
	
	srand(time(NULL));

	pthread_t* idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantidad);
	pthread_attr_t 	atributos;
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);

	pthread_mutex_init (&mutex, NULL);

	datos_thread = (tjugador*) malloc(sizeof(tjugador)*cantidad);

	for(i=0; i<cantidad; i++)
	{
		datos_thread[i].nro_jugador = i;
		datos_thread[i].cantidad_aciertos=0;
		datos_thread[i].bolillas = bolillas;
		pthread_create (&idHilo[i], &atributos, ThreadJugadores, &datos_thread[i]);
	}
		
	while(done==0)
	{
		
		pthread_mutex_lock (&mutex);
			if(g_control == 0) 
			{
				g_control = cantidad; //de jugadores				
				if(g_carton_lleno==0)
				{			
					encontrado=0;
					while(encontrado==0)
					{
						bolilla = rand()%(BOLILLA_HASTA+1-BOLILLA_DESDE)+BOLILLA_DESDE;
						encontrado = 1;
						for(j=0; j<BOLILLA_HASTA; j++)
						{
							if(bolilla == bolillas[j])
								encontrado=0;	
						}
					}
					bolillas[i++]=bolilla; 
					
					printf("BINGO: bolilla = %d\n", bolilla);	
				}
				else
				{
					done = 1;
				}
			}
		pthread_mutex_unlock (&mutex);
		usleep(TIEMPO_BOLILLAS*1000);	
	};

	for(i=0; i<cantidad; i++)
	{
		pthread_join (idHilo[i], NULL);
		printf("TERMINO\n");
		printf("PPAL: Jugador %d: %d aciertos\n", i+1, datos_thread[i].cantidad_aciertos);
	}			
	return 0;
}



