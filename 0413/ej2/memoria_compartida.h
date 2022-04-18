#ifndef __memoria_compartida_H
#define __memoria_compartida_H
#endif


typedef struct tipo_dato dato;


void* creo_memoria(int size, int* r_id_memoria, int clave_base);

void cierro_memoria(int *memoria, int id_memoria);
