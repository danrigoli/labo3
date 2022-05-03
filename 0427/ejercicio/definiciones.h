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
	MSG_CAJERO,		
	MSG_BANCO,			
} Destinos;

typedef enum
{
	EVT_NINGUNO,
	EVT_CONSULTA_SALDO,
	EVT_DEPOSITO,
	EVT_EXTRACCION,
	EVT_RTA_SALDO,
	EVT_RTA_SALDO_NOK,
	EVT_RTA_DEPOSITO_OK,
	EVT_RTA_DEPOSITO_NOK,
	EVT_RTA_EXTRACCION_OK,
	EVT_RTA_EXTRACCION_NOK
} Eventos;


#endif