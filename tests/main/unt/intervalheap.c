#include "utl.c"
#include "utl/assert.h"
#include "utl/log.h"
#include "utl/test.h"

#include "../src/intervalheap.c"

struct intervalheap_fval *ih_fval =
    &(struct intervalheap_fval){ .cmp = cmp,
                                     .cpy = copy,
                                     .str = str,
                                     .free = custom_free,
                                     .hash = hash,
                                     .pri = pri };

CMC_CREATE_UNIT(intervalheap_test, true, {
    CMC_CREATE_TEST(new, {
        struct intervalheap *ih = ih_new(1000000, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_not_equals(ptr, NULL, ih->buffer);
        cmc_assert_equals(size_t, 500000, ih_capacity(ih));
        cmc_assert_equals(size_t, 0, ih_count(ih));

        ih_free(ih);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        struct intervalheap *ih = ih_new(0, ih_fval);

        cmc_assert_equals(ptr, NULL, ih);

        if (ih)
            ih_free(ih);
    });

    CMC_CREATE_TEST(new[capacity], {
        struct intervalheap *ih = ih_new(1, ih_fval);

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

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        struct intervalheap *ih = ih_new(UINT64_MAX, ih_fval);

        cmc_assert_equals(ptr, NULL, ih);

        if (ih)
            ih_free(ih);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(ih_insert(ih, i));

        cmc_assert_equals(size_t, 50, ih_count(ih));

        ih_clear(ih);

        cmc_assert_equals(size_t, 0, ih_count(ih));
        cmc_assert_equals(size_t, 50, ih_capacity(ih));

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

    CMC_CREATE_TEST(insert[count], {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 0));

        cmc_assert_equals(size_t, 2, ih_count(ih));

        ih_free(ih);
    });

    CMC_CREATE_TEST(insert[max], {
        struct intervalheap *ih = ih_new(1, ih_fval);

        cmc_assert(ih_insert(ih, 0));
        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 2));

        cmc_assert_equals(size_t, 2, ih_max(ih));

        ih_free(ih);
    });

    CMC_CREATE_TEST(insert[min], {
        struct intervalheap *ih = ih_new(1, ih_fval);

        cmc_assert(ih_insert(ih, 0));
        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 2));

        cmc_assert_equals(size_t, 0, ih_min(ih));

        ih_free(ih);
    });

    CMC_CREATE_TEST(insert[remove_max remove_min], {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);

        cmc_assert(ih_insert(ih, 2));
        cmc_assert_equals(size_t, 2, ih->buffer[0].data[0]);

        cmc_assert(ih_insert(ih, 1));
        cmc_assert_equals(size_t, 1, ih->buffer[0].data[0]);
        cmc_assert_equals(size_t, 2, ih->buffer[0].data[1]);

        cmc_assert(ih_remove_min(ih));
        cmc_assert_equals(size_t, 2, ih->buffer[0].data[0]);

        cmc_assert(ih_insert(ih, 1));
        cmc_assert_equals(size_t, 1, ih->buffer[0].data[0]);
        cmc_assert_equals(size_t, 2, ih->buffer[0].data[1]);

        cmc_assert(ih_remove_max(ih));
        cmc_assert_equals(size_t, 1, ih->buffer[0].data[0]);

        ih_free(ih);
    });

    CMC_CREATE_TEST(flags, {
        struct intervalheap *ih = ih_new(100, ih_fval);

        cmc_assert_not_equals(ptr, NULL, ih);
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));

        ih->flag = cmc_flags.ERROR;

        // clear
        ih_clear(ih);
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));

        // customize
        ih->flag = cmc_flags.ERROR;
        ih_customize(ih, NULL, NULL);
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));

        // insert
        ih->flag = cmc_flags.ERROR;
        cmc_assert(ih_insert(ih, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));

        // remove_max
        cmc_assert(ih_remove_max(ih));
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));
        cmc_assert(!ih_remove_max(ih));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ih_flag(ih));

        // remove min
        cmc_assert(ih_insert(ih, 10));
        cmc_assert(ih_remove_min(ih));
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));
        cmc_assert(!ih_remove_min(ih));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ih_flag(ih));

        // update_max
        cmc_assert(!ih_update_max(ih, 20));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ih_flag(ih));
        cmc_assert(ih_insert(ih, 10));
        ih->flag = cmc_flags.ERROR;
        cmc_assert(ih_update_max(ih, 20));
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));

        // update_min
        ih->flag = cmc_flags.ERROR;
        cmc_assert(ih_update_min(ih, 20));
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));
        cmc_assert(ih_remove_min(ih));
        ih->flag = cmc_flags.ERROR;
        cmc_assert(!ih_update_max(ih, 20));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ih_flag(ih));

        // max min
        ih->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, (size_t){ 0 }, ih_max(ih));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ih_flag(ih));

        ih->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, (size_t){ 0 }, ih_min(ih));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ih_flag(ih));

        cmc_assert(ih_insert(ih, 10));
        ih->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 10, ih_max(ih));
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));

        ih->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 10, ih_min(ih));
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));

        // contains
        ih->flag = cmc_flags.ERROR;
        cmc_assert(ih_contains(ih, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));

        // copy_of
        ih->flag = cmc_flags.ERROR;
        struct intervalheap *ih2 = ih_copy_of(ih);
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih2));

        // equals
        ih->flag = cmc_flags.ERROR;
        ih2->flag = cmc_flags.ERROR;
        cmc_assert(ih_equals(ih, ih2));
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih));
        cmc_assert_equals(int32_t, cmc_flags.OK, ih_flag(ih2));

        ih_free(ih);
        ih_free(ih2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct intervalheap *ih =
            ih_new_custom(100, ih_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, ih);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(ih_insert(ih, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(ih_remove_max(ih));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(ih_insert(ih, 10));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(ih_remove_min(ih));
        cmc_assert_equals(int32_t, 2, total_delete);

        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 2));
        cmc_assert_equals(int32_t, 4, total_create);

        cmc_assert(ih_update_max(ih, 1));
        cmc_assert_equals(int32_t, 1, total_update);

        cmc_assert(ih_update_min(ih, 2));
        cmc_assert_equals(int32_t, 2, total_update);

        cmc_assert_equals(size_t, 2, ih_max(ih));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert_equals(size_t, 1, ih_min(ih));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert(ih_contains(ih, 1));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert(ih_resize(ih, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(ih_resize(ih, 50));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 4, total_create);
        cmc_assert_equals(int32_t, 3, total_read);
        cmc_assert_equals(int32_t, 2, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        ih_customize(ih, NULL, NULL);

        cmc_assert_equals(ptr, NULL, ih->callbacks);

        ih_clear(ih);
        cmc_assert(ih_insert(ih, 10));
        cmc_assert(ih_remove_max(ih));
        cmc_assert(ih_insert(ih, 10));
        cmc_assert(ih_remove_min(ih));
        cmc_assert(ih_insert(ih, 1));
        cmc_assert(ih_insert(ih, 2));
        cmc_assert(ih_update_max(ih, 1));
        cmc_assert(ih_update_min(ih, 2));
        cmc_assert_equals(size_t, 2, ih_max(ih));
        cmc_assert_equals(size_t, 1, ih_min(ih));
        cmc_assert(ih_contains(ih, 1));
        cmc_assert(ih_resize(ih, 1000));
        cmc_assert(ih_resize(ih, 50));

        cmc_assert_equals(int32_t, 4, total_create);
        cmc_assert_equals(int32_t, 3, total_read);
        cmc_assert_equals(int32_t, 2, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, ih->callbacks);

        ih_free(ih);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});
