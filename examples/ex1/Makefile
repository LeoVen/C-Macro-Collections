CC = gcc
CFLAGS = -Wall -Wextra
INCLUDE = ./
SRC = ../../src

main.exe: containers.o main.o $(SRC)/macro_containers.h
	$(CC) $^ -o $@

%.o: %.c $(SRC)/macro_containers.h
	$(CC) -I $(INCLUDE) -I $(SRC) $(CFLAGS) -c $< -o $@

run: main.exe $(SRC)/macro_containers.h
	./main.exe