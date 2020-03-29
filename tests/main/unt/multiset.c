#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/multiset.h>

CMC_GENERATE_MULTISET(ms, multiset, size_t)

struct multiset_ftab_val *ms_ftab_val =
    &(struct multiset_ftab_val){ .cmp = cmp,
                                 .cpy = copy,
                                 .str = str,
                                 .free = custom_free,
                                 .hash = hash,
                                 .pri = pri };

CMC_CREATE_UNIT(multiset_test, true, {
    CMC_CREATE_TEST(new, {
        struct multiset *set = ms_new(943722, 0.6, ms_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_not_equals(ptr, NULL, set->buffer);
        cmc_assert_equals(size_t, 0, ms_count(set));
        cmc_assert_greater_equals(size_t, ((size_t)(943722 / 0.6)),
                                  ms_capacity(set));

        ms_free(set);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        struct multiset *set = ms_new(0, 0.6, ms_ftab_val);

        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        struct multiset *set = ms_new(UINT64_MAX, 0.99, ms_ftab_val);

        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct multiset *set = ms_new(100, 0.6, ms_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(ms_insert(set, i));

        cmc_assert_equals(size_t, 50, ms_count(set));

        ms_clear(set);

        cmc_assert_equals(size_t, 0, ms_count(set));

        ms_free(set);
    });

    CMC_CREATE_TEST(insert[count cardinality multiplicity], {
        struct multiset *set = ms_new(100, 0.6, ms_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(ms_insert(set, 1));
        cmc_assert(ms_insert(set, 2));
        cmc_assert(ms_insert(set, 2));
        cmc_assert(ms_insert(set, 3));
        cmc_assert(ms_insert(set, 3));
        cmc_assert(ms_insert(set, 3));

        cmc_assert_equals(size_t, 3, ms_count(set));
        cmc_assert_equals(size_t, 6, ms_cardinality(set));

        for (size_t i = 0; i <= 3; i++)
            cmc_assert_equals(size_t, i, ms_multiplicity_of(set, i));

        ms_free(set);
    });

    CMC_CREATE_TEST(remove[count cardinality multiplicity], {
        struct multiset *set = ms_new(100, 0.6, ms_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(ms_insert(set, 1));
        cmc_assert(ms_insert(set, 2));
        cmc_assert(ms_insert(set, 2));
        cmc_assert(ms_insert(set, 3));
        cmc_assert(ms_insert(set, 3));
        cmc_assert(ms_insert(set, 3));

        cmc_assert_equals(size_t, 3, ms_count(set));
        cmc_assert_equals(size_t, 6, ms_cardinality(set));

        for (size_t i = 0; i <= 3; i++)
            cmc_assert_equals(size_t, i, ms_multiplicity_of(set, i));

        cmc_assert(ms_remove(set, 1));
        cmc_assert(ms_remove(set, 2));
        cmc_assert(ms_remove(set, 3));

        cmc_assert_equals(size_t, 2, ms_count(set));
        cmc_assert_equals(size_t, 3, ms_cardinality(set));

        cmc_assert_equals(size_t, 0, ms_multiplicity_of(set, 0));
        cmc_assert_equals(size_t, 0, ms_multiplicity_of(set, 1));
        cmc_assert_equals(size_t, 1, ms_multiplicity_of(set, 2));
        cmc_assert_equals(size_t, 2, ms_multiplicity_of(set, 3));

        ms_free(set);
    });

    CMC_CREATE_TEST(remove_all[count cardinality multiplicity], {
        struct multiset *set = ms_new(100, 0.6, ms_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(ms_insert(set, 1));
        cmc_assert(ms_insert(set, 2));
        cmc_assert(ms_insert(set, 2));
        cmc_assert(ms_insert(set, 3));
        cmc_assert(ms_insert(set, 3));
        cmc_assert(ms_insert(set, 3));

        cmc_assert_equals(size_t, 3, ms_count(set));
        cmc_assert_equals(size_t, 6, ms_cardinality(set));

        for (size_t i = 0; i <= 3; i++)
            cmc_assert_equals(size_t, i, ms_multiplicity_of(set, i));

        cmc_assert(ms_remove_all(set, 1));
        cmc_assert(ms_remove_all(set, 3));

        cmc_assert_equals(size_t, 1, ms_count(set));
        cmc_assert_equals(size_t, 2, ms_cardinality(set));

        cmc_assert_equals(size_t, 0, ms_multiplicity_of(set, 0));
        cmc_assert_equals(size_t, 0, ms_multiplicity_of(set, 1));
        cmc_assert_equals(size_t, 2, ms_multiplicity_of(set, 2));
        cmc_assert_equals(size_t, 0, ms_multiplicity_of(set, 3));

        ms_free(set);
    });

    CMC_CREATE_TEST(multiplicity, {
        struct multiset *set = ms_new(50, 0.6, ms_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 1000; i++)
            cmc_assert(ms_insert(set, i % 20));

        cmc_assert_equals(size_t, 20, ms_count(set));
        cmc_assert_equals(size_t, 1000, ms_cardinality(set));

        for (size_t i = 0; i < 20; i++)
            cmc_assert_equals(size_t, 50, ms_multiplicity_of(set, i));

        ms_free(set);
    });

    CMC_CREATE_TEST(flags, {
        struct multiset *set = ms_new(100, 0.6, ms_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        // clear
        set->flag = cmc_flags.ERROR;
        ms_clear(set);
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        // customize
        set->flag = cmc_flags.ERROR;
        ms_customize(set, NULL, NULL);
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        // insert
        set->flag = cmc_flags.ERROR;
        cmc_assert(ms_insert(set, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        set->flag = cmc_flags.ERROR;
        cmc_assert(ms_insert(set, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        // insert_many
        set->flag = cmc_flags.ERROR;
        cmc_assert(ms_insert_many(set, 1, 0));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        set->flag = cmc_flags.ERROR;
        cmc_assert(ms_insert_many(set, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        // update
        set->flag = cmc_flags.ERROR;
        cmc_assert(ms_update(set, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        set->flag = cmc_flags.ERROR;
        cmc_assert(ms_update(set, 10, 0));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        // remove
        cmc_assert(!ms_remove(set, 10));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, ms_flag(set));

        cmc_assert(ms_remove(set, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        // remove_all
        cmc_assert_equals(size_t, 0, ms_remove_all(set, 1));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ms_flag(set));

        cmc_assert(ms_insert_many(set, 1, 10));
        cmc_assert_equals(size_t, 0, ms_remove_all(set, 10));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, ms_flag(set));

        cmc_assert_equals(size_t, 10, ms_remove_all(set, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        // max min
        cmc_assert(!ms_max(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ms_flag(set));
        set->flag = cmc_flags.ERROR;
        cmc_assert(!ms_min(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ms_flag(set));

        cmc_assert(ms_insert(set, 1));
        set->flag = cmc_flags.ERROR;
        cmc_assert(ms_max(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));
        set->flag = cmc_flags.ERROR;
        cmc_assert(ms_min(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        // multiplicity_of
        set->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 0, ms_multiplicity_of(set, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        // contains
        set->flag = cmc_flags.ERROR;
        cmc_assert(!ms_contains(set, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));

        // copy_of
        set->flag = cmc_flags.ERROR;
        struct multiset *set2 = ms_copy_of(set);
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set2));

        // equals
        set->flag = cmc_flags.ERROR;
        set2->flag = cmc_flags.ERROR;
        cmc_assert(ms_equals(set, set2));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set));
        cmc_assert_equals(int32_t, cmc_flags.OK, ms_flag(set2));

        ms_free(set);
        ms_free(set2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct multiset *set =
            ms_new_custom(100, 0.8, ms_ftab_val, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, set);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(ms_insert(set, 1));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(ms_insert_many(set, 1, 3));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(ms_update(set, 1, 0));
        cmc_assert_equals(int32_t, 1, total_update);

        cmc_assert(ms_update(set, 1, 10));
        cmc_assert_equals(int32_t, 2, total_update);

        cmc_assert(ms_remove(set, 1));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(ms_remove_all(set, 1));
        cmc_assert_equals(int32_t, 2, total_delete);

        cmc_assert(ms_update(set, 1, 10));
        cmc_assert_equals(int32_t, 3, total_update);

        cmc_assert(ms_max(set, NULL));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert(ms_min(set, NULL));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert(ms_contains(set, 1));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert(ms_resize(set, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(ms_resize(set, 100));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 3, total_read);
        cmc_assert_equals(int32_t, 3, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        ms_customize(set, NULL, NULL);

        cmc_assert_equals(ptr, NULL, set->callbacks);

        ms_clear(set);
        cmc_assert(ms_insert(set, 1));
        cmc_assert(ms_insert_many(set, 1, 3));
        cmc_assert(ms_update(set, 1, 0));
        cmc_assert(ms_update(set, 1, 10));
        cmc_assert(ms_remove(set, 1));
        cmc_assert(ms_remove_all(set, 1));
        cmc_assert(ms_update(set, 1, 10));
        cmc_assert(ms_max(set, NULL));
        cmc_assert(ms_min(set, NULL));
        cmc_assert(ms_contains(set, 1));
        cmc_assert(ms_resize(set, 1000));
        cmc_assert(ms_resize(set, 100));

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 3, total_read);
        cmc_assert_equals(int32_t, 3, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, set->callbacks);

        ms_free(set);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});
