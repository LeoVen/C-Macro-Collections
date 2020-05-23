CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
CFLAGS += -Wno-unused -Wno-missing-braces
DBFLAGS = -g3
CVFLAGS = --coverage -O0
INCLUDE = ../../src/
UNIT = ./unt

main: FORCE
	$(CC) main.c -o main.exe $(CFLAGS) $(CVFLAGS) -I $(INCLUDE) -DCMC_TEST_COLOR
	./main.exe
	gcov ./main.c

nocov: FORCE
	$(CC) main.c -o main.exe $(CFLAGS) -I $(INCLUDE) -DCMC_TEST_COLOR
	./main.exe
	rm *.exe

bare: FORCE
	$(CC) main.c -o main.exe $(CFLAGS) -I $(INCLUDE)
	./main.exe
	rm *.exe

debug: FORCE
	$(CC) main.c -o main.exe $(CFLAGS) -I $(INCLUDE) -DCMC_TEST_COLOR $(DBFLAGS)

valgrind: debug
	valgrind --leak-check=full ./main.exe

all: bitset deque hashbidimap hashmap hashmultimap hashmultiset hashset heap intervalheap linkedlist list queue sortedlist stack treemap treeset foreach
	rm ./main.exe

bitset: $(UNIT)/bitset.c $(INCLUDE)/cmc/bitset.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR -DCMC_BITSET_WORD_TYPE=uint8_t
	./main.exe
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR -DCMC_BITSET_WORD_TYPE=uint16_t
	./main.exe
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR -DCMC_BITSET_WORD_TYPE=uint32_t
	./main.exe
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR -DCMC_BITSET_WORD_TYPE=uint64_t
	./main.exe

deque: $(UNIT)/deque.c $(INCLUDE)/cmc/deque.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

hashbidimap: $(UNIT)/hashbidimap.c $(INCLUDE)/cmc/hashbidimap.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

hashmap: $(UNIT)/hashmap.c $(INCLUDE)/cmc/hashmap.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

hashmultimap: $(UNIT)/hashmultimap.c $(INCLUDE)/cmc/hashmultimap.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

hashmultiset: $(UNIT)/hashmultiset.c $(INCLUDE)/cmc/hashmultiset.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

hashset: $(UNIT)/hashset.c $(INCLUDE)/cmc/hashset.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

heap: $(UNIT)/heap.c $(INCLUDE)/cmc/heap.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

intervalheap: $(UNIT)/intervalheap.c $(INCLUDE)/cmc/intervalheap.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

linkedlist: $(UNIT)/linkedlist.c $(INCLUDE)/cmc/linkedlist.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

list: $(UNIT)/list.c $(INCLUDE)/cmc/list.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

queue: $(UNIT)/queue.c $(INCLUDE)/cmc/queue.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

sortedlist: $(UNIT)/sortedlist.c $(INCLUDE)/cmc/sortedlist.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

stack: $(UNIT)/stack.c $(INCLUDE)/cmc/stack.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

treemap: $(UNIT)/treemap.c $(INCLUDE)/cmc/treemap.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

treeset: $(UNIT)/treeset.c $(INCLUDE)/cmc/treeset.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

foreach: $(UNIT)/foreach.c $(INCLUDE)/utl/foreach.h
	$(CC) $^ -o main.exe -I $(INCLUDE) -DCMC_TEST_MAIN -DCMC_TEST_COLOR
	./main.exe

FORCE:
