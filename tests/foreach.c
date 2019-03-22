#include <stdio.h>
#include <assert.h>
#include "../src/macro_collections.h"

CONTAINER_GENERATE(LIST, PUBLIC, l, list, /**/, /**/, int)
CONTAINER_GENERATE(STACK, PUBLIC, s, stack, /**/, /**/, int)
CONTAINER_GENERATE(QUEUE, PUBLIC, q, queue, /**/, /**/, int)
CONTAINER_GENERATE(DEQUE, PUBLIC, d, deque, /**/, /**/, int)
CONTAINER_GENERATE(LINKEDLIST, PUBLIC, ll, linked, /**/, /**/, int)

int main(int argc, char const *argv[])
{
    list *l = l_new(1000);
    stack *s = s_new(1000);
    queue *q = q_new(1000);
    deque *d = d_new(1000);
    linked *ll = ll_new();

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
    }

    int sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;

    FOR_EACH(l, list, , int, l, {
        sum0 += var;
    })

    FOR_EACH(s, stack, , int, s, {
        sum1 += var;
    })

    FOR_EACH(q, queue, , int, q, {
        sum2 += var;
    })

    FOR_EACH(d, deque, , int, d, {
        sum3 += var;
    })

    FOR_EACH(ll, linked, , int, ll, {
        sum4 += var;
    })

    if (sum0 == 50005000)
        printf("%10s PASSED\n", "LIST");
    if (sum4 == 50005000)
        printf("%10s PASSED\n", "LINKEDLIST");
    if (sum1 == 50005000)
        printf("%10s PASSED\n", "STACK");
    if (sum2 == 50005000)
        printf("%10s PASSED\n", "QUEUE");
    if (sum3 == 50005000)
        printf("%10s PASSED\n", "DEQUE");

    printf("\n\n");

    sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0;

    FOR_EACH_REV(ll, linked, , int, ll, {
        sum4 += var;
    })

    if (sum4 == 50005000)
        printf("%10s PASSED\n", "LINKEDLIST");
    else
        printf("%10s ERROR %d", "LINKEDLIST", sum4);

    l_free(l);
    ll_free(ll);
    s_free(s);
    q_free(q);
    d_free(d);

    return 0;
}
