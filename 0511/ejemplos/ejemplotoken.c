#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>

int main()
{
	int count = 0;
	char mensaje[100];

	char *token;
	token = NULL;

	strcpy(mensaje, "M0");
	strcat(mensaje, "|M1");
	strcat(mensaje, "|M2");

	token = strtok(mensaje, "|");
	while (token != NULL)
	{
		switch (count)
		{
			case 0:
				printf("%d %s\n", count, token);
			break;
			case 1:
				printf("%d %s\n", count, token);
			break;
			case 2:
				printf("%d %s\n", count, token);
			break;
		
			default:
		
			break;
		}
		token = strtok(NULL,"|");
		count++;
    }
}
