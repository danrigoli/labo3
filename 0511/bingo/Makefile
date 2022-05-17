CC = gcc-6 -std=gnu89 -pedantic-errors

all : bingo jugadores

bingo: bingo.c funciones.h definiciones.h bolillero.h funciones.o bolillero.o
	$(CC) -o bingo bingo.c bolillero.o funciones.o -pthread

jugadores : jugadores.c funciones.o definiciones.h jugador.h jugador.o  
	$(CC) -o jugadores jugadores.c jugador.o funciones.o -pthread

funciones.o : funciones.c funciones.h
	$(CC) -c funciones.c

jugador.o : jugador.c jugador.h 
	$(CC) -c jugador.c -pthread

bolillero.o: bolillero.c bolillero.h
	$(CC) -c bolillero.c -pthread

clean: 
	rm -rf *o bingo jugadores *~
