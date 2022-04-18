#ifndef __memoria_compartida_H
#define __memoria_compartida_H
#endif


typedef struct tipo_dato dato;

struct tipo_dato
{
    char nombre_jugador[100];
	int numero_pensado;
    int estado_acierto;
};

void* creo_memoria(int size, int* r_id_memoria, int clave_base);

void cierro_memoria(dato *memoria, int id_memoria);
