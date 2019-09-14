CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
CFLAGS += -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-label
CVFLAGS = --coverage -O0
INCLUDE = ../../src/

main: FORCE
	$(CC) main.c -o main $(CFLAGS) $(CVFLAGS) -I $(INCLUDE)
	./main
	gcov ./main.c

dev: FORCE
	$(CC) main.c -o main $(CFLAGS) $(CVFLAGS) -I $(INCLUDE)
	./main
	gcov ./main.c -n
	rm *.gcda *.gcno *.exe

windev: FORCE
	$(CC) main.c -o main $(CFLAGS) $(CVFLAGS) -I $(INCLUDE)
	./main
	gcov ./main.c -n
	del *.gcda *.gcno *.exe

FORCE:
