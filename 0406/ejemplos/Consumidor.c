#include <stdio.h>
#include <stdlib.h>
#define CANTIDAD_PARTIDA 10
#define INTERVALO_PRODUCTO 500
#define INTERVALO_PARTIDA 1000
#define LARGO 100

int main()
{
	char cadena[LARGO]; /* Un array lo suficientemente grande como para guardar la línea más larga del fichero */
	int cant_producto=0;
	FILE *consumidor; 

	while(1)
	{
		consumidor = fopen("producto.txt", "r");
		if (consumidor!=NULL)
		{
			printf("\nCONSUMIMOS\n");
			while (!feof(consumidor))
			{
				fscanf(consumidor,"%s",cadena);
				printf("%s\n",cadena);
				usleep(INTERVALO_PRODUCTO*1000);
				cant_producto++;
			}
			fclose(consumidor);
			if(cant_producto>CANTIDAD_PARTIDA)
			{
				printf("\nBORRAMOS\n");
				remove("producto.txt");
				cant_producto=0;
			}
			else
				printf("\nNADA\n");
		}
		else
			perror ("Error al abrir producto.txt");
		printf("\nESPERAMOS\n");
		usleep(INTERVALO_PARTIDA*1000);
	};
	return 0;
}
