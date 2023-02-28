#ifndef CMC_TESTS_UNT_INTERVALHEAP_H
#define CMC_TESTS_UNT_INTERVALHEAP_H

#include "utl.h"

#define V size_t
#define PFX ih
#define SNAME intervalheap
#include "cmc/intervalheap.h"

struct intervalheap_fval *ih_fval = &(struct intervalheap_fval){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

struct intervalheap_fval *ih_fval_counter = &(struct intervalheap_fval){
    .cmp = v_c_cmp, .cpy = v_c_cpy, .str = v_c_str, .free = v_c_free, .hash = v_c_hash, .pri = v_c_pri
};

struct cmc_alloc_node *ih_alloc_node =
    &(struct cmc_alloc_node){ .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free };

CMC_CREATE_UNIT(CMCIntervalHeap, true, {
    CMC_CREATE_TEST(PFX##_new(), {
        struct intervalheap *ih = ih_new(1000000, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_not_equals(ptr, NULL, ih->buffer);
        cmc_assert_equals(size_t, 500000, ih_capacity(ih));
        cmc_assert_equals(size_t, 0, ih_count(ih));

        ih_free(ih);

        ih = ih_new(0, ih_fval);

        cmc_assert_equals(ptr, NULL, ih);

        ih = ih_new(UINT64_MAX, ih_fval);

        cmc_assert_equals(ptr, NULL, ih);

        ih = ih_new(1, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 1, ih_capacity(ih));

        ih_free(ih);

        ih = ih_new(2, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 1, ih_capacity(ih));

        ih_free(ih);

        ih = ih_new(3, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 2, ih_capacity(ih));

        ih_free(ih);

        ih = ih_new(4, ih_fval);
        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 2, ih_capacity(ih));

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_new_custom(), {
        struct intervalheap *ih = ih_new_custom(1000000, ih_fval, ih_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_not_equals(ptr, NULL, ih->buffer);
        cmc_assert_equals(size_t, 500000, ih_capacity(ih));
        cmc_assert_equals(size_t, 0, ih_count(ih));
        cmc_assert_equals(ptr, ih_alloc_node, ih->alloc);
        cmc_assert_equals(ptr, callbacks, ih->callbacks);

        ih_free(ih);

        ih = ih_new_custom(0, ih_fval, NULL, NULL);

        cmc_assert_equals(ptr, NULL, ih);

        ih = ih_new_custom(UINT64_MAX, ih_fval, NULL, NULL);

        cmc_assert_equals(ptr, NULL, ih);

        ih = ih_new_custom(1, ih_fval, NULL, NULL);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 1, ih_capacity(ih));
        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, ih->alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, ih->alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, ih->alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, ih->alloc->free);
        cmc_assert_equals(ptr, NULL, ih->callbacks);

        ih_free(ih);

        ih = ih_new_custom(2, ih_fval, NULL, NULL);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 1, ih_capacity(ih));

        ih_free(ih);

        ih = ih_new_custom(3, ih_fval, NULL, NULL);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 2, ih_capacity(ih));

        ih_free(ih);

        ih = ih_new_custom(4, ih_fval, NULL, NULL);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(size_t, 2, ih_capacity(ih));

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_clear(), {
        v_total_free = 0;
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        for (size_t i = 0; i < 50; i++)
            ih_insert(ih, i);

        cmc_assert_equals(size_t, 50, ih_count(ih));

        ih_clear(ih);

        cmc_assert_equals(size_t, 0, ih_count(ih));
        cmc_assert_equals(size_t, 50, ih_capacity(ih));

        ih_free(ih);

        ih = ih_new(100, ih_fval_counter);

        ih_insert(ih, 10);
        ih_clear(ih);

        cmc_assert_equals(int32_t, 1, v_total_free);

        for (size_t i = 0; i < 10000; i++)
            ih_insert(ih, i);

        cmc_assert_equals(size_t, 10000, ih_count(ih));

        ih_clear(ih);

        cmc_assert_equals(int32_t, 10001, v_total_free);

        size_t sum = 0;
        for (size_t i = 0; i < ih->capacity; i++)
            sum += ih->buffer[i / 2][i % 2];

        cmc_assert_equals(size_t, 0, sum);

        ih_free(ih);

        cmc_assert_equals(int32_t, 10001, v_total_free);

        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_free(), {
        v_total_free = 0;
        struct intervalheap *ih = ih_new(100, ih_fval_counter);

        cmc_assert_not_equals(ptr, NULL, ih);

        ih_insert(ih, 10);
        ih_free(ih);

        cmc_assert_equals(int32_t, 1, v_total_free);

        ih = ih_new(100, ih_fval_counter);

        for (size_t i = 0; i < 10000; i++)
            ih_insert(ih, i);

        ih_free(ih);

        cmc_assert_equals(int32_t, 10001, v_total_free);

        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_customize(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        ih->flag = CMC_FLAG_ERROR;
        ih_customize(ih, ih_alloc_node, callbacks);

        cmc_assert_equals(ptr, ih_alloc_node, ih->alloc);
        cmc_assert_equals(ptr, callbacks, ih->callbacks);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih->flag);

        ih->flag = CMC_FLAG_ERROR;
        ih_customize(ih, NULL, NULL);

        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, ih->alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, ih->alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, ih->alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, ih->alloc->free);
        cmc_assert_equals(ptr, NULL, ih->callbacks);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih->flag);

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_insert(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        for (size_t i = 0; i < 100; i++)
            ih_insert(ih, 10);

        cmc_assert_equals(size_t, 100, ih_count(ih));

        ih_clear(ih);

        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 0));

        cmc_assert_equals(size_t, 2, ih_count(ih));

        ih_free(ih);

        // max
        ih = ih_new(1, ih_fval);

        cmc_assert(ih_insert(ih, 0));
        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 2));

        cmc_assert_equals(size_t, 2, ih_max(ih));

        ih_free(ih);

        // min
        ih = ih_new(1, ih_fval);

        cmc_assert(ih_insert(ih, 0));
        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 2));

        cmc_assert_equals(size_t, 0, ih_min(ih));

        ih_free(ih);

        // remove_max, remove_min
        ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        cmc_assert(ih_insert(ih, 2));
        cmc_assert_equals(size_t, 2, ih->buffer[0][0]);

        cmc_assert(ih_insert(ih, 1));
        cmc_assert_equals(size_t, 1, ih->buffer[0][0]);
        cmc_assert_equals(size_t, 2, ih->buffer[0][1]);

        cmc_assert(ih_remove_min(ih));
        cmc_assert_equals(size_t, 2, ih->buffer[0][0]);

        cmc_assert(ih_insert(ih, 1));
        cmc_assert_equals(size_t, 1, ih->buffer[0][0]);
        cmc_assert_equals(size_t, 2, ih->buffer[0][1]);

        cmc_assert(ih_remove_max(ih));
        cmc_assert_equals(size_t, 1, ih->buffer[0][0]);

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_remove_max(), {
        struct intervalheap *ih = ih_new_custom(100, ih_fval, NULL, callbacks);

        // count == 0
        cmc_assert(!ih_remove_max(ih));

        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, ih->flag);

        // count == 1
        cmc_assert(ih_insert(ih, 10));
        ih->flag = CMC_FLAG_ERROR;
        cmc_assert(ih_remove_max(ih));

        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih->flag);

        for (size_t i = 1; i <= 1000; i++)
            ih_insert(ih, i);

        for (size_t i = 1000; i > 0; i--)
        {
            cmc_assert_equals(size_t, i, ih_max(ih));
            cmc_assert(ih_remove_max(ih));
        }

        cmc_assert_equals(size_t, 0, ih_count(ih));

        for (size_t i = 1; i <= 1000; i++)
        {
            ih_insert(ih, i % 50);
        }

        cmc_assert_equals(size_t, 1000, ih_count(ih));

        for (int i = 49; i >= 0; i--)
        {
            for (size_t j = 0; j < 20; j++)
            {
                cmc_assert_equals(size_t, (size_t)i, ih_max(ih));
                cmc_assert(ih_remove_max(ih));
            }
        }

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_remove_min(), {
        struct intervalheap *ih = ih_new_custom(100, ih_fval, NULL, callbacks);

        // count == 0
        cmc_assert(!ih_remove_min(ih));

        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, ih->flag);

        // count == 1
        cmc_assert(ih_insert(ih, 10));
        ih->flag = CMC_FLAG_ERROR;
        cmc_assert(ih_remove_min(ih));

        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih->flag);

        for (size_t i = 1000; i >= 1; i--)
            ih_insert(ih, i);

        for (size_t i = 1; i <= 1000; i++)
        {
            cmc_assert_equals(size_t, i, ih_min(ih));
            cmc_assert(ih_remove_min(ih));
        }

        cmc_assert_equals(size_t, 0, ih_count(ih));

        for (size_t i = 1000; i >= 1; i--)
        {
            ih_insert(ih, i % 50);
        }

        cmc_assert_equals(size_t, 1000, ih_count(ih));

        for (int i = 0; i <= 49; i++)
        {
            for (size_t j = 0; j < 20; j++)
            {
                cmc_assert_equals(size_t, (size_t)i, ih_min(ih));
                cmc_assert(ih_remove_min(ih));
            }
        }

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_remove_max() + PFX##_remove_min(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        for (size_t i = 1; i <= 10000; i++)
        {
            ih_insert(ih, i % 50);
        }

        cmc_assert_equals(size_t, 10000, ih_count(ih));

        // i = min
        // j = max
        for (size_t i = 0, j = 49; i < j; i++, j--)
        {
            for (size_t k = 0; k < 200; k++)
            {
                cmc_assert_equals(size_t, i, ih_min(ih));
                cmc_assert_equals(size_t, j, ih_max(ih));
                cmc_assert(ih_remove_min(ih));
                cmc_assert(ih_remove_max(ih));
            }
        }

        cmc_assert_equals(size_t, 0, ih_count(ih));

        ih_free(ih);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct intervalheap *ih = ih_new(1, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(ih_insert(ih, i));

        cmc_assert_equals(size_t, 50, ih_count(ih));
        cmc_assert_greater(size_t, 50, ih_capacity(ih) * 2);

        ih_free(ih);
    });

    CMC_CREATE_TEST(flags, {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));

        ih->flag = CMC_FLAG_ERROR;

        // clear
        ih_clear(ih);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));

        // customize
        ih->flag = CMC_FLAG_ERROR;
        ih_customize(ih, NULL, NULL);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));

        // insert
        ih->flag = CMC_FLAG_ERROR;
        cmc_assert(ih_insert(ih, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));

        // remove_max
        cmc_assert(ih_remove_max(ih));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));
        cmc_assert(!ih_remove_max(ih));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, ih_flag(ih));

        // remove min
        cmc_assert(ih_insert(ih, 10));
        cmc_assert(ih_remove_min(ih));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));
        cmc_assert(!ih_remove_min(ih));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, ih_flag(ih));

        // update_max
        cmc_assert(!ih_update_max(ih, 20));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, ih_flag(ih));
        cmc_assert(ih_insert(ih, 10));
        ih->flag = CMC_FLAG_ERROR;
        cmc_assert(ih_update_max(ih, 20));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));

        // update_min
        ih->flag = CMC_FLAG_ERROR;
        cmc_assert(ih_update_min(ih, 20));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));
        cmc_assert(ih_remove_min(ih));
        ih->flag = CMC_FLAG_ERROR;
        cmc_assert(!ih_update_max(ih, 20));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, ih_flag(ih));

        // max min
        ih->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, (size_t){ 0 }, ih_max(ih));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, ih_flag(ih));

        ih->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, (size_t){ 0 }, ih_min(ih));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, ih_flag(ih));

        cmc_assert(ih_insert(ih, 10));
        ih->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, 10, ih_max(ih));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));

        ih->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, 10, ih_min(ih));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));

        // contains
        ih->flag = CMC_FLAG_ERROR;
        cmc_assert(ih_contains(ih, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));

        // copy_of
        ih->flag = CMC_FLAG_ERROR;
        struct intervalheap *ih2 = ih_copy_of(ih);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih2));

        // equals
        ih->flag = CMC_FLAG_ERROR;
        ih2->flag = CMC_FLAG_ERROR;
        cmc_assert(ih_equals(ih, ih2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, ih_flag(ih2));

        ih_free(ih);
        ih_free(ih2);
    });
});

CMC_CREATE_UNIT(CMCIntervalHeapIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_start(ih);

        cmc_assert_equals(ptr, ih, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(ih_iter_at_start(&it));
        cmc_assert(ih_iter_at_end(&it));

        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 2));

        it = ih_iter_start(ih);

        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, false, it.end);

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_end(ih);

        cmc_assert_equals(ptr, ih, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(ih_iter_at_start(&it));
        cmc_assert(ih_iter_at_end(&it));

        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 2));
        cmc_assert(ih_insert(ih, 3));

        it = ih_iter_end(ih);

        cmc_assert_equals(size_t, ih->count - 1, it.cursor);
        cmc_assert_equals(bool, false, it.start);

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_start(ih);

        // Empty checks
        cmc_assert(ih_iter_at_start(&it));
        it = ih_iter_end(ih);
        cmc_assert(ih_iter_at_start(&it));

        // Non-empty check
        cmc_assert(ih_insert(ih, 1));
        it = ih_iter_end(ih);
        cmc_assert(!ih_iter_at_start(&it));
        it = ih_iter_start(ih);
        cmc_assert(ih_iter_at_start(&it));

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_start(ih);

        // Empty check
        cmc_assert(ih_iter_at_end(&it));
        it = ih_iter_end(ih);
        cmc_assert(ih_iter_at_end(&it));

        // Non-empty check
        cmc_assert(ih_insert(ih, 1));
        it = ih_iter_end(ih);
        cmc_assert(ih_iter_at_end(&it));
        it = ih_iter_start(ih);
        cmc_assert(!ih_iter_at_end(&it));

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_start(ih);

        cmc_assert(!ih_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
        {
            if (i % 2 == 0)
                ih_insert(ih, i);
            else
                ih_insert(ih, i);
        }

        cmc_assert_equals(size_t, 100, ih->count);

        it = ih_iter_end(ih);
        cmc_assert(!ih_iter_at_start(&it));
        cmc_assert(ih_iter_at_end(&it));
        cmc_assert_equals(size_t, 99, it.cursor);

        cmc_assert(ih_iter_to_start(&it));
        cmc_assert(ih_iter_at_start(&it));
        cmc_assert(!ih_iter_at_end(&it));
        cmc_assert_equals(size_t, 0, it.cursor);

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_end(ih);

        cmc_assert(!ih_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
        {
            if (i % 2 == 0)
                ih_insert(ih, i);
            else
                ih_insert(ih, i);
        }

        it = ih_iter_start(ih);
        cmc_assert(ih_iter_at_start(&it));
        cmc_assert(!ih_iter_at_end(&it));

        cmc_assert(ih_iter_to_end(&it));
        cmc_assert(!ih_iter_at_start(&it));
        cmc_assert(ih_iter_at_end(&it));

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_start(ih);

        cmc_assert(!ih_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
                ih_insert(ih, i);
            else
                ih_insert(ih, i);
        }

        size_t sum = 0;
        for (it = ih_iter_start(ih); !ih_iter_at_end(&it); ih_iter_next(&it))
        {
            sum += ih_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        ih_iter_to_start(&it);
        do
        {
            sum += ih_iter_value(&it);
        } while (ih_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_end(ih);

        cmc_assert(!ih_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
                ih_insert(ih, i);
            else
                ih_insert(ih, i);
        }

        size_t sum = 0;
        for (it = ih_iter_end(ih); !ih_iter_at_start(&it); ih_iter_prev(&it))
        {
            sum += ih_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        ih_iter_to_end(&it);
        do
        {
            sum += ih_iter_value(&it);
        } while (ih_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_start(ih);

        cmc_assert(!ih_iter_advance(&it, 1));

        for (size_t i = 1; i <= 1000; i++)
            ih_insert(ih, i);

        it = ih_iter_start(ih);

        cmc_assert(!ih_iter_advance(&it, 0));
        cmc_assert(!ih_iter_advance(&it, ih->count));

        size_t sum = 0;
        for (it = ih_iter_start(ih);;)
        {
            sum += ih_iter_value(&it);

            if (!ih_iter_advance(&it, 2))
                break;

            cmc_assert_equals(size_t, 0, ih_iter_index(&it) % 2);
        }

        cmc_assert(ih_iter_to_start(&it));
        cmc_assert(ih_iter_next(&it));

        cmc_assert_equals(size_t, 1, ih_iter_index(&it));

        for (;;)
        {
            sum += ih_iter_value(&it);

            if (!ih_iter_advance(&it, 2))
                break;

            cmc_assert_equals(size_t, 1, ih_iter_index(&it) % 2);
        }

        cmc_assert_equals(size_t, 500500, sum);

        ih_iter_to_start(&it);
        cmc_assert(ih_iter_advance(&it, ih->count - 1));

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_end(ih);

        cmc_assert(!ih_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            ih_insert(ih, i);

        it = ih_iter_end(ih);

        cmc_assert(!ih_iter_rewind(&it, 0));
        cmc_assert(!ih_iter_rewind(&it, ih->count));

        size_t sum = 0;
        for (it = ih_iter_end(ih);;)
        {
            sum += ih_iter_value(&it);

            if (!ih_iter_rewind(&it, 2))
                break;

            cmc_assert_equals(size_t, 0, ih_iter_index(&it) % 2);
        }

        cmc_assert(ih_iter_to_end(&it));
        cmc_assert(ih_iter_prev(&it));

        cmc_assert_equals(size_t, ih->count - 2, ih_iter_index(&it));

        for (;;)
        {
            sum += ih_iter_value(&it);

            if (!ih_iter_rewind(&it, 2))
                break;

            cmc_assert_equals(size_t, 1, ih_iter_index(&it) % 2);
        }

        cmc_assert_equals(size_t, 500500, sum);

        ih_iter_to_end(&it);
        cmc_assert(ih_iter_rewind(&it, ih->count - 1));

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_end(ih);
        cmc_assert(!ih_iter_go_to(&it, 0));

        it = ih_iter_start(ih);
        cmc_assert(!ih_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            ih_insert(ih, i);

        cmc_assert_equals(size_t, 1001, ih->count);

        it = ih_iter_start(ih);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            ih_iter_go_to(&it, i);

            sum += ih_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(ih_iter_go_to(&it, i - 1));

            sum += ih_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        struct intervalheap_iter it = ih_iter_end(ih);

        cmc_assert_equals(size_t, (size_t){ 0 }, ih_iter_value(&it));

        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 3));
        cmc_assert(ih_insert(ih, 2));

        it = ih_iter_start(ih);

        cmc_assert_equals(size_t, 1, ih_iter_value(&it));
        cmc_assert(ih_iter_next(&it));
        cmc_assert_equals(size_t, 3, ih_iter_value(&it));

        ih_free(ih);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        for (size_t i = 0; i <= 1000; i++)
            ih_insert(ih, i);

        struct intervalheap_iter it = ih_iter_start(ih);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, ih_iter_index(&it));
            ih_iter_next(&it);
        }

        it = ih_iter_end(ih);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, ih_iter_index(&it));
            ih_iter_prev(&it);
        }

        ih_free(ih);
    });
});

#endif /* CMC_TESTS_UNT_INTERVALHEAP_H */
