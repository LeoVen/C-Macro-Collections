#include "containers.h"

CONTAINER_GENERATE_SOURCE(LIST, PUBLIC, il, int_list, , , int)
CONTAINER_GENERATE_SOURCE(STACK, PRIVATE, cs, char_stack, , , char)
CONTAINER_GENERATE_SOURCE(QUEUE, PUBLIC, queue, index_queue, , , size_t)
CONTAINER_GENERATE_SOURCE(DEQUE, PUBLIC, dq, deque, , , int)