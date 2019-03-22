#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "macro_containers.h"

CONTAINER_GENERATE_HEADER(LIST, PUBLIC, il, int_list, , , int)
CONTAINER_GENERATE_HEADER(STACK, PRIVATE, cs, char_stack, , , char)
CONTAINER_GENERATE_HEADER(QUEUE, PUBLIC, queue, index_queue, , , size_t)
CONTAINER_GENERATE_HEADER(DEQUE, PUBLIC, dq, deque, , , int)