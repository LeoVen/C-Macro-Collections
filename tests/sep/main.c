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

    ih_free(ih, NULL);
    mm_free(mm, NULL);
    ms_free(ms, NULL);

    return 0;
}
