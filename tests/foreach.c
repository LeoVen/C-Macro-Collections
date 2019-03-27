#include <stdio.h>
#include <assert.h>
#include "../src/macro_collections.h"

int cmp(int a, int b)
{
    return a - b;
}

COLLECTION_GENERATE(LIST, PUBLIC, l, list, /**/, /**/, int)
COLLECTION_GENERATE(STACK, PUBLIC, s, stack, /**/, /**/, int)
COLLECTION_GENERATE(QUEUE, PUBLIC, q, queue, /**/, /**/, int)
COLLECTION_GENERATE(DEQUE, PUBLIC, d, deque, /**/, /**/, int)
COLLECTION_GENERATE(LINKEDLIST, PUBLIC, ll, linked, /**/, /**/, int)
COLLECTION_GENERATE(HEAP, PUBLIC, h, heap, /**/, /**/, int)
COLLECTION_GENERATE(TREESET, PUBLIC, ts, tset, /**/, /**/, int)

int main(int argc, char const *argv[])
{
    list *l = l_new(1000);
    stack *s = s_new(1000);
    queue *q = q_new(1000);
    deque *d = d_new(1000);
    linked *ll = ll_new();
    heap *h = h_new(1000, MaxHeap, cmp);
    tset *ts = ts_new(cmp);

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
    }

    int sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0;

    FOR_EACH(l, list, int, l, {
        sum0 += var;
    })

    FOR_EACH(s, stack, int, s, {
        sum1 += var;
    })

    FOR_EACH(q, queue, int, q, {
        sum2 += var;
    })

    FOR_EACH(d, deque, int, d, {
        sum3 += var;
    })

    FOR_EACH(ll, linked, int, ll, {
        sum4 += var;
    })

    FOR_EACH(h, heap, int, h, {
        sum5 += var;
    })

    FOR_EACH(ts, tset, int, ts, {
        sum6 += var;
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

    printf("\n\n");

    sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0, sum6 = 0;

    FOR_EACH_REV(l, list, int, l, {
        sum0 += var;
    })

    FOR_EACH_REV(s, stack, int, s, {
        sum1 += var;
    })

    FOR_EACH_REV(q, queue, int, q, {
        sum2 += var;
    })

    FOR_EACH_REV(d, deque, int, d, {
        sum3 += var;
    })

    FOR_EACH_REV(ll, linked, int, ll, {
        sum4 += var;
    })

    FOR_EACH_REV(h, heap, int, h, {
        sum5 += var;
    })

    FOR_EACH_REV(ts, tset, int, ts, {
        sum6 += var;
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

    l_free(l);
    ll_free(ll);
    s_free(s);
    q_free(q);
    d_free(d);

    return 0;
}
