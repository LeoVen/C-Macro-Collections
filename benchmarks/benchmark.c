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
#include "macro_collections.h"
#include "utl/timer.h"
#include "util/twister.c"
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include <inttypes.h>

#define NTOTAL UINT64_C(10000000)
#define NMIN UINT64_C(100)

// Percentage % macro
#define PC(x) x * 100.0

static int intcmp(int a, int b)
{
    return a - b;
}

static size_t inthash(int t)
{
    size_t a = t;
    a += ~(a << 15);
    a ^= (a >> 10);
    a += (a << 3);
    a ^= (a >> 6);
    a += ~(a << 11);
    a ^= (a >> 16);
    return a;
}

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

COLLECTION_GENERATE(LIST, l, list, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(STACK, s, stack, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(QUEUE, q, queue, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(DEQUE, d, deque, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(LINKEDLIST, ll, linked, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(HEAP, h, heap, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(TREESET, ts, tset, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(TREEMAP, tm, tmap, /* FMOD */, int, int)
COLLECTION_GENERATE(HASHSET, hs, hset, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(HASHMAP, hm, hmap, /* FMOD */, int, int)

#define BENCHMARK(NAME, PFX, sname, initfunc, insertbody, removebody, searchbody)                                \
    void NAME##_io_benchmark(int *array, int *sarray, size_t s)                                                  \
    {                                                                                                            \
        printf("+------------------------------------------------------------ %10s\n", #NAME);                   \
        printf("+------------------------------------------------------------ \n");                              \
        timer_t timer, total;                                                                                    \
        int r, k, v;                                                                                             \
        size_t j;                                                                                                \
                                                                                                                 \
        sname *coll = initfunc;                                                                                  \
                                                                                                                 \
        TIMER_START(total)                                                                                       \
        TIMER_START(timer)                                                                                       \
                                                                                                                 \
        for (size_t i = 0; i < NTOTAL; i++)                                                                      \
        {                                                                                                        \
            insertbody;                                                                                          \
        }                                                                                                        \
                                                                                                                 \
        TIMER_STOP(timer)                                                                                        \
                                                                                                                 \
        TIMER_CALC(timer)                                                                                        \
                                                                                                                 \
        size_t s1 = PFX##_count(coll);                                                                           \
                                                                                                                 \
        printf("    INPUT %10s TOOK %8.0lf milliseconds for %8" PRIuMAX " elements\n", #NAME, timer.result, s1); \
                                                                                                                 \
        double input_time = timer.result;                                                                        \
                                                                                                                 \
        TIMER_START(timer)                                                                                       \
                                                                                                                 \
        for (size_t i = 0; i < s; i++)                                                                           \
        {                                                                                                        \
            searchbody;                                                                                          \
        }                                                                                                        \
                                                                                                                 \
        TIMER_STOP(timer)                                                                                        \
                                                                                                                 \
        TIMER_CALC(timer)                                                                                        \
                                                                                                                 \
        printf("   SEARCH %10s TOOK %8.0lf milliseconds for %8" PRIuMAX " elements\n", #NAME, timer.result, s);  \
                                                                                                                 \
        double search_time = timer.result;                                                                       \
                                                                                                                 \
        sname##_iter iter;                                                                                       \
                                                                                                                 \
        TIMER_START(timer)                                                                                       \
                                                                                                                 \
        for (PFX##_iter_init(&iter, coll); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))                       \
        {                                                                                                        \
        }                                                                                                        \
                                                                                                                 \
        TIMER_STOP(timer)                                                                                        \
                                                                                                                 \
        TIMER_CALC(timer)                                                                                        \
                                                                                                                 \
        printf("ITERATION %10s TOOK %8.0lf milliseconds for %8" PRIuMAX " elements\n", #NAME, timer.result, s1); \
                                                                                                                 \
        double iter_time = timer.result;                                                                         \
                                                                                                                 \
        TIMER_START(timer);                                                                                      \
                                                                                                                 \
        for (size_t i = 0; i < NTOTAL; i++)                                                                      \
        {                                                                                                        \
            removebody;                                                                                          \
        }                                                                                                        \
                                                                                                                 \
        TIMER_STOP(timer)                                                                                        \
                                                                                                                 \
        TIMER_CALC(timer)                                                                                        \
                                                                                                                 \
        size_t s2 = PFX##_count(coll);                                                                           \
                                                                                                                 \
        printf("   OUTPUT %10s TOOK %8.0lf milliseconds for %8" PRIuMAX " elements\n", #NAME, timer.result, s1); \
                                                                                                                 \
        double output_time = timer.result;                                                                       \
                                                                                                                 \
        TIMER_STOP(total)                                                                                        \
        TIMER_CALC(total)                                                                                        \
                                                                                                                 \
        PFX##_free(coll);                                                                                        \
                                                                                                                 \
        double adj_search_time = (total.result - search_time) + (search_time * (double)NTOTAL) / (double)s;      \
        double no_search = input_time + iter_time + output_time;                                                 \
                                                                                                                 \
        printf("+------------------------------------------------------------ \n");                              \
        printf("TOTAL TIME    : %.0lf milliseconds\n", total.result);                                            \
        printf("ADJUSTED TIME : %.0lf milliseconds\n", adj_search_time);                                         \
                                                                                                                 \
        printf("+------------------------------------------------------------ \n");                              \
        printf("    INPUT : %5.2lf%% (%5.2f%%)\n", PC(input_time / total.result), PC(input_time / no_search));   \
        printf("   SEARCH : %5.2lf%%\n", PC(search_time / total.result));                                        \
        printf("ITERATION : %5.2lf%% (%5.2f%%)\n", PC(iter_time / total.result), PC(iter_time / no_search));     \
        printf("   OUTPUT : %5.2lf%% (%5.2f%%)\n", PC(output_time / total.result), PC(output_time / no_search)); \
        printf("+------------------------------------------------------------ \n");                              \
                                                                                                                 \
        assert(s1 == NTOTAL && s2 == 0);                                                                         \
                                                                                                                 \
        printf("+------------------------------------------------------------ %10s\n\n", #NAME);                 \
    }

BENCHMARK(DEQUE, d, deque, d_new(NTOTAL), d_push_back(coll, array[i]), d_pop_back(coll), d_contains(coll, sarray[i], intcmp))
BENCHMARK(HASHMAP, hm, hmap, hm_new(NTOTAL, 0.8, intcmp, inthash), hm_insert(coll, array[i], array[i]), hm_remove(coll, array[i], &r), hm_contains(coll, sarray[i]))
BENCHMARK(HASHSET, hs, hset, hs_new(NTOTAL, 0.8, intcmp, inthash), hs_insert(coll, array[i]), hs_remove(coll, array[i]), hs_contains(coll, sarray[i]))
BENCHMARK(HEAP, h, heap, h_new(NTOTAL, MinHeap, intcmp), h_insert(coll, array[i]), h_remove(coll, &r), h_contains(coll, sarray[i]))
BENCHMARK(LINKEDLIST, ll, linked, ll_new(), ll_push_back(coll, array[i]), ll_pop_back(coll), ll_contains(coll, sarray[i], intcmp))
BENCHMARK(LIST, l, list, l_new(NTOTAL), l_push_back(coll, array[i]), l_pop_back(coll), l_contains(coll, sarray[i], intcmp))
BENCHMARK(QUEUE, q, queue, q_new(NTOTAL), q_enqueue(coll, array[i]), q_dequeue(coll), q_contains(coll, sarray[i], intcmp))
BENCHMARK(STACK, s, stack, s_new(NTOTAL), s_push(coll, array[i]), s_pop(coll), s_contains(coll, sarray[i], intcmp))
BENCHMARK(TREEMAP, tm, tmap, tm_new(intcmp), tm_insert(coll, array[i], array[i]), tm_remove(coll, array[i], &r), tm_contains(coll, sarray[i]))
BENCHMARK(TREESET, ts, tset, ts_new(intcmp), ts_insert(coll, array[i]), ts_remove(coll, array[i]), ts_contains(coll, sarray[i]))

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

    printf("\nNote: Adjusted Time refers to if the container had done a search over all %" PRIuMAX " elements.\n\n", NTOTAL);

    // Linear containers have to search for NMIN elements and non-linear (hash
    // and tree) have to search for all elements.
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

    free(array);
    free(sarray);

    return 0;
}
