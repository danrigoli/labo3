#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

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

int main ()
{
	struct timeval 	t_inicio;
	struct timeval 	t_final;

	gettimeofday(&t_inicio, 0);
	
		sleep(1);

	gettimeofday(&t_final, 0);

	printf("Tiemplo empleado %dms\n", difMilli(t_inicio, t_final));

	return 0;
}	

	