CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
CFLAGS += -Wno-unused-function -Wno-unused-paramter -Wno-unused-variable -Wno-unused-label
CVFLAGS = --coverage -O0
INCLUDE = ../../src/

main:
	$(CC) main.c -o main $(CFLAGS) $(CVFLAGS) -I $(INCLUDE)
	./main
	gcov ./main.c

dev:
	$(CC) main.c -o main $(CFLAGS) $(CVFLAGS) -I $(INCLUDE)
	./main
	gcov ./main.c -n
	rm *.gcda *.gcno *.exe
