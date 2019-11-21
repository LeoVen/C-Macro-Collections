#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cmc/list.h"
#include "cmc/stack.h"
#include "cmc/queue.h"
#include "cmc/deque.h"


CMC_GENERATE_LIST_HEADER(il, int_list, int)
CMC_GENERATE_STACK_HEADER(cs, char_stack, char)
CMC_GENERATE_QUEUE_HEADER(queue, index_queue, size_t)
CMC_GENERATE_DEQUE_HEADER(dq, deque, int)
