#ifndef CMC_TESTS_UNT_TREESET_H
#define CMC_TESTS_UNT_TREESET_H

#include "utl.h"

#define V size_t
#define PFX ts
#define SNAME treeset
#include "cmc/treeset.h"

struct treeset_fval *ts_fval = &(struct treeset_fval){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

CMC_CREATE_UNIT(CMCTreeSet, true, {
    CMC_CREATE_TEST(new, {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        ts_free(set);
    });

    CMC_CREATE_TEST(clear[count], {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 50; i++)
            ts_insert(set, i);

        cmc_assert_equals(size_t, 50, ts_count(set));

        ts_clear(set);

        cmc_assert_equals(size_t, 0, ts_count(set));
        cmc_assert_equals(ptr, NULL, set->root);

        ts_free(set);
    });

    CMC_CREATE_TEST(flags, {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ts_flag(set));

        // clear
        set->flag = CMC_FLAG_ERROR;
        ts_clear(set);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ts_flag(set));

        // insert
        set->flag = CMC_FLAG_ERROR;
        cmc_assert(ts_insert(set, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ts_flag(set));

        cmc_assert(!ts_insert(set, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_DUPLICATE, ts_flag(set));

        // remove
        cmc_assert(!ts_remove(set, 2));
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, ts_flag(set));

        cmc_assert(ts_remove(set, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ts_flag(set));

        cmc_assert(!ts_remove(set, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, ts_flag(set));

        // max min
        set->flag = CMC_FLAG_ERROR;
        cmc_assert(!ts_max(set, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, ts_flag(set));

        set->flag = CMC_FLAG_ERROR;
        cmc_assert(!ts_min(set, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, ts_flag(set));

        cmc_assert(ts_insert(set, 1));
        set->flag = CMC_FLAG_ERROR;
        cmc_assert(ts_max(set, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ts_flag(set));

        set->flag = CMC_FLAG_ERROR;
        cmc_assert(ts_min(set, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ts_flag(set));

        // copy_of
        set->flag = CMC_FLAG_ERROR;
        struct treeset *set2 = ts_copy_of(set);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ts_flag(set));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ts_flag(set2));

        // equals
        set->flag = CMC_FLAG_ERROR;
        set2->flag = CMC_FLAG_ERROR;
        cmc_assert(ts_equals(set, set2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ts_flag(set));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ts_flag(set2));

        ts_free(set);
        ts_free(set2);
    });
});

CMC_CREATE_UNIT(CMCTreeSetIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_start(set);

        cmc_assert_equals(ptr, set, it.target);
        cmc_assert_equals(ptr, NULL, it.cursor);
        cmc_assert_equals(ptr, NULL, it.first);
        cmc_assert_equals(ptr, NULL, it.last);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(ts_iter_at_start(&it));
        cmc_assert(ts_iter_at_end(&it));

        cmc_assert(ts_insert(set, 1));
        cmc_assert(ts_insert(set, 2));
        cmc_assert(ts_insert(set, 3));

        it = ts_iter_start(set);

        cmc_assert_equals(size_t, 0, it.index);

        cmc_assert_equals(size_t, 1, it.cursor->value);
        cmc_assert_equals(size_t, 1, it.first->value);
        cmc_assert_equals(size_t, 3, it.last->value);
        cmc_assert_equals(bool, false, it.end);

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_end(set);

        cmc_assert_equals(ptr, set, it.target);
        cmc_assert_equals(ptr, NULL, it.cursor);
        cmc_assert_equals(ptr, NULL, it.first);
        cmc_assert_equals(ptr, NULL, it.last);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(ts_iter_at_start(&it));
        cmc_assert(ts_iter_at_end(&it));

        cmc_assert(ts_insert(set, 1));
        cmc_assert(ts_insert(set, 2));
        cmc_assert(ts_insert(set, 3));

        it = ts_iter_end(set);

        cmc_assert_equals(size_t, set->count - 1, it.index);

        cmc_assert_equals(size_t, 3, it.cursor->value);
        cmc_assert_equals(size_t, 1, it.first->value);
        cmc_assert_equals(size_t, 3, it.last->value);
        cmc_assert_equals(bool, false, it.start);

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_start(set);

        // Empty checks
        cmc_assert(ts_iter_at_start(&it));
        it = ts_iter_end(set);
        cmc_assert(ts_iter_at_start(&it));

        // Non-empty checks
        cmc_assert(ts_insert(set, 1));
        cmc_assert(ts_insert(set, 2));
        it = ts_iter_end(set);
        cmc_assert(!ts_iter_at_start(&it));
        it = ts_iter_start(set);
        cmc_assert(ts_iter_at_start(&it));

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_start(set);

        // Empty check
        cmc_assert(ts_iter_at_end(&it));
        it = ts_iter_end(set);
        cmc_assert(ts_iter_at_end(&it));

        // Non-empty checks
        cmc_assert(ts_insert(set, 1));
        cmc_assert(ts_insert(set, 2));
        it = ts_iter_end(set);
        cmc_assert(ts_iter_at_end(&it));
        it = ts_iter_start(set);
        cmc_assert(!ts_iter_at_end(&it));

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_start(set);

        cmc_assert(!ts_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
            ts_insert(set, i);

        cmc_assert_equals(size_t, 100, set->count);

        it = ts_iter_end(set);

        cmc_assert(!ts_iter_at_start(&it));
        cmc_assert(ts_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, ts_iter_value(&it));

        cmc_assert(ts_iter_to_start(&it));

        cmc_assert(ts_iter_at_start(&it));
        cmc_assert(!ts_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, ts_iter_value(&it));

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_end(set);

        cmc_assert(!ts_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
            ts_insert(set, i);

        it = ts_iter_start(set);

        cmc_assert(ts_iter_at_start(&it));
        cmc_assert(!ts_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, ts_iter_value(&it));

        cmc_assert(ts_iter_to_end(&it));

        cmc_assert(!ts_iter_at_start(&it));
        cmc_assert(ts_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, ts_iter_value(&it));

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_start(set);

        cmc_assert(!ts_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
            ts_insert(set, i);

        size_t sum = 0;
        for (it = ts_iter_start(set); !ts_iter_at_end(&it); ts_iter_next(&it))
        {
            sum += ts_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        ts_iter_to_start(&it);
        do
        {
            sum += ts_iter_value(&it);
        } while (ts_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_end(set);

        cmc_assert(!ts_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
            ts_insert(set, i);

        size_t sum = 0;
        for (it = ts_iter_end(set); !ts_iter_at_start(&it); ts_iter_prev(&it))
        {
            sum += ts_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        ts_iter_to_end(&it);
        do
        {
            sum += ts_iter_value(&it);
        } while (ts_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_start(set);

        cmc_assert(!ts_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            ts_insert(set, i);

        it = ts_iter_start(set);

        cmc_assert(!ts_iter_advance(&it, 0));
        cmc_assert(!ts_iter_advance(&it, set->count));

        size_t sum = 0;
        for (it = ts_iter_start(set);;)
        {
            sum += ts_iter_value(&it);

            if (!ts_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        ts_iter_to_start(&it);
        cmc_assert(ts_iter_advance(&it, set->count - 1));

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_end(set);

        cmc_assert(!ts_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            ts_insert(set, i);

        it = ts_iter_end(set);

        cmc_assert(!ts_iter_rewind(&it, 0));
        cmc_assert(!ts_iter_rewind(&it, set->count));

        size_t sum = 0;
        for (it = ts_iter_end(set);;)
        {
            sum += ts_iter_value(&it);

            if (!ts_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        ts_iter_to_end(&it);
        cmc_assert(ts_iter_rewind(&it, set->count - 1));

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_end(set);
        cmc_assert(!ts_iter_go_to(&it, 0));

        it = ts_iter_start(set);
        cmc_assert(!ts_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            ts_insert(set, i);

        it = ts_iter_start(set);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            ts_iter_go_to(&it, i);

            sum += ts_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(ts_iter_go_to(&it, i - 1));

            sum += ts_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 0; i < 1001; i += 100)
        {
            cmc_assert(ts_iter_go_to(&it, i));
            cmc_assert_equals(size_t, i, ts_iter_index(&it));

            sum += ts_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct treeset_iter it = ts_iter_end(set);

        cmc_assert_equals(size_t, (size_t){ 0 }, ts_iter_value(&it));

        cmc_assert(ts_insert(set, 10));

        it = ts_iter_start(set);

        cmc_assert_equals(size_t, 10, ts_iter_value(&it));

        ts_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i <= 1000; i++)
            ts_insert(set, i);

        struct treeset_iter it = ts_iter_start(set);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, ts_iter_index(&it));
            ts_iter_next(&it);
        }

        it = ts_iter_end(set);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, ts_iter_index(&it));
            ts_iter_prev(&it);
        }

        ts_free(set);
    });
});

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = CMCTreeSet() + CMCTreeSetIter();

    printf(" +---------------------------------------------------------------+");
    printf("\n");
    printf(" | CMCTreeSet Suit : %-46s |\n", result == 0 ? "PASSED" : "FAILED");
    printf(" +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif

#endif /* CMC_TESTS_UNT_TREESET_H */
