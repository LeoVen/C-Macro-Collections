#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/deque.h>

CMC_GENERATE_DEQUE(d, deque, size_t)

CMC_CREATE_UNIT(deque_test, true, {
    CMC_CREATE_TEST(new, {
        deque *d = d_new(1000000);

        cmc_assert_not_equals(ptr, NULL, d);
        cmc_assert_not_equals(ptr, NULL, d->buffer);
        cmc_assert_equals(size_t, 1000000, d->capacity);
        cmc_assert_equals(size_t, 0, d_count(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(new [edge_case:capacity = 0], {
        deque *d = d_new(0);

        cmc_assert_equals(ptr, NULL, d);
    });

    CMC_CREATE_TEST(new [edge_case:capacity = UINT64_MAX], {
        deque *d = d_new(UINT64_MAX);

        cmc_assert_equals(ptr, NULL, d);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 0; i < 50; i++)
            d_push_back(d, i);

        cmc_assert_equals(size_t, 50, d_count(d));

        d_clear(d, NULL);

        cmc_assert_equals(size_t, 0, d_count(d));
        cmc_assert_equals(size_t, 100, d_capacity(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(buffer_growth [edge_case:capacity = 1], {
        deque *d = d_new(1);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 0; i < 50; i++)
            d_push_back(d, i);

        cmc_assert_equals(size_t, 50, d_count(d));
        cmc_assert_lesser_equals(size_t, d_capacity(d), d_count(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(push_front[count capacity], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 0; i < 150; i++)
            cmc_assert(d_push_front(d, i));

        cmc_assert_equals(size_t, 150, d_count(d));
        cmc_assert_greater(size_t, 100, d_capacity(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(push_front[item_preservation], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(d_push_front(d, i));

        size_t sum = 0;

        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
        {
            sum += d->buffer[i];
        }

        cmc_assert_equals(size_t, 5050, sum);

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(push_back[count capacity], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 0; i < 150; i++)
            cmc_assert(d_push_back(d, i));

        cmc_assert_equals(size_t, 150, d_count(d));
        cmc_assert_greater(size_t, 100, d_capacity(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(push_back[item_preservation], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(d_push_back(d, i));

        size_t sum = 0;

        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
        {
            sum += d->buffer[i];
        }

        cmc_assert_equals(size_t, 5050, sum);

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(pop_front[count capacity], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_pop_front(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_pop_front(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(pop_front[item_preservation], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 200; i++)
            cmc_assert(d_push_front(d, i));

        for (size_t i = 0; i < 100; i++)
            cmc_assert(d_pop_front(d));

        size_t sum = 0;

        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
        {
            sum += d->buffer[i];
        }

        cmc_assert_equals(size_t, 5050, sum);

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(pop_back[count capacity], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_pop_back(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_pop_back(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(pop_back[item_preservation], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 200; i++)
            cmc_assert(d_push_back(d, i));

        for (size_t i = 0; i < 100; i++)
            cmc_assert(d_pop_back(d));

        size_t sum = 0;

        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
        {
            sum += d->buffer[i];
        }

        cmc_assert_equals(size_t, 5050, sum);

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(Mixed IO, {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_pop_back(d));
        cmc_assert(d_pop_back(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_pop_back(d));
        cmc_assert(d_pop_back(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_pop_front(d));
        cmc_assert(d_pop_front(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_pop_front(d));
        cmc_assert(d_pop_front(d));

        cmc_assert_equals(size_t, 0, d_count(d));
        // ----------
        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_pop_back(d));
        cmc_assert(d_pop_front(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_pop_front(d));
        cmc_assert(d_pop_back(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_pop_back(d));
        cmc_assert(d_pop_front(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_pop_front(d));
        cmc_assert(d_pop_back(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        d_free(d, NULL);
    });
});
