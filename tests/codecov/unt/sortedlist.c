#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include "../src/sortedlist.c"

CMC_CREATE_UNIT(sortedlist_test, false, {
    CMC_CREATE_TEST(new, {
        sortedlist *sl = sl_new(1000000, cmp);

        cmc_assert_not_equals(ptr, NULL, sl);
        cmc_assert_not_equals(ptr, NULL, sl->buffer);
        cmc_assert_equals(size_t, 1000000, sl_capacity(sl));
        cmc_assert_equals(size_t, 0, sl_count(sl));
        cmc_assert_not_equals(ptr, NULL, sl->buffer);

        sl_free(sl, NULL);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        sortedlist *sl = sl_new(0, cmp);

        cmc_assert_equals(ptr, NULL, sl);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        sortedlist *sl = sl_new(UINT64_MAX, cmp);

        cmc_assert_equals(ptr, NULL, sl);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        sortedlist *sl = sl_new(100, cmp);

        cmc_assert_not_equals(ptr, NULL, sl);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(sl_insert(sl, i));

        cmc_assert_equals(size_t, 50, sl_count(sl));

        sl_clear(sl, NULL);

        cmc_assert_equals(size_t, 0, sl_count(sl));
        cmc_assert_equals(size_t, 100, sl_capacity(sl));

        sl_free(sl, NULL);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        sortedlist *sl = sl_new(1, cmp);

        cmc_assert_not_equals(ptr, NULL, sl);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(sl_insert(sl, i));

        sl_sort(sl);

        cmc_assert_equals(size_t, 50, sl_count(sl));
        cmc_assert_array_sorted_any(size_t, sl->buffer, cmp, 0, sl->count - 1);

        sl_free(sl, NULL);
    });

    CMC_CREATE_TEST(insert, {
        sortedlist *sl = sl_new(100, cmp);

        cmc_assert_not_equals(ptr, NULL, sl);

        cmc_assert(sl_insert(sl, 1));

        sl_free(sl, NULL);
    });

    CMC_CREATE_TEST(remove, {
        sortedlist *sl = sl_new(100, cmp);

        cmc_assert_not_equals(ptr, NULL, sl);

        cmc_assert(sl_insert(sl, 32));

        cmc_assert(sl_remove(sl, 0));

        sl_free(sl, NULL);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        sortedlist *sl = sl_new(100, cmp);

        cmc_assert_not_equals(ptr, NULL, sl);

        cmc_assert(!sl_remove(sl, 0));

        sl_free(sl, NULL);
    });

    CMC_CREATE_TEST(remove[out of range], {
        sortedlist *sl = sl_new(100, cmp);

        cmc_assert_not_equals(ptr, NULL, sl);

        cmc_assert(sl_insert(sl, 32));

        cmc_assert(!sl_remove(sl, 2));

        sl_free(sl, NULL);
    });

    CMC_CREATE_TEST(indexof, {
        sortedlist *sl = sl_new(1, cmp);

        cmc_assert_not_equals(ptr, NULL, sl);

        for (size_t i = 0; i < 20; i++)
            cmc_assert(sl_insert(sl, i % 10));

        /* 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9 */
        cmc_assert_equals(size_t, 4, sl_indexof(sl, 2, true));
        cmc_assert_equals(size_t, 5, sl_indexof(sl, 2, false));

        cmc_assert_equals(size_t, 0, sl_indexof(sl, 0, true));
        cmc_assert_equals(size_t, 1, sl_indexof(sl, 0, false));

        cmc_assert(sl_remove(sl, 0));

        cmc_assert_equals(size_t, 0, sl_indexof(sl, 0, true));
        cmc_assert_equals(size_t, 0, sl_indexof(sl, 0, false));

        cmc_assert(sl_remove(sl, 0));

        cmc_assert_equals(size_t, sl_count(sl), sl_indexof(sl, 0, true));
        cmc_assert_equals(size_t, sl_count(sl), sl_indexof(sl, 0, false));

        sl_free(sl, NULL);
    });
});
