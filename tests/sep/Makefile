CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
CFLAGS += -Wno-unused -Wno-missing-braces

main:
	$(CC) -c collections.c -o collections.o $(CFLAGS)
	$(CC) -c main.c -o main.o $(CFLAGS)
	$(CC) collections.o main.o -o sep.exe
	./sep.exe

clean:
	rm *.o

clear:
	rm *.o *.exe
