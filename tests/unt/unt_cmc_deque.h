#ifndef CMC_TESTS_UNT_CMC_EQUE_H
#define CMC_TESTS_UNT_CMC_EQUE_H

#include "utl.h"

#include "tst_cmc_deque.h"

struct deque_fval *d_fval = &(struct deque_fval){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

struct deque_fval *d_fval_counter = &(struct deque_fval){
    .cmp = v_c_cmp, .cpy = v_c_cpy, .str = v_c_str, .free = v_c_free, .hash = v_c_hash, .pri = v_c_pri
};

struct cmc_alloc_node *d_alloc_node =
    &(struct cmc_alloc_node){ .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free };

CMC_CREATE_UNIT(CMCDeque, true, {
    CMC_CREATE_TEST(PFX##_new(), {
        struct deque *d = d_new(1000000, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);
        cmc_assert_not_equals(ptr, NULL, d->buffer);
        cmc_assert_equals(size_t, 1000000, d->capacity);
        cmc_assert_equals(size_t, 0, d->count);
        cmc_assert_equals(size_t, 0, d->front);
        cmc_assert_equals(size_t, 0, d->back);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d->flag);
        cmc_assert_equals(ptr, d_fval, d->f_val);
        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, d->alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, d->alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, d->alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, d->alloc->free);
        cmc_assert_equals(ptr, NULL, d->callbacks);

        d_free(d);

        d = d_new(0, d_fval);
        cmc_assert_equals(ptr, NULL, d);

        d = d_new(1000, NULL);
        cmc_assert_equals(ptr, NULL, d);

        d = d_new(UINT64_MAX, d_fval);
        cmc_assert_equals(ptr, NULL, d);
    });

    CMC_CREATE_TEST(PFX##_new_custom(), {
        struct deque *d = d_new_custom(1000000, d_fval, d_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, d);
        cmc_assert_not_equals(ptr, NULL, d->buffer);
        cmc_assert_equals(size_t, 1000000, d->capacity);
        cmc_assert_equals(size_t, 0, d->count);
        cmc_assert_equals(size_t, 0, d->front);
        cmc_assert_equals(size_t, 0, d->back);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d->flag);
        cmc_assert_equals(ptr, d_fval, d->f_val);
        cmc_assert_equals(ptr, d_alloc_node, d->alloc);
        cmc_assert_equals(ptr, callbacks, d->callbacks);

        d_free(d);

        d = d_new_custom(0, d_fval, d_alloc_node, callbacks);
        cmc_assert_equals(ptr, NULL, d);

        d = d_new_custom(1000, NULL, d_alloc_node, callbacks);
        cmc_assert_equals(ptr, NULL, d);

        d = d_new_custom(UINT64_MAX, d_fval, d_alloc_node, callbacks);
        cmc_assert_equals(ptr, NULL, d);
    });

    CMC_CREATE_TEST(PFX##_init(), {
        struct deque d = d_init(1000000, d_fval);

        cmc_assert_not_equals(ptr, NULL, d.buffer);
        cmc_assert_equals(size_t, 1000000, d.capacity);
        cmc_assert_equals(size_t, 0, d.count);
        cmc_assert_equals(size_t, 0, d.front);
        cmc_assert_equals(size_t, 0, d.back);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d.flag);
        cmc_assert_equals(ptr, d_fval, d.f_val);
        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, d.alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, d.alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, d.alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, d.alloc->free);
        cmc_assert_equals(ptr, NULL, d.callbacks);

        d_release(d);

        d = d_init(0, d_fval);

        cmc_assert_equals(size_t, 0, d.capacity);
        cmc_assert_equals(ptr, NULL, d.buffer);

        d = d_init(1000, NULL);

        cmc_assert_equals(size_t, 0, d.capacity);
        cmc_assert_equals(ptr, NULL, d.buffer);

        d = d_init(UINT64_MAX, d_fval);

        cmc_assert_equals(size_t, 0, d.capacity);
        cmc_assert_equals(ptr, NULL, d.buffer);

        if (d.buffer)
            d_release(d);
    });

    CMC_CREATE_TEST(PFX##_init_custom(), {
        struct deque d = d_init_custom(1000000, d_fval, d_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, d.buffer);
        cmc_assert_equals(size_t, 1000000, d.capacity);
        cmc_assert_equals(size_t, 0, d.count);
        cmc_assert_equals(size_t, 0, d.front);
        cmc_assert_equals(size_t, 0, d.back);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d.flag);
        cmc_assert_equals(ptr, d_fval, d.f_val);
        cmc_assert_equals(ptr, d_alloc_node, d.alloc);
        cmc_assert_equals(ptr, callbacks, d.callbacks);

        d_release(d);

        d = d_init_custom(0, d_fval, d_alloc_node, callbacks);

        cmc_assert_equals(size_t, 0, d.capacity);
        cmc_assert_equals(ptr, NULL, d.buffer);

        d = d_init_custom(1000, NULL, d_alloc_node, callbacks);

        cmc_assert_equals(size_t, 0, d.capacity);
        cmc_assert_equals(ptr, NULL, d.buffer);

        d = d_init_custom(UINT64_MAX, d_fval, d_alloc_node, callbacks);

        cmc_assert_equals(size_t, 0, d.capacity);
        cmc_assert_equals(ptr, NULL, d.buffer);

        if (d.buffer)
            d_release(d);
    });

    CMC_CREATE_TEST(PFX##_clear(), {
        v_total_free = 0;
        struct deque *d = d_new(100, d_fval_counter);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 0; i < 50; i++)
            d_push_back(d, i);

        cmc_assert_equals(size_t, 50, d_count(d));

        d->flag = CMC_FLAG_ERROR;
        d_clear(d);

        cmc_assert_equals(size_t, 50, v_total_free);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        cmc_assert_equals(size_t, 0, d_count(d));
        cmc_assert_equals(size_t, 100, d_capacity(d));

        for (size_t i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
                d_push_front(d, i);
            else
                d_push_back(d, i);
        }

        cmc_assert_equals(size_t, 1000, d_count(d));

        size_t sum = 0;
        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
            sum += d->buffer[i];

        cmc_assert_equals(size_t, 500500, sum);

        d->flag = CMC_FLAG_ERROR;
        d_clear(d);

        cmc_assert_equals(size_t, 1050, v_total_free);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        sum = 0;
        for (size_t i = 0; i < d->capacity; i++)
            sum += d->buffer[i];

        cmc_assert_equals(size_t, 0, sum);

        d_free(d);

        cmc_assert_equals(size_t, 1050, v_total_free);

        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_free(), {
        v_total_free = 0;

        struct deque *d = d_new(100, d_fval_counter);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
                d_push_front(d, i);
            else
                d_push_back(d, i);
        }

        cmc_assert_equals(size_t, 1000, d_count(d));

        size_t sum = 0;
        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
            sum += d->buffer[i];

        cmc_assert_equals(size_t, 500500, sum);

        d_free(d);

        cmc_assert_equals(size_t, 1000, v_total_free);

        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_release(), {
        v_total_free = 0;

        struct deque d = d_init(100, d_fval_counter);

        cmc_assert_not_equals(ptr, NULL, d.buffer);

        for (size_t i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
                d_push_front(&d, i);
            else
                d_push_back(&d, i);
        }

        cmc_assert_equals(size_t, 1000, d_count(&d));

        size_t sum = 0;
        for (size_t i = d.front, j = 0; j < d.count; i = (i + 1) % d.capacity, j++)
            sum += d.buffer[i];

        cmc_assert_equals(size_t, 500500, sum);

        d_release(d);

        cmc_assert_equals(size_t, 1000, v_total_free);

        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_customize(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        d->flag = CMC_FLAG_ERROR;
        d_customize(d, d_alloc_node, callbacks);

        cmc_assert_equals(ptr, d_alloc_node, d->alloc);
        cmc_assert_equals(ptr, callbacks, d->callbacks);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        d->flag = CMC_FLAG_ERROR;
        d_customize(d, NULL, NULL);

        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, d->alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, d->alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, d->alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, d->alloc->free);
        cmc_assert_equals(ptr, NULL, d->callbacks);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_push_front(), {
        total_create = 0;
        struct deque *d = d_new_custom(100, d_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, d);

        d->flag = CMC_FLAG_ERROR;
        cmc_assert(d_push_front(d, 10));

        cmc_assert_equals(size_t, 1, d_count(d));
        cmc_assert_equals(int32_t, 1, total_create);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        d_customize(d, NULL, NULL);

        d_clear(d);

        for (size_t i = 0; i < 150; i++)
            d_push_front(d, i);

        cmc_assert_equals(size_t, 150, d_count(d));
        cmc_assert_greater(size_t, 100, d_capacity(d));

        d_free(d);

        d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 10000; i++)
            d_push_front(d, i);

        size_t sum = 0;

        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
        {
            sum += d->buffer[i];
        }

        cmc_assert_equals(size_t, 50005000, sum);

        d_free(d);

        cmc_assert_equals(int32_t, 1, total_create);
        total_create = 0;
    });

    CMC_CREATE_TEST(PFX##_push_back(), {
        total_create = 0;
        struct deque *d = d_new_custom(100, d_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, d);

        d->flag = CMC_FLAG_ERROR;
        cmc_assert(d_push_front(d, 10));

        cmc_assert_equals(size_t, 1, d_count(d));
        cmc_assert_equals(int32_t, 1, total_create);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        d_customize(d, NULL, NULL);

        d_clear(d);

        for (size_t i = 0; i < 150; i++)
            d_push_back(d, i);

        cmc_assert_equals(size_t, 150, d_count(d));
        cmc_assert_greater(size_t, 100, d_capacity(d));

        d_free(d);

        d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 10000; i++)
            d_push_back(d, i);

        size_t sum = 0;

        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
        {
            sum += d->buffer[i];
        }

        cmc_assert_equals(size_t, 50005000, sum);

        d_free(d);

        cmc_assert_equals(int32_t, 1, total_create);
        total_create = 0;
    });

    CMC_CREATE_TEST(PFX##_pop_front(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(!d_pop_front(d));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, d_flag(d));

        cmc_assert(d_push_front(d, 10));
        d->flag = CMC_FLAG_ERROR;
        cmc_assert(d_pop_front(d));

        cmc_assert_equals(size_t, 0, d_count(d));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_pop_front(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        d_free(d);

        d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 20000; i++)
            d_push_front(d, i);

        cmc_assert_equals(size_t, 20000, d_count(d));

        for (size_t i = 0; i < 10000; i++)
            d_pop_front(d);

        size_t sum = 0;

        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
        {
            sum += d->buffer[i];
        }

        cmc_assert_equals(size_t, 50005000, sum);

        d_clear(d);

        for (size_t i = 1; i <= 20000; i++)
            d_push_front(d, i);

        cmc_assert_equals(size_t, 20000, d_count(d));

        for (size_t i = 0; i < 10000; i++)
            d_pop_front(d);

        sum = 0;

        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
        {
            sum += d->buffer[i];
        }

        cmc_assert_equals(size_t, 50005000, sum);

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_pop_back(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(!d_pop_back(d));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, d_flag(d));

        cmc_assert(d_push_front(d, 10));
        d->flag = CMC_FLAG_ERROR;
        cmc_assert(d_pop_back(d));

        cmc_assert_equals(size_t, 0, d_count(d));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_pop_back(d));

        cmc_assert_equals(size_t, 0, d_count(d));

        d_free(d);

        d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 20000; i++)
            d_push_back(d, i);

        for (size_t i = 0; i < 10000; i++)
            d_pop_back(d);

        size_t sum = 0;

        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
        {
            sum += d->buffer[i];
        }

        cmc_assert_equals(size_t, 50005000, sum);

        d_clear(d);

        for (size_t i = 1; i <= 20000; i++)
            d_push_back(d, i);

        cmc_assert_equals(size_t, 20000, d_count(d));

        for (size_t i = 0; i < 10000; i++)
            d_pop_back(d);

        sum = 0;

        for (size_t i = d->front, j = 0; j < d->count; i = (i + 1) % d->capacity, j++)
        {
            sum += d->buffer[i];
        }

        cmc_assert_equals(size_t, 50005000, sum);

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_front(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        d_front(d);

        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, d_flag(d));

        cmc_assert(d_push_front(d, 99));
        d->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, 99, d_front(d));

        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        cmc_assert(d_push_back(d, 98));
        cmc_assert_equals(size_t, 99, d_front(d));
        cmc_assert(d_pop_front(d));
        cmc_assert_equals(size_t, 98, d_front(d));

        d_clear(d);

        cmc_assert(d_push_back(d, 99));
        cmc_assert_equals(size_t, 99, d_front(d));

        d_clear(d);

        cmc_assert_equals(size_t, 0, d_count(d));

        for (size_t i = 1; i <= 10000; i++)
        {
            if (i % 2 == 0)
                d_push_front(d, i);
            else
                d_push_back(d, i);
        }

        size_t sum = 0;
        for (size_t i = 0; i < 5000; i++)
        {
            sum += d_front(d);
            cmc_assert_equals(int32_t, 0, d_front(d) % 2);
            d_pop_front(d);
        }

        cmc_assert_equals(size_t, 5000, d_count(d));
        cmc_assert_equals(size_t, 25005000, sum);

        d_clear(d);

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_back(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        d_back(d);

        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, d_flag(d));

        cmc_assert(d_push_back(d, 99));
        d->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, 99, d_back(d));

        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        cmc_assert(d_push_front(d, 98));
        cmc_assert_equals(size_t, 99, d_back(d));
        cmc_assert(d_pop_back(d));
        cmc_assert_equals(size_t, 98, d_back(d));

        d_clear(d);

        cmc_assert(d_push_front(d, 99));
        cmc_assert_equals(size_t, 99, d_back(d));

        d_clear(d);

        cmc_assert_equals(size_t, 0, d_count(d));

        for (size_t i = 1; i <= 10000; i++)
        {
            if (i % 2 == 0)
                d_push_back(d, i);
            else
                d_push_front(d, i);
        }

        size_t sum = 0;
        for (size_t i = 0; i < 5000; i++)
        {
            sum += d_back(d);
            cmc_assert_equals(int32_t, 0, d_back(d) % 2);
            d_pop_back(d);
        }

        cmc_assert_equals(size_t, 5000, d_count(d));
        cmc_assert_equals(size_t, 25005000, sum);

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_contains(), {
        struct deque *d = d_new(100, d_fval);

        for (size_t i = 0; i < 100; i++)
        {
            if (i % 2 == 0)
                d_push_back(d, i);
            else
                d_push_front(d, i);
        }

        for (size_t i = 0; i < 100; i++)
            cmc_assert(d_contains(d, i));

        d_free(d);

        d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 1; i <= 20; i++)
            d_push_back(d, i);

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(d_contains(d, i));

        d_clear(d);

        for (size_t i = 1; i <= 20; i++)
            d_push_front(d, i);

        for (size_t i = 1; i <= 20; i++)
            cmc_assert(d_contains(d, i));

        d_free(d);

        d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(!d_contains(d, 10));

        d_free(d);

        d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(!d_contains(d, 10));

        cmc_assert(d_push_back(d, 100));

        cmc_assert(d_contains(d, 100));
        cmc_assert(!d_contains(d, 101));

        cmc_assert(d_pop_front(d));
        cmc_assert(d_push_front(d, 100));

        cmc_assert(d_contains(d, 100));
        cmc_assert(!d_contains(d, 101));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_empty(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(d_empty(d));

        cmc_assert(d_push_back(d, 1));

        cmc_assert(!d_empty(d));

        cmc_assert(d_pop_front(d));

        cmc_assert(d_empty(d));

        cmc_assert(d_push_front(d, 1));

        cmc_assert(!d_empty(d));

        cmc_assert(d_pop_back(d));

        cmc_assert(d_empty(d));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_full(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert(!d_full(d));

        for (size_t i = 0; i != d_capacity(d); i++)
            d_push_front(d, i);

        cmc_assert(d_full(d));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_count(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert_equals(size_t, 0, d_count(d));

        for (size_t i = 0; i < 100; i++)
            d_push_back(d, i);

        cmc_assert_equals(size_t, 100, d_count(d));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_capacity(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert_equals(size_t, 100, d_capacity(d));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_resize(), {
        total_resize = 0;
        struct deque *d = d_new_custom(100, d_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, d);

        cmc_assert_equals(size_t, 100, d_capacity(d));
        d->flag = CMC_FLAG_ERROR;
        cmc_assert(d_resize(d, 100));
        cmc_assert_equals(size_t, 100, d_capacity(d));

        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(d_resize(d, 200));
        cmc_assert_equals(size_t, 200, d_capacity(d));

        cmc_assert(d_resize(d, 100));
        cmc_assert_equals(size_t, 100, d_capacity(d));

        cmc_assert_equals(int32_t, 3, total_resize);

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

        cmc_assert(!d_resize(d, d_count(d) - 10));
        cmc_assert_equals(int32_t, CMC_FLAG_INVALID, d_flag(d));

        d_free(d);

        // item preservation
        d = d_new(100, d_fval);

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

        d_free(d);
        total_resize = 0;
    });

    CMC_CREATE_TEST(PFX##_copy_of(), {
        v_total_cpy = 0;
        struct deque *d1 = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d1);

        for (size_t i = 1; i <= 150; i++)
        {
            if (i % 2 != 0)
                d_push_back(d1, i);
            else
                d_push_front(d1, i);
        }

        struct deque *d2 = d_copy_of(d1);

        cmc_assert_not_equals(ptr, NULL, d2);
        cmc_assert_equals(size_t, d_count(d1), d_count(d2));

        for (size_t i1 = d1->front, i2 = d2->front, j = 0; j < d1->count; j++)
        {
            cmc_assert_equals(size_t, d1->buffer[i1], d2->buffer[i2]);

            i1 = (i1 + 1) % d1->capacity;
            i2 = (i2 + 1) % d2->capacity;
        }

        cmc_assert(d_push_front(d1, 10));
        cmc_assert(d_push_front(d2, 10));
        cmc_assert(d_push_back(d1, 11));
        cmc_assert(d_push_back(d2, 11));

        cmc_assert(d_equals(d1, d2));

        d_free(d1);
        d_free(d2);

        // v_total_cpy
        d1 = d_new_custom(100, d_fval_counter, d_alloc_node, NULL);

        cmc_assert_not_equals(ptr, NULL, d1);

        d2 = d_copy_of(d1);

        cmc_assert_not_equals(ptr, NULL, d1);

        cmc_assert_equals(int32_t, 0, v_total_cpy);

        d_free(d2);

        for (size_t i = 0; i < 1000; i++)
        {
            if (i % 2 != 0)
                d_push_back(d1, i);
            else
                d_push_front(d1, i);
        }

        cmc_assert_equals(size_t, 1000, d1->count);

        d2 = d_copy_of(d1);

        cmc_assert_equals(int32_t, 1000, v_total_cpy);

        d_free(d1);
        d_free(d2);
        v_total_cpy = 0;
    });

    CMC_CREATE_TEST(PFX##_equals(), {
        v_total_cmp = 0;
        struct deque *d1 = d_new(100, d_fval);
        struct deque *d2 = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d1);
        cmc_assert_not_equals(ptr, NULL, d2);

        for (size_t i = 1; i <= 150; i++)
        {
            if (i % 2 != 0)
            {
                d_push_back(d1, i);
                d_push_back(d2, i);
            }
            else
            {
                d_push_front(d1, i);
                d_push_front(d2, i);
            }
        }

        cmc_assert(d_equals(d1, d2));

        d_free(d1);
        d_free(d2);

        // from copy
        d1 = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d1);

        for (size_t i = 1; i <= 150; i++)
        {
            if (i % 2 != 0)
                cmc_assert(d_push_back(d1, i));
            else
                cmc_assert(d_push_front(d1, i));
        }

        d2 = d_copy_of(d1);

        cmc_assert_not_equals(ptr, NULL, d2);

        cmc_assert(d_equals(d1, d2));

        d_free(d1);
        d_free(d2);

        // count = 0
        d1 = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d1);

        d2 = d_copy_of(d1);

        cmc_assert_not_equals(ptr, NULL, d2);

        cmc_assert(d_equals(d1, d2));

        d_free(d1);
        d_free(d2);

        // false eq
        d1 = d_new(100, d_fval);
        d2 = d_new_custom(100, d_fval_counter, d_alloc_node, NULL);

        cmc_assert(d_equals(d1, d2));

        cmc_assert_equals(int32_t, 0, v_total_cmp);

        d_push_back(d1, 100);

        cmc_assert(!d_equals(d1, d2));

        d_push_back(d2, 100);

        cmc_assert(d_equals(d1, d2));

        d_push_front(d1, 10);
        d_push_front(d2, 10);

        cmc_assert(d_equals(d1, d2));

        d_push_back(d1, 40);
        d_push_back(d2, 41);

        cmc_assert(!d_equals(d1, d2));

        v_total_cmp = 0;

        d_clear(d1);
        d_clear(d2);

        for (size_t i = 0; i < 1000; i++)
        {
            if (i % 2 == 0)
            {
                d_push_back(d1, i);
                d_push_back(d2, i);
            }
            else
            {
                d_push_front(d1, i);
                d_push_front(d2, i);
            }
        }

        // d2's cmp is used and v_total_cmp is incremented each time
        cmc_assert(d_equals(d2, d1));

        cmc_assert_equals(int32_t, 1000, v_total_cmp);

        d_free(d1);
        d_free(d2);
        v_total_cmp = 0;
    });

    CMC_CREATE_TEST(Mixed IO, {
        struct deque *d = d_new(100, d_fval);

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

        d_free(d);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct deque *d = d_new(1, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 0; i < 50; i++)
            d_push_back(d, i);

        cmc_assert_equals(size_t, 50, d_count(d));
        cmc_assert_lesser_equals(size_t, d_capacity(d), d_count(d));

        d_free(d);
    });

    CMC_CREATE_TEST(buffer_growth[item preservation], {
        struct deque *d = d_new(100, d_fval);

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

        d_free(d);
    });

    CMC_CREATE_TEST(flags, {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        // customize
        d->flag = CMC_FLAG_ERROR;
        d_customize(d, &cmc_alloc_node_default, &(struct cmc_callbacks){ 0 });
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        // push_front
        d->flag = CMC_FLAG_ERROR;
        cmc_assert(d_push_front(d, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        // pop_front
        d->flag = CMC_FLAG_ERROR;
        cmc_assert(d_pop_front(d));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));
        cmc_assert(!d_pop_front(d));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, d_flag(d));

        // push_back
        cmc_assert(d_push_back(d, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        // pop_back
        d->flag = CMC_FLAG_ERROR;
        cmc_assert(d_pop_back(d));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));
        cmc_assert(!d_pop_back(d));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, d_flag(d));

        // front
        d->flag = CMC_FLAG_ERROR;
        cmc_assert(d_front(d) == 0);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, d_flag(d));

        cmc_assert(d_push_back(d, 1));
        d->flag = CMC_FLAG_ERROR;
        cmc_assert(d_front(d) == 1);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        // clear
        d->flag = CMC_FLAG_ERROR;
        d_clear(d);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        // back
        cmc_assert(d_back(d) == 0);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, d_flag(d));

        cmc_assert(d_push_front(d, 1));
        cmc_assert(d_back(d) == 1);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        // contains
        d->flag = CMC_FLAG_ERROR;
        cmc_assert(d_contains(d, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));

        // copy_of
        d->flag = CMC_FLAG_ERROR;
        struct deque *d2 = d_copy_of(d);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d2));

        // equals
        cmc_assert(d_pop_back(d));
        cmc_assert(d_pop_back(d2));
        d->flag = CMC_FLAG_ERROR;
        d2->flag = CMC_FLAG_ERROR;
        d_front(d);
        d_front(d2);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, d_flag(d));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, d_flag(d2));
        cmc_assert(d_equals(d, d2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, d_flag(d2));

        d_free(d);
        d_free(d2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct deque *d = d_new_custom(100, d_fval, NULL, callbacks);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(d_push_front(d, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(d_push_back(d, 10));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(d_pop_front(d));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(d_pop_back(d));
        cmc_assert_equals(int32_t, 2, total_delete);

        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_push_front(d, 5));
        cmc_assert_equals(int32_t, 4, total_create);

        cmc_assert_equals(size_t, 5, d_front(d));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert_equals(size_t, 10, d_back(d));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert(d_contains(d, 10));
        cmc_assert(!d_contains(d, 1));
        cmc_assert_equals(int32_t, 4, total_read);

        cmc_assert(d_resize(d, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(d_resize(d, 10));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 4, total_create);
        cmc_assert_equals(int32_t, 4, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        d_customize(d, NULL, NULL);

        cmc_assert_equals(ptr, NULL, d->callbacks);

        d_clear(d);
        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_push_back(d, 10));
        cmc_assert(d_pop_front(d));
        cmc_assert(d_pop_back(d));
        cmc_assert(d_push_front(d, 10));
        cmc_assert(d_push_front(d, 5));
        cmc_assert_equals(size_t, 5, d_front(d));
        cmc_assert_equals(size_t, 10, d_back(d));
        cmc_assert(d_contains(d, 10));
        cmc_assert(!d_contains(d, 1));
        cmc_assert(d_resize(d, 1000));
        cmc_assert(d_resize(d, 10));

        cmc_assert_equals(int32_t, 4, total_create);
        cmc_assert_equals(int32_t, 4, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, d->callbacks);

        d_free(d);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});

CMC_CREATE_UNIT(CMCDequeIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_start(d);

        cmc_assert_equals(ptr, d, it.target);
        cmc_assert_equals(size_t, d->front, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(d_iter_at_start(&it));
        cmc_assert(d_iter_at_end(&it));

        cmc_assert(d_push_front(d, 1));
        cmc_assert(d_push_front(d, 2));

        it = d_iter_start(d);

        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, false, it.end);
        cmc_assert_equals(size_t, d->front, it.cursor);

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_end(d);

        cmc_assert_equals(ptr, d, it.target);
        cmc_assert_equals(size_t, d->back, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(d_iter_at_start(&it));
        cmc_assert(d_iter_at_end(&it));

        cmc_assert(d_push_back(d, 1));
        cmc_assert(d_push_back(d, 2));
        cmc_assert(d_push_back(d, 3));

        it = d_iter_end(d);

        cmc_assert_equals(size_t, d->count - 1, it.index);
        cmc_assert_equals(size_t, d->back - 1, it.cursor);
        cmc_assert_equals(bool, false, it.start);

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_start(d);

        // Empty checks
        cmc_assert(d_iter_at_start(&it));
        it = d_iter_end(d);
        cmc_assert(d_iter_at_start(&it));

        // Non-empty checks
        cmc_assert(d_push_back(d, 1));
        it = d_iter_end(d);
        cmc_assert(!d_iter_at_start(&it));
        it = d_iter_start(d);
        cmc_assert(d_iter_at_start(&it));

        d_clear(d);

        cmc_assert(d_push_front(d, 1));
        it = d_iter_end(d);
        cmc_assert(!d_iter_at_start(&it));
        it = d_iter_start(d);
        cmc_assert(d_iter_at_start(&it));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_start(d);

        // Empty check
        cmc_assert(d_iter_at_end(&it));
        it = d_iter_end(d);
        cmc_assert(d_iter_at_end(&it));

        // Non-empty checks
        cmc_assert(d_push_back(d, 1));
        it = d_iter_end(d);
        cmc_assert(d_iter_at_end(&it));
        it = d_iter_start(d);
        cmc_assert(!d_iter_at_end(&it));

        d_clear(d);

        cmc_assert(d_push_front(d, 1));
        it = d_iter_end(d);
        cmc_assert(d_iter_at_end(&it));
        it = d_iter_start(d);
        cmc_assert(!d_iter_at_end(&it));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_start(d);

        cmc_assert(!d_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
        {
            if (i % 2 == 0)
                d_push_back(d, i);
            else
                d_push_front(d, i);
        }

        cmc_assert_equals(size_t, 100, d->count);

        it = d_iter_end(d);

        cmc_assert(!d_iter_at_start(&it));
        cmc_assert(d_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, d_iter_value(&it));

        cmc_assert(d_iter_to_start(&it));

        cmc_assert(d_iter_at_start(&it));
        cmc_assert(!d_iter_at_end(&it));

        cmc_assert_equals(size_t, 99, d_iter_value(&it));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_end(d);

        cmc_assert(!d_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
        {
            if (i % 2 == 0)
                d_push_back(d, i);
            else
                d_push_front(d, i);
        }

        it = d_iter_start(d);

        cmc_assert(d_iter_at_start(&it));
        cmc_assert(!d_iter_at_end(&it));

        cmc_assert_equals(size_t, 99, d_iter_value(&it));

        cmc_assert(d_iter_to_end(&it));

        cmc_assert(!d_iter_at_start(&it));
        cmc_assert(d_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, d_iter_value(&it));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_start(d);

        cmc_assert(!d_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
                d_push_back(d, i);
            else
                d_push_front(d, i);
        }

        size_t sum = 0;
        for (it = d_iter_start(d); !d_iter_at_end(&it); d_iter_next(&it))
        {
            sum += d_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        d_iter_to_start(&it);
        do
        {
            sum += d_iter_value(&it);
        } while (d_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_end(d);

        cmc_assert(!d_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
                d_push_back(d, i);
            else
                d_push_front(d, i);
        }

        size_t sum = 0;
        for (it = d_iter_end(d); !d_iter_at_start(&it); d_iter_prev(&it))
        {
            sum += d_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        d_iter_to_end(&it);
        do
        {
            sum += d_iter_value(&it);
        } while (d_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_start(d);

        cmc_assert(!d_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
        {
            if (i % 2 == 0)
                d_push_back(d, i); // will sum all even numbers
            else
                d_push_back(d, 0);
        }

        it = d_iter_start(d);

        cmc_assert(!d_iter_advance(&it, 0));
        cmc_assert(!d_iter_advance(&it, d->count));

        size_t sum = 0;
        for (it = d_iter_start(d);;)
        {
            sum += d_iter_value(&it);

            if (!d_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        d_iter_to_start(&it);
        cmc_assert(d_iter_advance(&it, d->count - 1));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_end(d);

        cmc_assert(!d_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
        {
            if (i % 2 == 0)
                d_push_front(d, i); // will sum all even numbers
            else
                d_push_front(d, 0);
        }

        it = d_iter_end(d);

        cmc_assert(!d_iter_rewind(&it, 0));
        cmc_assert(!d_iter_rewind(&it, d->count));

        size_t sum = 0;
        for (it = d_iter_end(d);;)
        {
            sum += d_iter_value(&it);

            if (!d_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        d_iter_to_end(&it);
        cmc_assert(d_iter_rewind(&it, d->count - 1));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_end(d);
        cmc_assert(!d_iter_go_to(&it, 0));

        it = d_iter_start(d);
        cmc_assert(!d_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            d_push_back(d, i);

        it = d_iter_start(d);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            d_iter_go_to(&it, i);

            sum += d_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(d_iter_go_to(&it, i - 1));

            sum += d_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 0; i < 1001; i += 100)
        {
            cmc_assert(d_iter_go_to(&it, i));
            cmc_assert_equals(size_t, i, d_iter_index(&it));

            sum += d_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_end(d);

        cmc_assert_equals(size_t, (size_t){ 0 }, d_iter_value(&it));

        cmc_assert(d_push_back(d, 10));

        it = d_iter_start(d);

        cmc_assert_equals(size_t, 10, d_iter_value(&it));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_rvalue(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        struct deque_iter it = d_iter_end(d);

        cmc_assert_equals(ptr, NULL, d_iter_rvalue(&it));

        cmc_assert(d_push_back(d, 10));

        it = d_iter_start(d);

        cmc_assert_not_equals(ptr, NULL, d_iter_rvalue(&it));
        cmc_assert_equals(size_t, 10, *d_iter_rvalue(&it));

        d_free(d);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct deque *d = d_new(100, d_fval);

        cmc_assert_not_equals(ptr, NULL, d);

        for (size_t i = 0; i <= 1000; i++)
            d_push_back(d, i);

        struct deque_iter it = d_iter_start(d);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, d_iter_index(&it));
            d_iter_next(&it);
        }

        it = d_iter_end(d);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, d_iter_index(&it));
            d_iter_prev(&it);
        }

        d_free(d);
    });
});

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = CMCDeque() + CMCDequeIter();

    printf(" +---------------------------------------------------------------+");
    printf("\n");
    printf(" | CMCDeque Suit : %-48s |\n", result == 0 ? "PASSED" : "FAILED");
    printf(" +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif

#endif /* CMC_TESTS_UNT_CMC_EQUE_H */
