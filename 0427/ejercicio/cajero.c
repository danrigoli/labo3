
#include "funciones.h"
#include "definiciones.h"

#define INTERVALO_PEDIDOS 1000

void procesar_evento(int id_cola_mensajes, mensaje msg)
{

	printf("Destino   %d\n", (int) msg.long_dest);
	printf("Remitente %d\n", msg.int_rte);
	printf("Evento    %d\n", msg.int_evento);
	printf("Mensaje   %s\n", msg.char_mensaje);

	char *split = strtok(msg.char_mensaje, "|");
	printf("Cuenta: %s\n", split);
	split = strtok(NULL, "|");

	switch (msg.int_evento)
	{

		case EVT_RTA_SALDO:
			printf("Saldo: %s\n", split);
		break;

		case EVT_RTA_SALDO_NOK:
			printf("Ocurrio un error al obtener el saldo: %s,\n", split);
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
	mensaje	msg;
	int choice = 0;
	int account_id = 0;
	char message[100];

	id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);

	printf("Inserte el numero de la cuenta: ");
	scanf("%d", &account_id);

	while(choice != 4)
	{

		printf("1. Consultar Saldo\n");
		printf("2. Depositar\n");
		printf("3. Extraer\n");
		printf("4. Salir\n");
		scanf("%d", &choice);
		printf("choice: %d\n", choice);
		switch (choice)
		{
			case 1:
				printf("Consultar Saldo\n");
				sprintf(message, "%d", account_id);
				enviar_mensaje(id_cola_mensajes , MSG_BANCO, MSG_CAJERO, EVT_CONSULTA_SALDO, message);
				recibir_mensaje(id_cola_mensajes, MSG_CAJERO, &msg);
				procesar_evento(id_cola_mensajes, msg);
			break;
			case 2:
				enviar_mensaje(id_cola_mensajes , MSG_BANCO, MSG_CAJERO, EVT_DEPOSITO, "DEPOSITO");
				recibir_mensaje(id_cola_mensajes, MSG_CAJERO, &msg);
				procesar_evento(id_cola_mensajes, msg);
			break;
			case 3:
				enviar_mensaje(id_cola_mensajes , MSG_BANCO, MSG_CAJERO, EVT_EXTRACCION, "RETIRO");
				recibir_mensaje(id_cola_mensajes, MSG_CAJERO, &msg);
				procesar_evento(id_cola_mensajes, msg);
			break;
			case 4:
				printf("Saliendo...\n");
			break;
			default:
				printf("Opcion no valida\n");
			break;
		}
		usleep (INTERVALO_PEDIDOS*1000);
	};
	
	return 0;
}
