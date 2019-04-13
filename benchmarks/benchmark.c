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
 * - Output
 * - Iteration
 * - Search (to be done)
 */
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
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

double rrandom()
{
    return (double)rand() / ((double)RAND_MAX + 1.0);
}

size_t randsize(size_t min, size_t max)
{
    return (size_t)floor(rrandom() * ((double)max - (double)min + 1.0) + (double)min);
}

void shuffle(int *array)
{
    for (size_t i = 0; i < 10000000; i++)
    {
        size_t j = randsize(0, 10000000);
        int tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
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

#define BENCHMARK(NAME, PFX, sname, initfunc, insertbody, removebody, iternext)                         \
    void NAME##_io_benchmark(int *array)                                                                \
    {                                                                                                   \
        printf("+------------------------------------------------------------ %10s\n", #NAME);          \
        timer_t timer;                                                                                  \
        int r, k, v;                                                                                    \
        size_t j;                                                                                       \
                                                                                                        \
        sname *collection = initfunc;                                                                   \
                                                                                                        \
        TIMER_START(timer)                                                                              \
                                                                                                        \
        for (int i = 0; i < 10000000; i++)                                                              \
        {                                                                                               \
            insertbody;                                                                                 \
        }                                                                                               \
                                                                                                        \
        TIMER_STOP(timer)                                                                               \
                                                                                                        \
        TIMER_CALC(timer)                                                                               \
                                                                                                        \
        size_t s1 = PFX##_count(collection);                                                            \
                                                                                                        \
        printf("    INPUT %10s TOOK %8.0lf milliseconds for %8lu elements\n", #NAME, timer.result, s1); \
                                                                                                        \
        sname##_iter iter;                                                                              \
                                                                                                        \
        TIMER_START(timer)                                                                              \
                                                                                                        \
        for (PFX##_iter_new(&iter, collection); !PFX##_iter_end(&iter);)                                \
        {                                                                                               \
            iternext;                                                                                   \
        }                                                                                               \
                                                                                                        \
        TIMER_STOP(timer)                                                                               \
                                                                                                        \
        TIMER_CALC(timer)                                                                               \
                                                                                                        \
        printf("ITERATION %10s TOOK %8.0lf milliseconds for %8lu elements\n", #NAME, timer.result, s1); \
                                                                                                        \
        TIMER_START(timer);                                                                             \
                                                                                                        \
        for (int i = 0; i < 10000000; i++)                                                              \
        {                                                                                               \
            removebody;                                                                                 \
        }                                                                                               \
                                                                                                        \
        TIMER_STOP(timer)                                                                               \
                                                                                                        \
        TIMER_CALC(timer)                                                                               \
                                                                                                        \
        size_t s2 = PFX##_count(collection);                                                            \
                                                                                                        \
        printf("   OUTPUT %10s TOOK %8.0lf milliseconds for %8lu elements\n", #NAME, timer.result, s1); \
                                                                                                        \
        PFX##_free(collection);                                                                         \
                                                                                                        \
        assert(s1 == 10000000 && s2 == 0);                                                              \
                                                                                                        \
        printf("+------------------------------------------------------------ %10s\n\n", #NAME);        \
    }

BENCHMARK(DEQUE, d, deque, d_new(10000000), d_push_back(collection, array[i]), d_pop_back(collection), d_iter_next(&iter, &k, &j))
BENCHMARK(HASHMAP, hm, hmap, hm_new(10000000, 0.8, intcmp, inthash), hm_insert(collection, array[i], array[i]), hm_remove(collection, array[i], &r), hm_iter_next(&iter, &k, &v, &j))
BENCHMARK(HASHSET, hs, hset, hs_new(10000000, 0.8, intcmp, inthash), hs_insert(collection, array[i]), hs_remove(collection, array[i]), hs_iter_next(&iter, &v, &j))
BENCHMARK(HEAP, h, heap, h_new(10000000, MaxHeap, intcmp), h_insert(collection, array[i]), h_remove(collection, &r), h_iter_next(&iter, &v, &j))
BENCHMARK(LINKEDLIST, ll, linked, ll_new(), ll_push_back(collection, array[i]), ll_pop_back(collection), ll_iter_next(&iter, &v, &j))
BENCHMARK(LIST, l, list, l_new(10000000), l_push_back(collection, array[i]), l_pop_back(collection), l_iter_next(&iter, &v, &j))
BENCHMARK(QUEUE, q, queue, q_new(10000000), q_enqueue(collection, array[i]), q_dequeue(collection), q_iter_next(&iter, &v, &j))
BENCHMARK(STACK, s, stack, s_new(10000000), s_push(collection, array[i]), s_pop(collection), s_iter_next(&iter, &v, &j))
BENCHMARK(TREEMAP, tm, tmap, tm_new(intcmp), tm_insert(collection, array[i], array[i]), tm_remove(collection, array[i], &r), tm_iter_next(&iter, &k, &v, &j))
BENCHMARK(TREESET, ts, tset, ts_new(intcmp), ts_insert(collection, array[i]), ts_remove(collection, array[i]), ts_iter_next(&iter, &v, &j))

int main(void)
{
    int *array = malloc(10000000 * sizeof(int));

    if (!array)
    {
        printf("Allocation failed\n");
        return 1;
    }

    for (int i = -5000000; i < 5000000; i++)
        array[i + 5000000] = i;

    shuffle(array);

    DEQUE_io_benchmark(array);
    HASHMAP_io_benchmark(array);
    HASHSET_io_benchmark(array);
    HEAP_io_benchmark(array);
    LINKEDLIST_io_benchmark(array);
    LIST_io_benchmark(array);
    QUEUE_io_benchmark(array);
    STACK_io_benchmark(array);
    TREEMAP_io_benchmark(array);
    TREESET_io_benchmark(array);

    free(array);

    return 0;
}
