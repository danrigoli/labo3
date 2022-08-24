#ifndef __memoria_compartida_H
#define __memoria_compartida_H
#endif

#define LARGO 100 

typedef struct tipo_dato dato;

struct tipo_dato
{
	int nro_panel;
	char mensaje[LARGO];
};

void* creo_memoria(int size, int* r_id_memoria, int clave_base);

void cierro_memoria(dato *memoria, int id_memoria);
