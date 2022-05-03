#include "funciones.h"
#include "definiciones.h"

#define INTERVALO_PEDIDOS 2000

void procesar_evento(int id_cola_mensajes, mensaje msg)
{
	char cadena[100];
	static int cantidad = 0;  //es inicializada solo una vez, mantiene el valor en las sucesivas llamadas.
	printf("Destino   %d\n", (int) msg.long_dest);
	printf("Remitente %d\n", msg.int_rte);
	printf("Evento    %d\n", msg.int_evento);
	printf("Mensaje   %s\n", msg.char_mensaje);
	switch (msg.int_evento)
	{
		case EVT_CONSULTA_STOCK:
			printf("Consulta Stock\n");
			sprintf(cadena, "%d", cantidad);
			enviar_mensaje(id_cola_mensajes , msg.int_rte, MSG_SUPERMERCADO, EVT_RESPUESTA_STOCK, cadena);
		break;
		case EVT_SUMA_STOCK:
			printf("Suma Stock\n");
			cantidad++;
			break;
		default:
			printf("\nEvento sin definir\n");
		break;
	}
	printf("------------------------------\n");
}
		
int main()
{
	int id_cola_mensajes;
	mensaje msg;

	id_cola_mensajes 	= creo_id_cola_mensajes(CLAVE_BASE);
	
	while(1)
	{
		recibir_mensaje(id_cola_mensajes, MSG_SUPERMERCADO, &msg);
		procesar_evento(id_cola_mensajes, msg);
	};
	
	return 0;
}


