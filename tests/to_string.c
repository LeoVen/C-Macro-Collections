// A visual check for to_string methods

#include "../src/macro_collections.h"

CMC_COLLECTION_GENERATE(     BIDIMAP, bm,   bmap,     int, int)
CMC_COLLECTION_GENERATE(       DEQUE,  d,  deque, /* K */, int)
CMC_COLLECTION_GENERATE(     HASHMAP, hm,   hmap,     int, int)
CMC_COLLECTION_GENERATE(     HASHSET, hs,   hset, /* K */, int)
CMC_COLLECTION_GENERATE(        HEAP,  h,   heap, /* K */, int)
CMC_COLLECTION_GENERATE(INTERVALHEAP, ih,  iheap, /* K */, int)
CMC_COLLECTION_GENERATE(  LINKEDLIST, ll, linked, /* K */, int)
CMC_COLLECTION_GENERATE(        LIST,  l,   list, /* K */, int)
CMC_COLLECTION_GENERATE(    MULTIMAP, mm,   mmap,     int, int)
CMC_COLLECTION_GENERATE(    MULTISET, ms,   mset, /* K */, int)
CMC_COLLECTION_GENERATE(       QUEUE,  q,  queue, /* K */, int)
CMC_COLLECTION_GENERATE(  SORTEDLIST, sl,  slist, /* K */, int)
CMC_COLLECTION_GENERATE(       STACK,  s,  stack, /* K */, int)
CMC_COLLECTION_GENERATE(     TREESET, ts,   tset, /* K */, int)
CMC_COLLECTION_GENERATE(     TREEMAP, tm,   tmap,     int, int)

int main(void)
{
    struct bmap *bm = bm_new_custom(100, 0.7, NULL, NULL, NULL, NULL);
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

    printf("%s\n", bm_to_string(bm).s);
    printf("%s\n", d_to_string(d).s);
    printf("%s\n", hm_to_string(hm).s);
    printf("%s\n", hs_to_string(hs).s);
    printf("%s\n", h_to_string(h).s);
    printf("%s\n", ih_to_string(ih).s);
    printf("%s\n", ll_to_string(ll).s);
    printf("%s\n", l_to_string(l).s);
    printf("%s\n", mm_to_string(mm).s);
    printf("%s\n", ms_to_string(ms).s);
    printf("%s\n", q_to_string(q).s);
    printf("%s\n", sl_to_string(sl).s);
    printf("%s\n", s_to_string(s).s);
    printf("%s\n", tm_to_string(tm).s);
    printf("%s\n", ts_to_string(ts).s);

    bm_free(bm, NULL);
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
}
