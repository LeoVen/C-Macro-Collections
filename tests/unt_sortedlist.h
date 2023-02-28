#ifndef CMC_TESTS_UNT_SORTEDLIST_H
#define CMC_TESTS_UNT_SORTEDLIST_H

#include "utl.h"

#define V size_t
#define PFX sl
#define SNAME sortedlist
#include "cmc/sortedlist.h"

struct sortedlist_fval *sl_fval = &(struct sortedlist_fval){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

CMC_CREATE_UNIT(CMCSortedList, true, {
    CMC_CREATE_TEST(new, {
        struct sortedlist *sl = sl_new(1000000, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);
        cmc_assert_not_equals(ptr, NULL, sl->buffer);
        cmc_assert_equals(size_t, 1000000, sl_capacity(sl));
        cmc_assert_equals(size_t, 0, sl_count(sl));
        cmc_assert_not_equals(ptr, NULL, sl->buffer);

        sl_free(sl);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        struct sortedlist *sl = sl_new(0, sl_fval);

        cmc_assert_equals(ptr, NULL, sl);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        struct sortedlist *sl = sl_new(UINT64_MAX, sl_fval);

        cmc_assert_equals(ptr, NULL, sl);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(sl_insert(sl, i));

        cmc_assert_equals(size_t, 50, sl_count(sl));

        sl_clear(sl);

        cmc_assert_equals(size_t, 0, sl_count(sl));
        cmc_assert_equals(size_t, 100, sl_capacity(sl));

        sl_free(sl);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct sortedlist *sl = sl_new(1, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(sl_insert(sl, i));

        sl_sort(sl);

        cmc_assert_equals(size_t, 50, sl_count(sl));
        cmc_assert_array_sorted_any(size_t, sl->buffer, cmc_size_cmp, 0, sl->count - 1);

        sl_free(sl);
    });

    CMC_CREATE_TEST(insert, {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        cmc_assert(sl_insert(sl, 1));

        sl_free(sl);
    });

    CMC_CREATE_TEST(remove, {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        cmc_assert(sl_insert(sl, 32));

        cmc_assert(sl_remove(sl, 0));

        sl_free(sl);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        cmc_assert(!sl_remove(sl, 0));

        sl_free(sl);
    });

    CMC_CREATE_TEST(remove[out of range], {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        cmc_assert(sl_insert(sl, 32));

        cmc_assert(!sl_remove(sl, 2));

        sl_free(sl);
    });

    CMC_CREATE_TEST(index_of, {
        struct sortedlist *sl = sl_new(1, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        for (size_t i = 0; i < 20; i++)
            cmc_assert(sl_insert(sl, i % 10));

        /* 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 */
        cmc_assert_equals(size_t, 4, sl_index_of(sl, 2, true));
        cmc_assert_equals(size_t, 5, sl_index_of(sl, 2, false));

        cmc_assert_equals(size_t, 0, sl_index_of(sl, 0, true));
        cmc_assert_equals(size_t, 1, sl_index_of(sl, 0, false));

        cmc_assert(sl_remove(sl, 0));

        cmc_assert_equals(size_t, 0, sl_index_of(sl, 0, true));
        cmc_assert_equals(size_t, 0, sl_index_of(sl, 0, false));

        cmc_assert(sl_remove(sl, 0));

        cmc_assert_equals(size_t, sl_count(sl), sl_index_of(sl, 0, true));
        cmc_assert_equals(size_t, sl_count(sl), sl_index_of(sl, 0, false));

        sl_free(sl);
    });

    CMC_CREATE_TEST(flags, {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));

        // clear
        sl->flag = CMC_FLAG_ERROR;
        sl_clear(sl);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));

        // customize
        sl->flag = CMC_FLAG_ERROR;
        sl_customize(sl, NULL, NULL);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));

        // insert
        sl->flag = CMC_FLAG_ERROR;
        cmc_assert(sl_insert(sl, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));

        // remove
        cmc_assert(!sl_remove(sl, 2));
        cmc_assert_equals(int32_t, CMC_FLAG_RANGE, sl_flag(sl));

        cmc_assert(sl_remove(sl, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));

        cmc_assert(!sl_remove(sl, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, sl_flag(sl));

        // max min
        sl->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, (size_t){ 0 }, sl_max(sl));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, sl_flag(sl));
        sl->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, (size_t){ 0 }, sl_min(sl));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, sl_flag(sl));

        cmc_assert(sl_insert(sl, 1));
        sl->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, 1, sl_max(sl));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));
        sl->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, 1, sl_min(sl));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));

        // get
        sl->flag = CMC_FLAG_ERROR;
        sl_get(sl, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_RANGE, sl_flag(sl));

        sl_get(sl, 0);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));

        cmc_assert(sl_remove(sl, 0));
        sl_get(sl, 0);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, sl_flag(sl));

        // index_of
        cmc_assert(sl_insert(sl, 1));
        sl_index_of(sl, 1, false);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));

        // contains
        cmc_assert(sl_contains(sl, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));

        // sort
        sl->flag = CMC_FLAG_ERROR;
        sl_sort(sl);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));

        // copy_of
        sl->flag = CMC_FLAG_ERROR;
        struct sortedlist *sl2 = sl_copy_of(sl);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl2));

        // equals
        sl->flag = CMC_FLAG_ERROR;
        sl2->flag = CMC_FLAG_ERROR;
        cmc_assert(sl_equals(sl, sl2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, sl_flag(sl2));

        sl_free(sl);
        sl_free(sl2);
    });
});

CMC_CREATE_UNIT(CMCSortedListIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_start(sl);

        cmc_assert_equals(ptr, sl, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(sl_iter_at_start(&it));
        cmc_assert(sl_iter_at_end(&it));

        cmc_assert(sl_insert(sl, 1));
        cmc_assert(sl_insert(sl, 2));

        it = sl_iter_start(sl);

        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, false, it.end);

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_end(sl);

        cmc_assert_equals(ptr, sl, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(sl_iter_at_start(&it));
        cmc_assert(sl_iter_at_end(&it));

        cmc_assert(sl_insert(sl, 1));
        cmc_assert(sl_insert(sl, 2));
        cmc_assert(sl_insert(sl, 3));

        it = sl_iter_end(sl);

        cmc_assert_equals(size_t, sl->count - 1, it.cursor);
        cmc_assert_equals(bool, false, it.start);

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_start(sl);

        // Empty checks
        cmc_assert(sl_iter_at_start(&it));
        it = sl_iter_end(sl);
        cmc_assert(sl_iter_at_start(&it));

        // Non-empty checks
        cmc_assert(sl_insert(sl, 1));
        it = sl_iter_end(sl);
        cmc_assert(!sl_iter_at_start(&it));
        it = sl_iter_start(sl);
        cmc_assert(sl_iter_at_start(&it));

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_start(sl);

        // Empty check
        cmc_assert(sl_iter_at_end(&it));
        it = sl_iter_end(sl);
        cmc_assert(sl_iter_at_end(&it));

        // Non-empty checks
        cmc_assert(sl_insert(sl, 1));
        it = sl_iter_end(sl);
        cmc_assert(sl_iter_at_end(&it));
        it = sl_iter_start(sl);
        cmc_assert(!sl_iter_at_end(&it));

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_start(sl);

        cmc_assert(!sl_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
            sl_insert(sl, i);

        cmc_assert_equals(size_t, 100, sl->count);

        it = sl_iter_end(sl);

        cmc_assert(!sl_iter_at_start(&it));
        cmc_assert(sl_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, sl_iter_value(&it));

        cmc_assert(sl_iter_to_start(&it));

        cmc_assert(sl_iter_at_start(&it));
        cmc_assert(!sl_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, sl_iter_value(&it));

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_end(sl);

        cmc_assert(!sl_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
            sl_insert(sl, i);

        it = sl_iter_start(sl);

        cmc_assert(sl_iter_at_start(&it));
        cmc_assert(!sl_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, sl_iter_value(&it));

        cmc_assert(sl_iter_to_end(&it));

        cmc_assert(!sl_iter_at_start(&it));
        cmc_assert(sl_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, sl_iter_value(&it));

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_start(sl);

        cmc_assert(!sl_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
            sl_insert(sl, i);

        size_t sum = 0;
        for (it = sl_iter_start(sl); !sl_iter_at_end(&it); sl_iter_next(&it))
        {
            sum += sl_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        sl_iter_to_start(&it);
        do
        {
            sum += sl_iter_value(&it);
        } while (sl_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_end(sl);

        cmc_assert(!sl_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
            sl_insert(sl, i);

        size_t sum = 0;
        for (it = sl_iter_end(sl); !sl_iter_at_start(&it); sl_iter_prev(&it))
        {
            sum += sl_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        sl_iter_to_end(&it);
        do
        {
            sum += sl_iter_value(&it);
        } while (sl_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_start(sl);

        cmc_assert(!sl_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            sl_insert(sl, i);

        it = sl_iter_start(sl);

        cmc_assert(!sl_iter_advance(&it, 0));
        cmc_assert(!sl_iter_advance(&it, sl->count));

        size_t sum = 0;
        for (it = sl_iter_start(sl);;)
        {
            sum += sl_iter_value(&it);

            if (!sl_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        sl_iter_to_start(&it);
        cmc_assert(sl_iter_advance(&it, sl->count - 1));

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_end(sl);

        cmc_assert(!sl_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            sl_insert(sl, i);

        it = sl_iter_end(sl);

        cmc_assert(!sl_iter_rewind(&it, 0));
        cmc_assert(!sl_iter_rewind(&it, sl->count));

        size_t sum = 0;
        for (it = sl_iter_end(sl);;)
        {
            sum += sl_iter_value(&it);

            if (!sl_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        sl_iter_to_end(&it);
        cmc_assert(sl_iter_rewind(&it, sl->count - 1));

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_end(sl);
        cmc_assert(!sl_iter_go_to(&it, 0));

        it = sl_iter_start(sl);
        cmc_assert(!sl_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            sl_insert(sl, i);

        it = sl_iter_start(sl);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            sl_iter_go_to(&it, i);

            sum += sl_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(sl_iter_go_to(&it, i - 1));

            sum += sl_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 0; i < 1001; i += 100)
        {
            cmc_assert(sl_iter_go_to(&it, i));
            cmc_assert_equals(size_t, i, sl_iter_index(&it));

            sum += sl_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        struct sortedlist_iter it = sl_iter_end(sl);

        cmc_assert_equals(size_t, (size_t){ 0 }, sl_iter_value(&it));

        cmc_assert(sl_insert(sl, 10));

        it = sl_iter_start(sl);

        cmc_assert_equals(size_t, 10, sl_iter_value(&it));

        sl_free(sl);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct sortedlist *sl = sl_new(100, sl_fval);

        cmc_assert_not_equals(ptr, NULL, sl);

        for (size_t i = 0; i <= 1000; i++)
            sl_insert(sl, i);

        struct sortedlist_iter it = sl_iter_start(sl);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, sl_iter_index(&it));
            sl_iter_next(&it);
        }

        it = sl_iter_end(sl);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, sl_iter_index(&it));
            sl_iter_prev(&it);
        }

        sl_free(sl);
    });
});

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = CMCSortedList() + CMCSortedListIter();

    printf(" +---------------------------------------------------------------+");
    printf("\n");
    printf(" | CMCSortedList Suit : %-43s |\n", result == 0 ? "PASSED" : "FAILED");
    printf(" +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif

#endif /* CMC_TESTS_UNT_SORTEDLIST_H */
