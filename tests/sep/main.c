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
    heap *h = h_new(1000, cmc_max_heap, intcmp);
    tset *ts = ts_new(intcmp);
    tmap *tm = tm_new(intcmp);
    hset *hs = hs_new(1000, 0.9, intcmp, inthash);
    hmap *hm = hm_new(1000, 0.9, intcmp, inthash);

    printf("+--------------------------------------------------+\n");
    printf("|       Separate Header and Source is Working      |\n");
    printf("+--------------------------------------------------+\n");

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

    return 0;
}
