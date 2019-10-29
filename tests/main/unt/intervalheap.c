#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/intervalheap.h>

CMC_GENERATE_INTERVALHEAP(ih, intervalheap, size_t)

CMC_CREATE_UNIT(intervalheap_test, true, {
    CMC_CREATE_TEST(new, {
        struct intervalheap *ih = ih_new(1000000, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_not_equals(ptr, NULL, ih->buffer);
        cmc_assert_equals(size_t, 500000, ih_capacity(ih));
        cmc_assert_equals(size_t, 0, ih_count(ih));

        ih_free(ih, NULL);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        struct intervalheap *ih = ih_new(0, cmp);

        cmc_assert_equals(ptr, NULL, ih);

        if (ih)
            ih_free(ih, NULL);
    });

    CMC_CREATE_TEST(new[capacity], {
        struct intervalheap *ih = ih_new(1, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 1, ih_capacity(ih));

        ih_free(ih, NULL);

        ih = ih_new(2, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 1, ih_capacity(ih));

        ih_free(ih, NULL);

        ih = ih_new(3, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 2, ih_capacity(ih));

        ih_free(ih, NULL);

        ih = ih_new(4, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 2, ih_capacity(ih));

        ih_free(ih, NULL);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        struct intervalheap *ih = ih_new(UINT64_MAX, cmp);

        cmc_assert_equals(ptr, NULL, ih);

        if (ih)
            ih_free(ih, NULL);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct intervalheap *ih = ih_new(100, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(ih_insert(ih, i));

        cmc_assert_equals(size_t, 50, ih_count(ih));

        ih_clear(ih, NULL);

        cmc_assert_equals(size_t, 0, ih_count(ih));
        cmc_assert_equals(size_t, 50, ih_capacity(ih));

        ih_free(ih, NULL);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct intervalheap *ih = ih_new(1, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(ih_insert(ih, i));

        cmc_assert_equals(size_t, 50, ih_count(ih));
        cmc_assert_greater(size_t, 50, ih_capacity(ih) * 2);

        ih_free(ih, NULL);
    });

    CMC_CREATE_TEST(insert[count], {
        struct intervalheap *ih = ih_new(100, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);

        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 0));

        cmc_assert_equals(size_t, 2, ih_count(ih));

        ih_free(ih, NULL);
    });

    CMC_CREATE_TEST(insert[max min], {
        struct intervalheap *ih = ih_new(1, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);

        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 0));

        size_t min;
        size_t max;

        cmc_assert(ih_min(ih, &min));
        cmc_assert(ih_max(ih, &max));

        cmc_assert_equals(size_t, 0, min);
        cmc_assert_equals(size_t, 1, max);

        ih_free(ih, NULL);
    });
});
