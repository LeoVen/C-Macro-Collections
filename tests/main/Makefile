CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
CFLAGS += -Wno-unused
CFLAGS += -DCMC_TEST_COLOR
INCLUDE = ../../src/

main: FORCE
	$(CC) main.c -o main $(CFLAGS) -I $(INCLUDE)
	./main

dev: FORCE
	$(CC) main.c -o main.exe $(CFLAGS) -I $(INCLUDE)
	./main.exe
	rm *.exe

windev: FORCE
	$(CC) main.c -o main $(CFLAGS) -I $(INCLUDE)
	./main
	del *.exe

FORCE:
