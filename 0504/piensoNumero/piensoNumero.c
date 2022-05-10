#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>


#define DESDE 		            1
#define CANTIDAD_NUMEROS        99
#define TIEMPO_COMPRUEBA_DESDE	500
#define TIEMPO_COMPRUEBA_HASTA  5000

pthread_mutex_t mutex;
int g_control = 0;

typedef struct tipo_jugador tjugador;
struct tipo_jugador
{
    int 	nro_jugador;
	int* 	alguien_acerto;	
	int  	penso;
	int     cantidad_aciertos;
};

void *ThreadJugadores (void *parametro)
{
	int random_number;
    int nro_jugador;
	int alguien_acerto;
	int penso_en;
	int	i;
    int ya_pensados[CANTIDAD_NUMEROS] = {0};
    int habilitado = 1;

    tjugador *datos_thread 	= (tjugador*) parametro;
    nro_jugador = datos_thread->nro_jugador;
	alguien_acerto = *datos_thread->alguien_acerto;
	penso_en = datos_thread->penso;
	
	
    while(alguien_acerto == 0)
    {   
        habilitado = 1;
        random_number = rand()%(CANTIDAD_NUMEROS+1-DESDE)+DESDE;
        for(i = 0; i < CANTIDAD_NUMEROS; i++)
        {
            if(random_number == ya_pensados[i]) habilitado = 0;	
        }
        if(habilitado == 1) {
            printf("\nJugador %d pensó en %d\n", nro_jugador, random_number);
            ya_pensados[datos_thread->cantidad_aciertos] = random_number;
            if (penso_en == random_number) {
                alguien_acerto = nro_jugador;
                *datos_thread->alguien_acerto = alguien_acerto;
            }
            
        datos_thread->cantidad_aciertos++;
        usleep(rand()%(TIEMPO_COMPRUEBA_HASTA-TIEMPO_COMPRUEBA_DESDE)+TIEMPO_COMPRUEBA_DESDE);
        }
    }
	
	pthread_exit ((void *)"Listo");
}
		
int main(int argc, char *argv[])
{
	int i, ctl = 0;
	int cantidad = 1;
	int pense = 0;
	tjugador *datos_thread;
	int alguien_acerto = 0;
	int j;
	pthread_t* idHilo;
    pthread_attr_t 	atributos;
    
	if (argc > 1)
		cantidad = atoi(argv[1]);

	printf("%d\n", cantidad);
	
	srand(time(NULL));
    
    pense = rand()%(CANTIDAD_NUMEROS+1-DESDE)+DESDE;
    printf("Pense en: %d\n", pense);
	idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantidad);
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);

	pthread_mutex_init (&mutex, NULL);

	datos_thread = (tjugador*) malloc(sizeof(tjugador)*cantidad);

	for(i = 0; i < cantidad; i++)
	{
		datos_thread[i].nro_jugador = i + 1;
		datos_thread[i].alguien_acerto = &alguien_acerto;
		datos_thread[i].penso = pense;
        datos_thread[i].cantidad_aciertos = 0;
		pthread_create (&idHilo[i], &atributos, ThreadJugadores, &datos_thread[i]);
	}
		
	while(alguien_acerto == 0)
	{

		pthread_mutex_lock (&mutex);
			if(g_control == 0) 
			{
				g_control = cantidad;				
				
			}
		pthread_mutex_unlock (&mutex);
        usleep(100000);
	};

	for(i=0; i< cantidad; i++)
	{
		pthread_join (idHilo[i], NULL);
		printf("TERMINO\n");
	}	

    printf("Adivino el jugador %d con %d aciertos\n", alguien_acerto, datos_thread[alguien_acerto - 1].cantidad_aciertos);
	
    return 0;
}



