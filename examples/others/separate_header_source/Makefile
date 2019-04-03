CC = gcc
CFLAGS = -Wall -Wextra
INCLUDE = ./
SRC = ../../../src

main.exe: containers.o main.o $(SRC)/macro_collections.h
	$(CC) $^ -o $@

%.o: %.c $(SRC)/macro_collections.h
	$(CC) -I $(INCLUDE) -I $(SRC) $(CFLAGS) -c $< -o $@

run: main.exe $(SRC)/macro_collections.h
	./main.exe