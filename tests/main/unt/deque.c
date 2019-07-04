#include "utl/assert.h"
#include "utl/test.h"
#include "../src/deque.c"

CMC_CREATE_UNIT(deque_test, true, {
    CMC_CREATE_TEST(new, {
        deque *d = d_new(1000000);

        cmc_assert_not_equals(ptr, NULL, d);

        bool passed = d->capacity == 1000000 && d->count == 0 && d->buffer;

        cmc_assert_equals(size_t, 1000000, d->capacity);
        cmc_assert_equals(size_t, 0, d_count(d));
        cmc_assert_not_equals(ptr, NULL, d->buffer);

        CMC_TEST_PASS_ELSE_FAIL(passed);

        d_free(d);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = 0], {
        deque *d = d_new(0);

        cmc_assert_equals(ptr, NULL, d);

        CMC_TEST_PASS_ELSE_FAIL(d == NULL);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = UINT64_MAX], {
        deque *d = d_new(UINT64_MAX);

        cmc_assert_equals(ptr, NULL, d);

        CMC_TEST_PASS_ELSE_FAIL(d == NULL);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 0; i < 50; i++)
            d_push_back(d, i);

        cmc_assert_equals(size_t, 50, d_count(d));

        d_clear(d);

        cmc_assert_equals(size_t, 0, d_count(d));
        cmc_assert_equals(size_t, 100, d_capacity(d));

        CMC_TEST_PASS_ELSE_FAIL(d_count(d) == 0 && d_capacity(d) == 100);

        d_free(d);
    });

    CMC_CREATE_TEST(buffer_growth[edge_case:capacity = 1], {
        deque *d = d_new(1);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 0; i < 50; i++)
            d_push_back(d, i);

        cmc_assert_equals(size_t, 50, d_count(d));
        cmc_assert_lesser_equals(size_t, d_capacity(d), d_count(d));

        CMC_TEST_PASS_ELSE_FAIL(d_count(d) == 50 && d_count(d) <= d_capacity(d));

        d_free(d);
    });

});
