#include "utl/assert.h"
#include "utl/test.h"
#include "../src/heap.c"

#include "utl.c"

CMC_CREATE_UNIT(heap_test, true, {
    CMC_CREATE_TEST(new, {
        heap *h = h_new(1000000, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);
        cmc_assert_not_equals(ptr, NULL, h->buffer);
        cmc_assert_equals(size_t, 1000000, h_capacity(h));
        cmc_assert_equals(size_t, 0, h_count(h));

        h_free(h);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = 0], {
        heap *h = h_new(0, cmc_max_heap, cmp);

        cmc_assert_equals(ptr, NULL, h);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = UINT64_MAX], {
        heap *h = h_new(UINT64_MAX, cmc_max_heap, cmp);

        cmc_assert_equals(ptr, NULL, h);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        heap *h = h_new(100, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i < 50; i++)
            h_insert(h, i);

        cmc_assert_equals(size_t, 50, h_count(h));

        h_clear(h);

        cmc_assert_equals(size_t, 0, h_count(h));
        cmc_assert_equals(size_t, 100, h_capacity(h));

        h_free(h);
    });

    CMC_CREATE_TEST(buffer_growth[edge_case:capacity = 1], {
        heap *h = h_new(1, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i < 50; i++)
            h_insert(h, i);

        cmc_assert_equals(size_t, 50, h_count(h));

        h_free(h);
    });

});
