CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
CFLAGS += -Wno-unused -Wno-missing-braces

main: FORCE
	$(CC) foreach.c -o foreach.exe $(CFLAGS)
	./foreach.exe
	$(CC) iterators.c -o iterators.exe $(CFLAGS)
	./iterators.exe
	$(CC) contains.c -o contains.exe $(CFLAGS)
	./contains.exe
	$(CC) types.c -o types.exe $(CFLAGS)
	./types.exe
	make -C ./sep

dev: main
	rm *.exe ./sep/*.o ./sep/*.exe

windev: main
	del *.exe .\\sep\\*.o .\\sep\\*.exe

FORCE:
