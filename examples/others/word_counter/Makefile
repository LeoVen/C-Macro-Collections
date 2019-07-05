CFLAGS = -Wall -Wextra -g
LIB = pthread
INCLUDE = ../../../src
VFLAGS = --leak-check=full

EXAMPLES = ex1.txt ex2.txt ex3.txt ex4.txt ex5.txt ex6.txt

SOURCES = 	../../../src/cmc/deque.h \
			../../../src/cmc/hashmap.h \
			../../../src/cmc/hashset.h \
			../../../src/cmc/heap.h \
			../../../src/cmc/linkedlist.h \
			../../../src/cmc/list.h \
			../../../src/cmc/queue.h \
			../../../src/cmc/stack.h \
			../../../src/cmc/treemap.h \
			../../../src/cmc/treeset.h \

main: counter.c
	gcc counter.c $(CFLAGS) -I $(INCLUDE) -l$(LIB)

example: main $(EXAMPLES)
	./a.out $(EXAMPLES)

valgrind: main $(EXAMPLES)
	valgrind $(VFLAGS) ./a.out $(EXAMPLES)

source: main
	./a.out $(SOURCES)
