CC=gcc
CFLAGS=-Wall -fprofile-arcs -ftest-coverage -g -O0
INCLUDE=-I ../src

main:
	$(CC) ./main.c $(INCLUDE) $(CFLAGS) -o $@.exe
	valgrind --leak-check=yes ./$@.exe
	gcov -p main.gcda

clean:
	rm ./*.gcov ./*.gcda ./*.gcno ./*.exe -f
