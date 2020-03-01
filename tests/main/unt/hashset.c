#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/hashset.h>

CMC_GENERATE_HASHSET(hs, hashset, size_t)

struct hashset_ftab_val *hs_ftab_val =
    &(struct hashset_ftab_val){ .cmp = cmp,
                                .cpy = copy,
                                .str = str,
                                .free = custom_free,
                                .hash = hash,
                                .pri = pri };

CMC_CREATE_UNIT(hashset_test, true, {
    CMC_CREATE_TEST(new, {
        struct hashset *set = hs_new(943722, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_not_equals(ptr, NULL, set->buffer);
        cmc_assert_equals(size_t, 0, hs_count(set));
        cmc_assert_greater_equals(size_t, (943722 / 0.6), hs_capacity(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        struct hashset *set = hs_new(0, 0.6, hs_ftab_val);

        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        struct hashset *set = hs_new(UINT64_MAX, 0.99, hs_ftab_val);

        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 50; i++)
            hs_insert(set, i);

        cmc_assert_equals(size_t, 50, hs_count(set));

        hs_clear(set);

        cmc_assert_equals(size_t, 0, hs_count(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(insert, {
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hs_insert(set, 1));
        cmc_assert_equals(size_t, 1, hs_count(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[smallest capacity], {
        struct hashset *set = hs_new(1, 0.99, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hs_capacity(set));
        cmc_assert(hs_insert(set, 1));

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[element position], {
        struct hashset *set = hs_new(1, 0.99, hs_ftab_val);

        // Temporary change
        // Using the numhash the key is the hash itself
        hs_ftab_val->hash = numhash;

        cmc_assert_not_equals(ptr, NULL, set);

        size_t capacity = hs_capacity(set);

        cmc_assert_greater(size_t, 0, capacity);

        cmc_assert(hs_insert(set, capacity - 1));
        cmc_assert(hs_insert(set, capacity));

        cmc_assert_equals(size_t, capacity - 1,
                          set->buffer[capacity - 1].value);
        cmc_assert_equals(size_t, capacity, set->buffer[0].value);

        hs_ftab_val->hash = hash;

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[distance], {
        struct hashset *set = hs_new(500, 0.6, hs_ftab_val);

        // Temporary change
        hs_ftab_val->hash = hash0;

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 200; i++)
            cmc_assert(hs_insert(set, i));

        // Everything is hashed to 0 so key 0 should have dist 0, key 1 dist 1,
        // etc
        for (size_t i = 0; i < 200; i++)
            cmc_assert_equals(size_t, i, set->buffer[i].dist);

        hs_ftab_val->hash = hash;

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[distance wrap], {
        struct hashset *set = hs_new(1, 0.99, hs_ftab_val);

        // Temporary change
        hs_ftab_val->hash = hashcapminus1;

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

        hs_ftab_val->hash = hash;

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[distances], {
        struct hashset *set = hs_new(1, 0.99, hs_ftab_val);

        // Temporary change
        hs_ftab_val->hash = hashcapminus4;

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hs_capacity(set));

        for (size_t i = 0; i < 6; i++)
            cmc_assert(hs_insert(set, i));

        for (size_t i = 0; i < 6; i++)
            cmc_assert_equals(size_t, i, hs_impl_get_entry(set, i)->dist);

        hs_ftab_val->hash = hash;

        hs_free(set);
    });

    CMC_CREATE_TEST(insert[buffer growth and item preservation], {
        struct hashset *set = hs_new(1, 0.99, hs_ftab_val);

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
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hs_insert(set, i));

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hs_remove(set, i));

        hs_free(set);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hs_remove(set, i));

        hs_free(set);
    });

    CMC_CREATE_TEST(remove[key not found], {
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hs_insert(set, i));

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hs_remove(set, i));

        hs_free(set);
    });

    CMC_CREATE_TEST(max, {
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hs_insert(set, i));

        size_t val;
        cmc_assert(hs_max(set, &val));

        cmc_assert_equals(size_t, 100, val);

        hs_free(set);
    });

    CMC_CREATE_TEST(max[count = 0], {
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(!hs_min(set, NULL));

        hs_free(set);
    });

    CMC_CREATE_TEST(min, {
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hs_insert(set, i));

        size_t val;
        cmc_assert(hs_min(set, &val));

        cmc_assert_equals(size_t, 1, val);

        hs_free(set);
    });

    CMC_CREATE_TEST(min[count = 0], {
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(!hs_min(set, NULL));

        hs_free(set);
    });

    CMC_CREATE_TEST(contains, {
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hs_insert(set, 987654321));

        cmc_assert(hs_contains(set, 987654321));

        hs_free(set);
    });

    CMC_CREATE_TEST(contains[count = 0], {
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(!hs_contains(set, 987654321));

        hs_free(set);
    });

    CMC_CREATE_TEST(contains[sum], {
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

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
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hs_empty(set));

        cmc_assert(hs_insert(set, 1));

        cmc_assert(!hs_empty(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(full, {
        struct hashset *set =
            hs_new(cmc_hashtable_primes[0], 0.99999, hs_ftab_val);

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
        struct hashset *set = hs_new(100, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i < 100; i++)
        {
            cmc_assert(hs_insert(set, i));
            cmc_assert_equals(size_t, i, hs_count(set));
        }

        hs_free(set);
    });

    CMC_CREATE_TEST(capacity, {
        struct hashset *set = hs_new(2500, 0.6, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_greater_equals(size_t, (size_t)(2500 / 0.6),
                                  hs_capacity(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(capacity[small], {
        struct hashset *set = hs_new(1, 0.99, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hs_capacity(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(load, {
        struct hashset *set = hs_new(1, 0.99, hs_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_in_range(double, 0.99 - 0.0001, 0.99 + 0.0001, hs_load(set));

        hs_free(set);
    });

    CMC_CREATE_TEST(flags, {
        struct hashset *set = hs_new(1, 0.99, hs_ftab_val);

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
});
