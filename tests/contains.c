#include "../src/macro_collections.h"
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

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

    bool a[13] = {false, false, false, false, false, false, false, false, false, false, false, false, false},
         b[13] = {false, false, false, false, false, false, false, false, false, false, false, false, false},
         c[13] = {true, true, true, true, true, true, true, true, true, true, true, true, true};

    b[0] = d_contains(d, 1, intcmp);
    b[1] = hm_contains(hm, 1);
    b[2] = hs_contains(hs, 1);
    b[3] = h_contains(h, 1);
    b[4] = ll_contains(ll, 1, intcmp);
    b[5] = l_contains(l, 1, intcmp);
    b[6] = q_contains(q, 1, intcmp);
    b[7] = s_contains(s, 1, intcmp);
    b[8] = tm_contains(tm, 1);
    b[9] = ts_contains(ts, 1);

    b[10] = ih_contains(ih, 1);
    b[11] = mm_contains(mm, 1);
    b[12] = ms_contains(ms, 1);

    for (int i = 1; i < 1001; i++)
    {
        if (i % 2 == 0)
        {
            d_push_front(d, i);
            l_push_front(l, i);
            ll_push_front(ll, i);
        }
        else
        {
            d_push_back(d, i);
            l_push_back(l, i);
            ll_push_back(ll, i);
        }

        s_push(s, i);
        q_enqueue(q, i);
        h_insert(h, i);
        ts_insert(ts, i);
        tm_insert(tm, i, i);
        hs_insert(hs, i);
        hm_insert(hm, i, i);

        ih_insert(ih, i);
        mm_insert(mm, i, i);
        ms_insert(ms, i);
    }

    for (int i = 1; i < 1001; i++)
    {
        c[0] = c[0] && d_contains(d, i, intcmp);
        c[1] = c[1] && hm_contains(hm, i);
        c[2] = c[2] && hs_contains(hs, i);
        c[3] = c[3] && h_contains(h, i);
        c[4] = c[4] && ll_contains(ll, i, intcmp);
        c[5] = c[5] && l_contains(l, i, intcmp);
        c[6] = c[6] && q_contains(q, i, intcmp);
        c[7] = c[7] && s_contains(s, i, intcmp);
        c[8] = c[8] && tm_contains(tm, i);
        c[9] = c[9] && ts_contains(ts, i);

        c[10] = c[10] && ih_contains(ih, i);
        c[11] = c[11] && mm_contains(mm, i);
        c[12] = c[12] && ms_contains(ms, i);
    }

    for (int i = -500; i < 1; i++)
    {
        a[0] = a[0] || d_contains(d, i, intcmp);
        a[1] = a[1] || hm_contains(hm, i);
        a[2] = a[2] || hs_contains(hs, i);
        a[3] = a[3] || h_contains(h, i);
        a[4] = a[4] || ll_contains(ll, i, intcmp);
        a[5] = a[5] || l_contains(l, i, intcmp);
        a[6] = a[6] || q_contains(q, i, intcmp);
        a[7] = a[7] || s_contains(s, i, intcmp);
        a[8] = a[8] || tm_contains(tm, i);
        a[9] = a[9] || ts_contains(ts, i);

        a[10] = a[10] || ih_contains(ih, i);
        a[11] = a[11] || mm_contains(mm, i);
        a[12] = a[12] || ms_contains(ms, i);
    }

    for (int i = 1001; i < 1500; i++)
    {
        a[0] = a[0] || d_contains(d, i, intcmp);
        a[1] = a[1] || hm_contains(hm, i);
        a[2] = a[2] || hs_contains(hs, i);
        a[3] = a[3] || h_contains(h, i);
        a[4] = a[4] || ll_contains(ll, i, intcmp);
        a[5] = a[5] || l_contains(l, i, intcmp);
        a[6] = a[6] || q_contains(q, i, intcmp);
        a[7] = a[7] || s_contains(s, i, intcmp);
        a[8] = a[8] || tm_contains(tm, i);
        a[9] = a[9] || ts_contains(ts, i);

        a[10] = a[10] || ih_contains(ih, i);
        a[11] = a[11] || mm_contains(mm, i);
        a[12] = a[12] || ms_contains(ms, i);
    }

    d_clear(d, NULL);
    hm_clear(hm, NULL);
    hs_clear(hs, NULL);
    h_clear(h, NULL);
    ll_clear(ll, NULL);
    l_clear(l, NULL);
    q_clear(q, NULL);
    s_clear(s, NULL);
    tm_clear(tm, NULL);
    ts_clear(ts, NULL);

    ih_clear(ih, NULL);
    mm_clear(mm, NULL);
    ms_clear(ms, NULL);

    b[0] = b[0] || d_contains(d, 1, intcmp);
    b[1] = b[1] || hm_contains(hm, 1);
    b[2] = b[2] || hs_contains(hs, 1);
    b[3] = b[3] || h_contains(h, 1);
    b[4] = b[4] || ll_contains(ll, 1, intcmp);
    b[5] = b[5] || l_contains(l, 1, intcmp);
    b[6] = b[6] || q_contains(q, 1, intcmp);
    b[7] = b[7] || s_contains(s, 1, intcmp);
    b[8] = b[8] || tm_contains(tm, 1);
    b[9] = b[9] || ts_contains(ts, 1);

    b[10] = b[10] || ih_contains(ih, 1);
    b[11] = b[11] || mm_contains(mm, 1);
    b[12] = b[12] || ms_contains(ms, 1);

    printf("-------------------- CONTAINS --------------------\n");
    if (c[0] && !a[0] && !b[0])
        printf("%12s PASSED\n", "DEQUE");
    else
        printf("%12s FAILED\n", "DEQUE");
    if (c[1] && !a[1] && !b[1])
        printf("%12s PASSED\n", "HASHMAP");
    else
        printf("%12s FAILED\n", "HASHMAP");
    if (c[2] && !a[2] && !b[2])
        printf("%12s PASSED\n", "HASHSET");
    else
        printf("%12s FAILED\n", "HASHSET");
    if (c[3] && !a[3] && !b[3])
        printf("%12s PASSED\n", "HEAP");
    else
        printf("%12s FAILED\n", "HEAP");
    if (c[4] && !a[4] && !b[4])
        printf("%12s PASSED\n", "LINKEDLIST");
    else
        printf("%12s FAILED\n", "LINKEDLIST");
    if (c[5] && !a[5] && !b[5])
        printf("%12s PASSED\n", "LIST");
    else
        printf("%12s FAILED\n", "LIST");
    if (c[6] && !a[6] && !b[6])
        printf("%12s PASSED\n", "QUEUE");
    else
        printf("%12s FAILED\n", "QUEUE");
    if (c[7] && !a[7] && !b[7])
        printf("%12s PASSED\n", "STACK");
    else
        printf("%12s FAILED\n", "STACK");
    if (c[8] && !a[8] && !b[8])
        printf("%12s PASSED\n", "TREEMAP");
    else
        printf("%12s FAILED\n", "TREEMAP");
    if (c[9] && !a[9] && !b[9])
        printf("%12s PASSED\n", "TREESET");
    else
        printf("%12s FAILED\n", "TREESET");
    if (c[10] && !a[10] && !b[10])
        printf("%12s PASSED\n", "INTERVALHEAP");
    else
        printf("%12s FAILED\n", "INTERVALHEAP");
    if (c[11] && !a[11] && !b[11])
        printf("%12s PASSED\n", "MULTIMAP");
    else
        printf("%12s FAILED\n", "MULTIMAP");
    if (c[12] && !a[12] && !b[12])
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
