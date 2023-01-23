/**
 * benchmark.c
 *
 * Creation Date: 13/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * Benchmarks:
 * - Input
 * - Search
 * - Iteration
 * - Output
 */
#include <assert.h>
#include <inttypes.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "util/twister.c"

#include "cmc/cor/hashtable.h"
#include "cmc/cor/heap.h"
#include "cmc/utl/foreach.h"
#include "cmc/utl/futils.h"
#include "cmc/utl/timer.h"

#define NTOTAL UINT64_C(10000000)
#define NMIN UINT64_C(100)

// Percentage % macro
#define PC(x) x * 100.0

size_t randsize(size_t min, size_t max, mt_state_ptr st)
{
    return (size_t)floor(twist_real(st) * ((double)max - (double)min + 1.0) + (double)min);
}

void shuffle(int *array, mt_state_ptr st)
{
    for (size_t i = 0; i < NTOTAL; i++)
    {
        size_t j = randsize(0, NTOTAL - 1, st);
        int tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
}

struct
{
    CMC_DEF_FTAB_CMP(size_t);
    CMC_DEF_FTAB_CPY(size_t);
    CMC_DEF_FTAB_STR(size_t);
    CMC_DEF_FTAB_FREE(size_t);
    CMC_DEF_FTAB_HASH(size_t);
    CMC_DEF_FTAB_PRI(size_t);
} int_ftab = { cmc_size_cmp, NULL, cmc_size_str, NULL, cmc_size_hash, cmc_size_cmp };

#define CMC_ARGS_FALLTHROUGH
#define V int
#define K int
#define CMC_EXT_ITER

#define SNAME bitset
#define PFX bs
#include "cmc/bitset.h"
#define SNAME deque
#define PFX d
#include "cmc/deque.h"
#define SNAME hashbidimap
#define PFX hbm
#include "cmc/hashbidimap.h"
#define SNAME hashmap
#define PFX hm
#include "cmc/hashmap.h"
#define SNAME hashmultimap
#define PFX hmm
#include "cmc/hashmultimap.h"
#define SNAME hashmultiset
#define PFX hms
#include "cmc/hashmultiset.h"
#define SNAME hashset
#define PFX hs
#include "cmc/hashset.h"
#define SNAME heap
#define PFX h
#include "cmc/heap.h"
#define SNAME intervalheap
#define PFX ih
#include "cmc/intervalheap.h"
#define SNAME linkedlist
#define PFX ll
#include "cmc/linkedlist.h"
#define SNAME list
#define PFX l
#include "cmc/list.h"
#define SNAME queue
#define PFX q
#include "cmc/queue.h"
#define SNAME sortedlist
#define PFX sl
#include "cmc/sortedlist.h"
#define SNAME stack
#define PFX s
#include "cmc/stack.h"
#define SNAME treemap
#define PFX tm
#include "cmc/treemap.h"
#define SNAME treeset
#define PFX ts
#include "cmc/treeset.h"

#undef CMC_ARGS_FALLTHROUGH
#include "cmc/cor/undef.h"

#define BENCHMARK(NAME, PFX, SNAME, initfunc, insertbody, removebody, searchbody) \
    void NAME##_io_benchmark(int *array, int *sarray, size_t s) \
    { \
        printf("+------------------------------------------------------------ %10s\n", #NAME); \
        printf("+------------------------------------------------------------ \n"); \
        struct cmc_timer timer, total; \
        CMC_UNUSED int r; \
\
        struct SNAME *coll = initfunc; \
\
        cmc_timer_start(total); \
        cmc_timer_start(timer); \
\
        for (size_t i = 0; i < NTOTAL; i++) \
        { \
            insertbody; \
        } \
\
        cmc_timer_stop(timer); \
\
        size_t s1 = PFX##_count(coll); \
\
        printf("    INPUT %10s TOOK %8.0lf milliseconds for %8" PRIuMAX " elements\n", #NAME, timer.result, s1); \
\
        double input_time = timer.result; \
\
        cmc_timer_start(timer); \
\
        for (size_t i = 0; i < s; i++) \
        { \
            searchbody; \
        } \
\
        cmc_timer_stop(timer); \
\
        printf("   SEARCH %10s TOOK %8.0lf milliseconds for %8" PRIuMAX " elements\n", #NAME, timer.result, s); \
\
        double search_time = timer.result; \
\
        cmc_timer_start(timer); \
\
        CMC_FOREACH (PFX, SNAME, iter, coll) \
        { \
        } \
\
        cmc_timer_stop(timer); \
\
        printf("ITERATION %10s TOOK %8.0lf milliseconds for %8" PRIuMAX " elements\n", #NAME, timer.result, s1); \
\
        double iter_time = timer.result; \
\
        cmc_timer_start(timer); \
\
        for (size_t i = 0; i < NTOTAL; i++) \
        { \
            removebody; \
        } \
\
        cmc_timer_stop(timer); \
\
        size_t s2 = PFX##_count(coll); \
\
        printf("   OUTPUT %10s TOOK %8.0lf milliseconds for %8" PRIuMAX " elements\n", #NAME, timer.result, s1); \
\
        double output_time = timer.result; \
\
        cmc_timer_stop(total); \
\
        PFX##_free(coll); \
\
        double adj_search_time = (total.result - search_time) + (search_time * (double)NTOTAL) / (double)s; \
        double no_search = input_time + iter_time + output_time; \
\
        printf("+------------------------------------------------------------ \n"); \
        printf("TOTAL TIME    : %.0lf milliseconds\n", total.result); \
        printf("ADJUSTED TIME : %.0lf milliseconds\n", adj_search_time); \
\
        printf("+------------------------------------------------------------ \n"); \
        printf("    INPUT : %5.2lf%% (%5.2f%%)\n", PC(input_time / total.result), PC(input_time / no_search)); \
        printf("   SEARCH : %5.2lf%%\n", PC(search_time / total.result)); \
        printf("ITERATION : %5.2lf%% (%5.2f%%)\n", PC(iter_time / total.result), PC(iter_time / no_search)); \
        printf("   OUTPUT : %5.2lf%% (%5.2f%%)\n", PC(output_time / total.result), PC(output_time / no_search)); \
        printf("+------------------------------------------------------------ \n"); \
\
        assert(s1 == NTOTAL && s2 == 0); \
\
        printf("+------------------------------------------------------------ %10s\n\n", #NAME); \
    }

BENCHMARK(DEQUE, d, deque, d_new(NTOTAL, (struct deque_fval *)&int_ftab), d_push_back(coll, array[i]), d_pop_back(coll),
          d_contains(coll, sarray[i]))
BENCHMARK(HASHMAP, hm, hashmap, hm_new(NTOTAL, 0.6, (struct hashmap_fkey *)&int_ftab, (struct hashmap_fval *)&int_ftab),
          hm_insert(coll, array[i], array[i]), hm_remove(coll, array[i], &r), hm_contains(coll, sarray[i]))
BENCHMARK(HASHSET, hs, hashset, hs_new(NTOTAL, 0.6, (struct hashset_fval *)&int_ftab), hs_insert(coll, array[i]),
          hs_remove(coll, array[i]), hs_contains(coll, sarray[i]))
BENCHMARK(HEAP, h, heap, h_new(NTOTAL, CMC_MIN_HEAP, (struct heap_fval *)&int_ftab), h_insert(coll, array[i]),
          h_remove(coll), h_contains(coll, sarray[i]))
BENCHMARK(LINKEDLIST, ll, linkedlist, ll_new((struct linkedlist_fval *)&int_ftab), ll_push_back(coll, array[i]),
          ll_pop_back(coll), ll_contains(coll, sarray[i]))
BENCHMARK(LIST, l, list, l_new(NTOTAL, (struct list_fval *)&int_ftab), l_push_back(coll, array[i]), l_pop_back(coll),
          l_contains(coll, sarray[i]))
BENCHMARK(QUEUE, q, queue, q_new(NTOTAL, (struct queue_fval *)&int_ftab), q_enqueue(coll, array[i]), q_dequeue(coll),
          q_contains(coll, sarray[i]))
BENCHMARK(STACK, s, stack, s_new(NTOTAL, (struct stack_fval *)&int_ftab), s_push(coll, array[i]), s_pop(coll),
          s_contains(coll, sarray[i]))
BENCHMARK(TREEMAP, tm, treemap, tm_new((struct treemap_fkey *)&int_ftab, (struct treemap_fval *)&int_ftab),
          tm_insert(coll, array[i], array[i]), tm_remove(coll, array[i], &r), tm_contains(coll, sarray[i]))
BENCHMARK(TREESET, ts, treeset, ts_new((struct treeset_fval *)&int_ftab), ts_insert(coll, array[i]),
          ts_remove(coll, array[i]), ts_contains(coll, sarray[i]))

BENCHMARK(INTERVALHEAP, ih, intervalheap, ih_new(NTOTAL, (struct intervalheap_fval *)&int_ftab),
          ih_insert(coll, array[i]), ih_remove_max(coll), ih_contains(coll, sarray[i]))
BENCHMARK(MULTIMAP, hmm, hashmultimap,
          hmm_new(NTOTAL, 0.8, (struct hashmultimap_fkey *)&int_ftab, (struct hashmultimap_fval *)&int_ftab),
          hmm_insert(coll, array[i], array[i]), hmm_remove(coll, array[i], &r), hmm_contains(coll, sarray[i]))
BENCHMARK(MULTISET, hms, hashmultiset, hms_new(NTOTAL, 0.6, (struct hashmultiset_fval *)&int_ftab),
          hms_insert(coll, array[i]), hms_remove(coll, array[i]), hms_contains(coll, sarray[i]))

int main(void)
{
    // Using the Mersenne Twister to generate random numbers. This seed
    // guarantees that everytime this program runs it will generate the same
    // random numbers. The random numbers influence on the shuffling of the
    // array.
    mt_state st;
    twist_init(&st, UINT64_C(12604951237));

    int *array = malloc(NTOTAL * sizeof(int));
    int *sarray = malloc(NTOTAL * sizeof(int));

    if (!array)
    {
        printf("Allocation failed\n");
        return 1;
    }

    for (int i = -5000000; i < 5000000; i++)
        array[i + 5000000] = i;

    shuffle(array, &st);

    memcpy(sarray, array, NTOTAL * sizeof(int));

    shuffle(sarray, &st);

    printf("\nNote: Adjusted Time refers to if the container had done a search over all %" PRIuMAX " elements.\n\n",
           NTOTAL);

    // Linear containers have to search for NMIN elements and non-linear (hash
    // and tree) have to search for all elements.
    struct cmc_timer timer;
    cmc_timer_start(timer);

    DEQUE_io_benchmark(array, sarray, NMIN);
    HASHMAP_io_benchmark(array, sarray, NTOTAL);
    HASHSET_io_benchmark(array, sarray, NTOTAL);
    HEAP_io_benchmark(array, sarray, NMIN);
    LINKEDLIST_io_benchmark(array, sarray, NMIN);
    LIST_io_benchmark(array, sarray, NMIN);
    QUEUE_io_benchmark(array, sarray, NMIN);
    STACK_io_benchmark(array, sarray, NMIN);
    TREEMAP_io_benchmark(array, sarray, NTOTAL);
    TREESET_io_benchmark(array, sarray, NTOTAL);

    //     INTERVALHEAP_io_benchmark(array, array, NMIN);
    //     MULTIMAP_io_benchmark(array, array, NTOTAL);
    //     MULTISET_io_benchmark(array, array, NTOTAL);

    cmc_timer_stop(timer);

    printf("+-----------------------------------------------------------+ \n");
    printf("| Total Benchmark Running Time : %12.0lf seconds       | \n", timer.result / 1000);
    printf("+-----------------------------------------------------------+ \n");

    free(array);
    free(sarray);

    return 0;
}
