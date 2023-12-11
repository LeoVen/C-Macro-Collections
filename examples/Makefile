CC=gcc
CFLAGS=-Wall -Wextra -Werror -O3
INCLUDE=-I ..

ifndef FILE
main:
	@echo Please define a FILE parameter to run. Examples:
	@echo "  dfs"
	@echo "  example"
	@echo "  hanoi"
	@echo "  heapsort"
	@echo "  ice_cream"
	@echo "  sieve"
else
$(FILE): $(FILE).c
	$(CC) $< -o $@.exe $(CFLAGS) $(INCLUDE)
	./$@.exe
endif


clean:
	rm *.exe -f
	rm *.txt -f


memcheck:
ifndef FILE
	$(error FILE parameter must be set first to check for leaks)
else
	$(CC) $(FILE).c -o mem.exe $(CFLAGS) $(INCLUDE) -g
	valgrind --leak-check=yes ./mem.exe
endif
