CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
CFLAGS += -Wno-unused-function -Wno-unused-paramter -Wno-unused-variable -Wno-unused-label

main:
	$(CC) foreach.c -o foreach $(CFLAGS)
	./foreach
	$(CC) iterators.c -o iterators $(CFLAGS)
	./iterators
	$(CC) contains.c -o contains $(CFLAGS)
	./contains
	$(CC) types.c -o types $(CFLAGS)
	./types
	$(CC) -c ./sep/sep.c -o ./sep/sep.o $(CFLAGS)
	$(CC) -c ./sep/main.c -o ./sep/main.o $(CFLAGS)
	$(CC) ./sep/sep.o ./sep/main.o -o ./sep/sep
	./sep/sep
