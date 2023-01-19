CC=gcc
CFLAGS=-Wall -Wextra -Werror -fprofile-arcs -ftest-coverage -g -O0
INCLUDE=-I ../src

main:
	mkdir -p ./out
	$(CC) ./main.c $(INCLUDE) $(CFLAGS) -o ./out/$@.exe
	valgrind --leak-check=yes ./out/$@.exe
	gcov -p ./out/$@.gcda

dev: main
	mkdir -p ./coverage
	mv *.gcov ./coverage

clean:
	rm ./*.gcov ./*.gcda ./*.gcno ./*.exe -f
	rm ./coverage ./out -rf
