#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include "../src/hashset.c"

CMC_CREATE_UNIT(hashset_test, false, {
    CMC_CREATE_TEST(new, {
        hashset *set = hs_new(943722, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_not_equals(ptr, NULL, set->buffer);
        cmc_assert_equals(size_t, 0, hs_count(set));
        cmc_assert_greater_equals(size_t, (943722 / 0.6), hs_capacity(set));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        hashset *set = hs_new(0, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        hashset *set = hs_new(UINT64_MAX, 0.99, cmp, hash);

        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 50; i++)
            hs_insert(set, i);

        cmc_assert_equals(size_t, 50, hs_count(set));

        hs_clear(set, NULL);

        cmc_assert_equals(size_t, 0, hs_count(set));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(insert, {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hs_insert(set, 1));
        cmc_assert_equals(size_t, 1, hs_count(set));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(insert[smallest capacity], {
        hashset *set = hs_new(1, 0.99, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hs_capacity(set));
        cmc_assert(hs_insert(set, 1));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(insert[element position], {
        // Using the numhash the key is the hash itself
        hashset *set = hs_new(1, 0.99, cmp, numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        size_t capacity = hs_capacity(set);

        cmc_assert_greater(size_t, 0, capacity);

        cmc_assert(hs_insert(set, capacity - 1));
        cmc_assert(hs_insert(set, capacity));

        cmc_assert_equals(size_t, capacity - 1, set->buffer[capacity - 1].value);
        cmc_assert_equals(size_t, capacity, set->buffer[0].value);

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(insert[distance], {
        hashset *set = hs_new(500, 0.6, cmp, hash0);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 200; i++)
            cmc_assert(hs_insert(set, i));

        // Everything is hashed to 0 so key 0 should have dist 0, key 1 dist 1, etc
        for (size_t i = 0; i < 200; i++)
            cmc_assert_equals(size_t, i, set->buffer[i].dist);

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(insert[distance wrap], {
        hashset *set = hs_new(1, 0.99, cmp, hashcapminus1);

        cmc_assert_not_equals(ptr, NULL, set);

        size_t capacity = hs_capacity(set);

        // Just to be sure, not part of the test
        cmc_assert_equals(size_t, cmc_hashtable_primes[0] - 1, hashcapminus1(0));
        cmc_assert_equals(size_t, capacity - 1, hashcapminus1(0));

        cmc_assert(hs_insert(set, 0));
        cmc_assert(hs_insert(set, 1));

        cmc_assert_equals(size_t, 0, set->buffer[capacity - 1].dist);
        cmc_assert_equals(size_t, 1, set->buffer[0].dist);

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(insert[buffer growth and item preservation], {
        hashset *set = hs_new(1, 0.99, cmp, hash);

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

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(remove, {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hs_insert(set, i));

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hs_remove(set, i));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hs_remove(set, i));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(remove[key not found], {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hs_insert(set, i));

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hs_remove(set, i));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(max, {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hs_insert(set, i));

        size_t val;
        cmc_assert(hs_max(set, &val));

        cmc_assert_equals(size_t, 100, val);

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(max[count = 0], {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(!hs_min(set, NULL));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(min, {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hs_insert(set, i));

        size_t val;
        cmc_assert(hs_min(set, &val));

        cmc_assert_equals(size_t, 1, val);

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(min[count = 0], {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(!hs_min(set, NULL));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(contains, {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hs_insert(set, 987654321));

        cmc_assert(hs_contains(set, 987654321));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(contains[count = 0], {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(!hs_contains(set, 987654321));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(contains[sum], {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 101; i <= 200; i++)
            cmc_assert(hs_insert(set, i));

        size_t sum = 0;
        for (size_t i = 1; i <= 300; i++)
            if (hs_contains(set, i))
                sum += i;

        cmc_assert_equals(size_t, 15050, sum);

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(empty, {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hs_empty(set));

        cmc_assert(hs_insert(set, 1));

        cmc_assert(!hs_empty(set));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(full, {
        hashset *set = hs_new(cmc_hashtable_primes[0], 0.99999, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hs_capacity(set));

        for (size_t i = 0; i < hs_capacity(set); i++)
            cmc_assert(hs_insert(set, i));

        cmc_assert(hs_full(set));

        cmc_assert(hs_insert(set, 10000));

        cmc_assert_equals(size_t, cmc_hashtable_primes[1], hs_capacity(set));

        cmc_assert(!hs_full(set));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(count, {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i < 100; i++)
        {
            cmc_assert(hs_insert(set, i));
            cmc_assert_equals(size_t, i, hs_count(set));
        }

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(capacity, {
        hashset *set = hs_new(2500, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_greater_equals(size_t, (size_t)(2500 / 0.6), hs_capacity(set));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(capacity[small], {
        hashset *set = hs_new(1, 0.99, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hs_capacity(set));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(load, {
        hashset *set = hs_new(1, 0.99, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert_in_range(double, 0.99 - 0.0001, 0.99 + 0.0001, hs_load(set));

        hs_free(set, NULL);
    });
});
