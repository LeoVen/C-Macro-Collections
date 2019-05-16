#include "../src/macro_collections.h"
#include <stdio.h>
#include <assert.h>

int intcmp(int a, int b)
{
    return a - b;
}

size_t inthash(int t)
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
COLLECTION_GENERATE(TREEMAP, PUBLIC, tm, tmap, /* FMOD */, int, double)
COLLECTION_GENERATE(HASHSET, PUBLIC, hs, hset, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(HASHMAP, PUBLIC, hm, hmap, /* FMOD */, int, double)

#define ITERATOR_TEST(NAME, PFX, sname, initfunc, insertbody, sumbody, validation1, validation2) \
    void NAME##_iterator_test(void)                                                              \
    {                                                                                            \
        int sum1 = 0;                                                                            \
        double sum2 = 0.0;                                                                       \
                                                                                                 \
        sname *coll = initfunc;                                                                  \
        for (int i = 0; i < 10; i++)                                                             \
        {                                                                                        \
            insertbody;                                                                          \
        }                                                                                        \
                                                                                                 \
        sname##_iter iter;                                                                       \
        PFX##_iter_init(&iter, coll);                                                            \
                                                                                                 \
        for (int j = 0; j < 4; j++)                                                              \
        {                                                                                        \
            while (!PFX##_iter_end(&iter))                                                       \
            {                                                                                    \
                sumbody;                                                                         \
                PFX##_iter_next(&iter);                                                          \
            }                                                                                    \
            while (!PFX##_iter_start(&iter))                                                     \
            {                                                                                    \
                sumbody;                                                                         \
                PFX##_iter_prev(&iter);                                                          \
            }                                                                                    \
        }                                                                                        \
                                                                                                 \
        for (PFX##_iter_to_start(&iter); !PFX##_iter_end(&iter); PFX##_iter_next(&iter))         \
        {                                                                                        \
            sumbody;                                                                             \
        }                                                                                        \
                                                                                                 \
        for (PFX##_iter_to_end(&iter); !PFX##_iter_start(&iter); PFX##_iter_prev(&iter))         \
        {                                                                                        \
            sumbody;                                                                             \
        }                                                                                        \
                                                                                                 \
        PFX##_free(coll);                                                                        \
                                                                                                 \
        if (validation1 && validation2)                                                          \
            printf("%10s PASSED\n", #NAME);                                                      \
        else                                                                                     \
            printf("%10s FAILED\n", #NAME);                                                      \
    }

ITERATOR_TEST(DEQUE, d, deque, d_new(20), d_push_front(coll, i), sum1 += d_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(HASHMAP, hm, hmap, hm_new(50, 0.6, intcmp, inthash), hm_insert(coll, i, (double)i / 10.0), sum1 += hm_iter_key(&iter); sum2 += hm_iter_value(&iter), sum1 == 450, sum2 == 45.0)
ITERATOR_TEST(HASHSET, hs, hset, hs_new(50, 0.6, intcmp, inthash), hs_insert(coll, i), sum1 += hs_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(HEAP, h, heap, h_new(20, MaxHeap, intcmp), h_insert(coll, i), sum1 += h_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(LINKEDLIST, ll, linked, ll_new(), ll_push_back(coll, i), sum1 += ll_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(LIST, l, list, l_new(20), l_push_back(coll, i), sum1 += l_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(QUEUE, q, queue, q_new(20), q_enqueue(coll, i), sum1 += q_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(STACK, s, stack, s_new(20), s_push(coll, i), sum1 += s_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(TREEMAP, tm, tmap, tm_new(intcmp), tm_insert(coll, i, (double)i / 10.0), sum1 += tm_iter_key(&iter); sum2 += tm_iter_value(&iter), sum1 == 450, sum2 == 45.0)
ITERATOR_TEST(TREESET, ts, tset, ts_new(intcmp), ts_insert(coll, i), sum1 += ts_iter_value(&iter), sum1 == 450, 1)

int main(void)
{
    printf("\n-------------------- ITERATORS --------------------\n");
    DEQUE_iterator_test();
    HASHMAP_iterator_test();
    HASHSET_iterator_test();
    HEAP_iterator_test();
    LINKEDLIST_iterator_test();
    LIST_iterator_test();
    QUEUE_iterator_test();
    STACK_iterator_test();
    TREEMAP_iterator_test();
    TREESET_iterator_test();

    return 0;
}
