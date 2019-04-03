#include <stdio.h>
#include <assert.h>
#include "../src/macro_collections.h"

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

COLLECTION_GENERATE(LIST, PUBLIC, l, list, /**/, /**/, int)
COLLECTION_GENERATE(STACK, PUBLIC, s, stack, /**/, /**/, int)
COLLECTION_GENERATE(QUEUE, PUBLIC, q, queue, /**/, /**/, int)
COLLECTION_GENERATE(DEQUE, PUBLIC, d, deque, /**/, /**/, int)
COLLECTION_GENERATE(LINKEDLIST, PUBLIC, ll, linked, /**/, /**/, int)
COLLECTION_GENERATE(HEAP, PUBLIC, h, heap, /**/, /**/, int)
COLLECTION_GENERATE(TREESET, PUBLIC, ts, tset, /**/, /**/, int)
COLLECTION_GENERATE(TREEMAP, PUBLIC, tm, tmap, /**/, int, int)
COLLECTION_GENERATE(HASHSET, PUBLIC, hs, hset, /**/, /**/, int)
COLLECTION_GENERATE(HASHMAP, PUBLIC, hm, hmap, /* FMOD */, int, int)

int main(int argc, char const *argv[])
{
    list *l = l_new(1000);
    stack *s = s_new(1000);
    queue *q = q_new(1000);
    deque *d = d_new(1000);
    linked *ll = ll_new();
    heap *h = h_new(1000, MaxHeap, intcmp);
    tset *ts = ts_new(intcmp);
    tmap *tm = tm_new(intcmp);
    hset *hs = hs_new(1000, 0.9, intcmp, inthash);
    hmap *hm = hm_new(1000, 0.9, intcmp, inthash);

    for (int i = 1; i < 10001; i++)
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

    int sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0,
        sum7 = 0, sum8 = 0, sum9 = 0, sum10 = 0, sum11 = 0;

    FOR_EACH(l, list, int, l, {
        sum0 += value;
    })

    FOR_EACH(s, stack, int, s, {
        sum1 += value;
    })

    FOR_EACH(q, queue, int, q, {
        sum2 += value;
    })

    FOR_EACH(d, deque, int, d, {
        sum3 += value;
    })

    FOR_EACH(ll, linked, int, ll, {
        sum4 += value;
    })

    FOR_EACH(h, heap, int, h, {
        sum5 += value;
    })

    FOR_EACH(ts, tset, int, ts, {
        sum6 += value;
    })

    FOR_EACH_MAP(tm, tmap, int, int, tm, {
        sum7 += key;
        sum8 += value;
    })

    FOR_EACH(hs, hset, int, hs, {
        sum9 += value;
    })

    FOR_EACH_MAP(hm, hmap, int, int, hm, {
        sum10 += key;
        sum11 += value;
    })

    if (sum0 == 50005000)
        printf("%10s PASSED\n", "LIST");
    if (sum1 == 50005000)
        printf("%10s PASSED\n", "STACK");
    if (sum2 == 50005000)
        printf("%10s PASSED\n", "QUEUE");
    if (sum3 == 50005000)
        printf("%10s PASSED\n", "DEQUE");
    if (sum4 == 50005000)
        printf("%10s PASSED\n", "LINKEDLIST");
    if (sum5 == 50005000)
        printf("%10s PASSED\n", "HEAP");
    if (sum6 == 50005000)
        printf("%10s PASSED\n", "TREESET");
    if (sum7 == 50005000 && sum8 == 50005000)
        printf("%10s PASSED\n", "TREEMAP");
    if (sum9 == 50005000)
        printf("%10s PASSED\n", "HASHSET");
    if (sum10 == 50005000 && sum11 == 50005000)
        printf("%10s PASSED\n", "HASHMAP");

    printf("\n\n");

    sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0,
    sum7 = 0, sum8 = 0, sum9 = 0, sum10 = 0, sum11 = 0;

    FOR_EACH_REV(l, list, int, l, {
        sum0 += value;
    })

    FOR_EACH_REV(s, stack, int, s, {
        sum1 += value;
    })

    FOR_EACH_REV(q, queue, int, q, {
        sum2 += value;
    })

    FOR_EACH_REV(d, deque, int, d, {
        sum3 += value;
    })

    FOR_EACH_REV(ll, linked, int, ll, {
        sum4 += value;
    })

    FOR_EACH_REV(h, heap, int, h, {
        sum5 += value;
    })

    FOR_EACH_REV(ts, tset, int, ts, {
        sum6 += value;
    })

    FOR_EACH_MAP_REV(tm, tmap, int, int, tm, {
        sum7 += key;
        sum8 += value;
    })

    FOR_EACH_REV(hs, hset, int, hs, {
        sum9 += value;
    })

    FOR_EACH_MAP_REV(tm, tmap, int, int, tm, {
        sum10 += key;
        sum11 += value;
    })

    if (sum0 == 50005000)
        printf("%10s PASSED\n", "LIST");
    if (sum1 == 50005000)
        printf("%10s PASSED\n", "STACK");
    if (sum2 == 50005000)
        printf("%10s PASSED\n", "QUEUE");
    if (sum3 == 50005000)
        printf("%10s PASSED\n", "DEQUE");
    if (sum4 == 50005000)
        printf("%10s PASSED\n", "LINKEDLIST");
    if (sum5 == 50005000)
        printf("%10s PASSED\n", "HEAP");
    if (sum6 == 50005000)
        printf("%10s PASSED\n", "TREESET");
    if (sum7 == 50005000 && sum8 == 50005000)
        printf("%10s PASSED\n", "TREEMAP");
    if (sum9 == 50005000)
        printf("%10s PASSED\n", "HASHSET");
    if (sum10 == 50005000 && sum11 == 50005000)
        printf("%10s PASSED\n", "HASHMAP");

    l_free(l);
    ll_free(ll);
    s_free(s);
    q_free(q);
    d_free(d);
    ts_free(ts);
    tm_free(tm);

    return 0;
}
