CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
CFLAGS += -Wno-unused -Wno-missing-braces
DBFLAGS = -g3
CVFLAGS = --coverage -O0
INCLUDE = ../../src/

main: FORCE
	$(CC) main.c -o main.exe $(CFLAGS) $(CVFLAGS) -I $(INCLUDE) -DCMC_TEST_COLOR
	./main.exe
	gcov ./main.c

dev: FORCE
	$(CC) main.c -o main.exe $(CFLAGS) $(CVFLAGS) -I $(INCLUDE) -DCMC_TEST_COLOR
	./main.exe
	gcov ./main.c -n
	rm *.gcda *.gcno *.exe

windev: FORCE
	$(CC) main.c -o main.exe $(CFLAGS) $(CVFLAGS) -I $(INCLUDE) -DCMC_TEST_COLOR
	./main.exe
	gcov ./main.c -n
	del *.gcda *.gcno *.exe

nocov: FORCE
	$(CC) main.c -o main.exe $(CFLAGS) -I $(INCLUDE) -DCMC_TEST_COLOR
	./main.exe
	rm *.exe

bare: FORCE
	$(CC) main.c -o main.exe $(CFLAGS) -I $(INCLUDE)
	./main.exe
	rm *.exe

debug: FORCE
	$(CC) main.c -o main.exe $(CFLAGS) -I $(INCLUDE) -DCMC_TEST_COLOR $(DBFLAGS)

valgrind: debug
	valgrind --leak-check=full ./main.exe

FORCE:
