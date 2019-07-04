#include "utl/assert.h"
#include "utl/test.h"
#include "../src/heap.c"

#include "utl.c"

CMC_CREATE_UNIT(heap_test, true, {
    CMC_CREATE_TEST(new, {
        heap *h = h_new(1000000, MaxHeap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        bool passed = h->capacity == 1000000 && h->count == 0 && h->buffer;

        cmc_assert_equals(size_t, 1000000, h->capacity);
        cmc_assert_equals(size_t, 0, h_count(h));
        cmc_assert_not_equals(ptr, NULL, h->buffer);

        CMC_TEST_PASS_ELSE_FAIL(passed);

        h_free(h);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = 0], {
        heap *h = h_new(0, MaxHeap, cmp);

        cmc_assert_equals(ptr, NULL, h);

        CMC_TEST_PASS_ELSE_FAIL(h == NULL);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = UINT64_MAX], {
        heap *h = h_new(UINT64_MAX, MaxHeap, cmp);

        cmc_assert_equals(ptr, NULL, h);

        CMC_TEST_PASS_ELSE_FAIL(h == NULL);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        heap *h = h_new(100, MaxHeap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i < 50; i++)
            h_insert(h, i);

        cmc_assert_equals(size_t, 50, h_count(h));

        h_clear(h);

        cmc_assert_equals(size_t, 0, h_count(h));
        cmc_assert_equals(size_t, 100, h_capacity(h));

        CMC_TEST_PASS_ELSE_FAIL(h_count(h) == 0 && h_capacity(h) == 100);

        h_free(h);
    });

    CMC_CREATE_TEST(buffer_growth[edge_case:capacity = 1], {
        heap *h = h_new(1, MaxHeap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i < 50; i++)
            h_insert(h, i);

        cmc_assert_equals(size_t, 50, h_count(h));

        CMC_TEST_PASS_ELSE_FAIL(h_count(h) == 50);

        h_free(h);
    });

});
