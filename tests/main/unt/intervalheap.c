#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <ext/intervalheap.h>

CMC_GENERATE_INTERVALHEAP(ih, intervalheap, size_t)

CMC_CREATE_UNIT(intervalheap_test, true, {
    CMC_CREATE_TEST(new, {
        intervalheap *ih = ih_new(1000000, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_not_equals(ptr, NULL, ih->buffer);
        cmc_assert_equals(size_t, 500000, ih_capacity(ih));
        cmc_assert_equals(size_t, 0, ih_count(ih));

        ih_free(ih, NULL);
    });

    CMC_CREATE_TEST(new[edge_case capacity = 0], {
        intervalheap *ih = ih_new(0, cmp);

        cmc_assert_equals(ptr, NULL, ih);

        if (ih)
            ih_free(ih, NULL);
    });

    CMC_CREATE_TEST(new[edge_case capacity = UINT64_MAX], {
        intervalheap *ih = ih_new(UINT64_MAX, cmp);

        cmc_assert_equals(ptr, NULL, ih);

        if (ih)
            ih_free(ih, NULL);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        intervalheap *ih = ih_new(100, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);

        for (size_t i = 0; i < 50; i++)
            ih_insert(ih, i);

        cmc_assert_equals(size_t, 50, ih_count(ih));

        ih_clear(ih, NULL);

        cmc_assert_equals(size_t, 0, ih_count(ih));
        cmc_assert_equals(size_t, 50, ih_capacity(ih));

        ih_free(ih, NULL);
    });

    CMC_CREATE_TEST(buffer_growth[edge_case capacity = 1], {
        intervalheap *ih = ih_new(1, cmp);

        cmc_assert_not_equals(ptr, NULL, ih);

        for (size_t i = 0; i < 50; i++)
            ih_insert(ih, i);

        cmc_assert_equals(size_t, 50, ih_count(ih));

        ih_free(ih, NULL);
    });
});
