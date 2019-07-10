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

int main(void)
{
    list *l = l_new(1000);
    stack *s = s_new(1000);
    queue *q = q_new(1000);
    deque *d = d_new(1000);
    linked *ll = ll_new();
    heap *h = h_new(1000, cmc_max_heap, intcmp);
    tset *ts = ts_new(intcmp);
    tmap *tm = tm_new(intcmp);
    hset *hs = hs_new(1000, 0.9, intcmp, inthash);
    hmap *hm = hm_new(1000, 0.9, intcmp, inthash);

    bool a[10] = {false, false, false, false, false, false, false, false, false, false},
         b[10] = {false, false, false, false, false, false, false, false, false, false},
         c[10] = {true, true, true, true, true, true, true, true, true, true};

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
    }

    d_clear(d);
    hm_clear(hm);
    hs_clear(hs);
    h_clear(h);
    ll_clear(ll);
    l_clear(l);
    q_clear(q);
    s_clear(s);
    tm_clear(tm);
    ts_clear(ts);

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

    printf("-------------------- CONTAINS --------------------\n");
    if (c[0] && !a[0] && !b[0])
        printf("%10s PASSED\n", "DEQUE");
    else
        printf("%10s FAILED\n", "DEQUE");
    if (c[1] && !a[1] && !b[1])
        printf("%10s PASSED\n", "HASHMAP");
    else
        printf("%10s FAILED\n", "HASHMAP");
    if (c[2] && !a[2] && !b[2])
        printf("%10s PASSED\n", "HASHSET");
    else
        printf("%10s FAILED\n", "HASHSET");
    if (c[3] && !a[3] && !b[3])
        printf("%10s PASSED\n", "HEAP");
    else
        printf("%10s FAILED\n", "HEAP");
    if (c[4] && !a[4] && !b[4])
        printf("%10s PASSED\n", "LINKEDLIST");
    else
        printf("%10s FAILED\n", "LINKEDLIST");
    if (c[5] && !a[5] && !b[5])
        printf("%10s PASSED\n", "LIST");
    else
        printf("%10s FAILED\n", "LIST");
    if (c[6] && !a[6] && !b[6])
        printf("%10s PASSED\n", "QUEUE");
    else
        printf("%10s FAILED\n", "QUEUE");
    if (c[7] && !a[7] && !b[7])
        printf("%10s PASSED\n", "STACK");
    else
        printf("%10s FAILED\n", "STACK");
    if (c[8] && !a[8] && !b[8])
        printf("%10s PASSED\n", "TREEMAP");
    else
        printf("%10s FAILED\n", "TREEMAP");
    if (c[9] && !a[9] && !b[9])
        printf("%10s PASSED\n", "TREESET");
    else
        printf("%10s FAILED\n", "TREESET");

    l_free(l);
    ll_free(ll);
    s_free(s);
    q_free(q);
    d_free(d);
    h_free(h);
    ts_free(ts);
    tm_free(tm);
    hs_free(hs);
    hm_free(hm);

    return 0;
}
