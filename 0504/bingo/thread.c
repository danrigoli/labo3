#include "thread.h"

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
            /* si g_control es 0, es porque todos los jugadores pasaron, entonces se cambia el valor de g_control a la cantidad de jugadores y se saca otra bolilla */		
			if(g_control != 0)
			{
				g_control--;
                /* si nadie completo el carton, se fija si la bolilla esta en su carton */
				if(g_carton_lleno == 0)
				{
                    /* busca en el carton si esta la bolilla */
					for(i = 0; i < CANT_CARTON; i++)
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

                    printf("Carton del jugador %d\n", nro_jugador+1);
					for(j = 0; j < CANT_CARTON; j++)
					{			
						printf(" %d ", carton[j]);
					}
                    /* si el jugador completo el carton g_carton_lleno = 1 */
					if(cantidad_aciertos == CANT_CARTON)
					{
						g_carton_lleno = nro_jugador+1;
					}
					printf("\nAciertos de jugador %d: %d \n", nro_jugador + 1, cantidad_aciertos);				
				}
				else
				{
					/* algun jugador completo el carton */
					bingo = 1;
				}
			}
		pthread_mutex_unlock (&mutex);

		usleep(TIEMPO_COMPRUEBA*1000);		

	};

	datos_thread->cantidad_aciertos = cantidad_aciertos;
	pthread_exit ((void *)"Listo");
}