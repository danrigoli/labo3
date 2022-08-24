#ifndef _THREAD_H
#define _THREAD_H


#include <pthread.h>

/* estructura tipo caballo */
struct tipo_caballo
{
    int 	nro_caballo;
    int     id_cola;
    int     ganador;
};

/* definicion de tipo tcaballo */
typedef struct tipo_caballo tcaballo;

/* declaro funcion de thread jugador */
void *ThreadCaballo (void *parametro);

#endif