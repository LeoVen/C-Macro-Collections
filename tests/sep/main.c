#include "sep.h"
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

    printf("+--------------------------------------------------+\n");
    printf("|       Separate Header and Source is Working      |\n");
    printf("+--------------------------------------------------+\n");

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