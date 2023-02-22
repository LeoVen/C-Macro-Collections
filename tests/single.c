#include <inttypes.h>
#include <stdio.h>

#include "cmc/utl/log.h"

#define CMC_ARGS_FALLTHROUGH

#define CMC_EXT_INIT
#define CMC_EXT_ITER
#define CMC_EXT_NODE
#define CMC_EXT_SEQ
#define CMC_EXT_SETF
#define CMC_EXT_STR

#include "unt_bitset.h"
#include "unt_deque.h"
#include "unt_hashbidimap.h"
#include "unt_hashmap.h"
#include "unt_hashmultimap.h"
#include "unt_hashmultiset.h"
#include "unt_hashset.h"
#include "unt_heap.h"
#include "unt_intervalheap.h"
#include "unt_linkedlist.h"
#include "unt_list.h"
#include "unt_queue.h"
#include "unt_sortedlist.h"
#include "unt_stack.h"
#include "unt_treemap.h"
#include "unt_treeset.h"

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
    cmc_log_config.tlevel = CMC_LOG_TRACE;

    struct cmc_timer timer;

    cmc_timer_start(timer);
    uintmax_t tests = 0, units = 0;

    cmc_run(CMC_(CMC, TEST), units, tests);
    cmc_run(CMC_(CMC, CMC_(TEST, Iter)), units, tests);

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
