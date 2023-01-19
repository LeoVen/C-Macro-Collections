// A visual check for to_string methods

#include "cmc/utl/futils.h"

struct
{
    CMC_DEF_FTAB_CMP(size_t);
    CMC_DEF_FTAB_CPY(size_t);
    CMC_DEF_FTAB_STR(size_t);
    CMC_DEF_FTAB_FREE(size_t);
    CMC_DEF_FTAB_HASH(size_t);
    CMC_DEF_FTAB_PRI(size_t);
} int_ftab = { cmc_size_cmp, NULL, cmc_size_str, NULL, cmc_size_hash, cmc_size_cmp };

int cmc_unt_tostring(void)
{
    // clang-format off
    printf(" +---------------------------------------------------------------+\n");
    printf(" |                  C Macro Collections Library                  |\n");
    printf(" |                                                               |\n");
    printf(" |                      Print Visual Test                        |\n");
    printf(" |                                                               |\n");
    printf(" | https://github.com/LeoVen/C-Macro-Collections                 |\n");
    printf(" | Leonardo Vencovsky (leonardo.vencovsky@gmail.com)             |\n");
    printf(" +---------------------------------------------------------------+\n\n");
    // clang-format on

    struct deque *d = d_new(100, (struct deque_fval *)&int_ftab);
    struct hashbidimap *hbm =
        hbm_new(100, 0.8, (struct hashbidimap_fkey *)&int_ftab, (struct hashbidimap_fval *)&int_ftab);
    struct hashmap *hm = hm_new(100, 0.6, (struct hashmap_fkey *)&int_ftab, (struct hashmap_fval *)&int_ftab);
    struct hashmultimap *hmm =
        hmm_new(100, 0.8, (struct hashmultimap_fkey *)&int_ftab, (struct hashmultimap_fval *)&int_ftab);
    struct hashmultiset *hms = hms_new(100, 0.6, (struct hashmultiset_fval *)&int_ftab);
    struct hashset *hs = hs_new(100, 0.6, (struct hashset_fval *)&int_ftab);
    struct heap *h = h_new(100, CMC_MAX_HEAP, (struct heap_fval *)&int_ftab);
    struct intervalheap *ih = ih_new(100, (struct intervalheap_fval *)&int_ftab);
    struct linkedlist *ll = ll_new((struct linkedlist_fval *)&int_ftab);
    struct list *l = l_new(100, (struct list_fval *)&int_ftab);
    struct queue *q = q_new(100, (struct queue_fval *)&int_ftab);
    struct sortedlist *sl = sl_new(100, (struct sortedlist_fval *)&int_ftab);
    struct stack *s = s_new(100, (struct stack_fval *)&int_ftab);
    struct treemap *tm = tm_new((struct treemap_fkey *)&int_ftab, (struct treemap_fval *)&int_ftab);
    struct treeset *ts = ts_new((struct treeset_fval *)&int_ftab);

    d_to_string(d, stdout);
    d_print(d, stdout, "\n[ ", ", ", " ]\n");
    hbm_to_string(hbm, stdout);
    hbm_print(hbm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hm_to_string(hm, stdout);
    hm_print(hm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hmm_to_string(hmm, stdout);
    hmm_print(hmm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hms_to_string(hms, stdout);
    hms_print(hms, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hs_to_string(hs, stdout);
    hs_print(hs, stdout, "\n[ ", ", ", " ]\n");
    h_to_string(h, stdout);
    h_print(h, stdout, "\n[ ", ", ", " ]\n");
    ih_to_string(ih, stdout);
    ih_print(ih, stdout, "\n[ ", ", ", " ]\n");
    ll_to_string(ll, stdout);
    ll_print(ll, stdout, "\n[ ", ", ", " ]\n");
    l_to_string(l, stdout);
    l_print(l, stdout, "\n[ ", ", ", " ]\n");
    q_to_string(q, stdout);
    q_print(q, stdout, "\n[ ", ", ", " ]\n");
    sl_to_string(sl, stdout);
    sl_print(sl, stdout, "\n[ ", ", ", " ]\n");
    s_to_string(s, stdout);
    s_print(s, stdout, "\n[ ", ", ", " ]\n");
    tm_to_string(tm, stdout);
    tm_print(tm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    ts_to_string(ts, stdout);
    ts_print(ts, stdout, "\n[ ", ", ", " ]\n");

    fprintf(stdout, "\n");

    d_push_back(d, 0);
    hbm_insert(hbm, 0, 0);
    hm_insert(hm, 0, 0);
    hmm_insert(hmm, 0, 0);
    hms_insert(hms, 0);
    hs_insert(hs, 0);
    h_insert(h, 0);
    ih_insert(ih, 0);
    ll_push_back(ll, 0);
    l_push_back(l, 0);
    q_enqueue(q, 0);
    sl_insert(sl, 0);
    s_push(s, 0);
    tm_insert(tm, 0, 0);
    ts_insert(ts, 0);

    d_to_string(d, stdout);
    d_print(d, stdout, "\n[ ", ", ", " ]\n");
    hbm_to_string(hbm, stdout);
    hbm_print(hbm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hm_to_string(hm, stdout);
    hm_print(hm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hmm_to_string(hmm, stdout);
    hmm_print(hmm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hms_to_string(hms, stdout);
    hms_print(hms, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hs_to_string(hs, stdout);
    hs_print(hs, stdout, "\n[ ", ", ", " ]\n");
    h_to_string(h, stdout);
    h_print(h, stdout, "\n[ ", ", ", " ]\n");
    ih_to_string(ih, stdout);
    ih_print(ih, stdout, "\n[ ", ", ", " ]\n");
    ll_to_string(ll, stdout);
    ll_print(ll, stdout, "\n[ ", ", ", " ]\n");
    l_to_string(l, stdout);
    l_print(l, stdout, "\n[ ", ", ", " ]\n");
    q_to_string(q, stdout);
    q_print(q, stdout, "\n[ ", ", ", " ]\n");
    sl_to_string(sl, stdout);
    sl_print(sl, stdout, "\n[ ", ", ", " ]\n");
    s_to_string(s, stdout);
    s_print(s, stdout, "\n[ ", ", ", " ]\n");
    tm_to_string(tm, stdout);
    tm_print(tm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    ts_to_string(ts, stdout);
    ts_print(ts, stdout, "\n[ ", ", ", " ]\n");

    for (size_t i = 1; i < 20; i++)
    {
        d_push_back(d, i);
        hbm_insert(hbm, i, i);
        hm_insert(hm, i, i);
        hmm_insert(hmm, i, i);
        hms_insert(hms, i);
        hs_insert(hs, i);
        h_insert(h, i);
        ih_insert(ih, i);
        ll_push_back(ll, i);
        l_push_back(l, i);
        q_enqueue(q, i);
        sl_insert(sl, i);
        s_push(s, i);
        tm_insert(tm, i, i);
        ts_insert(ts, i);
    }

    fprintf(stdout, "\n");

    d_to_string(d, stdout);
    d_print(d, stdout, "\n[ ", ", ", " ]\n");
    hbm_to_string(hbm, stdout);
    hbm_print(hbm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hm_to_string(hm, stdout);
    hm_print(hm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hmm_to_string(hmm, stdout);
    hmm_print(hmm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hms_to_string(hms, stdout);
    hms_print(hms, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hs_to_string(hs, stdout);
    hs_print(hs, stdout, "\n[ ", ", ", " ]\n");
    h_to_string(h, stdout);
    h_print(h, stdout, "\n[ ", ", ", " ]\n");
    ih_to_string(ih, stdout);
    ih_print(ih, stdout, "\n[ ", ", ", " ]\n");
    ll_to_string(ll, stdout);
    ll_print(ll, stdout, "\n[ ", ", ", " ]\n");
    l_to_string(l, stdout);
    l_print(l, stdout, "\n[ ", ", ", " ]\n");
    q_to_string(q, stdout);
    q_print(q, stdout, "\n[ ", ", ", " ]\n");
    sl_to_string(sl, stdout);
    sl_print(sl, stdout, "\n[ ", ", ", " ]\n");
    s_to_string(s, stdout);
    s_print(s, stdout, "\n[ ", ", ", " ]\n");
    tm_to_string(tm, stdout);
    tm_print(tm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    ts_to_string(ts, stdout);
    ts_print(ts, stdout, "\n[ ", ", ", " ]\n");

    d_free(d);
    hbm_free(hbm);
    hm_free(hm);
    hmm_free(hmm);
    hms_free(hms);
    hs_free(hs);
    h_free(h);
    ih_free(ih);
    ll_free(ll);
    l_free(l);
    q_free(q);
    sl_free(sl);
    s_free(s);
    tm_free(tm);
    ts_free(ts);

    return 0;
}
