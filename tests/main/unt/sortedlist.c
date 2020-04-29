#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "../src/sortedlist.c"

struct sortedlist_fval *sl_fval =
    &(struct sortedlist_fval){ .cmp = cmc_size_cmp,
                               .cpy = NULL,
                               .str = cmc_size_str,
                               .free = NULL,
                               .hash = cmc_size_hash,
                               .pri = cmc_size_cmp };

CMC_CREATE_UNIT(SortedList, true, {
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
        cmc_assert_array_sorted_any(size_t, sl->buffer, cmc_size_cmp, 0,
                                    sl->count - 1);

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
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));

        // clear
        sl->flag = cmc_flags.ERROR;
        sl_clear(sl);
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));

        // customize
        sl->flag = cmc_flags.ERROR;
        sl_customize(sl, NULL, NULL);
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));

        // insert
        sl->flag = cmc_flags.ERROR;
        cmc_assert(sl_insert(sl, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));

        // remove
        cmc_assert(!sl_remove(sl, 2));
        cmc_assert_equals(int32_t, cmc_flags.RANGE, sl_flag(sl));

        cmc_assert(sl_remove(sl, 0));
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));

        cmc_assert(!sl_remove(sl, 0));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, sl_flag(sl));

        // max min
        sl->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, (size_t){ 0 }, sl_max(sl));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, sl_flag(sl));
        sl->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, (size_t){ 0 }, sl_min(sl));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, sl_flag(sl));

        cmc_assert(sl_insert(sl, 1));
        sl->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 1, sl_max(sl));
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));
        sl->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 1, sl_min(sl));
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));

        // get
        sl->flag = cmc_flags.ERROR;
        sl_get(sl, 1);
        cmc_assert_equals(int32_t, cmc_flags.RANGE, sl_flag(sl));

        sl_get(sl, 0);
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));

        cmc_assert(sl_remove(sl, 0));
        sl_get(sl, 0);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, sl_flag(sl));

        // index_of
        cmc_assert(sl_insert(sl, 1));
        sl_index_of(sl, 1, false);
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));

        // contains
        cmc_assert(sl_contains(sl, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));

        // sort
        sl->flag = cmc_flags.ERROR;
        sl_sort(sl);
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));

        // copy_of
        sl->flag = cmc_flags.ERROR;
        struct sortedlist *sl2 = sl_copy_of(sl);
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl2));

        // equals
        sl->flag = cmc_flags.ERROR;
        sl2->flag = cmc_flags.ERROR;
        cmc_assert(sl_equals(sl, sl2));
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl));
        cmc_assert_equals(int32_t, cmc_flags.OK, sl_flag(sl2));

        sl_free(sl);
        sl_free(sl2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct sortedlist *sl = sl_new_custom(100, sl_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, sl);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(sl_insert(sl, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(sl_remove(sl, 0));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(sl_insert(sl, 1));
        cmc_assert(sl_insert(sl, 0));
        cmc_assert_equals(int32_t, 3, total_create);

        cmc_assert_equals(size_t, 1, sl_max(sl));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert_equals(size_t, 0, sl_min(sl));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert_equals(size_t, 0, sl_get(sl, 0));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert_equals(size_t, 0, sl_index_of(sl, 0, true));
        cmc_assert_equals(int32_t, 4, total_read);

        cmc_assert(sl_contains(sl, 1));
        cmc_assert_equals(int32_t, 5, total_read);

        cmc_assert(sl_resize(sl, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(sl_resize(sl, 50));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 3, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        sl_customize(sl, NULL, NULL);

        sl_clear(sl);
        cmc_assert(sl_insert(sl, 10));
        cmc_assert(sl_remove(sl, 0));
        cmc_assert(sl_insert(sl, 1));
        cmc_assert(sl_insert(sl, 0));
        cmc_assert_equals(size_t, 1, sl_max(sl));
        cmc_assert_equals(size_t, 0, sl_min(sl));
        cmc_assert_equals(size_t, 0, sl_get(sl, 0));
        cmc_assert_equals(size_t, 0, sl_index_of(sl, 0, true));
        cmc_assert(sl_contains(sl, 1));

        cmc_assert_equals(int32_t, 3, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, sl->callbacks);

        sl_free(sl);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});
