#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/queue.h>

CMC_GENERATE_QUEUE(q, queue, size_t)

CMC_CREATE_UNIT(queue_test, true, {
    CMC_CREATE_TEST(new, {
        struct queue *q = q_new(1000000);

        cmc_assert_not_equals(ptr, NULL, q);
        cmc_assert_not_equals(ptr, NULL, q->buffer);
        cmc_assert_equals(size_t, 1000000, q_capacity(q));
        cmc_assert_equals(size_t, 0, q_count(q));
        cmc_assert_not_equals(ptr, NULL, q->buffer);

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        struct queue *q = q_new(0);

        cmc_assert_equals(ptr, NULL, q);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        struct queue *q = q_new(UINT64_MAX);

        cmc_assert_equals(ptr, NULL, q);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert_equals(size_t, 50, q_count(q));

        q_clear(q, NULL);

        cmc_assert_equals(size_t, 0, q_count(q));
        cmc_assert_equals(size_t, 100, q_capacity(q));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct queue *q = q_new(1);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert_equals(size_t, 50, q_count(q));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(buffer_growth[item preservation], {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 1; i <= 1200; i++)
        {
            q_enqueue(q, i);
        }

        size_t sum = 0;
        for (size_t i = q->front, j = 0; j < q->count; i = (i + 1) % q->capacity, j++)
            sum += q->buffer[i];

        cmc_assert_equals(size_t, 1200, q_count(q));
        cmc_assert_equals(size_t, 720600, sum);

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(enqueue[count capacity], {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 0; i < 150; i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert_equals(size_t, 150, q_count(q));
        cmc_assert_greater(size_t, 100, q_capacity(q));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(enqueue[item preservation], {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(q_enqueue(q, i));

        size_t sum = 0;

        for (size_t i = q->front, j = 0; j < q->count; i = (i + 1) % q->capacity, j++)
        {
            sum += q->buffer[i];
        }

        cmc_assert_equals(size_t, 5050, sum);

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(dequeue[count capacity], {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert(q_enqueue(q, 10));
        cmc_assert(q_dequeue(q));

        cmc_assert_equals(size_t, 0, q_count(q));

        cmc_assert(q_enqueue(q, 10));
        cmc_assert(q_dequeue(q));

        cmc_assert_equals(size_t, 0, q_count(q));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(dequeue[item preservation], {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 200; i > 0; i--)
            cmc_assert(q_enqueue(q, i));

        for (size_t i = 0; i < 100; i++)
            cmc_assert(q_dequeue(q));

        size_t sum = 0;

        for (size_t i = q->front, j = 0; j < q->count; i = (i + 1) % q->capacity, j++)
        {
            sum += q->buffer[i];
        }

        cmc_assert_equals(size_t, 5050, sum);

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(peek[count = 0], {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        size_t r = q_peek(q);

        cmc_assert_equals(size_t, (size_t){0}, r);

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(peek[sum], {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        size_t sum = 0;

        for (size_t i = 0; i <= 100; i++)
            cmc_assert(q_enqueue(q, i));

        while (!q_empty(q))
        {
            size_t r = q_peek(q);

            sum += r;

            cmc_assert(q_dequeue(q));
        }

        cmc_assert_equals(size_t, 5050, sum);

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(contains, {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(q_enqueue(q, i));

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(q_contains(q, i, cmp));

        q_clear(q, NULL);

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(!q_contains(q, i, cmp));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(contains[count = 0], {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert(!q_contains(q, 10, cmp));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(contains[count = 1], {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert(!q_contains(q, 10, cmp));

        cmc_assert(q_enqueue(q, 100));

        cmc_assert(q_contains(q, 100, cmp));
        cmc_assert(!q_contains(q, 101, cmp));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(empty, {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert(q_empty(q));

        cmc_assert(q_enqueue(q, 1));

        cmc_assert(!q_empty(q));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(full, {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert(!q_full(q));

        for (size_t i = 0; i != q_capacity(q); i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert(q_full(q));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(count, {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert_equals(size_t, 0, q_count(q));

        for (size_t i = 0; i < 100; i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert_equals(size_t, 100, q_count(q));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(capacity, {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert_equals(size_t, 100, q_capacity(q));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(resize, {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 0; i < 150; i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert_equals(size_t, 150, q_count(q));
        cmc_assert(q_resize(q, q_count(q)));
        cmc_assert_equals(size_t, 150, q_capacity(q));

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(resize[item preservation], {
        struct queue *q = q_new(100);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 1; i <= 150; i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert(q_resize(q, q_count(q)));

        size_t sum = 0;

        for (size_t i = q->front, j = 0; j < q->count; i = (i + 1) % q->capacity, j++)
        {
            sum += q->buffer[i];
        }

        cmc_assert_equals(size_t, 11325, sum);

        q_free(q, NULL);
    });

    CMC_CREATE_TEST(copy_of, {
        struct queue *d1 = q_new(100);

        cmc_assert_not_equals(ptr, NULL, d1);

        for (size_t i = 1; i <= 150; i++)
            cmc_assert(q_enqueue(d1, i));

        struct queue *d2 = q_copy_of(d1, NULL);

        cmc_assert_not_equals(ptr, NULL, d2);
        cmc_assert_equals(size_t, q_count(d1), q_count(d2));

        for (size_t i1 = d1->front, i2 = d2->front, j = 0; j < d1->count; j++)
        {
            cmc_assert_equals(size_t, d1->buffer[i1], d2->buffer[i2]);

            i1 = (i1 + 1) % d1->capacity;
            i2 = (i2 + 1) % d2->capacity;
        }

        q_free(d1, NULL);
        q_free(d2, NULL);
    });

    CMC_CREATE_TEST(equals, {
        struct queue *d1 = q_new(100);
        struct queue *d2 = q_new(100);

        cmc_assert_not_equals(ptr, NULL, d1);
        cmc_assert_not_equals(ptr, NULL, d2);

        for (size_t i = 1; i <= 150; i++)
        {
            cmc_assert(q_enqueue(d1, i));
            cmc_assert(q_enqueue(d2, i));
        }

        cmc_assert(q_equals(d1, d2, cmp));

        q_free(d1, NULL);
        q_free(d2, NULL);
    });

    CMC_CREATE_TEST(equals[from copy], {
        struct queue *d1 = q_new(100);

        cmc_assert_not_equals(ptr, NULL, d1);

        for (size_t i = 1; i <= 150; i++)
        {
            cmc_assert(q_enqueue(d1, i));
        }

        struct queue *d2 = q_copy_of(d1, NULL);

        cmc_assert_not_equals(ptr, NULL, d2);

        cmc_assert(q_equals(d1, d2, cmp));

        q_free(d1, NULL);
        q_free(d2, NULL);
    });

    CMC_CREATE_TEST(equals[count = 0], {
        struct queue *d1 = q_new(100);

        cmc_assert_not_equals(ptr, NULL, d1);

        struct queue *d2 = q_copy_of(d1, NULL);

        cmc_assert_not_equals(ptr, NULL, d2);

        cmc_assert(q_equals(d1, d2, cmp));

        q_free(d1, NULL);
        q_free(d2, NULL);
    });
});
