#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "../src/heap.c"

struct heap_fval *h_fval = &(struct heap_fval){ .cmp = cmc_size_cmp,
                                                .cpy = NULL,
                                                .str = cmc_size_str,
                                                .free = NULL,
                                                .hash = cmc_size_hash,
                                                .pri = cmc_size_cmp };

CMC_CREATE_UNIT(Heap, true, {
    CMC_CREATE_TEST(new, {
        struct heap *h = h_new(1000000, cmc_max_heap, h_fval);

        cmc_assert_not_equals(ptr, NULL, h);
        cmc_assert_not_equals(ptr, NULL, h->buffer);
        cmc_assert_equals(size_t, 1000000, h_capacity(h));
        cmc_assert_equals(size_t, 0, h_count(h));

        h_free(h);
    });

    CMC_CREATE_TEST(new[edge_case capacity = 0], {
        struct heap *h = h_new(0, cmc_max_heap, h_fval);

        cmc_assert_equals(ptr, NULL, h);
    });

    CMC_CREATE_TEST(new[edge_case capacity = UINT64_MAX], {
        struct heap *h = h_new(UINT64_MAX, cmc_max_heap, h_fval);

        cmc_assert_equals(ptr, NULL, h);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct heap *h = h_new(100, cmc_max_heap, h_fval);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i < 50; i++)
            h_insert(h, i);

        cmc_assert_equals(size_t, 50, h_count(h));

        h_clear(h);

        cmc_assert_equals(size_t, 0, h_count(h));
        cmc_assert_equals(size_t, 100, h_capacity(h));

        h_free(h);
    });

    CMC_CREATE_TEST(buffer_growth[edge_case capacity = 1], {
        struct heap *h = h_new(1, cmc_max_heap, h_fval);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(h_insert(h, i));

        cmc_assert_equals(size_t, 50, h_count(h));

        h_free(h);
    });

    CMC_CREATE_TEST(insert, {
        struct heap *h = h_new(100, cmc_max_heap, h_fval);

        cmc_assert_not_equals(ptr, NULL, h);

        cmc_assert(h_insert(h, 10));

        h_free(h);
    });

    CMC_CREATE_TEST(insert[item preservation], {
        struct heap *h = h_new(50, cmc_max_heap, h_fval);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i <= 100; i++)
            cmc_assert(h_insert(h, i));

        size_t sum = 0;
        for (size_t i = 0; i < h_count(h); i++)
            sum += h->buffer[i];

        cmc_assert_equals(size_t, 5050, sum);

        h_free(h);
    });

    CMC_CREATE_TEST(remove, {
        struct heap *h = h_new(50, cmc_max_heap, h_fval);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 0; i <= 100; i++)
            cmc_assert(h_insert(h, i));

        cmc_assert_equals(size_t, 100, h_peek(h));
        cmc_assert(h_remove(h));
        cmc_assert_equals(size_t, 99, h_peek(h));

        h_free(h);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        struct heap *h = h_new(100, cmc_max_heap, h_fval);

        cmc_assert_not_equals(ptr, NULL, h);

        cmc_assert(!h_remove(h));

        h_free(h);
    });

    CMC_CREATE_TEST(remove[sorted], {
        struct heap *h = h_new(100, cmc_min_heap, h_fval);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(h_insert(h, i));

        size_t r;
        size_t i = 0;
        size_t arr[100];
        while (!h_empty(h))
        {
            arr[i++] = h_peek(h);
            cmc_assert(h_remove(h));
        }

        cmc_assert_array_sorted_any(size_t, arr, cmc_size_cmp, 0, 99);

        h_free(h);
    });

    CMC_CREATE_TEST(peek, {
        struct heap *h = h_new(100, cmc_max_heap, h_fval);

        cmc_assert_not_equals(ptr, NULL, h);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(h_insert(h, i));

        cmc_assert_equals(size_t, 100, h_peek(h));

        h_free(h);
    });

    CMC_CREATE_TEST(peek[count = 0], {
        struct heap *h = h_new(100, cmc_max_heap, h_fval);

        cmc_assert_not_equals(ptr, NULL, h);

        cmc_assert_equals(size_t, (size_t){ 0 }, h_peek(h));

        h_free(h);
    });

    CMC_CREATE_TEST(flags, {
        struct heap *h = h_new(100, cmc_max_heap, h_fval);

        cmc_assert_not_equals(ptr, NULL, h);
        cmc_assert_equals(int32_t, cmc_flags.OK, h_flag(h));

        h->flag = cmc_flags.ERROR;

        // insert
        cmc_assert(h_insert(h, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, h_flag(h));

        h->flag = cmc_flags.ERROR;

        // remove
        cmc_assert(h_remove(h));
        cmc_assert_equals(int32_t, cmc_flags.OK, h_flag(h));

        cmc_assert(!h_remove(h));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, h_flag(h));

        // peek
        cmc_assert(h_insert(h, 10));
        h_peek(h);
        cmc_assert_equals(int32_t, cmc_flags.OK, h_flag(h));
        cmc_assert(h_remove(h));
        h_peek(h);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, h_flag(h));

        // clear
        h_clear(h);
        cmc_assert_equals(int32_t, cmc_flags.OK, h_flag(h));

        h->flag = cmc_flags.ERROR;

        // contains
        h_contains(h, 10);
        cmc_assert_equals(int32_t, cmc_flags.OK, h_flag(h));

        h->flag = cmc_flags.ERROR;

        // copy of
        struct heap *h2 = h_copy_of(h);
        cmc_assert_equals(int32_t, cmc_flags.OK, h_flag(h));
        cmc_assert_equals(int32_t, cmc_flags.OK, h_flag(h2));

        h->flag = cmc_flags.ERROR;
        h2->flag = cmc_flags.ERROR;

        // equals
        cmc_assert(h_equals(h, h2));
        cmc_assert_equals(int32_t, cmc_flags.OK, h_flag(h));
        cmc_assert_equals(int32_t, cmc_flags.OK, h_flag(h2));

        h_free(h);
        h_free(h2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct heap *h =
            h_new_custom(100, cmc_max_heap, h_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, h);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(h_insert(h, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(h_remove(h));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(h_insert(h, 10));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert_equals(size_t, 10, h_peek(h));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert(h_contains(h, 10));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert(h_resize(h, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(h_resize(h, 100));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 2, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        h_customize(h, NULL, NULL);

        cmc_assert_equals(ptr, NULL, h->callbacks);

        h_clear(h);
        cmc_assert(h_insert(h, 10));
        cmc_assert(h_remove(h));
        cmc_assert(h_insert(h, 10));
        cmc_assert_equals(size_t, 10, h_peek(h));
        cmc_assert(h_contains(h, 10));
        cmc_assert(h_resize(h, 1000));
        cmc_assert(h_resize(h, 100));

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 2, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, h->callbacks);

        h_free(h);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});
