CFLAGS= -Wno-unused-function -Wno-unused-paramter -Wno-unused-variable -std=c11 -Wall -Wextra

main:
	gcc foreach.c -o foreach $(CFLAGS)
	./foreach
	gcc iterators.c -o iterators $(CFLAGS)
	./iterators
	gcc contains.c -o contains $(CFLAGS)
	./contains
	gcc types.c -o types $(CFLAGS)
	./types
	gcc -c ./sep/sep.c -o ./sep/sep.o $(CFLAGS)
	gcc -c ./sep/main.c -o ./sep/main.o $(CFLAGS)
	gcc ./sep/sep.o ./sep/main.o -o ./sep/sep
	./sep/sep
