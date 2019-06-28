#include "../src/macro_collections.h"
#include <stdio.h>
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

    FOR_EACH(d, deque, d, {
        sum3 += d_iter_value(&iter);
    });

    FOR_EACH(hm, hmap, hm, {
        sum10 += hm_iter_key(&iter);
        sum11 += hm_iter_value(&iter);
    });

    FOR_EACH(hs, hset, hs, {
        sum9 += hs_iter_value(&iter);
    });

    FOR_EACH(h, heap, h, {
        sum5 += h_iter_value(&iter);
    });

    FOR_EACH(ll, linked, ll, {
        sum4 += ll_iter_value(&iter);
    });

    FOR_EACH(l, list, l, {
        sum0 += l_iter_value(&iter);
    });

    FOR_EACH(q, queue, q, {
        sum2 += q_iter_value(&iter);
    });

    FOR_EACH(s, stack, s, {
        sum1 += s_iter_value(&iter);
    });

    FOR_EACH(tm, tmap, tm, {
        sum7 += tm_iter_key(&iter);
        sum8 += tm_iter_value(&iter);
    });

    FOR_EACH(ts, tset, ts, {
        sum6 += ts_iter_value(&iter);
    });

    printf("\n-------------------- FOR_EACH --------------------\n");
    if (sum3 == 50005000)
        printf("%10s PASSED\n", "DEQUE");
    else
        printf("%10s FAILED\n", "DEQUE");
    if (sum9 == 50005000)
        printf("%10s PASSED\n", "HASHSET");
    else
        printf("%10s FAILED\n", "HASHSET");
    if (sum10 == 50005000 && sum11 == 50005000)
        printf("%10s PASSED\n", "HASHMAP");
    else
        printf("%10s FAILED\n", "HASHMAP");
    if (sum5 == 50005000)
        printf("%10s PASSED\n", "HEAP");
    else
        printf("%10s FAILED\n", "HEAP");
    if (sum4 == 50005000)
        printf("%10s PASSED\n", "LINKEDLIST");
    else
        printf("%10s FAILED\n", "LINKEDLIST");
    if (sum0 == 50005000)
        printf("%10s PASSED\n", "LIST");
    else
        printf("%10s FAILED\n", "LIST");
    if (sum2 == 50005000)
        printf("%10s PASSED\n", "QUEUE");
    else
        printf("%10s FAILED\n", "QUEUE");
    if (sum1 == 50005000)
        printf("%10s PASSED\n", "STACK");
    else
        printf("%10s FAILED\n", "STACK");
    if (sum7 == 50005000 && sum8 == 50005000)
        printf("%10s PASSED\n", "TREEMAP");
    else
        printf("%10s FAILED\n", "TREEMAP");
    if (sum6 == 50005000)
        printf("%10s PASSED\n", "TREESET");
    else
        printf("%10s FAILED\n", "TREESET");

    printf("\n\n");

    sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0,
    sum7 = 0, sum8 = 0, sum9 = 0, sum10 = 0, sum11 = 0;

    FOR_EACH_REV(d, deque, d, {
        sum3 += d_iter_value(&iter);
    });

    FOR_EACH_REV(hm, hmap, hm, {
        sum10 += hm_iter_key(&iter);
        sum11 += hm_iter_value(&iter);
    });

    FOR_EACH_REV(hs, hset, hs, {
        sum9 += hs_iter_value(&iter);
    });

    FOR_EACH_REV(h, heap, h, {
        sum5 += h_iter_value(&iter);
    });

    FOR_EACH_REV(ll, linked, ll, {
        sum4 += ll_iter_value(&iter);
    });

    FOR_EACH_REV(l, list, l, {
        sum0 += l_iter_value(&iter);
    });

    FOR_EACH_REV(q, queue, q, {
        sum2 += q_iter_value(&iter);
    });

    FOR_EACH_REV(s, stack, s, {
        sum1 += s_iter_value(&iter);
    });

    FOR_EACH_REV(tm, tmap, tm, {
        sum7 += tm_iter_key(&iter);
        sum8 += tm_iter_value(&iter);
    });

    FOR_EACH_REV(ts, tset, ts, {
        sum6 += ts_iter_value(&iter);
    });

    printf("-------------------- FOR_EACH_REV --------------------\n");
    if (sum3 == 50005000)
        printf("%10s PASSED\n", "DEQUE");
    else
        printf("%10s FAILED\n", "DEQUE");
    if (sum10 == 50005000 && sum11 == 50005000)
        printf("%10s PASSED\n", "HASHMAP");
    else
        printf("%10s FAILED\n", "HASHMAP");
    if (sum9 == 50005000)
        printf("%10s PASSED\n", "HASHSET");
    else
        printf("%10s FAILED\n", "HASHSET");
    if (sum5 == 50005000)
        printf("%10s PASSED\n", "HEAP");
    else
        printf("%10s FAILED\n", "HEAP");
    if (sum4 == 50005000)
        printf("%10s PASSED\n", "LINKEDLIST");
    else
        printf("%10s FAILED\n", "LINKEDLIST");
    if (sum0 == 50005000)
        printf("%10s PASSED\n", "LIST");
    else
        printf("%10s FAILED\n", "LIST");
    if (sum2 == 50005000)
        printf("%10s PASSED\n", "QUEUE");
    else
        printf("%10s FAILED\n", "QUEUE");
    if (sum1 == 50005000)
        printf("%10s PASSED\n", "STACK");
    else
        printf("%10s FAILED\n", "STACK");
    if (sum7 == 50005000 && sum8 == 50005000)
        printf("%10s PASSED\n", "TREEMAP");
    else
        printf("%10s FAILED\n", "TREEMAP");
    if (sum6 == 50005000)
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
