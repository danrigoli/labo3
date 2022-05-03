
#include "funciones.h"
#include "definiciones.h"

#define INTERVALO_PEDIDOS 1000

int id_cola_mensajes;
mensaje	msg;
int choice = 0;
int account_id = 0;
int dep_ext = 0;
char message[100];

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
			printf("Ocurrio un error al obtener el saldo: %s \n", split);
		break;

		case EVT_RTA_DEPOSITO_OK:
			printf("Deposito realizado con exito: %s \n", split);
		break;

		case EVT_RTA_DEPOSITO_NOK:
			printf("Ocurrio un error al depositar: %s \n", split);
		break;

		case EVT_RTA_EXTRACCION_OK:
			printf("Retiro realizado con exito: %s \n", split);
		break;

		case EVT_RTA_EXTRACCION_NOK:
			printf("Ocurrio un error al retirar: %s \n", split);
		break;

		default:
			printf("\nEvento sin definir\n");
		break;
	}
	printf("------------------------------\n");
}

int main()
{

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
				printf("Consultando Saldo...\n");
				sprintf(message, "%d", account_id);
				enviar_mensaje(id_cola_mensajes , MSG_BANCO, MSG_CAJERO, EVT_CONSULTA_SALDO, message);
				recibir_mensaje(id_cola_mensajes, MSG_CAJERO, &msg);
				procesar_evento(id_cola_mensajes, msg);
			break;
			case 2:
				printf("Ingrese el total a depositar: ");
				scanf("%d", &dep_ext);
				sprintf(message, "%d|%d", account_id, dep_ext);
				enviar_mensaje(id_cola_mensajes , MSG_BANCO, MSG_CAJERO, EVT_DEPOSITO, message);
				recibir_mensaje(id_cola_mensajes, MSG_CAJERO, &msg);
				procesar_evento(id_cola_mensajes, msg);
			break;
			case 3:
				printf("Ingrese el total a extraer: ");
				scanf("%d", &dep_ext);
				sprintf(message, "%d|%d", account_id, dep_ext);
				enviar_mensaje(id_cola_mensajes , MSG_BANCO, MSG_CAJERO, EVT_EXTRACCION, message);
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
