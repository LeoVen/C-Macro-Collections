CC=gcc
CFLAGS=-Wall -Wextra -Werror
INCLUDE=-I ../src

example: example.c
	$(CC) $< -o $@.exe $(CFLAGS) $(INCLUDE)

run: example
	./example.exe
