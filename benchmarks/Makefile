CC=gcc
INCLUDE=-I ../src
CFLAGS=-Wall -Wextra -Werror

benchmark: benchmark.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o ./$@.exe -lm
	./$@.exe
