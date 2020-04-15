#include "utl.c"
#include "utl/assert.h"
#include "utl/log.h"
#include "utl/test.h"

#include "../src/treeset.c"

struct treeset_fval *ts_fval =
    &(struct treeset_fval){ .cmp = cmp,
                                .cpy = copy,
                                .str = str,
                                .free = custom_free,
                                .hash = hash,
                                .pri = pri };

CMC_CREATE_UNIT(treeset_test, true, {
    CMC_CREATE_TEST(new, {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        ts_free(set);
    });

    CMC_CREATE_TEST(clear[count], {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 50; i++)
            ts_insert(set, i);

        cmc_assert_equals(size_t, 50, ts_count(set));

        ts_clear(set);

        cmc_assert_equals(size_t, 0, ts_count(set));
        cmc_assert_equals(ptr, NULL, set->root);

        ts_free(set);
    });

    CMC_CREATE_TEST(flags, {
        struct treeset *set = ts_new(ts_fval);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_equals(int32_t, cmc_flags.OK, ts_flag(set));

        // clear
        set->flag = cmc_flags.ERROR;
        ts_clear(set);
        cmc_assert_equals(int32_t, cmc_flags.OK, ts_flag(set));

        // insert
        set->flag = cmc_flags.ERROR;
        cmc_assert(ts_insert(set, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, ts_flag(set));

        // remove
        cmc_assert(!ts_remove(set, 2));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, ts_flag(set));

        cmc_assert(ts_remove(set, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, ts_flag(set));

        cmc_assert(!ts_remove(set, 1));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ts_flag(set));

        // max min
        set->flag = cmc_flags.ERROR;
        cmc_assert(!ts_max(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ts_flag(set));

        set->flag = cmc_flags.ERROR;
        cmc_assert(!ts_min(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ts_flag(set));

        cmc_assert(ts_insert(set, 1));
        set->flag = cmc_flags.ERROR;
        cmc_assert(ts_max(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, ts_flag(set));

        set->flag = cmc_flags.ERROR;
        cmc_assert(ts_min(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, ts_flag(set));

        // copy_of
        set->flag = cmc_flags.ERROR;
        struct treeset *set2 = ts_copy_of(set);
        cmc_assert_equals(int32_t, cmc_flags.OK, ts_flag(set));
        cmc_assert_equals(int32_t, cmc_flags.OK, ts_flag(set2));

        // equals
        set->flag = cmc_flags.ERROR;
        set2->flag = cmc_flags.ERROR;
        cmc_assert(ts_equals(set, set2));
        cmc_assert_equals(int32_t, cmc_flags.OK, ts_flag(set));
        cmc_assert_equals(int32_t, cmc_flags.OK, ts_flag(set2));

        ts_free(set);
        ts_free(set2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct treeset *set = ts_new_custom(ts_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, set);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(ts_insert(set, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(ts_remove(set, 10));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(ts_insert(set, 1));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(ts_max(set, NULL));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert(ts_min(set, NULL));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert(ts_contains(set, 1));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 3, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 0, total_resize);

        ts_customize(set, NULL, NULL);

        ts_clear(set);
        cmc_assert(ts_insert(set, 10));
        cmc_assert(ts_remove(set, 10));
        cmc_assert(ts_insert(set, 1));
        cmc_assert(ts_max(set, NULL));
        cmc_assert(ts_min(set, NULL));
        cmc_assert(ts_contains(set, 1));

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 3, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 0, total_resize);

        cmc_assert_equals(ptr, NULL, set->callbacks);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        ts_free(set);
    });
});
