CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
CFLAGS += -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unused-label

main: FORCE
	$(CC) foreach.c -o foreach.exe $(CFLAGS)
	./foreach.exe
	$(CC) iterators.c -o iterators.exe $(CFLAGS)
	./iterators.exe
	$(CC) contains.c -o contains.exe $(CFLAGS)
	./contains.exe
	$(CC) types.c -o types.exe $(CFLAGS)
	./types.exe
	$(CC) -c ./sep/sep.c -o ./sep/sep.o $(CFLAGS)
	$(CC) -c ./sep/main.c -o ./sep/main.o $(CFLAGS)
	$(CC) ./sep/sep.o ./sep/main.o -o ./sep/sep.exe
	./sep/sep.exe

dev: main
	rm *.exe ./sep/*.o ./sep/*.exe

windev: main
	del *.exe .\\sep\\*.o .\\sep\\*.exe

FORCE:
