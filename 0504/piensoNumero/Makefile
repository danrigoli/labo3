CC 	= gcc -std=gnu89 -pedantic-errors -pthread

all : piensoNumero 

piensoNumero : piensoNumero.c thread.c
	$(CC) -o piensoNumero piensoNumero.c thread.c

clean: rm -rf *o piensoNumero