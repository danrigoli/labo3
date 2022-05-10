#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include <sys/time.h>


int funcionPrimos(int n)
{
	int c=0,c2=0,res=0,nc=0, total=0;
	for(c=1;c<=n;c++)
	{
		for(c2=1;c2<=c;c2++)
		{
			res=c%c2;
			if(res==0)
				nc=nc+1;
		}
		if(nc==2)
			total+=c;
		nc=0;
	}
	return total;	
}
		   
void *funcionPrimosThread (void *parametro)
{
	int c=0,c2=0,res=0,nc=0, total=0;

	int* ptr = (int*) parametro; //recibo el parametro y lo casteo como int *;	

	int n= *ptr; //en n cargo el contenido apuntado por *ptr.


	printf("THREAD: Voy a calcular los %d numeros primos y los sumo\n",n);

	total = funcionPrimos(n);

	printf("THREAD: %d -> El total es %d\n", n, total);
	
	*ptr = total; //utilizo *ptr para cargar el parametro de entrada el dato de salida.
	pthread_exit ((void*)0); //Esto solo puedo utilizarlo para devolver valores de status, no puedo devolver valores creados en la propia funcion thread.
}

int difMilli(struct timeval tiempo_start, struct timeval tiempo_end)
{
	int sec_start, micro_start;
	int start;
	int sec_end, micro_end;
	int end;

	sec_start   = tiempo_start.tv_sec & 0xFFF;
	micro_start = tiempo_start.tv_usec;
	start = (sec_start *1000) + (micro_start/1000);

	sec_end   = tiempo_end.tv_sec & 0xFFF;
	micro_end = tiempo_end.tv_usec;
	end = (sec_end *1000) + (micro_end/1000);
	return end - start;
}

int main (int argc, char *argv[])
{
	char con_thread = 's';
	int i, total=0;
	int cantidad = argc -1;
	int* num = (int*) malloc(sizeof(int)*cantidad);
	
	struct timeval 	t_inicio;
	struct timeval 	t_final;

	
	pthread_t* idHilo = (pthread_t* ) malloc(sizeof(pthread_t)*cantidad);
	pthread_attr_t 	atributos;
	
	
	pthread_attr_init (&atributos);
	pthread_attr_setdetachstate (&atributos, PTHREAD_CREATE_JOINABLE);
	printf("PPAL: Cantidad = %d\n", cantidad);
	
	printf("PPAL: Con thread (s/n)");
	scanf(" %c", &con_thread);

	if (con_thread=='s')
		printf("\nPPAL: CON THREAD\n");
	else
		printf("\nPPAL: SIN THREAD\n");
	gettimeofday(&t_inicio, 0);


	for(i=0; i<cantidad; i++)
	{
		num[i]=atoi(argv[i+1]);	
		if (con_thread=='s')
			pthread_create (&idHilo[i], &atributos, funcionPrimosThread, &num[i]);
		else
			total +=funcionPrimos(num[i]); 
	}
	
	if (con_thread=='s')
	{
		for(i=0; i<cantidad; i++)
		{
			printf("PPAL: Voy a esperar al thread %d\n", i);
			pthread_join (idHilo[i], NULL);
			printf("PPAL: Recibi: %d\n", num[i]);
			total +=num[i];
		}
	}
	
	printf("PPAL: El total es %d\n", total);
	
	gettimeofday(&t_final, 0);

	printf("PPAL: Tiemplo empleado %dms\n", difMilli(t_inicio, t_final));
	free(idHilo);
    return 0;
}
