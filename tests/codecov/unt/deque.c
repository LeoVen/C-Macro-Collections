#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include "../src/deque.c"

CMC_CREATE_UNIT(deque_test, false, {
    CMC_CREATE_TEST(new, {
        deque *d = d_new(1000000);

        cmc_assert_not_equals(ptr, NULL, d);
        cmc_assert_not_equals(ptr, NULL, d->buffer);
        cmc_assert_equals(size_t, 1000000, d->capacity);
        cmc_assert_equals(size_t, 0, d_count(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        deque *d = d_new(0);

        cmc_assert_equals(ptr, NULL, d);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
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

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        deque *d = d_new(1);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 0; i < 50; i++)
            d_push_back(d, i);

        cmc_assert_equals(size_t, 50, d_count(d));
        cmc_assert_lesser_equals(size_t, d_capacity(d), d_count(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(buffer_growth[item preservation], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 1200; i++)
        {
            if (i % 2 == 0)
                d_push_front(d, i);
            else
                d_push_back(d, i);
        }

        size_t sum = 0;
        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
            sum += d->buffer[i];

        cmc_assert_equals(size_t, 1200, d_count(d));
        cmc_assert_equals(size_t, 720600, sum);

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

    CMC_CREATE_TEST(push_front[item preservation], {
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

    CMC_CREATE_TEST(push_back[item preservation], {
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

    CMC_CREATE_TEST(pop_front[item preservation], {
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

    CMC_CREATE_TEST(pop_back[item preservation], {
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

    CMC_CREATE_TEST(front[count = 0], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        size_t r = d_front(d);

        cmc_assert_equals(size_t, (size_t){0}, r);

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(front[sum], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        size_t sum = 0;

        for (size_t i = 0; i <= 100; i++)
        {
            if (i % 2 == 0)
                cmc_assert(d_push_back(d, i));
            else
                cmc_assert(d_push_front(d, i));
        }

        while (!d_empty(d))
        {
            size_t r = d_front(d);

            sum += r;

            cmc_assert(d_pop_front(d));
        }

        cmc_assert_equals(size_t, 5050, sum);

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(back[count = 0], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        size_t r = d_back(d);

        cmc_assert_equals(size_t, (size_t){0}, r);

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(back[sum], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        size_t sum = 0;

        for (size_t i = 0; i <= 100; i++)
        {
            if (i % 2 == 0)
                cmc_assert(d_push_back(d, i));
            else
                cmc_assert(d_push_front(d, i));
        }

        while (!d_empty(d))
        {
            size_t r = d_back(d);

            sum += r;

            cmc_assert(d_pop_back(d));
        }

        cmc_assert_equals(size_t, 5050, sum);

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(contains, {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(d_push_back(d, i));

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(d_contains(d, i, cmp));

        d_clear(d, NULL);

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(d_push_front(d, i));

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(d_contains(d, i, cmp));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(contains[count = 0], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(!d_contains(d, 10, cmp));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(contains[count = 1], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(!d_contains(d, 10, cmp));

        cmc_assert(d_push_back(d, 100));

        cmc_assert(d_contains(d, 100, cmp));
        cmc_assert(!d_contains(d, 101, cmp));

        cmc_assert(d_pop_front(d));
        cmc_assert(d_push_front(d, 100));

        cmc_assert(d_contains(d, 100, cmp));
        cmc_assert(!d_contains(d, 101, cmp));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(empty, {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(d_empty(d));

        cmc_assert(d_push_front(d, 1));

        cmc_assert(!d_empty(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(full, {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(!d_full(d));

        for (size_t i = 0; i != d_capacity(d); i++)
            cmc_assert(d_push_front(d, i));

        cmc_assert(d_full(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(count, {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert_equals(size_t, 0, d_count(d));

        for (size_t i = 0; i < 100; i++)
            cmc_assert(d_push_back(d, i));

        cmc_assert_equals(size_t, 100, d_count(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(capacity, {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert_equals(size_t, 100, d_capacity(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(resize, {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 0; i < 150; i++)
        {
            if (i % 2 == 0)
                cmc_assert(d_push_back(d, i));
            else
                cmc_assert(d_push_front(d, i));
        }

        cmc_assert_equals(size_t, 150, d_count(d));
        cmc_assert(d_resize(d, d_count(d)));
        cmc_assert_equals(size_t, 150, d_capacity(d));

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(resize[item preservation], {
        deque *d = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 150; i++)
        {
            if (i % 2 != 0)
                cmc_assert(d_push_back(d, i));
            else
                cmc_assert(d_push_front(d, i));
        }

        cmc_assert(d_resize(d, d_count(d)));

        size_t sum = 0;

        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
        {
            sum += d->buffer[i];
        }

        cmc_assert_equals(size_t, 11325, sum);

        d_free(d, NULL);
    });

    CMC_CREATE_TEST(copy_of, {
        deque *d1 = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d1);

        for (size_t i = 1; i <= 150; i++)
        {
            if (i % 2 != 0)
                cmc_assert(d_push_back(d1, i));
            else
                cmc_assert(d_push_front(d1, i));
        }

        deque *d2 = d_copy_of(d1, NULL);

        cmc_assert_not_equals(ptr, NULL, d2);
        cmc_assert_equals(size_t, d_count(d1), d_count(d2));

        for (size_t i1 = d1->front, i2 = d2->front, j = 0; j < d1->count; j++)
        {
            cmc_assert_equals(size_t, d1->buffer[i1], d2->buffer[i2]);

            i1 = (i1 + 1) % d1->capacity;
            i2 = (i2 + 1) % d2->capacity;
        }

        d_free(d1, NULL);
        d_free(d2, NULL);
    });

    CMC_CREATE_TEST(equals, {
        deque *d1 = d_new(100);
        deque *d2 = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d1);
        cmc_assert_not_equals(ptr, NULL, d2);

        for (size_t i = 1; i <= 150; i++)
        {
            if (i % 2 != 0)
            {
                cmc_assert(d_push_back(d1, i));
                cmc_assert(d_push_back(d2, i));
            }
            else
            {
                cmc_assert(d_push_front(d1, i));
                cmc_assert(d_push_front(d2, i));
            }
        }

        cmc_assert(d_equals(d1, d2, cmp));

        d_free(d1, NULL);
        d_free(d2, NULL);
    });

    CMC_CREATE_TEST(equals[from copy], {
        deque *d1 = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d1);

        for (size_t i = 1; i <= 150; i++)
        {
            if (i % 2 != 0)
                cmc_assert(d_push_back(d1, i));
            else
                cmc_assert(d_push_front(d1, i));
        }

        deque *d2 = d_copy_of(d1, NULL);

        cmc_assert_not_equals(ptr, NULL, d2);

        cmc_assert(d_equals(d1, d2, cmp));

        d_free(d1, NULL);
        d_free(d2, NULL);
    });

    CMC_CREATE_TEST(equals[count = 0], {
        deque *d1 = d_new(100);

        cmc_assert_not_equals(ptr, NULL, d1);

        deque *d2 = d_copy_of(d1, NULL);

        cmc_assert_not_equals(ptr, NULL, d2);

        cmc_assert(d_equals(d1, d2, cmp));

        d_free(d1, NULL);
        d_free(d2, NULL);
    });
});
