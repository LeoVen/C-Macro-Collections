#include <utl/timer.h>

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

#include "unt/intervalheap.c"
#include "unt/multimap.c"
#include "unt/multiset.c"
#include "unt/sortedlist.c"

int main(void)
{
    cmc_timer timer;

    cmc_timer_start(timer);
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
    failed += intervalheap_test();
    failed += multimap_test();
    failed += multiset_test();
    failed += sortedlist_test();

    cmc_timer_stop(timer);
    cmc_timer_calc(timer);

    printf("+--------------------------------------------------+\n");
    printf("| Total running time : %14.0lf milliseconds |\n", timer.result);
    printf("+--------------------------------------------------+\n");

    return failed;
}
