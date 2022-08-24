CC = gcc-6 -std=gnu89 -pedantic-errors

all : hipodromo main

hipodromo: hipodromo.c funciones.h definiciones.h funciones.o
	$(CC) -o hipodromo hipodromo.c funciones.o

main : main.c funciones.o definiciones.h caballo.h caballo.o  
	$(CC) -o main main.c caballo.o funciones.o -pthread

funciones.o : funciones.c funciones.h
	$(CC) -c funciones.c

caballo.o : caballo.c caballo.h 
	$(CC) -c caballo.c -pthread

clean: 
	rm -rf *o hipodromo main *~
