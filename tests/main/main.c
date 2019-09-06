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

#include "unt/multimap.c"
#include "unt/multiset.c"

int main(void)
{
    uintmax_t failed = 0;

    failed += deque_test();
    failed += hashmap_test();
    failed += hashset_test();
    failed += heap_test();
    failed += linkedlist_test();
    failed += list_test();
    failed += queue_test();
    failed += stack_test();
    failed += treemap_test();
    failed += treeset_test();

    /* ext */
    failed += multimap_test();
    failed += multiset_test();

    return failed;
}
