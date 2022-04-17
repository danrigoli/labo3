#include <stdio.h>
#include <stdlib.h>
#define CANTIDAD 10
#define INTERVALO 1
#define LARGO 100

int main()
{
	char cadena[LARGO]; /* Un array lo suficientemente grande como para guardar la línea más larga del fichero */
	int i;
	FILE *productor; 
	FILE *consumidor;

	printf("PRODUCIMOS\n");
	productor = fopen("producto.txt", "w");
	if (productor==NULL)
	{
		perror ("Error al abrir producto.txt");
	   return -1;
	}

	for (i=0; i<CANTIDAD; i++)
	{
		sprintf(cadena, "PRODUCTO-%02d\n", i);
		printf("%s", cadena);
		//fputs(cadena, productor);
		fprintf(productor,"%s",cadena);		
		sleep(INTERVALO);
	}
	fclose(productor);

	printf("\nCONSUMIMOS\n");
	consumidor = fopen("producto.txt", "r");
	if (productor==NULL)
	{
	   perror ("Error al abrir producto.txt");
	   return -1;
	}
	while (!feof(consumidor))
	{
		fscanf(consumidor,"%s",cadena);
		printf("%s\n", cadena);
		sleep(INTERVALO);
	}
	fclose(consumidor);
	return 0;
}
