#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "macro_collections.h"

COLLECTION_GENERATE_HEADER(LIST, PUBLIC, il, int_list, , , int)
COLLECTION_GENERATE_HEADER(STACK, PRIVATE, cs, char_stack, , , char)
COLLECTION_GENERATE_HEADER(QUEUE, PUBLIC, queue, index_queue, , , size_t)
COLLECTION_GENERATE_HEADER(DEQUE, PUBLIC, dq, deque, , , int)