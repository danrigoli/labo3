#include "funciones.h"
#include "definiciones.h"

#define INTERVALO_PEDIDOS 2000
#define CANT_CLIENTES 100
int clientes[CANT_CLIENTES];

void fill_clients() {
	int i;
	for (i = 0; i < CANT_CLIENTES; i++) {
		// random amount between 0 and 12345
		clientes[i] = rand() % 12345;
	}
}

void procesar_evento(int id_cola_mensajes, mensaje msg)
{
	char cadena[100];

	printf("Destino   %d\n", (int) msg.long_dest);
	printf("Remitente %d\n", msg.int_rte);
	printf("Evento    %d\n", msg.int_evento);
	printf("Mensaje   %s\n", msg.char_mensaje);

	switch (msg.int_evento)
	{
		case EVT_CONSULTA_SALDO:
			printf("Consultando saldo\n");
			int cuenta = atoi(msg.char_mensaje);
			if (cuenta < CANT_CLIENTES) {
				printf("Saldo de la cuenta %d es %d\n", cuenta, clientes[cuenta]);
				sprintf(cadena, "%d|%d", cuenta, clientes[cuenta]);
				printf("CADENA %s\n", cadena);
				enviar_mensaje(id_cola_mensajes , msg.int_rte, MSG_BANCO, EVT_RTA_SALDO, cadena);
			} else {
				printf("Cuenta no existe\n");
				sprintf(cadena, "%d|CLIENTE NO ENCONTRADO", cuenta);
				enviar_mensaje(id_cola_mensajes , msg.int_rte, MSG_BANCO, EVT_RTA_SALDO_NOK, cadena);
			}
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
	fill_clients();
	id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);
	
	while(1)
	{
		recibir_mensaje(id_cola_mensajes, MSG_BANCO, &msg);
		procesar_evento(id_cola_mensajes, msg);
	};
	
	return 0;
}


