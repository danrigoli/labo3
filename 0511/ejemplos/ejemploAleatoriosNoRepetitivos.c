#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define CANTIDAD 10
#define DESDE 1
#define HASTA 20

int* generarRandom(int desde, int hasta, int cantidad)
{
	/* * * El 'desde' no es incluido en la seleccion del numero aleatorio. Ejemplo: para obtener el valor '0' debo arrancar desde el valor '-1' * * */ 

	int n,num,i,j,hastaRand,temp;
	int* vector = (int*) malloc(cantidad*sizeof(int));
	
	desde--;
	if((hasta-desde) < cantidad)
	{		
		printf("deben haber mas numeros que cantidades, %d es menor que %d\n",(hasta-desde),cantidad);
		free(vector);
		exit (0);
		    
	}
    

	hastaRand = (hasta-desde)/cantidad ;
   
	
	temp = desde;
	
	for (n=0 ; n < cantidad ; n++)
	{	
		num = (rand()%(hastaRand)) + 1;
		vector[n] = temp + num;
		//printf("el numero es %d: %d =%d \n", vector[n],hastaRand,num);	
     		temp = temp + hastaRand;
	}

	//printf("Reordeno---------\n");	
	for (i=cantidad-1 ; i > 0 ; i--)
	{
		num = rand()%(i);
	 	j = vector[i];
		vector[i] = vector[num];
		vector[num] = j;
	}

	return vector;

}




int main(int argc, char *argv[])
{	
	//cargo valores por defecto en caso de no pasarle por parametros.
	int desde = DESDE;
	int hasta = HASTA;
	int cantidad = CANTIDAD;
	int n;
	int* vector;
	
	//Siempre poner la funcion srand() en el main. 
	srand(time(NULL));
	
	if (argc > 3)
	{
		desde = atoi(argv[1]);
		hasta = atoi(argv[2]);
	    	cantidad = atoi(argv[3]);	
	}

	//Llamo a la funcion que me trae los numeros radom.
	vector = generarRandom(desde,hasta,cantidad);
	
	
	for (n=0; n<cantidad ; n++)
	{	
		printf("%d \n", vector[n]);	
	}


	return 0;
}
