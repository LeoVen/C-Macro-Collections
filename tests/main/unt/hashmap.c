#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/hashmap.h>

CMC_GENERATE_HASHMAP(hm, hashmap, size_t, size_t)

CMC_CREATE_UNIT(hashmap_test, true, {
    CMC_CREATE_TEST(new, {
        struct hashmap *map = hm_new(943722, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, hm_count(map));
        cmc_assert_greater_equals(size_t, (943722 / 0.6), hm_capacity(map));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        struct hashmap *map = hm_new(0, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        struct hashmap *map = hm_new(UINT64_MAX, 0.99, cmp, hash);

        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 50; i++)
            hm_insert(map, i, i);

        cmc_assert_equals(size_t, 50, hm_count(map));

        hm_clear(map, NULL);

        cmc_assert_equals(size_t, 0, hm_count(map));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(insert, {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert_equals(size_t, 1, hm_count(map));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(insert[smallest capacity], {
        struct hashmap *map = hm_new(1, 0.99, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));
        cmc_assert(hm_insert(map, 1, 1));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(insert[element position], {
        // Using the numhash the key is the hash itself
        struct hashmap *map = hm_new(1, 0.99, cmp, numhash);

        cmc_assert_not_equals(ptr, NULL, map);

        size_t capacity = hm_capacity(map);

        cmc_assert_greater(size_t, 0, capacity);

        cmc_assert(hm_insert(map, capacity - 1, capacity - 1));
        cmc_assert(hm_insert(map, capacity, capacity));

        cmc_assert_equals(size_t, capacity - 1, map->buffer[capacity - 1].key);
        cmc_assert_equals(size_t, capacity, map->buffer[0].key);

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(insert[distance], {
        struct hashmap *map = hm_new(500, 0.6, cmp, hash0);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 200; i++)
            cmc_assert(hm_insert(map, i, i));

        // Everything is hashed to 0 so key 0 should have dist 0, key 1 dist 1, etc
        for (size_t i = 0; i < 200; i++)
            cmc_assert_equals(size_t, i, map->buffer[i].dist);

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(insert[distance wrap], {
        struct hashmap *map = hm_new(1, 0.99, cmp, hashcapminus1);

        cmc_assert_not_equals(ptr, NULL, map);

        size_t capacity = hm_capacity(map);

        // Just to be sure, not part of the test
        cmc_assert_equals(size_t, cmc_hashtable_primes[0] - 1, hashcapminus1(0));
        cmc_assert_equals(size_t, capacity - 1, hashcapminus1(0));

        cmc_assert(hm_insert(map, 0, 0));
        cmc_assert(hm_insert(map, 1, 1));

        cmc_assert_equals(size_t, 0, map->buffer[capacity - 1].dist);
        cmc_assert_equals(size_t, 1, map->buffer[0].dist);

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(insert[distances], {
        struct hashmap *map = hm_new(1, 0.99, cmp, hashcapminus4);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));

        for (size_t i = 0; i < 6; i++)
            cmc_assert(hm_insert(map, i, i));

        for (size_t i = 0; i < 6; i++)
            cmc_assert_equals(size_t, i, hm_impl_get_entry(map, i)->dist);

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(insert[buffer growth and item preservation], {
        struct hashmap *map = hm_new(1, 0.99, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));

        size_t p = 0;
        for (size_t i = 0; i < 100000; i++)
        {
            if (hm_full(map))
                p++;

            cmc_assert(hm_insert(map, i, i));
        }

        cmc_assert_equals(size_t, cmc_hashtable_primes[p], hm_capacity(map));

        for (size_t i = 0; i < 100000; i++)
            cmc_assert(hm_contains(map, i));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(update, {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 1, 1));

        size_t old;
        cmc_assert(hm_update(map, 1, 2, &old));

        cmc_assert_equals(size_t, 1, old);

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(update[key not found], {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hm_insert(map, i, i));

        cmc_assert(!hm_update(map, 120, 120, NULL));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(remove, {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hm_insert(map, i, i));

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hm_remove(map, i, NULL));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hm_remove(map, i, NULL));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(remove[key not found], {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hm_insert(map, i, i));

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hm_remove(map, i, NULL));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(max, {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hm_insert(map, i, i));

        size_t key;
        size_t val;
        cmc_assert(hm_max(map, &key, &val));

        cmc_assert_equals(size_t, 100, key);
        cmc_assert_equals(size_t, 100, val);

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(max[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(!hm_min(map, NULL, NULL));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(min, {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hm_insert(map, i, i));

        size_t key;
        size_t val;
        cmc_assert(hm_min(map, &key, &val));

        cmc_assert_equals(size_t, 1, key);
        cmc_assert_equals(size_t, 1, val);

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(min[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(!hm_min(map, NULL, NULL));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(get, {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 4321, 1234));

        cmc_assert_equals(size_t, 1234, hm_get(map, 4321));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(get[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, (size_t){0}, hm_get(map, 4321));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(get_ref, {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 4321, 1234));

        size_t *result = hm_get_ref(map, 4321);

        cmc_assert_not_equals(ptr, NULL, result);
        cmc_assert_equals(size_t, 1234, *result);

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(get_ref[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(ptr, NULL, hm_get_ref(map, 4321));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(contains, {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 987654321, 1));

        cmc_assert(hm_contains(map, 987654321));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(contains[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(!hm_contains(map, 987654321));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(contains[sum], {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 101; i <= 200; i++)
            cmc_assert(hm_insert(map, i, i));

        size_t sum = 0;
        for (size_t i = 1; i <= 300; i++)
            if (hm_contains(map, i))
                sum += i;

        cmc_assert_equals(size_t, 15050, sum);

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(empty, {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_empty(map));

        cmc_assert(hm_insert(map, 1, 1));

        cmc_assert(!hm_empty(map));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(full, {
        struct hashmap *map = hm_new(cmc_hashtable_primes[0], 0.99999, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));

        for (size_t i = 0; i < hm_capacity(map); i++)
            cmc_assert(hm_insert(map, i, i));

        cmc_assert(hm_full(map));

        cmc_assert(hm_insert(map, 10000, 10000));

        cmc_assert_equals(size_t, cmc_hashtable_primes[1], hm_capacity(map));

        cmc_assert(!hm_full(map));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(count, {
        struct hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i < 100; i++)
        {
            cmc_assert(hm_insert(map, i, i));
            cmc_assert_equals(size_t, i, hm_count(map));
        }

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(capacity, {
        struct hashmap *map = hm_new(2500, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_greater_equals(size_t, (size_t)(2500 / 0.6), hm_capacity(map));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(capacity[small], {
        struct hashmap *map = hm_new(1, 0.99, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(load, {
        struct hashmap *map = hm_new(1, 0.99, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_in_range(double, 0.99 - 0.0001, 0.99 + 0.0001, hm_load(map));

        hm_free(map, NULL);
    });
});
