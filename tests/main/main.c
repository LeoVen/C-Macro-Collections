#include "utl/timer.h"

#include "unt/bitset.c"
#include "unt/deque.c"
#include "unt/hashbidimap.c"
#include "unt/hashmap.c"
#include "unt/hashmultimap.c"
#include "unt/hashmultiset.c"
#include "unt/hashset.c"
#include "unt/heap.c"
#include "unt/intervalheap.c"
#include "unt/linkedlist.c"
#include "unt/list.c"
#include "unt/queue.c"
#include "unt/sortedlist.c"
#include "unt/stack.c"
#include "unt/treemap.c"
#include "unt/treeset.c"

#define cmc_run(unit, unit_fails, test_fails) \
    do                                        \
    {                                         \
        uintmax_t f = unit();                 \
                                              \
        if (f != 0)                           \
            unit_fails += 1;                  \
                                              \
        test_fails += f;                      \
                                              \
    } while (0)

int main(void)
{
    // clang-format off
    printf(" +---------------------------------------------------------------+\n");
    printf(" |                  C Macro Collections Library                  |\n");
    printf(" |                                                               |\n");
    printf(" |                          Test Suite                           |\n");
    printf(" |                                                               |\n");
    printf(" | https://github.com/LeoVen/C-Macro-Collections                 |\n");
    printf(" | Leonardo Vencovsky (leonardo.vencovsky@gmail.com)             |\n");
    printf(" +---------------------------------------------------------------+\n\n");
    // clang-format on

    struct cmc_timer timer;

    cmc_timer_start(timer);
    uintmax_t tests = 0, units = 0;

    cmc_run(BitSet, units, tests);
    cmc_run(BitSetIter, units, tests);
    cmc_run(Deque, units, tests);
    cmc_run(DequeIter, units, tests);
    cmc_run(HashBidiMap, units, tests);
    cmc_run(HashBidiMapIter, units, tests);
    cmc_run(HashMap, units, tests);
    cmc_run(HashMapIter, units, tests);
    cmc_run(HashMultiMap, units, tests);
    cmc_run(HashMultiMapIter, units, tests);
    cmc_run(HashMultiSet, units, tests);
    cmc_run(HashMultiSetIter, units, tests);
    cmc_run(HashSet, units, tests);
    cmc_run(HashSetIter, units, tests);
    cmc_run(Heap, units, tests);
    cmc_run(HeapIter, units, tests);
    cmc_run(IntervalHeap, units, tests);
    cmc_run(IntervalHeapIter, units, tests);
    cmc_run(LinkedList, units, tests);
    cmc_run(LinkedListIter, units, tests);
    cmc_run(List, units, tests);
    cmc_run(ListIter, units, tests);
    cmc_run(Queue, units, tests);
    cmc_run(QueueIter, units, tests);
    cmc_run(SortedList, units, tests);
    cmc_run(SortedListIter, units, tests);
    cmc_run(Stack, units, tests);
    cmc_run(StackIter, units, tests);
    cmc_run(TreeMap, units, tests);
    cmc_run(TreeMapIter, units, tests);
    cmc_run(TreeSet, units, tests);
    cmc_run(TreeSetIter, units, tests);

    cmc_timer_stop(timer);

    printf(
        " +---------------------------------------------------------------+\n");
    printf(
        " |                            SUMMARY                            |\n");
    printf(
        " +---------------------------------------------------------------+\n");
    printf(" | Total Units Failed   : %25" PRIuMAX " units        |\n", units);
    printf(" | Total Tests Failed   : %25" PRIuMAX " tests        |\n", tests);
    printf(" | Total Asserts Failed : %25" PRIuMAX " asserts      |\n",
           cmc_assert_failed);
    printf(" | Total Runtime        : %25.0lf milliseconds |\n", timer.result);
    printf(
        " +---------------------------------------------------------------+\n");

    return tests;
}
