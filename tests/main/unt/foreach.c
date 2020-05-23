#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "utl/foreach.h"

#include "../src/bitset.c"
#include "../src/deque.c"
#include "../src/hashbidimap.c"
#include "../src/hashmap.c"
#include "../src/hashmultimap.c"
#include "../src/hashmultiset.c"
#include "../src/hashset.c"
#include "../src/heap.c"
#include "../src/intervalheap.c"
#include "../src/linkedlist.c"
#include "../src/list.c"
#include "../src/queue.c"
#include "../src/sortedlist.c"
#include "../src/stack.c"
#include "../src/treemap.c"
#include "../src/treeset.c"

CMC_CREATE_UNIT(ForEach, true, {
    // CMC_CREATE_TEST(CMC<BitSet>, {
    //     struct bitset *bs = bs_new(10000);

    //     // WIP

    //     bs_free(bs);
    // });

    CMC_CREATE_TEST(CMC<Deque>, {
        struct deque *d =
            d_new(10000, &(struct deque_fval){ .cmp = cmc_size_cmp,
                                               .cpy = NULL,
                                               .str = cmc_size_str,
                                               .free = NULL,
                                               .hash = cmc_size_hash,
                                               .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            d_push_back(d, i);

        size_t sum = 0;
        CMC_FOREACH (d, deque, it, d)
        {
            sum += d_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sum);

        sum = 0;
        CMC_FOREACH_REV (d, deque, it, d)
        {
            sum += d_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sum);

        d_free(d);
    });

    // CMC_CREATE_TEST(CMC<HashBidiMap>, {
    //     struct hashbidimap *hbm =
    //         hbm_new(10000, 0.7,
    //                 &(struct hashbidimap_fkey){ .cmp = cmc_size_cmp,
    //                                             .cpy = NULL,
    //                                             .str = cmc_size_str,
    //                                             .free = NULL,
    //                                             .hash = cmc_size_hash,
    //                                             .pri = cmc_size_cmp },
    //                 &(struct hashbidimap_fval){ .cmp = cmc_size_cmp,
    //                                             .cpy = NULL,
    //                                             .str = cmc_size_str,
    //                                             .free = NULL,
    //                                             .hash = cmc_size_hash,
    //                                             .pri = cmc_size_cmp });

    //     // WIP

    //     hbm_free(hbm);
    // });

    CMC_CREATE_TEST(CMC<HashMap>, {
        struct hashmap *hm =
            hm_new(10000, 0.6,
                   &(struct hashmap_fkey){ .cmp = cmc_size_cmp,
                                           .cpy = NULL,
                                           .str = cmc_size_str,
                                           .free = NULL,
                                           .hash = cmc_size_hash,
                                           .pri = cmc_size_cmp },
                   &(struct hashmap_fval){ .cmp = cmc_size_cmp,
                                           .cpy = NULL,
                                           .str = cmc_size_str,
                                           .free = NULL,
                                           .hash = cmc_size_hash,
                                           .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            hm_insert(hm, i, i);

        size_t sumk = 0;
        size_t sumv = 0;
        CMC_FOREACH (hm, hashmap, it, hm)
        {
            sumk += hm_iter_key(&it);
            sumv += hm_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumk);
        cmc_assert_equals(size_t, 50005000, sumv);

        sumk = 0;
        sumv = 0;
        CMC_FOREACH_REV (hm, hashmap, it, hm)
        {
            sumk += hm_iter_key(&it);
            sumv += hm_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumk);
        cmc_assert_equals(size_t, 50005000, sumv);

        hm_free(hm);
    });

    CMC_CREATE_TEST(CMC<HashMultiMap>, {
        struct hashmultimap *hmm =
            hmm_new(10000, 0.6,
                    &(struct hashmultimap_fkey){ .cmp = cmc_size_cmp,
                                                 .cpy = NULL,
                                                 .str = cmc_size_str,
                                                 .free = NULL,
                                                 .hash = cmc_size_hash,
                                                 .pri = cmc_size_cmp },
                    &(struct hashmultimap_fval){ .cmp = cmc_size_cmp,
                                                 .cpy = NULL,
                                                 .str = cmc_size_str,
                                                 .free = NULL,
                                                 .hash = cmc_size_hash,
                                                 .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            hmm_insert(hmm, i, i);

        size_t sumk = 0;
        size_t sumv = 0;
        CMC_FOREACH (hmm, hashmultimap, it, hmm)
        {
            sumk += hmm_iter_key(&it);
            sumv += hmm_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumk);
        cmc_assert_equals(size_t, 50005000, sumv);

        sumk = 0;
        sumv = 0;
        CMC_FOREACH_REV (hmm, hashmultimap, it, hmm)
        {
            sumk += hmm_iter_key(&it);
            sumv += hmm_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumk);
        cmc_assert_equals(size_t, 50005000, sumv);

        hmm_free(hmm);
    });

    CMC_CREATE_TEST(CMC<HashMultiSet>, {
        struct hashmultiset *hms =
            hms_new(10000, 0.6,
                    &(struct hashmultiset_fval){ .cmp = cmc_size_cmp,
                                                 .cpy = NULL,
                                                 .str = cmc_size_str,
                                                 .free = NULL,
                                                 .hash = cmc_size_hash,
                                                 .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            hms_insert_many(hms, i, i);

        size_t sumk = 0;
        size_t sumv = 0;
        CMC_FOREACH (hms, hashmultiset, it, hms)
        {
            sumk += hms_iter_value(&it);
            sumv += hms_iter_multiplicity(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumk);
        cmc_assert_equals(size_t, 50005000, sumv);

        sumk = 0;
        sumv = 0;
        CMC_FOREACH_REV (hms, hashmultiset, it, hms)
        {
            sumk += hms_iter_value(&it);
            sumv += hms_iter_multiplicity(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumk);
        cmc_assert_equals(size_t, 50005000, sumv);

        hms_free(hms);
    });

    CMC_CREATE_TEST(CMC<HashSet>, {
        struct hashset *hs =
            hs_new(10000, 0.6,
                   &(struct hashset_fval){ .cmp = cmc_size_cmp,
                                           .cpy = NULL,
                                           .str = cmc_size_str,
                                           .free = NULL,
                                           .hash = cmc_size_hash,
                                           .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            hs_insert(hs, i);

        size_t sumv = 0;
        CMC_FOREACH (hs, hashset, it, hs)
        {
            sumv += hs_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        sumv = 0;
        CMC_FOREACH_REV (hs, hashset, it, hs)
        {
            sumv += hs_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        hs_free(hs);
    });

    CMC_CREATE_TEST(CMC<Heap>, {
        struct heap *h = h_new(10000, cmc_max_heap,
                               &(struct heap_fval){ .cmp = cmc_size_cmp,
                                                    .cpy = NULL,
                                                    .str = cmc_size_str,
                                                    .free = NULL,
                                                    .hash = cmc_size_hash,
                                                    .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            h_insert(h, i);

        size_t sumv = 0;
        CMC_FOREACH (h, heap, it, h)
        {
            sumv += h_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        sumv = 0;
        CMC_FOREACH_REV (h, heap, it, h)
        {
            sumv += h_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        h_free(h);
    });

    CMC_CREATE_TEST(CMC<IntervalHeap>, {
        struct intervalheap *ih =
            ih_new(10000, &(struct intervalheap_fval){ .cmp = cmc_size_cmp,
                                                       .cpy = NULL,
                                                       .str = cmc_size_str,
                                                       .free = NULL,
                                                       .hash = cmc_size_hash,
                                                       .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            ih_insert(ih, i);

        size_t sumv = 0;
        CMC_FOREACH (ih, intervalheap, it, ih)
        {
            sumv += ih_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        sumv = 0;
        CMC_FOREACH_REV (ih, intervalheap, it, ih)
        {
            sumv += ih_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        ih_free(ih);
    });

    CMC_CREATE_TEST(CMC<LinkedList>, {
        struct linkedlist *ll =
            ll_new(&(struct linkedlist_fval){ .cmp = cmc_size_cmp,
                                              .cpy = NULL,
                                              .str = cmc_size_str,
                                              .free = NULL,
                                              .hash = cmc_size_hash,
                                              .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            ll_push_back(ll, i);

        size_t sumv = 0;
        CMC_FOREACH (ll, linkedlist, it, ll)
        {
            sumv += ll_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        sumv = 0;
        CMC_FOREACH_REV (ll, linkedlist, it, ll)
        {
            sumv += ll_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        ll_free(ll);
    });

    CMC_CREATE_TEST(CMC<List>, {
        struct list *l =
            l_new(10000, &(struct list_fval){ .cmp = cmc_size_cmp,
                                              .cpy = NULL,
                                              .str = cmc_size_str,
                                              .free = NULL,
                                              .hash = cmc_size_hash,
                                              .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            l_push_back(l, i);

        size_t sumv = 0;
        CMC_FOREACH (l, list, it, l)
        {
            sumv += l_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        sumv = 0;
        CMC_FOREACH_REV (l, list, it, l)
        {
            sumv += l_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        l_free(l);
    });

    CMC_CREATE_TEST(CMC<Queue>, {
        struct queue *q =
            q_new(10000, &(struct queue_fval){ .cmp = cmc_size_cmp,
                                               .cpy = NULL,
                                               .str = cmc_size_str,
                                               .free = NULL,
                                               .hash = cmc_size_hash,
                                               .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            q_enqueue(q, i);

        size_t sumv = 0;
        CMC_FOREACH (q, queue, it, q)
        {
            sumv += q_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        sumv = 0;
        CMC_FOREACH_REV (q, queue, it, q)
        {
            sumv += q_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        q_free(q);
    });

    CMC_CREATE_TEST(CMC<SortedList>, {
        struct sortedlist *sl =
            sl_new(10000, &(struct sortedlist_fval){ .cmp = cmc_size_cmp,
                                                     .cpy = NULL,
                                                     .str = cmc_size_str,
                                                     .free = NULL,
                                                     .hash = cmc_size_hash,
                                                     .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            sl_insert(sl, i);

        size_t sumv = 0;
        CMC_FOREACH (sl, sortedlist, it, sl)
        {
            sumv += sl_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        sumv = 0;
        CMC_FOREACH_REV (sl, sortedlist, it, sl)
        {
            sumv += sl_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        sl_free(sl);
    });

    CMC_CREATE_TEST(CMC<Stack>, {
        struct stack *s =
            s_new(10000, &(struct stack_fval){ .cmp = cmc_size_cmp,
                                               .cpy = NULL,
                                               .str = cmc_size_str,
                                               .free = NULL,
                                               .hash = cmc_size_hash,
                                               .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            s_push(s, i);

        size_t sumv = 0;
        CMC_FOREACH (s, stack, it, s)
        {
            sumv += s_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        sumv = 0;
        CMC_FOREACH_REV (s, stack, it, s)
        {
            sumv += s_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        s_free(s);
    });

    CMC_CREATE_TEST(CMC<TreeMap>, {
        struct treemap *tm =
            tm_new(&(struct treemap_fkey){ .cmp = cmc_size_cmp,
                                           .cpy = NULL,
                                           .str = cmc_size_str,
                                           .free = NULL,
                                           .hash = cmc_size_hash,
                                           .pri = cmc_size_cmp },
                   &(struct treemap_fval){ .cmp = cmc_size_cmp,
                                           .cpy = NULL,
                                           .str = cmc_size_str,
                                           .free = NULL,
                                           .hash = cmc_size_hash,
                                           .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            tm_insert(tm, i, i);

        size_t sumk = 0;
        size_t sumv = 0;
        CMC_FOREACH (tm, treemap, it, tm)
        {
            sumk += tm_iter_key(&it);
            sumv += tm_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumk);
        cmc_assert_equals(size_t, 50005000, sumv);

        sumk = 0;
        sumv = 0;
        CMC_FOREACH_REV (tm, treemap, it, tm)
        {
            sumk += tm_iter_key(&it);
            sumv += tm_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumk);
        cmc_assert_equals(size_t, 50005000, sumv);

        tm_free(tm);
    });

    CMC_CREATE_TEST(CMC<TreeSet>, {
        struct treeset *ts =
            ts_new(&(struct treeset_fval){ .cmp = cmc_size_cmp,
                                           .cpy = NULL,
                                           .str = cmc_size_str,
                                           .free = NULL,
                                           .hash = cmc_size_hash,
                                           .pri = cmc_size_cmp });

        for (size_t i = 1; i <= 10000; i++)
            ts_insert(ts, i);

        size_t sumv = 0;
        CMC_FOREACH (ts, treeset, it, ts)
        {
            sumv += ts_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        sumv = 0;
        CMC_FOREACH_REV (ts, treeset, it, ts)
        {
            sumv += ts_iter_value(&it);
        }
        cmc_assert_equals(size_t, 50005000, sumv);

        ts_free(ts);
    });
});

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = ForEach();

    printf(
        " +---------------------------------------------------------------+");
    printf("\n");
    printf(" | ForEach Suit : %-46s |\n", result == 0 ? "PASSED" : "FAILED");
    printf(
        " +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif
