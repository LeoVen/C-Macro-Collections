#include <inttypes.h>
#include <stdio.h>

#include "unt/cmc/bitset.h"
#include "unt/cmc/deque.h"
#include "unt/cmc/hashbidimap.h"
#include "unt/cmc/hashmap.h"
#include "unt/cmc/hashmultimap.h"
#include "unt/cmc/hashmultiset.h"
#include "unt/cmc/hashset.h"
#include "unt/cmc/heap.h"
#include "unt/cmc/intervalheap.h"
#include "unt/cmc/linkedlist.h"
#include "unt/cmc/list.h"
#include "unt/cmc/queue.h"
#include "unt/cmc/sortedlist.h"
#include "unt/cmc/stack.h"
#include "unt/cmc/treemap.h"
#include "unt/cmc/treeset.h"

#include "unt/utl/foreach.h"

#include "utl/assert.h"
#include "utl/timer.h"

#include "./unt/utl.h"

#define cmc_run(unit, unit_fails, test_fails) \
    do \
    { \
        uintmax_t f = unit(); \
        if (f != 0) \
            unit_fails += 1; \
        test_fails += f; \
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

    cmc_run(CMCBitSet, units, tests);
    cmc_run(CMCBitSetIter, units, tests);
    cmc_run(CMCDeque, units, tests);
    cmc_run(CMCDequeIter, units, tests);
    cmc_run(CMCHashBidiMap, units, tests);
    cmc_run(CMCHashBidiMapIter, units, tests);
    cmc_run(CMCHashMap, units, tests);
    cmc_run(CMCHashMapIter, units, tests);
    cmc_run(CMCHashMultiMap, units, tests);
    cmc_run(CMCHashMultiMapIter, units, tests);
    cmc_run(CMCHashMultiSet, units, tests);
    cmc_run(CMCHashMultiSetIter, units, tests);
    cmc_run(CMCHashSet, units, tests);
    cmc_run(CMCHashSetIter, units, tests);
    cmc_run(CMCHeap, units, tests);
    cmc_run(CMCHeapIter, units, tests);
    cmc_run(CMCIntervalHeap, units, tests);
    cmc_run(CMCIntervalHeapIter, units, tests);
    cmc_run(CMCLinkedList, units, tests);
    cmc_run(CMCLinkedListIter, units, tests);
    cmc_run(CMCList, units, tests);
    cmc_run(CMCListIter, units, tests);
    cmc_run(CMCQueue, units, tests);
    cmc_run(CMCQueueIter, units, tests);
    cmc_run(CMCSortedList, units, tests);
    cmc_run(CMCSortedListIter, units, tests);
    cmc_run(CMCStack, units, tests);
    cmc_run(CMCStackIter, units, tests);
    cmc_run(CMCTreeMap, units, tests);
    cmc_run(CMCTreeMapIter, units, tests);
    cmc_run(CMCTreeSet, units, tests);
    cmc_run(CMCTreeSetIter, units, tests);

    cmc_run(ForEach, units, tests);

    cmc_timer_stop(timer);

    printf(" +---------------------------------------------------------------+\n");
    printf(" |                            SUMMARY                            |\n");
    printf(" +---------------------------------------------------------------+\n");
    printf(" | Total Units Failed   : %25" PRIuMAX " units        |\n", units);
    printf(" | Total Tests Failed   : %25" PRIuMAX " tests        |\n", tests);
    printf(" | Total Asserts Failed : %25" PRIuMAX " asserts      |\n", cmc_assert_failed);
    printf(" | Total Runtime        : %25.0lf milliseconds |\n", timer.result);
    printf(" +---------------------------------------------------------------+\n");

    return tests;
}
