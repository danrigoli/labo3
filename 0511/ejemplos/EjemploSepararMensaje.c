#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANTIDAD 3


char** separar(char *mensaje, char *caracter){
   
	int count = 0; 
	char** array;	

	//creo temp para poder primero contar la cantidad de iteraciones
	char *temp = malloc(strlen(mensaje));
	char *token;
	token = NULL;
	sprintf(temp,"%s\n",mensaje);
	token = strtok(temp, caracter);
 
	//Cuento las iteraciones (asi se el tamaño del array que necesito)
	while (token != NULL)
	{
        	token = strtok(NULL,caracter);
        	count++;
    	}

	//una vez que tengo el tamaño del array creo el array con un maloc
	array=(char**)malloc(sizeof(char*)*count);

	//empiezo a iterar por el texto
	count = 0;
	token = strtok(mensaje, caracter);
	while (token != NULL)
	{
	        //le asigno su tamaño
	        array[count]= (char*)malloc(sizeof(char)*strlen(token));

	        //le paso al vector el dato 
	        sprintf(array[count],"%s\n", token);

	        //printf("pasando a %d: %s\n",count,array[count]);
	        token = strtok(NULL,caracter);

	        count++;
	}
    
  
	return array;
}

int main(int argc, char *argv[])
{
	int i;
	char **array;
	char test[100];

	//Genero el mensaje y lo guardo en la variable test - de tipo char, cadena de texto.
	sprintf(test,"Esto|es|test");

	//Ejecuto la funcion separar y asigno el resultado en array
    	array  = separar(test,"|");

    
    	for(i=0 ; i<CANTIDAD ; i++)
	{
    		printf("dato %d: %s\n",i,array[i]);
    	}

	return 0;
}

