#include "unt/deque.c"
#include "unt/hashmap.c"
#include "unt/hashset.c"
#include "unt/heap.c"
#include "unt/linkedlist.c"
#include "unt/list.c"
#include "unt/queue.c"
#include "unt/stack.c"
#include "unt/treemap.c"
#include "unt/treeset.c"

int main(void)
{
    deque_test();
    hashmap_test();
    hashset_test();
    heap_test();
    linkedlist_test();
    list_test();
    queue_test();
    stack_test();
    treemap_test();
    treeset_test();

    return 0;
}
