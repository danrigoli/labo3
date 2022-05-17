CC = gcc-6 -std=gnu89 -pedantic-errors

all : revolver main

revolver: revolver.c funciones.h definiciones.h funciones.o
	$(CC) -o revolver revolver.c funciones.o

main : main.c funciones.o definiciones.h jugador.h jugador.o  
	$(CC) -o main main.c jugador.o funciones.o -pthread

funciones.o : funciones.c funciones.h
	$(CC) -c funciones.c

jugador.o : jugador.c jugador.h 
	$(CC) -c jugador.c -pthread

clean: 
	rm -rf *o revolver main *~
