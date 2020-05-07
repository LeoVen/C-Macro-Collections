#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "../src/hashset.c"

struct hashset_fval *hs_fval = &(struct hashset_fval){ .cmp = cmc_size_cmp,
                                                       .cpy = NULL,
                                                       .str = cmc_size_str,
                                                       .free = NULL,
                                                       .hash = cmc_size_hash,
                                                       .pri = cmc_size_cmp };

struct hashset_fval *hs_fval_counter = &(struct hashset_fval){ .cmp = v_c_cmp,
                                                               .cpy = v_c_cpy,
                                                               .str = v_c_str,
                                                               .free = v_c_free,
                                                               .hash = v_c_hash,
                                                               .pri = v_c_pri };

struct cmc_alloc_node *hs_alloc_node = &(struct cmc_alloc_node){
    .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free
};

CMC_CREATE_UNIT(HashSet, true, {
    CMC_CREATE_TEST(PFX##_new(), {
        struct hashset *set = hs_new(943722, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_not_equals(ptr, NULL, set->buffer);
        cmc_assert_equals(size_t, 0, set->count);
        cmc_assert_equals(double, 0.6, set->load);
        cmc_assert_equals(int32_t, cmc_flags.OK, set->flag);
        cmc_assert_equals(ptr, hs_fval, set->f_val);
        cmc_assert_equals(ptr, &cmc_alloc_node_default, set->alloc);
        cmc_assert_equals(ptr, NULL, set->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hs_capacity(set));

        hs_free(set);

        set = hs_new(0, 0.6, hs_fval);
        cmc_assert_equals(ptr, NULL, set);

        set = hs_new(UINT64_MAX, 0.99, hs_fval);
        cmc_assert_equals(ptr, NULL, set);

        set = hs_new(1000, 0.6, NULL);
        cmc_assert_equals(ptr, NULL, set);

        set = hs_new(1000, 0.6, NULL);
        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(PFX##_new_custom(), {
        struct hashset *set =
            hs_new_custom(943722, 0.6, hs_fval, hs_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_not_equals(ptr, NULL, set->buffer);
        cmc_assert_equals(size_t, 0, set->count);
        cmc_assert_equals(double, 0.6, set->load);
        cmc_assert_equals(int32_t, cmc_flags.OK, set->flag);
        cmc_assert_equals(ptr, hs_fval, set->f_val);
        cmc_assert_equals(ptr, hs_alloc_node, set->alloc);
        cmc_assert_equals(ptr, callbacks, set->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hs_capacity(set));

        hs_free(set);

        set = hs_new_custom(0, 0.6, hs_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, set);

        set = hs_new_custom(UINT64_MAX, 0.99, hs_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, set);

        set = hs_new_custom(1000, 0.6, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(PFX##_clear(), {
        v_total_free = 0;
        struct hashset *set = hs_new(100, 0.6, hs_fval_counter);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i <= 1000; i++)
            hs_insert(set, i);

        cmc_assert_equals(size_t, 1000, set->count);

        set->flag = cmc_flags.ERROR;
        hs_clear(set);

        cmc_assert_equals(size_t, 0, set->count);
        cmc_assert_equals(int32_t, cmc_flags.OK, set->flag);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        hs_free(set);
        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_free(), {
        v_total_free = 0;
        struct hashset *set = hs_new(100, 0.6, hs_fval_counter);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i <= 1000; i++)
            hs_insert(set, i);

        cmc_assert_equals(size_t, 1000, set->count);

        hs_free(set);

        cmc_assert_equals(int32_t, 1000, v_total_free);

        set = hs_new(1000, 0.6, hs_fval_counter);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_not_equals(ptr, NULL, set->buffer);

        hs_free(set);

        cmc_assert_equals(int32_t, 1000, v_total_free);
        v_total_free = 0;
    });

    CMC_CREATE_TEST(insert, {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hs_insert(set, 1));
        cmc_assert_equals(size_t, 1, hs_count(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[smallest capacity], {
        struct hashset *set = hs_new(1, 0.99, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hs_capacity(set));
        cmc_assert(hs_insert(set, 1));

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[element position], {
        struct hashset *set = hs_new(1, 0.99, hs_fval);

        // Temporary change
        // Using the numhash the key is the hash itself
        hs_fval->hash = numhash;

        cmc_assert_not_equals(ptr, NULL, set);

        size_t capacity = hs_capacity(set);

        cmc_assert_greater(size_t, 0, capacity);

        cmc_assert(hs_insert(set, capacity - 1));
        cmc_assert(hs_insert(set, capacity));

        cmc_assert_equals(size_t, capacity - 1,
                          set->buffer[capacity - 1].value);
        cmc_assert_equals(size_t, capacity, set->buffer[0].value);

        hs_fval->hash = cmc_size_hash;

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[distance], {
        struct hashset *set = hs_new(500, 0.6, hs_fval);

        // Temporary change
        hs_fval->hash = hash0;

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 200; i++)
            cmc_assert(hs_insert(set, i));

        // Everything is hashed to 0 so key 0 should have dist 0, key 1 dist 1,
        // etc
        for (size_t i = 0; i < 200; i++)
            cmc_assert_equals(size_t, i, set->buffer[i].dist);

        hs_fval->hash = cmc_size_hash;

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[distance wrap], {
        struct hashset *set = hs_new(1, 0.99, hs_fval);

        // Temporary change
        hs_fval->hash = hashcapminus1;

        cmc_assert_not_equals(ptr, NULL, set);

        size_t capacity = hs_capacity(set);

        // Just to be sure, not part of the test
        cmc_assert_equals(size_t, cmc_hashtable_primes[0] - 1,
                          hashcapminus1(0));
        cmc_assert_equals(size_t, capacity - 1, hashcapminus1(0));

        cmc_assert(hs_insert(set, 0));
        cmc_assert(hs_insert(set, 1));

        cmc_assert_equals(size_t, 0, set->buffer[capacity - 1].dist);
        cmc_assert_equals(size_t, 1, set->buffer[0].dist);

        hs_fval->hash = cmc_size_hash;

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[distances], {
        struct hashset *set = hs_new(1, 0.99, hs_fval);

        // Temporary change
        hs_fval->hash = hashcapminus4;

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hs_capacity(set));

        for (size_t i = 0; i < 6; i++)
            cmc_assert(hs_insert(set, i));

        for (size_t i = 0; i < 6; i++)
            cmc_assert_equals(size_t, i, hs_impl_get_entry(set, i)->dist);

        hs_fval->hash = cmc_size_hash;

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[buffer growth and item preservation], {
        struct hashset *set = hs_new(1, 0.99, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hs_capacity(set));

        size_t p = 0;
        for (size_t i = 0; i < 100000; i++)
        {
            if (hs_full(set))
                p++;

            cmc_assert(hs_insert(set, i));
        }

        cmc_assert_equals(size_t, cmc_hashtable_primes[p], hs_capacity(set));

        for (size_t i = 0; i < 100000; i++)
            cmc_assert(hs_contains(set, i));

        hs_free(set);
    });

    CMC_CREATE_TEST(remove, {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hs_insert(set, i));

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hs_remove(set, i));

        hs_free(set);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hs_remove(set, i));

        hs_free(set);
    });

    CMC_CREATE_TEST(remove[key not found], {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hs_insert(set, i));

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hs_remove(set, i));

        hs_free(set);
    });

    CMC_CREATE_TEST(max, {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hs_insert(set, i));

        size_t val;
        cmc_assert(hs_max(set, &val));

        cmc_assert_equals(size_t, 100, val);

        hs_free(set);
    });

    CMC_CREATE_TEST(max[count = 0], {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(!hs_min(set, NULL));

        hs_free(set);
    });

    CMC_CREATE_TEST(min, {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hs_insert(set, i));

        size_t val;
        cmc_assert(hs_min(set, &val));

        cmc_assert_equals(size_t, 1, val);

        hs_free(set);
    });

    CMC_CREATE_TEST(min[count = 0], {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(!hs_min(set, NULL));

        hs_free(set);
    });

    CMC_CREATE_TEST(contains, {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hs_insert(set, 987654321));

        cmc_assert(hs_contains(set, 987654321));

        hs_free(set);
    });

    CMC_CREATE_TEST(contains[count = 0], {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(!hs_contains(set, 987654321));

        hs_free(set);
    });

    CMC_CREATE_TEST(contains[sum], {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 101; i <= 200; i++)
            cmc_assert(hs_insert(set, i));

        size_t sum = 0;
        for (size_t i = 1; i <= 300; i++)
            if (hs_contains(set, i))
                sum += i;

        cmc_assert_equals(size_t, 15050, sum);

        hs_free(set);
    });

    CMC_CREATE_TEST(empty, {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hs_empty(set));

        cmc_assert(hs_insert(set, 1));

        cmc_assert(!hs_empty(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(full, {
        struct hashset *set = hs_new(cmc_hashtable_primes[0], 0.99999, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hs_capacity(set));

        for (size_t i = 0; i < hs_capacity(set); i++)
            cmc_assert(hs_insert(set, i));

        cmc_assert(hs_full(set));

        cmc_assert(hs_insert(set, 10000));

        cmc_assert_equals(size_t, cmc_hashtable_primes[1], hs_capacity(set));

        cmc_assert(!hs_full(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(count, {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i < 100; i++)
        {
            cmc_assert(hs_insert(set, i));
            cmc_assert_equals(size_t, i, hs_count(set));
        }

        hs_free(set);
    });

    CMC_CREATE_TEST(capacity, {
        struct hashset *set = hs_new(2500, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_greater_equals(size_t, (size_t)(2500 / 0.6),
                                  hs_capacity(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(capacity[small], {
        struct hashset *set = hs_new(1, 0.99, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hs_capacity(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(load, {
        struct hashset *set = hs_new(1, 0.99, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_in_range(double, 0.99 - 0.0001, 0.99 + 0.0001, hs_load(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(flags, {
        struct hashset *set = hs_new(1, 0.99, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_equals(int32_t, cmc_flags.OK, hs_flag(set));

        // insert
        cmc_assert(hs_insert(set, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, hs_flag(set));
        cmc_assert(!hs_insert(set, 1));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hs_flag(set));

        // clear
        hs_clear(set);
        cmc_assert_equals(int32_t, cmc_flags.OK, hs_flag(set));

        cmc_assert(hs_insert(set, 1));

        // remove
        cmc_assert(!hs_remove(set, 2));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hs_flag(set));

        cmc_assert(hs_remove(set, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, hs_flag(set));

        cmc_assert(!hs_remove(set, 1));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hs_flag(set));

        // max
        cmc_assert(hs_insert(set, 1));
        cmc_assert(hs_max(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hs_flag(set));

        cmc_assert(hs_remove(set, 1));
        cmc_assert(!hs_max(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hs_flag(set));

        // min
        cmc_assert(hs_insert(set, 1));
        cmc_assert(hs_min(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hs_flag(set));

        cmc_assert(hs_remove(set, 1));
        cmc_assert(!hs_min(set, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hs_flag(set));

        // contains
        hs_contains(set, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, hs_flag(set));

        // copy
        set->flag = cmc_flags.ERROR;
        struct hashset *set2 = hs_copy_of(set);

        cmc_assert_equals(int32_t, cmc_flags.OK, hs_flag(set));
        cmc_assert_equals(int32_t, cmc_flags.OK, hs_flag(set2));

        size_t tmp = set->capacity;
        set->capacity = 0;

        struct hashset *set3 = hs_copy_of(set);
        cmc_assert_equals(ptr, NULL, set3);

        cmc_assert_equals(int32_t, cmc_flags.ERROR, hs_flag(set));

        set->capacity = tmp;

        cmc_assert(hs_equals(set, set2));
        cmc_assert_equals(int32_t, cmc_flags.OK, hs_flag(set));
        cmc_assert_equals(int32_t, cmc_flags.OK, hs_flag(set2));

        hs_free(set);
        hs_free(set2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct hashset *set = hs_new_custom(100, 0.6, hs_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, set);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(hs_insert(set, 1));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(hs_remove(set, 1));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(hs_insert(set, 1));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(hs_max(set, NULL));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert(hs_min(set, NULL));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert(hs_contains(set, 1));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert(hs_resize(set, 10000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(hs_resize(set, 500));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 3, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        hs_customize(set, NULL, NULL);

        cmc_assert_equals(ptr, NULL, set->callbacks);

        hs_clear(set);
        cmc_assert(hs_resize(set, 10000));
        cmc_assert(hs_resize(set, 500));
        cmc_assert(hs_insert(set, 1));
        cmc_assert(hs_remove(set, 1));
        cmc_assert(hs_insert(set, 1));
        cmc_assert(hs_max(set, NULL));
        cmc_assert(hs_min(set, NULL));
        cmc_assert(hs_contains(set, 1));

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 3, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, set->callbacks);

        hs_free(set);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});

struct hashset_fval *hs_fval_numhash =
    &(struct hashset_fval){ .cmp = cmc_size_cmp,
                            .cpy = NULL,
                            .str = cmc_size_str,
                            .free = NULL,
                            .hash = numhash,
                            .pri = cmc_size_cmp };

CMC_CREATE_UNIT(HashSetIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_start(set);

        cmc_assert_equals(ptr, set, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(size_t, 0, it.first);
        cmc_assert_equals(size_t, 0, it.last);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(hs_iter_at_start(&it));
        cmc_assert(hs_iter_at_end(&it));

        cmc_assert(hs_insert(set, 1));
        cmc_assert(hs_insert(set, 2));
        cmc_assert(hs_insert(set, 3));

        it = hs_iter_start(set);

        cmc_assert_equals(size_t, 0, it.index);

        cmc_assert_equals(size_t, 1, it.cursor);
        cmc_assert_equals(size_t, 1, it.first);
        cmc_assert_equals(size_t, 3, it.last);

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_end(set);

        cmc_assert_equals(ptr, set, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(size_t, 0, it.first);
        cmc_assert_equals(size_t, 0, it.last);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(hs_iter_at_start(&it));
        cmc_assert(hs_iter_at_end(&it));

        cmc_assert(hs_insert(set, 1));
        cmc_assert(hs_insert(set, 2));
        cmc_assert(hs_insert(set, 3));

        it = hs_iter_end(set);

        cmc_assert_equals(size_t, set->count - 1, it.index);

        cmc_assert_equals(size_t, 3, it.cursor);
        cmc_assert_equals(size_t, 1, it.first);
        cmc_assert_equals(size_t, 3, it.last);

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_start(set);

        // Empty checks
        cmc_assert(hs_iter_at_start(&it));
        it = hs_iter_end(set);
        cmc_assert(hs_iter_at_start(&it));

        // Non-empty checks
        cmc_assert(hs_insert(set, 1));
        cmc_assert(hs_insert(set, 2));
        it = hs_iter_end(set);
        cmc_assert(!hs_iter_at_start(&it));
        it = hs_iter_start(set);
        cmc_assert(hs_iter_at_start(&it));

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_start(set);

        // Empty check
        cmc_assert(hs_iter_at_end(&it));
        it = hs_iter_end(set);
        cmc_assert(hs_iter_at_end(&it));

        // Non-empty checks
        cmc_assert(hs_insert(set, 1));
        cmc_assert(hs_insert(set, 2));
        it = hs_iter_end(set);
        cmc_assert(hs_iter_at_end(&it));
        it = hs_iter_start(set);
        cmc_assert(!hs_iter_at_end(&it));

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_start(set);

        cmc_assert(!hs_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
            hs_insert(set, i);

        cmc_assert_equals(size_t, 100, set->count);

        it = hs_iter_end(set);

        cmc_assert(!hs_iter_at_start(&it));
        cmc_assert(hs_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, hs_iter_value(&it));

        cmc_assert(hs_iter_to_start(&it));

        cmc_assert(hs_iter_at_start(&it));
        cmc_assert(!hs_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, hs_iter_value(&it));

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_end(set);

        cmc_assert(!hs_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
            hs_insert(set, i);

        it = hs_iter_start(set);

        cmc_assert(hs_iter_at_start(&it));
        cmc_assert(!hs_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, hs_iter_value(&it));

        cmc_assert(hs_iter_to_end(&it));

        cmc_assert(!hs_iter_at_start(&it));
        cmc_assert(hs_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, hs_iter_value(&it));

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_start(set);

        cmc_assert(!hs_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
            hs_insert(set, i);

        size_t sum = 0;
        for (it = hs_iter_start(set); !hs_iter_at_end(&it); hs_iter_next(&it))
        {
            sum += hs_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        hs_iter_to_start(&it);
        do
        {
            sum += hs_iter_value(&it);
        } while (hs_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_end(set);

        cmc_assert(!hs_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
            hs_insert(set, i);

        size_t sum = 0;
        for (it = hs_iter_end(set); !hs_iter_at_start(&it); hs_iter_prev(&it))
        {
            sum += hs_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        hs_iter_to_end(&it);
        do
        {
            sum += hs_iter_value(&it);
        } while (hs_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_start(set);

        cmc_assert(!hs_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            hs_insert(set, i);

        it = hs_iter_start(set);

        cmc_assert(!hs_iter_advance(&it, 0));
        cmc_assert(!hs_iter_advance(&it, set->count));

        size_t sum = 0;
        for (it = hs_iter_start(set);;)
        {
            sum += hs_iter_value(&it);

            if (!hs_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        hs_iter_to_start(&it);
        cmc_assert(hs_iter_advance(&it, set->count - 1));

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_end(set);

        cmc_assert(!hs_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            hs_insert(set, i);

        it = hs_iter_end(set);

        cmc_assert(!hs_iter_rewind(&it, 0));
        cmc_assert(!hs_iter_rewind(&it, set->count));

        size_t sum = 0;
        for (it = hs_iter_end(set);;)
        {
            sum += hs_iter_value(&it);

            if (!hs_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        hs_iter_to_end(&it);
        cmc_assert(hs_iter_rewind(&it, set->count - 1));

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_end(set);
        cmc_assert(!hs_iter_go_to(&it, 0));

        it = hs_iter_start(set);
        cmc_assert(!hs_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            hs_insert(set, i);

        it = hs_iter_start(set);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            hs_iter_go_to(&it, i);

            sum += hs_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(hs_iter_go_to(&it, i - 1));

            sum += hs_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 0; i < 1001; i += 100)
        {
            cmc_assert(hs_iter_go_to(&it, i));
            cmc_assert_equals(size_t, i, hs_iter_index(&it));

            sum += hs_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashset_iter it = hs_iter_end(set);

        cmc_assert_equals(size_t, (size_t){ 0 }, hs_iter_value(&it));

        cmc_assert(hs_insert(set, 10));

        it = hs_iter_start(set);

        cmc_assert_equals(size_t, 10, hs_iter_value(&it));

        hs_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct hashset *set = hs_new(100, 0.6, hs_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i <= 1000; i++)
            hs_insert(set, i);

        struct hashset_iter it = hs_iter_start(set);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, hs_iter_index(&it));
            hs_iter_next(&it);
        }

        it = hs_iter_end(set);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, hs_iter_index(&it));
            hs_iter_prev(&it);
        }

        hs_free(set);
    });
});
