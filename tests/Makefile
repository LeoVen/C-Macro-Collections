CC=gcc
CFLAGS=-Wall -fprofile-arcs -ftest-coverage -g -O0
INCLUDE=-I ../src

main:
	$(CC) ./main.c $(INCLUDE) $(CFLAGS) -o $@.exe
	valgrind --leak-check=yes ./$@.exe
	mkdir -p ./coverage
	cd ./coverage && gcov -p ../main.gcda

clean:
	rm ./*.gcda ./*.gcno ./*.exe -f
	rm ./coverage -rf
