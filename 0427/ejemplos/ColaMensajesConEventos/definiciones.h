#ifndef _DEFINICIONES_H
#define _DEFINICIONES_H

#define LARGO_DESCRIPCION 100
#define CANT_SEMAFORO 1


typedef struct tipo_datos datos;
struct tipo_datos
{
	int dato;
};

typedef enum
{
	MSG_NADIE,				
	MSG_SUPERMERCADO,		
	MSG_REPOSITOR,			
	MSG_CLIENTE	
}Destinos;

typedef enum
{
	EVT_NINGUNO,
	EVT_CONSULTA_STOCK,
	EVT_RESPUESTA_STOCK,
	EVT_SUMA_STOCK,
	EVT_RESTA_STOCK
}Eventos;


#endif