#ifndef CMC_TESTS_UNT_QUEUE_H
#define CMC_TESTS_UNT_QUEUE_H

#include "utl.h"

#define V size_t
#define PFX q
#define SNAME queue
#define CMC_EXT_QUEUE_INIT
#define CMC_EXT_QUEUE_ITER
#define CMC_EXT_QUEUE_STR
#include "cmc/queue.h"

struct queue_fval *q_fval = &(struct queue_fval){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

CMC_CREATE_UNIT(CMCQueue, true, {
    CMC_CREATE_TEST(new, {
        struct queue *q = q_new(1000000, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);
        cmc_assert_not_equals(ptr, NULL, q->buffer);
        cmc_assert_equals(size_t, 1000000, q_capacity(q));
        cmc_assert_equals(size_t, 0, q_count(q));
        cmc_assert_not_equals(ptr, NULL, q->buffer);

        q_free(q);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        struct queue *q = q_new(0, q_fval);

        cmc_assert_equals(ptr, NULL, q);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        struct queue *q = q_new(UINT64_MAX, q_fval);

        cmc_assert_equals(ptr, NULL, q);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert_equals(size_t, 50, q_count(q));

        q_clear(q);

        cmc_assert_equals(size_t, 0, q_count(q));
        cmc_assert_equals(size_t, 100, q_capacity(q));

        q_free(q);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct queue *q = q_new(1, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert_equals(size_t, 50, q_count(q));

        q_free(q);
    });

    CMC_CREATE_TEST(buffer_growth[item preservation], {
        struct queue *q = q_new(100, q_fval);

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

        q_free(q);
    });

    CMC_CREATE_TEST(enqueue[count capacity], {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 0; i < 150; i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert_equals(size_t, 150, q_count(q));
        cmc_assert_greater(size_t, 100, q_capacity(q));

        q_free(q);
    });

    CMC_CREATE_TEST(enqueue[item preservation], {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(q_enqueue(q, i));

        size_t sum = 0;

        for (size_t i = q->front, j = 0; j < q->count; i = (i + 1) % q->capacity, j++)
        {
            sum += q->buffer[i];
        }

        cmc_assert_equals(size_t, 5050, sum);

        q_free(q);
    });

    CMC_CREATE_TEST(dequeue[count capacity], {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert(q_enqueue(q, 10));
        cmc_assert(q_dequeue(q));

        cmc_assert_equals(size_t, 0, q_count(q));

        cmc_assert(q_enqueue(q, 10));
        cmc_assert(q_dequeue(q));

        cmc_assert_equals(size_t, 0, q_count(q));

        q_free(q);
    });

    CMC_CREATE_TEST(dequeue[item preservation], {
        struct queue *q = q_new(100, q_fval);

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

        q_free(q);
    });

    CMC_CREATE_TEST(peek[count = 0], {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        size_t r = q_peek(q);

        cmc_assert_equals(size_t, (size_t){ 0 }, r);

        q_free(q);
    });

    CMC_CREATE_TEST(peek[sum], {
        struct queue *q = q_new(100, q_fval);

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

        q_free(q);
    });

    CMC_CREATE_TEST(contains, {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(q_enqueue(q, i));

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(q_contains(q, i));

        q_clear(q);

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(!q_contains(q, i));

        q_free(q);
    });

    CMC_CREATE_TEST(contains[count = 0], {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert(!q_contains(q, 10));

        q_free(q);
    });

    CMC_CREATE_TEST(contains[count = 1], {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert(!q_contains(q, 10));

        cmc_assert(q_enqueue(q, 100));

        cmc_assert(q_contains(q, 100));
        cmc_assert(!q_contains(q, 101));

        q_free(q);
    });

    CMC_CREATE_TEST(empty, {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert(q_empty(q));

        cmc_assert(q_enqueue(q, 1));

        cmc_assert(!q_empty(q));

        q_free(q);
    });

    CMC_CREATE_TEST(full, {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert(!q_full(q));

        for (size_t i = 0; i != q_capacity(q); i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert(q_full(q));

        q_free(q);
    });

    CMC_CREATE_TEST(count, {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert_equals(size_t, 0, q_count(q));

        for (size_t i = 0; i < 100; i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert_equals(size_t, 100, q_count(q));

        q_free(q);
    });

    CMC_CREATE_TEST(capacity, {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        cmc_assert_equals(size_t, 100, q_capacity(q));

        q_free(q);
    });

    CMC_CREATE_TEST(resize, {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 0; i < 150; i++)
            cmc_assert(q_enqueue(q, i));

        cmc_assert_equals(size_t, 150, q_count(q));
        cmc_assert(q_resize(q, q_count(q)));
        cmc_assert_equals(size_t, 150, q_capacity(q));

        q_free(q);
    });

    CMC_CREATE_TEST(resize[item preservation], {
        struct queue *q = q_new(100, q_fval);

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

        q_free(q);
    });

    CMC_CREATE_TEST(copy_of, {
        struct queue *q1 = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q1);

        for (size_t i = 1; i <= 150; i++)
            cmc_assert(q_enqueue(q1, i));

        struct queue *q2 = q_copy_of(q1);

        cmc_assert_not_equals(ptr, NULL, q2);
        cmc_assert_equals(size_t, q_count(q1), q_count(q2));

        for (size_t i1 = q1->front, i2 = q2->front, j = 0; j < q1->count; j++)
        {
            cmc_assert_equals(size_t, q1->buffer[i1], q2->buffer[i2]);

            i1 = (i1 + 1) % q1->capacity;
            i2 = (i2 + 1) % q2->capacity;
        }

        q_free(q1);
        q_free(q2);
    });

    CMC_CREATE_TEST(equals, {
        struct queue *q1 = q_new(100, q_fval);
        struct queue *q2 = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q1);
        cmc_assert_not_equals(ptr, NULL, q2);

        for (size_t i = 1; i <= 150; i++)
        {
            cmc_assert(q_enqueue(q1, i));
            cmc_assert(q_enqueue(q2, i));
        }

        cmc_assert(q_equals(q1, q2));

        q_free(q1);
        q_free(q2);
    });

    CMC_CREATE_TEST(equals[from copy], {
        struct queue *q1 = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q1);

        for (size_t i = 1; i <= 150; i++)
        {
            cmc_assert(q_enqueue(q1, i));
        }

        struct queue *q2 = q_copy_of(q1);

        cmc_assert_not_equals(ptr, NULL, q2);

        cmc_assert(q_equals(q1, q2));

        q_free(q1);
        q_free(q2);
    });

    CMC_CREATE_TEST(equals[count = 0], {
        struct queue *q1 = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q1);

        struct queue *q2 = q_copy_of(q1);

        cmc_assert_not_equals(ptr, NULL, q2);

        cmc_assert(q_equals(q1, q2));

        q_free(q1);
        q_free(q2);
    });

    CMC_CREATE_TEST(flag, {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, q_flag(q));

        // clear
        q->flag = CMC_FLAG_ERROR;
        q_clear(q);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, q_flag(q));

        // customize
        q->flag = CMC_FLAG_ERROR;
        q_customize(q, NULL, NULL);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, q_flag(q));

        // enqueue
        q->flag = CMC_FLAG_ERROR;
        q_enqueue(q, 10);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, q_flag(q));

        // dequeue
        q->flag = CMC_FLAG_ERROR;
        q_dequeue(q);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, q_flag(q));

        q_dequeue(q);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, q_flag(q));

        // peek
        q->flag = CMC_FLAG_ERROR;
        q_peek(q);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, q_flag(q));

        cmc_assert(q_enqueue(q, 10));
        q_peek(q);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, q_flag(q));

        // contains
        q->flag = CMC_FLAG_ERROR;
        cmc_assert(q_contains(q, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, q_flag(q));

        // copy_of
        q->flag = CMC_FLAG_OK;
        struct queue *q2 = q_copy_of(q);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, q_flag(q));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, q_flag(q2));

        // equals
        q->flag = CMC_FLAG_OK;
        q2->flag = CMC_FLAG_OK;
        cmc_assert(q_equals(q, q2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, q_flag(q));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, q_flag(q2));

        q_free(q);
        q_free(q2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct queue *q = q_new_custom(100, q_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, q);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(q_enqueue(q, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(q_dequeue(q));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(q_enqueue(q, 10));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert_equals(size_t, 10, q_peek(q));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert(q_contains(q, 10));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert(q_resize(q, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(q_resize(q, 100));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 2, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        q_customize(q, NULL, NULL);

        cmc_assert_equals(ptr, NULL, q->callbacks);

        q_clear(q);
        cmc_assert(q_enqueue(q, 10));
        cmc_assert(q_dequeue(q));
        cmc_assert(q_enqueue(q, 10));
        cmc_assert_equals(size_t, 10, q_peek(q));
        cmc_assert(q_contains(q, 10));
        cmc_assert(q_resize(q, 1000));
        cmc_assert(q_resize(q, 100));

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 2, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, q->callbacks);

        q_free(q);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});

CMC_CREATE_UNIT(CMCQueueIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_start(q);

        cmc_assert_equals(ptr, q, it.target);
        cmc_assert_equals(size_t, q->front, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(q_iter_at_start(&it));
        cmc_assert(q_iter_at_end(&it));

        cmc_assert(q_enqueue(q, 1));
        cmc_assert(q_enqueue(q, 2));

        it = q_iter_start(q);

        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, false, it.end);
        cmc_assert_equals(size_t, q->front, it.cursor);

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_end(q);

        cmc_assert_equals(ptr, q, it.target);
        cmc_assert_equals(size_t, q->back, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(q_iter_at_start(&it));
        cmc_assert(q_iter_at_end(&it));

        cmc_assert(q_enqueue(q, 1));
        cmc_assert(q_enqueue(q, 2));
        cmc_assert(q_enqueue(q, 3));

        it = q_iter_end(q);

        cmc_assert_equals(size_t, q->count - 1, it.index);
        cmc_assert_equals(size_t, q->back - 1, it.cursor);
        cmc_assert_equals(bool, false, it.start);

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_start(q);

        // Empty checks
        cmc_assert(q_iter_at_start(&it));
        it = q_iter_end(q);
        cmc_assert(q_iter_at_start(&it));

        // Non-empty check
        cmc_assert(q_enqueue(q, 1));
        it = q_iter_end(q);
        cmc_assert(!q_iter_at_start(&it));
        it = q_iter_start(q);
        cmc_assert(q_iter_at_start(&it));

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_start(q);

        // Empty check
        cmc_assert(q_iter_at_end(&it));
        it = q_iter_end(q);
        cmc_assert(q_iter_at_end(&it));

        // Non-empty check
        cmc_assert(q_enqueue(q, 1));
        it = q_iter_end(q);
        cmc_assert(q_iter_at_end(&it));
        it = q_iter_start(q);
        cmc_assert(!q_iter_at_end(&it));

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_start(q);

        cmc_assert(!q_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
            q_enqueue(q, i);

        cmc_assert_equals(size_t, 100, q->count);

        it = q_iter_end(q);

        cmc_assert(!q_iter_at_start(&it));
        cmc_assert(q_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, q_iter_value(&it));

        cmc_assert(q_iter_to_start(&it));

        cmc_assert(q_iter_at_start(&it));
        cmc_assert(!q_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, q_iter_value(&it));

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_end(q);

        cmc_assert(!q_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
            q_enqueue(q, i);

        it = q_iter_start(q);

        cmc_assert(q_iter_at_start(&it));
        cmc_assert(!q_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, q_iter_value(&it));

        cmc_assert(q_iter_to_end(&it));

        cmc_assert(!q_iter_at_start(&it));
        cmc_assert(q_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, q_iter_value(&it));

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_start(q);

        cmc_assert(!q_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
            q_enqueue(q, i);

        size_t sum = 0;
        for (it = q_iter_start(q); !q_iter_at_end(&it); q_iter_next(&it))
        {
            sum += q_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        q_iter_to_start(&it);
        do
        {
            sum += q_iter_value(&it);
        } while (q_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_end(q);

        cmc_assert(!q_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
            q_enqueue(q, i);

        size_t sum = 0;
        for (it = q_iter_end(q); !q_iter_at_start(&it); q_iter_prev(&it))
        {
            sum += q_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        q_iter_to_end(&it);
        do
        {
            sum += q_iter_value(&it);
        } while (q_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_start(q);

        cmc_assert(!q_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            q_enqueue(q, i);

        it = q_iter_start(q);

        cmc_assert(!q_iter_advance(&it, 0));
        cmc_assert(!q_iter_advance(&it, q->count));

        size_t sum = 0;
        for (it = q_iter_start(q);;)
        {
            sum += q_iter_value(&it);

            if (!q_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        q_iter_to_start(&it);
        cmc_assert(q_iter_advance(&it, q->count - 1));

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_end(q);

        cmc_assert(!q_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            q_enqueue(q, i);

        it = q_iter_end(q);

        cmc_assert(!q_iter_rewind(&it, 0));
        cmc_assert(!q_iter_rewind(&it, q->count));

        size_t sum = 0;
        for (it = q_iter_end(q);;)
        {
            sum += q_iter_value(&it);

            if (!q_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        q_iter_to_end(&it);
        cmc_assert(q_iter_rewind(&it, q->count - 1));

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_end(q);
        cmc_assert(!q_iter_go_to(&it, 0));

        it = q_iter_start(q);
        cmc_assert(!q_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            q_enqueue(q, i);

        it = q_iter_start(q);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            q_iter_go_to(&it, i);

            sum += q_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(q_iter_go_to(&it, i - 1));

            sum += q_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 0; i < 1001; i += 100)
        {
            cmc_assert(q_iter_go_to(&it, i));
            cmc_assert_equals(size_t, i, q_iter_index(&it));

            sum += q_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_end(q);

        cmc_assert_equals(size_t, (size_t){ 0 }, q_iter_value(&it));

        cmc_assert(q_enqueue(q, 10));

        it = q_iter_start(q);

        cmc_assert_equals(size_t, 10, q_iter_value(&it));

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_rvalue(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        struct queue_iter it = q_iter_end(q);

        cmc_assert_equals(ptr, NULL, q_iter_rvalue(&it));

        cmc_assert(q_enqueue(q, 10));

        it = q_iter_start(q);

        cmc_assert_not_equals(ptr, NULL, q_iter_rvalue(&it));
        cmc_assert_equals(size_t, 10, *q_iter_rvalue(&it));

        q_free(q);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct queue *q = q_new(100, q_fval);

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 0; i <= 1000; i++)
            q_enqueue(q, i);

        struct queue_iter it = q_iter_start(q);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, q_iter_index(&it));
            q_iter_next(&it);
        }

        it = q_iter_end(q);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, q_iter_index(&it));
            q_iter_prev(&it);
        }

        q_free(q);
    });
});

#endif /* CMC_TESTS_UNT_QUEUE_H */
