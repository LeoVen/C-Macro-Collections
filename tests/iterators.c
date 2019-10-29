#include "../src/macro_collections.h"
#include <stdio.h>

int intcmp(int a, int b)
{
    return (a > b) - (a < b);
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

CMC_COLLECTION_GENERATE(LIST, l, list, /* K */, int)
CMC_COLLECTION_GENERATE(STACK, s, stack, /* K */, int)
CMC_COLLECTION_GENERATE(QUEUE, q, queue, /* K */, int)
CMC_COLLECTION_GENERATE(DEQUE, d, deque, /* K */, int)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll, linked, /* K */, int)
CMC_COLLECTION_GENERATE(HEAP, h, heap, /* K */, int)
CMC_COLLECTION_GENERATE(TREESET, ts, tset, /* K */, int)
CMC_COLLECTION_GENERATE(TREEMAP, tm, tmap, int, double)
CMC_COLLECTION_GENERATE(HASHSET, hs, hset, /* K */, int)
CMC_COLLECTION_GENERATE(HASHMAP, hm, hmap, int, double)

CMC_COLLECTION_GENERATE(INTERVALHEAP, ih, iheap, /* K */, int)
CMC_COLLECTION_GENERATE(MULTIMAP, mm, mmap, int, double)
CMC_COLLECTION_GENERATE(MULTISET, ms, mset, /* K */, int)

#define ITERATOR_TEST(NAME, PFX, sname, initfunc, insertbody, sumbody, validation1, validation2) \
    void NAME##_iterator_test(void)                                                              \
    {                                                                                            \
        int sum1 = 0;                                                                            \
        double sum2 = 0.0;                                                                       \
                                                                                                 \
        struct sname *coll = initfunc;                                                           \
        for (int i = 0; i < 10; i++)                                                             \
        {                                                                                        \
            insertbody;                                                                          \
        }                                                                                        \
                                                                                                 \
        struct sname##_iter iter;                                                                \
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
        PFX##_free(coll, NULL);                                                                  \
                                                                                                 \
        if ((validation1) && (validation2))                                                      \
            printf("%12s PASSED\n", #NAME);                                                      \
        else                                                                                     \
            printf("%12s FAILED\n", #NAME);                                                      \
    }

ITERATOR_TEST(DEQUE, d, deque, d_new(20), d_push_front(coll, i), sum1 += d_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(HASHMAP, hm, hmap, hm_new(50, 0.6, intcmp, inthash), hm_insert(coll, i, (double)i / 10.0), sum1 += hm_iter_key(&iter); sum2 += hm_iter_value(&iter), sum1 == 450, (int)sum2 == (int)45)
ITERATOR_TEST(HASHSET, hs, hset, hs_new(50, 0.6, intcmp, inthash), hs_insert(coll, i), sum1 += hs_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(HEAP, h, heap, h_new(20, cmc_max_heap, intcmp), h_insert(coll, i), sum1 += h_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(LINKEDLIST, ll, linked, ll_new(), ll_push_back(coll, i), sum1 += ll_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(LIST, l, list, l_new(20), l_push_back(coll, i), sum1 += l_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(QUEUE, q, queue, q_new(20), q_enqueue(coll, i), sum1 += q_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(STACK, s, stack, s_new(20), s_push(coll, i), sum1 += s_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(TREEMAP, tm, tmap, tm_new(intcmp), tm_insert(coll, i, (double)i / 10.0), sum1 += tm_iter_key(&iter); sum2 += tm_iter_value(&iter), sum1 == 450, (int)sum2 == (int)45)
ITERATOR_TEST(TREESET, ts, tset, ts_new(intcmp), ts_insert(coll, i), sum1 += ts_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(INTERVALHEAP, ih, iheap, ih_new(20, intcmp), ih_insert(coll, i), sum1 += ih_iter_value(&iter), sum1 == 450, 1)
ITERATOR_TEST(MULTIMAP, mm, mmap, mm_new(50, 0.8, intcmp, inthash), mm_insert(coll, i, (double)i / 10.0), sum1 += mm_iter_key(&iter); sum2 += mm_iter_value(&iter), sum1 == 450, (int)sum2 == (int)45)
ITERATOR_TEST(MULTISET, ms, mset, ms_new(50, 0.6, intcmp, inthash), ms_insert(coll, i), sum1 += ms_iter_value(&iter); sum2 += ms_iter_multiplicity(&iter), sum1 == 450, (size_t)sum2 == 100)

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

    INTERVALHEAP_iterator_test();
    MULTIMAP_iterator_test();
    MULTISET_iterator_test();

    printf("\n\n");

    return 0;
}
