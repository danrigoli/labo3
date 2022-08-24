CC = gcc-6 -std=gnu89 -pedantic-errors

all : bolera jugadores

bolera: bolera.c funciones.h definiciones.h funciones.o
	$(CC) -o bolera bolera.c funciones.o

jugadores: jugadores.c funciones.o definiciones.h jugador.h jugador.o  
	$(CC) -o jugadores jugadores.c jugador.o funciones.o -pthread

funciones.o : funciones.c funciones.h
	$(CC) -c funciones.c

jugador.o : jugador.c jugador.h 
	$(CC) -c jugador.c -pthread

clean: 
	rm -rf *o bolera jugadores *~
