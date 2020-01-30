#include "collections.h"
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
    struct bmap *bm =
        bm_new_custom(100, 0.7, &(struct bmap_ftab_key){ 0 },
                      &(struct bmap_ftab_val){ 0 }, NULL, NULL);
    struct deque *d = d_new_custom(100, NULL, NULL);
    struct hmap *hm = hm_new_custom(100, 0.6, NULL, NULL, NULL, NULL);
    struct hset *hs = hs_new_custom(100, 0.6, NULL, NULL, NULL, NULL);
    struct heap *h = h_new_custom(100, cmc_max_heap, NULL, NULL, NULL);
    struct iheap *ih = ih_new_custom(100, NULL, NULL, NULL);
    struct linked *ll = ll_new_custom(NULL, NULL);
    struct list *l = l_new_custom(100, NULL, NULL);
    struct mmap *mm = mm_new_custom(100, 0.8, NULL, NULL, NULL, NULL);
    struct mset *ms = ms_new_custom(100, 0.6, NULL, NULL, NULL, NULL);
    struct queue *q = q_new_custom(100, NULL, NULL);
    struct slist *sl = sl_new_custom(100, NULL, NULL, NULL);
    struct stack *s = s_new_custom(100, NULL, NULL);
    struct tmap *tm = tm_new_custom(NULL, NULL, NULL);
    struct tset *ts = ts_new_custom(NULL, NULL, NULL);

    printf("+--------------------------------------------------+\n");
    printf("|       Separate Header and Source is Working      |\n");
    printf("+--------------------------------------------------+\n");

    bm_free(bm);
    d_free(d, NULL);
    hm_free(hm, NULL);
    hs_free(hs, NULL);
    h_free(h, NULL);
    ih_free(ih, NULL);
    ll_free(ll, NULL);
    l_free(l, NULL);
    mm_free(mm, NULL);
    ms_free(ms, NULL);
    q_free(q, NULL);
    sl_free(sl, NULL);
    s_free(s, NULL);
    tm_free(tm, NULL);
    ts_free(ts, NULL);

    return 0;
}
