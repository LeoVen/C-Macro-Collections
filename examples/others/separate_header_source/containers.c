#include "containers.h"

COLLECTION_GENERATE_SOURCE(LIST, PUBLIC, il, int_list, , , int)
COLLECTION_GENERATE_SOURCE(STACK, PRIVATE, cs, char_stack, , , char)
COLLECTION_GENERATE_SOURCE(QUEUE, PUBLIC, queue, index_queue, , , size_t)
COLLECTION_GENERATE_SOURCE(DEQUE, PUBLIC, dq, deque, , , int)