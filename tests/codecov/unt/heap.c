#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include "../src/heap.c"

CMC_CREATE_UNIT(heap_test, false, {
    CMC_CREATE_TEST(new, {
        heap *h = h_new(1000000, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);
        cmc_assert_not_equals(ptr, NULL, h->buffer);
        cmc_assert_equals(size_t, 1000000, h_capacity(h));
        cmc_assert_equals(size_t, 0, h_count(h));

        h_free(h, NULL);
    });

    CMC_CREATE_TEST(new[edge_case capacity = 0], {
        heap *h = h_new(0, cmc_max_heap, cmp);

        cmc_assert_equals(ptr, NULL, h);
    });

    CMC_CREATE_TEST(new[edge_case capacity = UINT64_MAX], {
        heap *h = h_new(UINT64_MAX, cmc_max_heap, cmp);

        cmc_assert_equals(ptr, NULL, h);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        heap *h = h_new(100, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i < 50; i++)
            h_insert(h, i);

        cmc_assert_equals(size_t, 50, h_count(h));

        h_clear(h, NULL);

        cmc_assert_equals(size_t, 0, h_count(h));
        cmc_assert_equals(size_t, 100, h_capacity(h));

        h_free(h, NULL);
    });

    CMC_CREATE_TEST(buffer_growth[edge_case capacity = 1], {
        heap *h = h_new(1, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(h_insert(h, i));

        cmc_assert_equals(size_t, 50, h_count(h));

        h_free(h, NULL);
    });

    CMC_CREATE_TEST(insert, {
        heap *h = h_new(100, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        cmc_assert(h_insert(h, 10));

        h_free(h, NULL);
    });

    CMC_CREATE_TEST(insert[item preservation], {
        heap *h = h_new(50, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i <= 100; i++)
            cmc_assert(h_insert(h, i));

        size_t sum = 0;
        for (size_t i = 0; i < h_count(h); i++)
            sum += h->buffer[i];

        cmc_assert_equals(size_t, 5050, sum);

        h_free(h, NULL);
    });

    CMC_CREATE_TEST(remove, {
        heap *h = h_new(50, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i <= 100; i++)
            cmc_assert(h_insert(h, i));

        size_t r;
        cmc_assert(h_remove(h, &r));

        cmc_assert_equals(size_t, 100, r);

        h_free(h, NULL);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        heap *h = h_new(100, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        size_t _;
        cmc_assert(!h_remove(h, &_));

        h_free(h, NULL);
    });

    CMC_CREATE_TEST(remove[sorted], {
        heap *h = h_new(100, cmc_min_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(h_insert(h, i));

        size_t r;
        size_t i = 0;
        size_t arr[100];
        while (!h_empty(h))
        {
            cmc_assert(h_remove(h, &r));

            arr[i++] = r;
        }

        cmc_assert_array_sorted_any(size_t, arr, cmp, 0, 99);

        h_free(h, NULL);
    });

    CMC_CREATE_TEST(peek, {
        heap *h = h_new(100, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(h_insert(h, i));

        cmc_assert_equals(size_t, 100, h_peek(h));

        h_free(h, NULL);
    });

    CMC_CREATE_TEST(peek[count = 0], {
        heap *h = h_new(100, cmc_max_heap, cmp);

        cmc_assert_not_equals(ptr, NULL, h);

        cmc_assert_equals(size_t, (size_t){0}, h_peek(h));

        h_free(h, NULL);
    });
});
