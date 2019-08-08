#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "macro_collections.h"

COLLECTION_GENERATE_HEADER(LIST, il, int_list, , , int)
COLLECTION_GENERATE_HEADER(STACK, cs, char_stack, , , char)
COLLECTION_GENERATE_HEADER(QUEUE, queue, index_queue, , , size_t)
COLLECTION_GENERATE_HEADER(DEQUE, dq, deque, , , int)
