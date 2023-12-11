CC=gcc
INCLUDE=-I ..
CFLAGS=-Wall -Wextra -Werror

benchmark: benchmark.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o ./$@.exe -lm
	./$@.exe

clean:
	rm *.exe -f
	rm *.txt -f
