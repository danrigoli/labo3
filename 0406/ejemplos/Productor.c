#include <stdio.h>
#include <stdlib.h>
#define CANTIDAD_PARTIDA 10
#define INTERVALO_PRODUCTO 1000
#define INTERVALO_PARTIDA 5000
#define LARGO 100

int main()
{
	char cadena[LARGO]; /* Un array lo suficientemente grande como para guardar la línea más larga del fichero */
	int nro_producto=0, nro_partida=0;
	FILE *productor; 

	while(1)
	{
		printf("\nPRODUCIMOS-PARTIDA-%04d\n",nro_partida);
		productor = fopen("producto.txt", "a");
		if (productor!=NULL)
		{
			for (nro_producto=0; nro_producto<CANTIDAD_PARTIDA; nro_producto++)
			{
				sprintf(cadena, "PRODUCTO-%04d-%04d\n", nro_partida, nro_producto);
				printf("%s", cadena);
				fprintf(productor,"%s",cadena);
				usleep(INTERVALO_PRODUCTO*1000);
			}
			fclose(productor);
			nro_partida++;
			printf("\nESPERAMOS\n");
			usleep(INTERVALO_PARTIDA*1000);
		}
		else
		{
			perror ("Error al abrir producto.txt");
		}
	};
	return 0;
}
