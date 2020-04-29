#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "../src/hashmap.c"

struct hashmap_fkey *hm_fkey = &(struct hashmap_fkey){ .cmp = cmc_size_cmp,
                                                       .cpy = NULL,
                                                       .str = cmc_size_str,
                                                       .free = NULL,
                                                       .hash = cmc_size_hash,
                                                       .pri = cmc_size_cmp };

struct hashmap_fval *hm_fval = &(struct hashmap_fval){ .cmp = cmc_size_cmp,
                                                       .cpy = NULL,
                                                       .str = cmc_size_str,
                                                       .free = NULL,
                                                       .hash = cmc_size_hash,
                                                       .pri = cmc_size_cmp };

struct hashmap_fkey *hm_fkey_counter = &(struct hashmap_fkey){ .cmp = k_c_cmp,
                                                               .cpy = k_c_cpy,
                                                               .str = k_c_str,
                                                               .free = k_c_free,
                                                               .hash = k_c_hash,
                                                               .pri = k_c_pri };

struct hashmap_fval *hm_fval_counter = &(struct hashmap_fval){ .cmp = v_c_cmp,
                                                               .cpy = v_c_cpy,
                                                               .str = v_c_str,
                                                               .free = v_c_free,
                                                               .hash = v_c_hash,
                                                               .pri = v_c_pri };

struct cmc_alloc_node *hm_alloc_node = &(struct cmc_alloc_node){
    .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free
};

CMC_CREATE_UNIT(HashMap, true, {
    CMC_CREATE_TEST(PFX##_new(), {
        struct hashmap *map = hm_new(943722, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
        cmc_assert_equals(ptr, hm_fkey, map->f_key);
        cmc_assert_equals(ptr, hm_fval, map->f_val);
        cmc_assert_equals(ptr, &cmc_alloc_node_default, map->alloc);
        cmc_assert_equals(ptr, NULL, map->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hm_capacity(map));

        hm_free(map);

        map = hm_new(0, 0.6, hm_fkey, hm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = hm_new(UINT64_MAX, 0.99, hm_fkey, hm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = hm_new(1000, 0.6, hm_fkey, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hm_new(1000, 0.6, NULL, hm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = hm_new(1000, 0.6, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(PFX##_new_custom(), {
        struct hashmap *map = hm_new_custom(943722, 0.6, hm_fkey, hm_fval,
                                            hm_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
        cmc_assert_equals(ptr, hm_fkey, map->f_key);
        cmc_assert_equals(ptr, hm_fval, map->f_val);
        cmc_assert_equals(ptr, hm_alloc_node, map->alloc);
        cmc_assert_equals(ptr, callbacks, map->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hm_capacity(map));

        hm_free(map);

        map = hm_new_custom(0, 0.6, hm_fkey, hm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hm_new_custom(UINT64_MAX, 0.99, hm_fkey, hm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hm_new_custom(1000, 0.6, hm_fkey, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hm_new_custom(1000, 0.6, NULL, hm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hm_new_custom(1000, 0.6, NULL, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(PFX##_init(), {
        struct hashmap map = hm_init(943722, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map.buffer);
        cmc_assert_equals(size_t, 0, map.count);
        cmc_assert_equals(double, 0.6, map.load);
        cmc_assert_equals(int32_t, cmc_flags.OK, map.flag);
        cmc_assert_equals(ptr, hm_fkey, map.f_key);
        cmc_assert_equals(ptr, hm_fval, map.f_val);
        cmc_assert_equals(ptr, &cmc_alloc_node_default, map.alloc);
        cmc_assert_equals(ptr, NULL, map.callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hm_capacity(&map));

        hm_release(map);

        map = hm_init(0, 0.6, hm_fkey, hm_fval);
        cmc_assert_equals(ptr, NULL, map.buffer);

        map = hm_init(UINT64_MAX, 0.99, hm_fkey, hm_fval);
        cmc_assert_equals(ptr, NULL, map.buffer);

        map = hm_init(1000, 0.6, hm_fkey, NULL);
        cmc_assert_equals(ptr, NULL, map.buffer);

        map = hm_init(1000, 0.6, NULL, hm_fval);
        cmc_assert_equals(ptr, NULL, map.buffer);

        map = hm_init(1000, 0.6, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map.buffer);
    });

    CMC_CREATE_TEST(PFX##_init_custom(), {
        struct hashmap map = hm_init_custom(943722, 0.6, hm_fkey, hm_fval,
                                            hm_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, map.buffer);
        cmc_assert_equals(size_t, 0, map.count);
        cmc_assert_equals(double, 0.6, map.load);
        cmc_assert_equals(int32_t, cmc_flags.OK, map.flag);
        cmc_assert_equals(ptr, hm_fkey, map.f_key);
        cmc_assert_equals(ptr, hm_fval, map.f_val);
        cmc_assert_equals(ptr, hm_alloc_node, map.alloc);
        cmc_assert_equals(ptr, callbacks, map.callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hm_capacity(&map));

        hm_release(map);

        map = hm_init_custom(0, 0.6, hm_fkey, hm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map.buffer);

        map = hm_init_custom(UINT64_MAX, 0.99, hm_fkey, hm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map.buffer);

        map = hm_init_custom(1000, 0.6, hm_fkey, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map.buffer);

        map = hm_init_custom(1000, 0.6, NULL, hm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map.buffer);

        map = hm_init_custom(1000, 0.6, NULL, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map.buffer);
    });

    CMC_CREATE_TEST(PFX##_clear(), {
        k_total_free = 0;
        v_total_free = 0;
        struct hashmap *map =
            hm_new(100, 0.6, hm_fkey_counter, hm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            hm_insert(map, i, i);

        cmc_assert_equals(size_t, 1000, map->count);

        size_t *r = hm_get_ref(map, 100);

        cmc_assert_equals(size_t, 100, *r);

        map->flag = cmc_flags.ERROR;
        hm_clear(map);

        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        // The buffer is still valid and all zeroes
        cmc_assert_equals(size_t, 0, *r);

        hm_free(map);
        k_total_free = 0;
        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_free(), {
        k_total_free = 0;
        v_total_free = 0;
        struct hashmap *map =
            hm_new(100, 0.6, hm_fkey_counter, hm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            hm_insert(map, i, i);

        cmc_assert_equals(size_t, 1000, map->count);

        hm_free(map);

        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        map = hm_new(1000, 0.6, hm_fkey_counter, hm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);

        hm_free(map);

        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);
        k_total_free = 0;
        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_release(), {
        k_total_free = 0;
        v_total_free = 0;
        struct hashmap map =
            hm_init(100, 0.6, hm_fkey_counter, hm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map.buffer);

        for (size_t i = 1; i <= 1000; i++)
            hm_insert(&map, i, i);

        cmc_assert_equals(size_t, 1000, map.count);

        hm_release(map);

        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        map = hm_init(1000, 0.6, hm_fkey_counter, hm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map.buffer);

        hm_release(map);

        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);
        k_total_free = 0;
        v_total_free = 0;
    });

    CMC_CREATE_TEST(insert, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert_equals(size_t, 1, hm_count(map));

        hm_free(map);
    });

    CMC_CREATE_TEST(insert[smallest capacity], {
        struct hashmap *map = hm_new(1, 0.99, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));
        cmc_assert(hm_insert(map, 1, 1));

        hm_free(map);
    });

    CMC_CREATE_TEST(insert[element position], {
        struct hashmap *map = hm_new(1, 0.99, hm_fkey, hm_fval);

        // Temporary change
        // Using the numhash the key is the hash itself
        hm_fkey->hash = numhash;

        cmc_assert_not_equals(ptr, NULL, map);

        size_t capacity = hm_capacity(map);

        cmc_assert_greater(size_t, 0, capacity);

        cmc_assert(hm_insert(map, capacity - 1, capacity - 1));
        cmc_assert(hm_insert(map, capacity, capacity));

        cmc_assert_equals(size_t, capacity - 1, map->buffer[capacity - 1].key);
        cmc_assert_equals(size_t, capacity, map->buffer[0].key);

        hm_fkey->hash = cmc_size_hash;

        hm_free(map);
    });

    CMC_CREATE_TEST(insert[distance], {
        struct hashmap *map = hm_new(500, 0.6, hm_fkey, hm_fval);

        // Temporary change
        hm_fkey->hash = hash0;

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 200; i++)
            cmc_assert(hm_insert(map, i, i));

        // Everything is hashed to 0 so key 0 should have dist 0, key 1 dist 1,
        // etc
        for (size_t i = 0; i < 200; i++)
            cmc_assert_equals(size_t, i, map->buffer[i].dist);

        hm_fkey->hash = cmc_size_hash;

        hm_free(map);
    });

    CMC_CREATE_TEST(insert[distance wrap], {
        struct hashmap *map = hm_new(1, 0.99, hm_fkey, hm_fval);

        // Temporary change
        hm_fkey->hash = hashcapminus1;

        cmc_assert_not_equals(ptr, NULL, map);

        size_t capacity = hm_capacity(map);

        // Just to be sure, not part of the test
        cmc_assert_equals(size_t, cmc_hashtable_primes[0] - 1,
                          hashcapminus1(0));
        cmc_assert_equals(size_t, capacity - 1, hashcapminus1(0));

        cmc_assert(hm_insert(map, 0, 0));
        cmc_assert(hm_insert(map, 1, 1));

        cmc_assert_equals(size_t, 0, map->buffer[capacity - 1].dist);
        cmc_assert_equals(size_t, 1, map->buffer[0].dist);

        hm_fkey->hash = cmc_size_hash;

        hm_free(map);
    });

    CMC_CREATE_TEST(insert[distances], {
        struct hashmap *map = hm_new(1, 0.99, hm_fkey, hm_fval);

        // Temporary change
        hm_fkey->hash = hashcapminus4;

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));

        for (size_t i = 0; i < 6; i++)
            cmc_assert(hm_insert(map, i, i));

        for (size_t i = 0; i < 6; i++)
            cmc_assert_equals(size_t, i, hm_impl_get_entry(map, i)->dist);

        hm_fkey->hash = cmc_size_hash;

        hm_free(map);
    });

    CMC_CREATE_TEST(insert[buffer growth and item preservation], {
        struct hashmap *map = hm_new(1, 0.99, hm_fkey, hm_fval);

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

        hm_free(map);
    });

    CMC_CREATE_TEST(update, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 1, 1));

        size_t old;
        cmc_assert(hm_update(map, 1, 2, &old));

        cmc_assert_equals(size_t, 1, old);

        hm_free(map);
    });

    CMC_CREATE_TEST(update[key not found], {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hm_insert(map, i, i));

        cmc_assert(!hm_update(map, 120, 120, NULL));

        hm_free(map);
    });

    CMC_CREATE_TEST(remove, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hm_insert(map, i, i));

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hm_remove(map, i, NULL));

        hm_free(map);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hm_remove(map, i, NULL));

        hm_free(map);
    });

    CMC_CREATE_TEST(remove[key not found], {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hm_insert(map, i, i));

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hm_remove(map, i, NULL));

        hm_free(map);
    });

    CMC_CREATE_TEST(max, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hm_insert(map, i, i));

        size_t key;
        size_t val;
        cmc_assert(hm_max(map, &key, &val));

        cmc_assert_equals(size_t, 100, key);
        cmc_assert_equals(size_t, 100, val);

        hm_free(map);
    });

    CMC_CREATE_TEST(max[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(!hm_min(map, NULL, NULL));

        hm_free(map);
    });

    CMC_CREATE_TEST(min, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hm_insert(map, i, i));

        size_t key;
        size_t val;
        cmc_assert(hm_min(map, &key, &val));

        cmc_assert_equals(size_t, 1, key);
        cmc_assert_equals(size_t, 1, val);

        hm_free(map);
    });

    CMC_CREATE_TEST(min[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(!hm_min(map, NULL, NULL));

        hm_free(map);
    });

    CMC_CREATE_TEST(get, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 4321, 1234));

        cmc_assert_equals(size_t, 1234, hm_get(map, 4321));

        hm_free(map);
    });

    CMC_CREATE_TEST(get[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, (size_t){ 0 }, hm_get(map, 4321));

        hm_free(map);
    });

    CMC_CREATE_TEST(get_ref, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 4321, 1234));

        size_t *result = hm_get_ref(map, 4321);

        cmc_assert_not_equals(ptr, NULL, result);
        cmc_assert_equals(size_t, 1234, *result);

        hm_free(map);
    });

    CMC_CREATE_TEST(get_ref[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(ptr, NULL, hm_get_ref(map, 4321));

        hm_free(map);
    });

    CMC_CREATE_TEST(contains, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 987654321, 1));

        cmc_assert(hm_contains(map, 987654321));

        hm_free(map);
    });

    CMC_CREATE_TEST(contains[count = 0], {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(!hm_contains(map, 987654321));

        hm_free(map);
    });

    CMC_CREATE_TEST(contains[sum], {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 101; i <= 200; i++)
            cmc_assert(hm_insert(map, i, i));

        size_t sum = 0;
        for (size_t i = 1; i <= 300; i++)
            if (hm_contains(map, i))
                sum += i;

        cmc_assert_equals(size_t, 15050, sum);

        hm_free(map);
    });

    CMC_CREATE_TEST(empty, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_empty(map));

        cmc_assert(hm_insert(map, 1, 1));

        cmc_assert(!hm_empty(map));

        hm_free(map);
    });

    CMC_CREATE_TEST(full, {
        struct hashmap *map =
            hm_new(cmc_hashtable_primes[0], 0.99999, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));

        for (size_t i = 0; i < hm_capacity(map); i++)
            cmc_assert(hm_insert(map, i, i));

        cmc_assert(hm_full(map));

        cmc_assert(hm_insert(map, 10000, 10000));

        cmc_assert_equals(size_t, cmc_hashtable_primes[1], hm_capacity(map));

        cmc_assert(!hm_full(map));

        hm_free(map);
    });

    CMC_CREATE_TEST(count, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i < 100; i++)
        {
            cmc_assert(hm_insert(map, i, i));
            cmc_assert_equals(size_t, i, hm_count(map));
        }

        hm_free(map);
    });

    CMC_CREATE_TEST(capacity, {
        struct hashmap *map = hm_new(2500, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_greater_equals(size_t, (size_t)(2500 / 0.6),
                                  hm_capacity(map));

        hm_free(map);
    });

    CMC_CREATE_TEST(capacity[small], {
        struct hashmap *map = hm_new(1, 0.99, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));

        hm_free(map);
    });

    CMC_CREATE_TEST(load, {
        struct hashmap *map = hm_new(1, 0.99, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_in_range(double, 0.99 - 0.0001, 0.99 + 0.0001, hm_load(map));

        hm_free(map);
    });

    CMC_CREATE_TEST(flags, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));

        // insert
        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));
        cmc_assert(!hm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hm_flag(map));

        // clear
        hm_clear(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));

        // update
        cmc_assert(!hm_update(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hm_flag(map));

        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));

        cmc_assert(!hm_update(map, 2, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hm_flag(map));

        cmc_assert(hm_update(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));

        // remove
        cmc_assert(!hm_remove(map, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hm_flag(map));

        cmc_assert(hm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));

        cmc_assert(!hm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hm_flag(map));

        // max
        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert(hm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));

        cmc_assert(hm_remove(map, 1, NULL));
        cmc_assert(!hm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hm_flag(map));

        // min
        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert(hm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));

        cmc_assert(hm_remove(map, 1, NULL));
        cmc_assert(!hm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hm_flag(map));

        // get
        hm_get(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hm_flag(map));

        cmc_assert(hm_insert(map, 1, 1));
        hm_get(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));

        hm_get(map, 2);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hm_flag(map));

        hm_clear(map);

        // get_ref
        hm_get_ref(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hm_flag(map));

        cmc_assert(hm_insert(map, 1, 1));
        hm_get_ref(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));

        hm_get_ref(map, 2);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hm_flag(map));

        // contains
        hm_contains(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));

        // copy
        struct hashmap *map2 = hm_copy_of(map);

        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map2));

        size_t tmp = map->capacity;
        map->capacity = 0;

        struct hashmap *map3 = hm_copy_of(map);
        cmc_assert_equals(ptr, NULL, map3);

        cmc_assert_equals(int32_t, cmc_flags.ERROR, hm_flag(map));

        map->capacity = tmp;

        cmc_assert(hm_equals(map, map2));
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, hm_flag(map2));

        hm_free(map);
        hm_free(map2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct hashmap *map =
            hm_new_custom(100, 0.6, hm_fkey, hm_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(hm_update(map, 1, 10, NULL));
        cmc_assert_equals(int32_t, 1, total_update);

        cmc_assert(hm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(hm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(hm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert(hm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert_equals(size_t, 2, hm_get(map, 1));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert_not_equals(ptr, NULL, hm_get_ref(map, 1));
        cmc_assert_equals(int32_t, 4, total_read);

        cmc_assert(hm_contains(map, 1));
        cmc_assert_equals(int32_t, 5, total_read);

        cmc_assert(hm_resize(map, 10000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(hm_resize(map, 500));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 1, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        hm_customize(map, NULL, NULL);

        cmc_assert_equals(ptr, NULL, map->callbacks);

        hm_clear(map);
        cmc_assert(hm_insert(map, 1, 2));
        cmc_assert(hm_update(map, 1, 10, NULL));
        cmc_assert(hm_remove(map, 1, NULL));
        cmc_assert(hm_insert(map, 1, 2));
        cmc_assert(hm_max(map, NULL, NULL));
        cmc_assert(hm_min(map, NULL, NULL));
        cmc_assert_equals(size_t, 2, hm_get(map, 1));
        cmc_assert_not_equals(ptr, NULL, hm_get_ref(map, 1));
        cmc_assert(hm_contains(map, 1));
        cmc_assert(hm_resize(map, 10000));
        cmc_assert(hm_resize(map, 500));

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 1, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, map->callbacks);

        hm_free(map);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});
