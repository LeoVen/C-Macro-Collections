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

});
