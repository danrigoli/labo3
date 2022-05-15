CC 	= gcc -std=gnu89 -pedantic-errors -pthread

all : bingo 

bingo : bingo.c thread.c
	$(CC) -o bingo bingo.c thread.c

clean: rm -rf *o bingo