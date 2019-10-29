#include "../src/macro_collections.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

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
CMC_COLLECTION_GENERATE(TREEMAP, tm, tmap, int, int)
CMC_COLLECTION_GENERATE(HASHSET, hs, hset, /* K */, int)
CMC_COLLECTION_GENERATE(HASHMAP, hm, hmap, int, int)

CMC_COLLECTION_GENERATE(INTERVALHEAP, ih, iheap, , int)
CMC_COLLECTION_GENERATE(MULTIMAP, mm, mmap, int, int)
CMC_COLLECTION_GENERATE(MULTISET, ms, mset, , int)

int main(void)
{
    struct list *l = l_new(1000);
    struct stack *s = s_new(1000);
    struct queue *q = q_new(1000);
    struct deque *d = d_new(1000);
    struct linked *ll = ll_new();
    struct heap *h = h_new(1000, cmc_max_heap, intcmp);
    struct tset *ts = ts_new(intcmp);
    struct tmap *tm = tm_new(intcmp);
    struct hset *hs = hs_new(1000, 0.6, intcmp, inthash);
    struct hmap *hm = hm_new(1000, 0.6, intcmp, inthash);
    struct iheap *ih = ih_new(1000, intcmp);
    struct mmap *mm = mm_new(1000, 0.8, intcmp, inthash);
    struct mset *ms = ms_new(1000, 0.6, intcmp, inthash);

    bool p = true;

    for (int i = 1; i < 10001; i++)
    {
        if (i % 2 == 0)
        {
            assert(d_push_front(d, i));
            assert(l_push_front(l, i));
            assert(ll_push_front(ll, i));
        }
        else
        {
            assert(d_push_back(d, i));
            assert(l_push_back(l, i));
            assert(ll_push_back(ll, i));
        }

        assert(s_push(s, i));
        assert(q_enqueue(q, i));
        assert(h_insert(h, i));
        assert(ts_insert(ts, i));
        assert(tm_insert(tm, i, i));
        assert(hs_insert(hs, i));
        assert(hm_insert(hm, i, i));

        assert(ih_insert(ih, i));
        assert(mm_insert(mm, i, i));
        assert(ms_insert(ms, i));
    }

    int sums[16] = {0};

    CMC_FOR_EACH(d, deque, d, {
        sums[0] += d_iter_value(&iter);
    });

    CMC_FOR_EACH(hm, hmap, hm, {
        sums[1] += hm_iter_key(&iter);
        sums[2] += hm_iter_value(&iter);
    });

    CMC_FOR_EACH(hs, hset, hs, {
        sums[3] += hs_iter_value(&iter);
    });

    CMC_FOR_EACH(h, heap, h, {
        sums[4] += h_iter_value(&iter);
    });

    CMC_FOR_EACH(ll, linked, ll, {
        sums[5] += ll_iter_value(&iter);
    });

    CMC_FOR_EACH(l, list, l, {
        sums[6] += l_iter_value(&iter);
    });

    CMC_FOR_EACH(q, queue, q, {
        sums[7] += q_iter_value(&iter);
    });

    CMC_FOR_EACH(s, stack, s, {
        sums[8] += s_iter_value(&iter);
    });

    CMC_FOR_EACH(tm, tmap, tm, {
        sums[9] += tm_iter_key(&iter);
        sums[10] += tm_iter_value(&iter);
    });

    CMC_FOR_EACH(ts, tset, ts, {
        sums[11] += ts_iter_value(&iter);
    });

    CMC_FOR_EACH(ih, iheap, ih, {
        sums[12] += ih_iter_value(&iter);
    });

    CMC_FOR_EACH(mm, mmap, mm, {
        sums[13] += mm_iter_key(&iter);
        sums[14] += mm_iter_value(&iter);
    });

    CMC_FOR_EACH(ms, mset, ms, {
        sums[15] += ms_iter_value(&iter);
    });

    printf("\n-------------------- CMC_FOR_EACH --------------------\n");
    if (sums[0] == 50005000)
        printf("%12s PASSED\n", "DEQUE");
    else
        printf("%12s FAILED\n", "DEQUE");
    if (sums[1] == 50005000 && sums[2] == 50005000)
        printf("%12s PASSED\n", "HASHMAP");
    else
        printf("%12s FAILED\n", "HASHMAP");
    if (sums[3] == 50005000)
        printf("%12s PASSED\n", "HASHSET");
    else
        printf("%12s FAILED\n", "HASHSET");
    if (sums[4] == 50005000)
        printf("%12s PASSED\n", "HEAP");
    else
        printf("%12s FAILED\n", "HEAP");
    if (sums[5] == 50005000)
        printf("%12s PASSED\n", "LINKEDLIST");
    else
        printf("%12s FAILED\n", "LINKEDLIST");
    if (sums[6] == 50005000)
        printf("%12s PASSED\n", "LIST");
    else
        printf("%12s FAILED\n", "LIST");
    if (sums[7] == 50005000)
        printf("%12s PASSED\n", "QUEUE");
    else
        printf("%12s FAILED\n", "QUEUE");
    if (sums[8] == 50005000)
        printf("%12s PASSED\n", "STACK");
    else
        printf("%12s FAILED\n", "STACK");
    if (sums[9] == 50005000 && sums[10] == 50005000)
        printf("%12s PASSED\n", "TREEMAP");
    else
        printf("%12s FAILED\n", "TREEMAP");
    if (sums[11] == 50005000)
        printf("%12s PASSED\n", "TREESET");
    else
        printf("%12s FAILED\n", "TREESET");
    if (sums[12] == 50005000)
        printf("%12s PASSED\n", "INTERVALHEAP");
    else
        printf("%12s FAILED\n", "INTERVALHEAP");
    if (sums[13] == 50005000 && sums[14] == 50005000)
        printf("%12s PASSED\n", "MULTIMAP");
    else
        printf("%12s FAILED\n", "MULTIMAP");
    if (sums[15] == 50005000)
        printf("%12s PASSED\n", "MULTISET");
    else
        printf("%12s FAILED\n", "MULTISET");

    printf("\n\n");

    memset(sums, 0, sizeof sums);

    CMC_FOR_EACH_REV(d, deque, d, {
        sums[0] += d_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(hm, hmap, hm, {
        sums[1] += hm_iter_key(&iter);
        sums[2] += hm_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(hs, hset, hs, {
        sums[3] += hs_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(h, heap, h, {
        sums[4] += h_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(ll, linked, ll, {
        sums[5] += ll_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(l, list, l, {
        sums[6] += l_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(q, queue, q, {
        sums[7] += q_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(s, stack, s, {
        sums[8] += s_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(tm, tmap, tm, {
        sums[9] += tm_iter_key(&iter);
        sums[10] += tm_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(ts, tset, ts, {
        sums[11] += ts_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(ih, iheap, ih, {
        sums[12] += ih_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(mm, mmap, mm, {
        sums[13] += mm_iter_key(&iter);
        sums[14] += mm_iter_value(&iter);
    });

    CMC_FOR_EACH_REV(ms, mset, ms, {
        sums[15] += ms_iter_value(&iter);
    });

    printf("-------------------- CMC_FOR_EACH_REV --------------------\n");
    if (sums[0] == 50005000)
        printf("%12s PASSED\n", "DEQUE");
    else
        printf("%12s FAILED\n", "DEQUE");
    if (sums[1] == 50005000 && sums[2] == 50005000)
        printf("%12s PASSED\n", "HASHMAP");
    else
        printf("%12s FAILED\n", "HASHMAP");
    if (sums[3] == 50005000)
        printf("%12s PASSED\n", "HASHSET");
    else
        printf("%12s FAILED\n", "HASHSET");
    if (sums[4] == 50005000)
        printf("%12s PASSED\n", "HEAP");
    else
        printf("%12s FAILED\n", "HEAP");
    if (sums[5] == 50005000)
        printf("%12s PASSED\n", "LINKEDLIST");
    else
        printf("%12s FAILED\n", "LINKEDLIST");
    if (sums[6] == 50005000)
        printf("%12s PASSED\n", "LIST");
    else
        printf("%12s FAILED\n", "LIST");
    if (sums[7] == 50005000)
        printf("%12s PASSED\n", "QUEUE");
    else
        printf("%12s FAILED\n", "QUEUE");
    if (sums[8] == 50005000)
        printf("%12s PASSED\n", "STACK");
    else
        printf("%12s FAILED\n", "STACK");
    if (sums[9] == 50005000 && sums[10] == 50005000)
        printf("%12s PASSED\n", "TREEMAP");
    else
        printf("%12s FAILED\n", "TREEMAP");
    if (sums[11] == 50005000)
        printf("%12s PASSED\n", "TREESET");
    else
        printf("%12s FAILED\n", "TREESET");
    if (sums[12] == 50005000)
        printf("%12s PASSED\n", "INTERVALHEAP");
    else
        printf("%12s FAILED\n", "INTERVALHEAP");
    if (sums[13] == 50005000 && sums[14] == 50005000)
        printf("%12s PASSED\n", "MULTIMAP");
    else
        printf("%12s FAILED\n", "MULTIMAP");
    if (sums[15] == 50005000)
        printf("%12s PASSED\n", "MULTISET");
    else
        printf("%12s FAILED\n", "MULTISET");

    printf("\n\n");

    l_free(l, NULL);
    ll_free(ll, NULL);
    s_free(s, NULL);
    q_free(q, NULL);
    d_free(d, NULL);
    h_free(h, NULL);
    ts_free(ts, NULL);
    tm_free(tm, NULL);
    hs_free(hs, NULL);
    hm_free(hm, NULL);

    ih_free(ih, NULL);
    mm_free(mm, NULL);
    ms_free(ms, NULL);

    return 0;
}
