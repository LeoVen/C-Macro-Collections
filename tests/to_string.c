// A visual check for to_string methods

#include "../src/macro_collections.h"

CMC_COLLECTION_GENERATE(BIDIMAP, bm, bmap, int, int)
CMC_COLLECTION_GENERATE(DEQUE, d, deque, /* K */, int)
CMC_COLLECTION_GENERATE(HASHMAP, hm, hmap, int, int)
CMC_COLLECTION_GENERATE(HASHSET, hs, hset, /* K */, int)
CMC_COLLECTION_GENERATE(HEAP, h, heap, /* K */, int)
CMC_COLLECTION_GENERATE(INTERVALHEAP, ih, iheap, /* K */, int)
CMC_COLLECTION_GENERATE(LINKEDLIST, ll, linked, /* K */, int)
CMC_COLLECTION_GENERATE(LIST, l, list, /* K */, int)
CMC_COLLECTION_GENERATE(MULTIMAP, mm, mmap, int, int)
CMC_COLLECTION_GENERATE(MULTISET, ms, mset, /* K */, int)
CMC_COLLECTION_GENERATE(QUEUE, q, queue, /* K */, int)
CMC_COLLECTION_GENERATE(SORTEDLIST, sl, slist, /* K */, int)
CMC_COLLECTION_GENERATE(STACK, s, stack, /* K */, int)
CMC_COLLECTION_GENERATE(TREESET, ts, tset, /* K */, int)
CMC_COLLECTION_GENERATE(TREEMAP, tm, tmap, int, int)

int main(void)
{
    struct bmap *bm = bm_new_custom(100, 0.7, &(struct bmap_ftab_key){ 0 },
                                    &(struct bmap_ftab_val){ 0 }, NULL, NULL);
    struct deque *d =
        d_new_custom(100, &(struct deque_ftab_val){ 0 }, NULL, NULL);
    struct hmap *hm = hm_new_custom(100, 0.6, &(struct hmap_ftab_key){ 0 },
                                    &(struct hmap_ftab_val){ 0 }, NULL, NULL);
    struct hset *hs =
        hs_new_custom(100, 0.6, &(struct hset_ftab_val){ 0 }, NULL, NULL);
    struct heap *h = h_new_custom(100, cmc_max_heap,
                                  &(struct heap_ftab_val){ 0 }, NULL, NULL);
    struct iheap *ih =
        ih_new_custom(100, &(struct iheap_ftab_val){ 0 }, NULL, NULL);
    struct linked *ll =
        ll_new_custom(&(struct linked_ftab_val){ 0 }, NULL, NULL);
    struct list *l =
        l_new_custom(100, &(struct list_ftab_val){ 0 }, NULL, NULL);
    struct mmap *mm = mm_new_custom(100, 0.8, &(struct mmap_ftab_key){ 0 },
                                    &(struct mmap_ftab_val){ 0 }, NULL, NULL);
    struct mset *ms =
        ms_new_custom(100, 0.6, &(struct mset_ftab_val){ 0 }, NULL, NULL);
    struct queue *q =
        q_new_custom(100, &(struct queue_ftab_val){ 0 }, NULL, NULL);
    struct slist *sl =
        sl_new_custom(100, &(struct slist_ftab_val){ 0 }, NULL, NULL);
    struct stack *s =
        s_new_custom(100, &(struct stack_ftab_val){ 0 }, NULL, NULL);
    struct tmap *tm = tm_new_custom(&(struct tmap_ftab_key){ 0 },
                                    &(struct tmap_ftab_val){ 0 }, NULL, NULL);
    struct tset *ts = ts_new_custom(&(struct tset_ftab_val){ 0 }, NULL, NULL);

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

    bm_free(bm);
    d_free(d);
    hm_free(hm);
    hs_free(hs);
    h_free(h);
    ih_free(ih);
    ll_free(ll);
    l_free(l);
    mm_free(mm);
    ms_free(ms);
    q_free(q);
    sl_free(sl);
    s_free(s);
    tm_free(tm);
    ts_free(ts);
}
