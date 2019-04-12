/**
 * iteration.c
 *
 * Creation Date: 12/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 * Iteration benchmarks.
 */
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "macro_collections.h"
#include "timer.h"

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

COLLECTION_GENERATE(LIST, PUBLIC, l, list, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(STACK, PUBLIC, s, stack, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(QUEUE, PUBLIC, q, queue, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(DEQUE, PUBLIC, d, deque, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(LINKEDLIST, PUBLIC, ll, linked, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(HEAP, PUBLIC, h, heap, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(TREESET, PUBLIC, ts, tset, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(TREEMAP, PUBLIC, tm, tmap, /* FMOD */, int, int)
COLLECTION_GENERATE(HASHSET, PUBLIC, hs, hset, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(HASHMAP, PUBLIC, hm, hmap, /* FMOD */, int, int)

#define ITERATION_BENCHMARK(NAME, PFX, sname, initfunc, insertbody, iternext)                                                \
    void NAME##_iteration_benchmark(void)                                                                                    \
    {                                                                                                                        \
        timer_t timer;                                                                                                       \
                                                                                                                             \
        sname *collection = initfunc;                                                                                        \
                                                                                                                             \
        for (int i = -5000000; i < 5000000; i++)                                                                             \
        {                                                                                                                    \
            insertbody;                                                                                                      \
        }                                                                                                                    \
                                                                                                                             \
        sname##_iter iter;                                                                                                   \
                                                                                                                             \
        size_t i;                                                                                                            \
        int k, v;                                                                                                            \
        int64_t sum = 0;                                                                                                     \
                                                                                                                             \
        TIMER_START(timer)                                                                                                   \
                                                                                                                             \
        for (PFX##_iter_new(&iter, collection); !PFX##_iter_end(&iter);)                                                     \
        {                                                                                                                    \
            iternext;                                                                                                        \
            sum += k;                                                                                                        \
        }                                                                                                                    \
                                                                                                                             \
        TIMER_STOP(timer)                                                                                                    \
                                                                                                                             \
        TIMER_CALC(timer)                                                                                                    \
                                                                                                                             \
        printf("ITERATION %10s TOOK %.2lf milliseconds over %9lu elements\n", #NAME, timer.result, PFX##_count(collection)); \
                                                                                                                             \
        PFX##_free(collection);                                                                                              \
                                                                                                                             \
        assert(sum == -5000000);                                                                                             \
    }

ITERATION_BENCHMARK(DEQUE, d, deque, d_new(10000000), d_push_back(collection, i), d_iter_next(&iter, &k, &i))
ITERATION_BENCHMARK(HASHMAP, hm, hmap, hm_new(10000000, 0.8, intcmp, inthash), hm_insert(collection, i, i), hm_iter_next(&iter, &k, &v, &i))
ITERATION_BENCHMARK(HASHSET, hs, hset, hs_new(10000000, 0.8, intcmp, inthash), hs_insert(collection, i), hs_iter_next(&iter, &k, &i))
ITERATION_BENCHMARK(HEAP, h, heap, h_new(10000000, MaxHeap, intcmp), h_insert(collection, i), h_iter_next(&iter, &k, &i))
ITERATION_BENCHMARK(LINKEDLIST, ll, linked, ll_new(), ll_push_back(collection, i), ll_iter_next(&iter, &k, &i))
ITERATION_BENCHMARK(LIST, l, list, l_new(10000000), l_push_back(collection, i), l_iter_next(&iter, &k, &i))
ITERATION_BENCHMARK(QUEUE, q, queue, q_new(10000000), q_enqueue(collection, i), q_iter_next(&iter, &k, &i))
ITERATION_BENCHMARK(STACK, s, stack, s_new(10000000), s_push(collection, i), s_iter_next(&iter, &k, &i))
ITERATION_BENCHMARK(TREEMAP, tm, tmap, tm_new(intcmp), tm_insert(collection, i, i), tm_iter_next(&iter, &k, &v, &i))
ITERATION_BENCHMARK(TREESET, ts, tset, ts_new(intcmp), ts_insert(collection, i), ts_iter_next(&iter, &k, &i))

int main(int argc, char const *argv[])
{
    DEQUE_iteration_benchmark();
    HASHMAP_iteration_benchmark();
    HASHSET_iteration_benchmark();
    HEAP_iteration_benchmark();
    LINKEDLIST_iteration_benchmark();
    LIST_iteration_benchmark();
    QUEUE_iteration_benchmark();
    STACK_iteration_benchmark();
    TREEMAP_iteration_benchmark();
    TREESET_iteration_benchmark();

    return 0;
}
