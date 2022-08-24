#ifndef _DEFINICIONES_H
#define _DEFINICIONES_H
 

/* enum de destinos */
typedef enum
{
	MSG_NADIE,
	MSG_BOLERA,
	MSG_JUGADOR
} Destinos;

/* enum de eventos */
typedef enum
{
	EVT_NINGUNO,
	EVT_INICIO,
	EVT_TIRO,
	EVT_RESULTADO,
	EVT_FIN
} Eventos;


#endif
