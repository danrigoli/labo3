#include "funciones.h"
#include "definiciones.h"

#define INTERVALO_PEDIDOS 2000
#define CANT_CLIENTES 100

int i, cuenta_id, id_cola_mensajes;
cuenta memoria_cuentas[100];
char cadena[100];
cuenta cuenta_encontrada;
cuenta error_cuenta = {-1, -1};
mensaje msg;

void fill_cuentas(cuenta* cuentas) {
	for (i = 0; i <= CANT_CLIENTES; i++) {
		// random amount between 0 and 12345
		cuentas[i].codigo = i+1;
		cuentas[i].balance = rand() % 12345;
	}
}

cuenta find_client(int id_cliente) {
	for (i = 0; i < CANT_CLIENTES; i++)
	{
		if (memoria_cuentas[i].codigo == id_cliente)
			return memoria_cuentas[i];
	}
	return error_cuenta;
	
}

void send_error(int id_cola_mensajes, int remitente_id, int rta, int cuenta_id, char cadena[100], char error[50]) {
	printf("Cuenta no existe\n");
	sprintf(cadena, "%d|%s", cuenta_id, error);
	enviar_mensaje(id_cola_mensajes , remitente_id, MSG_BANCO, rta, cadena);
}

void send_success(int id_cola_mensajes, int remitente_id, int rta, int cuenta_id, int balance, char cadena[100]) {
	sprintf(cadena, "%d|%d", cuenta_id, balance);
	enviar_mensaje(id_cola_mensajes , remitente_id, MSG_BANCO, rta, cadena);
}

void procesar_evento(int id_cola_mensajes, mensaje msg, cuenta *cuentas) {

	printf("Destino   %d\n", (int) msg.long_dest);
	printf("Remitente %d\n", msg.int_rte);
	printf("Evento    %d\n", msg.int_evento);
	printf("Mensaje   %s\n", msg.char_mensaje);

	char *split = strtok(msg.char_mensaje, "|");
	cuenta_id = atoi(split);

	switch (msg.int_evento)
	{
		case EVT_CONSULTA_SALDO:

			printf("Consultando saldo \n");
			cuenta_encontrada = find_client(cuenta_id);
			if (cuenta_encontrada.codigo != -1) {
			sprintf(cadena, "%d|%d", cuenta_id, cuenta_encontrada.balance);
			enviar_mensaje(id_cola_mensajes , msg.int_rte, MSG_BANCO, EVT_RTA_SALDO, cadena);
			} else send_error(id_cola_mensajes, msg.int_rte, EVT_RTA_SALDO_NOK, cuenta_id, cadena, "Cliente no encontrado");

		break;

		case EVT_DEPOSITO:

			printf("Depositando \n");
			cuenta_encontrada = find_client(cuenta_id);
			if (cuenta_encontrada.codigo != -1) {
				split = strtok(NULL, "|");
				cuenta_encontrada.balance += atoi(split);
				sprintf(cadena, "%d|%d", cuenta_id, cuenta_encontrada.balance);
				enviar_mensaje(id_cola_mensajes , msg.int_rte, MSG_BANCO, EVT_RTA_DEPOSITO_OK, cadena);
			} else send_error(id_cola_mensajes, msg.int_rte, EVT_RTA_DEPOSITO_NOK, cuenta_id, cadena, "Cliente no encontrado");

		break;
		case EVT_EXTRACCION:
		
			printf("Extrayendo \n");
			if (cuenta_encontrada.codigo != -1) {
				split = strtok(NULL, "|");
				if (cuenta_encontrada.balance >= atoi(split)) {
				cuenta_encontrada.balance -= atoi(split);
				send_success(id_cola_mensajes, msg.int_rte, EVT_RTA_EXTRACCION_OK, cuenta_id, cuenta_encontrada.balance, cadena);
				} else send_error(id_cola_mensajes, msg.int_rte, EVT_RTA_EXTRACCION_NOK, cuenta_id, cadena, "No hay suficiente saldo");
			} else send_error(id_cola_mensajes, msg.int_rte, EVT_RTA_EXTRACCION_NOK, cuenta_id, cadena, "Cliente no encontrado");
		break;

		default:
			printf("\nEvento sin definir\n");
		break;
	}

	printf("------------------------------\n");
}

int main()
{
	
	fill_cuentas(memoria_cuentas); 

	id_cola_mensajes = creo_id_cola_mensajes(CLAVE_BASE);
	
	while(1)
	{
		recibir_mensaje(id_cola_mensajes, MSG_BANCO, &msg);
		procesar_evento(id_cola_mensajes, msg, memoria_cuentas);
	};
	
	return 0;
}


