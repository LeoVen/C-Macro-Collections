CC=gcc
CFLAGS=-Wall -Wextra -Werror -O3
INCLUDE=-I ../src

dfs: dfs.c
	$(CC) $< -o $@.exe $(CFLAGS) $(INCLUDE)
	./$@.exe

example: example.c
	$(CC) $< -o $@.exe $(CFLAGS) $(INCLUDE)
	./$@.exe

hanoi: hanoi.c
	$(CC) $< -o $@.exe $(CFLAGS) $(INCLUDE)
	./$@.exe

heapsort: heapsort.c
	$(CC) $< -o $@.exe $(CFLAGS) $(INCLUDE)
	./$@.exe

sieve: sieve.c
	$(CC) $< -o $@.exe $(CFLAGS) $(INCLUDE)
	./$@.exe


clean:
	rm *.exe
	rm *.txt


memcheck:
ifndef FILE
	$(error FILE parameter must be set first to check for leaks)
else
	$(CC) $(FILE) -o mem.exe $(CFLAGS) $(INCLUDE) -g
	valgrind --leak-check=yes ./mem.exe
endif
