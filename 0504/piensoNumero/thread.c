#include "thread.h"


void *ThreadJugadores (void *parametro)
{
    /* variables locales de jugador*/
	int random_number;
    int nro_jugador;
	int penso_en;
	int	i;
    int ya_pensados[CANTIDAD_NUMEROS] = {0};
    int no_pensado = 1;
    int done = 0;
    tjugador *datos_thread 	= (tjugador*) parametro;

    /* guardo nro jugador y el numero que penso main */
    nro_jugador = datos_thread->nro_jugador;
	penso_en = datos_thread->penso;
	
	/* aca agregue la variable done porque si la variable alguien_acerto mientras el jugador esperaba el mutex
       iba a intentar aunque un jugador ya haya ganado, de esta manera no participa si alguien ya gano */
    while(done == 0)
    {   
        /* espera a que el mutex este libre y lo lockea*/
        pthread_mutex_lock (&mutex);
        /* si no hay nadie que acerto, intento adivinar */
        if (*datos_thread->alguien_acerto == 0) {
            no_pensado = 1;
            random_number = rand()%(CANTIDAD_NUMEROS+1-DESDE)+DESDE;
            /* si el numero random no esta en el array ya_pensados, consulto si es el numero que penso main */
            for(i = 0; i < CANTIDAD_NUMEROS; i++)
            {
                if(random_number == ya_pensados[i]) no_pensado = 0;	
            }
            if(no_pensado == 1) {
                /* si es el numero que penso main, cambio alguien_acerto a nro_jugador */
                printf("\nJugador %d pensó en %d\n", nro_jugador, random_number);
                ya_pensados[datos_thread->cantidad_aciertos] = random_number;
                if (penso_en == random_number) {
                    *datos_thread->alguien_acerto = nro_jugador;
                }
                /* incremento cantidad de aciertos */
                datos_thread->cantidad_aciertos++;
            }
        } else {
            /* si alguien ya acerto, no hago nada y corto el loop */
            done = 1;
        }
        /* desbloqueo el mutex */
        pthread_mutex_unlock (&mutex);
        usleep(rand()%(TIEMPO_COMPRUEBA_HASTA-TIEMPO_COMPRUEBA_DESDE)+TIEMPO_COMPRUEBA_DESDE);
    }
	
	pthread_exit ((void *)"Listo");
}
		