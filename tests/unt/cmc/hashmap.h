#ifndef CMC_TESTS_UNT_CMC_ASHMAP_H
#define CMC_TESTS_UNT_CMC_ASHMAP_H

#include "../utl.h"

#include "cmc/hashmap.h"

struct hashmap_fkey *hm_fkey = &(struct hashmap_fkey){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

struct hashmap_fval *hm_fval = &(struct hashmap_fval){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

struct hashmap_fkey *hm_fkey_counter = &(struct hashmap_fkey){
    .cmp = k_c_cmp, .cpy = k_c_cpy, .str = k_c_str, .free = k_c_free, .hash = k_c_hash, .pri = k_c_pri
};

struct hashmap_fval *hm_fval_counter = &(struct hashmap_fval){
    .cmp = v_c_cmp, .cpy = v_c_cpy, .str = v_c_str, .free = v_c_free, .hash = v_c_hash, .pri = v_c_pri
};

struct cmc_alloc_node *hm_alloc_node =
    &(struct cmc_alloc_node){ .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free };

CMC_CREATE_UNIT(CMCHashMap, true, {
    CMC_CREATE_TEST(PFX##_new(), {
        struct hashmap *map = hm_new(943722, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, map->flag);
        cmc_assert_equals(ptr, hm_fkey, map->f_key);
        cmc_assert_equals(ptr, hm_fval, map->f_val);
        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, map->alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, map->alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, map->alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, map->alloc->free);
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
        struct hashmap *map = hm_new_custom(943722, 0.6, hm_fkey, hm_fval, hm_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, map->flag);
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
        cmc_assert_equals(int32_t, CMC_FLAG_OK, map.flag);
        cmc_assert_equals(ptr, hm_fkey, map.f_key);
        cmc_assert_equals(ptr, hm_fval, map.f_val);
        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, map.alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, map.alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, map.alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, map.alloc->free);
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
        cmc_assert_equals(int32_t, CMC_FLAG_OK, map.flag);
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
        struct hashmap *map = hm_new(100, 0.6, hm_fkey_counter, hm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            hm_insert(map, i, i);

        cmc_assert_equals(size_t, 1000, map->count);

        size_t *r = hm_get_ref(map, 100);

        cmc_assert_equals(size_t, 100, *r);

        map->flag = CMC_FLAG_ERROR;
        hm_clear(map);

        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, map->flag);
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
        struct hashmap *map = hm_new(100, 0.6, hm_fkey_counter, hm_fval_counter);

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

    CMC_CREATE_TEST(PFX##_customize(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        map->flag = CMC_FLAG_ERROR;
        hm_customize(map, hm_alloc_node, callbacks);

        cmc_assert_equals(ptr, hm_alloc_node, map->alloc);
        cmc_assert_equals(ptr, callbacks, map->callbacks);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, map->flag);

        map->flag = CMC_FLAG_ERROR;
        hm_customize(map, NULL, NULL);

        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, map->alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, map->alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, map->alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, map->alloc->free);
        cmc_assert_equals(ptr, NULL, map->callbacks);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, map->flag);

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_insert(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        map->flag = CMC_FLAG_ERROR;
        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, map->flag);
        cmc_assert_equals(size_t, 1, hm_count(map));

        hm_free(map);

        map = hm_new(1, 0.99, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));
        cmc_assert(hm_insert(map, 1, 1));

        hm_free(map);

        // element position
        map = hm_new(1, 0.99, hm_fkey, hm_fval);

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

        // distance
        map = hm_new(500, 0.6, hm_fkey, hm_fval);

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

        // distance wrap
        map = hm_new(1, 0.99, hm_fkey, hm_fval);

        // Temporary change
        hm_fkey->hash = hashcapminus1;

        cmc_assert_not_equals(ptr, NULL, map);

        capacity = hm_capacity(map);

        // Just to be sure, not part of the test
        cmc_assert_equals(size_t, cmc_hashtable_primes[0] - 1, hashcapminus1(0));
        cmc_assert_equals(size_t, capacity - 1, hashcapminus1(0));

        cmc_assert(hm_insert(map, 0, 0));
        cmc_assert(hm_insert(map, 1, 1));

        cmc_assert_equals(size_t, 0, map->buffer[capacity - 1].dist);
        cmc_assert_equals(size_t, 1, map->buffer[0].dist);

        hm_fkey->hash = cmc_size_hash;

        hm_free(map);

        // distances
        map = hm_new(1, 0.99, hm_fkey, hm_fval);

        // Temporary change
        hm_fkey->hash = hashcapminus4;

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));

        for (size_t i = 0; i < 6; i++)
            cmc_assert(hm_insert(map, i, i));

        // for (size_t i = 0; i < 6; i++)
        //     cmc_assert_equals(size_t, i, hm_impl_get_entry(map, i)->dist);

        hm_fkey->hash = cmc_size_hash;

        hm_free(map);

        // buffer growth and item preservation
        map = hm_new(1, 0.99, hm_fkey, hm_fval);

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

    CMC_CREATE_TEST(PFX##_update(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 1, 1));

        size_t old;
        cmc_assert(hm_update(map, 1, 2, &old));

        cmc_assert_equals(size_t, 1, old);

        hm_free(map);

        // key not found
        map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            hm_insert(map, i, i);

        cmc_assert(!hm_update(map, 120, 120, NULL));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_remove(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            hm_insert(map, i, i);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hm_remove(map, i, NULL));

        hm_free(map);

        // count = 0
        map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hm_remove(map, i, NULL));

        hm_free(map);

        // key not found
        map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            hm_insert(map, i, i);

        cmc_assert_equals(size_t, 100, map->count);

        for (size_t i = 100; i < 200; i++)
            cmc_assert(!hm_remove(map, i, NULL));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_max(), {
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

        // count = 0
        map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(!hm_min(map, NULL, NULL));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_min(), {
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

        // count = 0
        map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(!hm_min(map, NULL, NULL));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_get(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 4321, 1234));

        cmc_assert_equals(size_t, 1234, hm_get(map, 4321));

        hm_free(map);

        // count = 0
        map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, (size_t){ 0 }, hm_get(map, 4321));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_get_ref(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 4321, 1234));

        size_t *result = hm_get_ref(map, 4321);

        cmc_assert_not_equals(ptr, NULL, result);
        cmc_assert_equals(size_t, 1234, *result);

        hm_free(map);

        // count = 0
        map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(ptr, NULL, hm_get_ref(map, 4321));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_contains(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_insert(map, 987654321, 1));

        cmc_assert(hm_contains(map, 987654321));

        hm_free(map);

        // count = 0
        map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(!hm_contains(map, 987654321));

        hm_free(map);

        // sum
        map = hm_new(100, 0.6, hm_fkey, hm_fval);

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

    CMC_CREATE_TEST(PFX##_empty(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hm_empty(map));

        cmc_assert(hm_insert(map, 1, 1));

        cmc_assert(!hm_empty(map));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_full(), {
        struct hashmap *map = hm_new(cmc_hashtable_primes[0], 0.99999, hm_fkey, hm_fval);

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

    CMC_CREATE_TEST(PFX##_count(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i < 100; i++)
        {
            cmc_assert(hm_insert(map, i, i));
            cmc_assert_equals(size_t, i, hm_count(map));
        }

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_capacity(), {
        struct hashmap *map = hm_new(2500, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_greater_equals(size_t, (size_t)(2500 / 0.6), hm_capacity(map));

        hm_free(map);

        // small capacity
        map = hm_new(1, 0.99, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, cmc_hashtable_primes[0], hm_capacity(map));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_load(), {
        struct hashmap *map = hm_new(1, 0.99, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_in_range(double, 0.99 - 0.0001, 0.99 + 0.0001, hm_load(map));

        hm_free(map);
    });

    CMC_CREATE_TEST(flags, {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));

        // insert
        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));
        cmc_assert(!hm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, CMC_FLAG_DUPLICATE, hm_flag(map));

        // clear
        hm_clear(map);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));

        // update
        cmc_assert(!hm_update(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hm_flag(map));

        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));

        cmc_assert(!hm_update(map, 2, 1, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hm_flag(map));

        cmc_assert(hm_update(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));

        // remove
        cmc_assert(!hm_remove(map, 2, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hm_flag(map));

        cmc_assert(hm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));

        cmc_assert(!hm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hm_flag(map));

        // max
        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert(hm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));

        cmc_assert(hm_remove(map, 1, NULL));
        cmc_assert(!hm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hm_flag(map));

        // min
        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert(hm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));

        cmc_assert(hm_remove(map, 1, NULL));
        cmc_assert(!hm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hm_flag(map));

        // get
        hm_get(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hm_flag(map));

        cmc_assert(hm_insert(map, 1, 1));
        hm_get(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));

        hm_get(map, 2);
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hm_flag(map));

        hm_clear(map);

        // get_ref
        hm_get_ref(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hm_flag(map));

        cmc_assert(hm_insert(map, 1, 1));
        hm_get_ref(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));

        hm_get_ref(map, 2);
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hm_flag(map));

        // contains
        hm_contains(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));

        // copy
        struct hashmap *map2 = hm_copy_of(map);

        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map2));

        size_t tmp = map->capacity;
        map->capacity = 0;

        struct hashmap *map3 = hm_copy_of(map);
        cmc_assert_equals(ptr, NULL, map3);

        cmc_assert_equals(int32_t, CMC_FLAG_ERROR, hm_flag(map));

        map->capacity = tmp;

        cmc_assert(hm_equals(map, map2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hm_flag(map2));

        hm_free(map);
        hm_free(map2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct hashmap *map = hm_new_custom(100, 0.6, hm_fkey, hm_fval, NULL, callbacks);

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

struct hashmap_fkey *hm_fkey_numhash = &(struct hashmap_fkey){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = numhash, .pri = cmc_size_cmp
};

CMC_CREATE_UNIT(CMCHashMapIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey_numhash, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_start(map);

        cmc_assert_equals(ptr, map, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(size_t, 0, it.first);
        cmc_assert_equals(size_t, 0, it.last);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(hm_iter_at_start(&it));
        cmc_assert(hm_iter_at_end(&it));

        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert(hm_insert(map, 2, 2));
        cmc_assert(hm_insert(map, 3, 3));

        it = hm_iter_start(map);

        cmc_assert_equals(size_t, 0, it.index);

        cmc_assert_equals(size_t, 1, it.cursor);
        cmc_assert_equals(size_t, 1, it.first);
        cmc_assert_equals(size_t, 3, it.last);
        cmc_assert_equals(bool, false, it.end);

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey_numhash, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_end(map);

        cmc_assert_equals(ptr, map, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(size_t, 0, it.first);
        cmc_assert_equals(size_t, 0, it.last);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(hm_iter_at_start(&it));
        cmc_assert(hm_iter_at_end(&it));

        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert(hm_insert(map, 2, 2));
        cmc_assert(hm_insert(map, 3, 3));

        it = hm_iter_end(map);

        cmc_assert_equals(size_t, map->count - 1, it.index);

        cmc_assert_equals(size_t, 3, it.cursor);
        cmc_assert_equals(size_t, 1, it.first);
        cmc_assert_equals(size_t, 3, it.last);
        cmc_assert_equals(bool, false, it.start);

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_start(map);

        // Empty checks
        cmc_assert(hm_iter_at_start(&it));
        it = hm_iter_end(map);
        cmc_assert(hm_iter_at_start(&it));

        // Non-empty checks
        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert(hm_insert(map, 2, 2));
        it = hm_iter_end(map);
        cmc_assert(!hm_iter_at_start(&it));
        it = hm_iter_start(map);
        cmc_assert(hm_iter_at_start(&it));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_start(map);

        // Empty check
        cmc_assert(hm_iter_at_end(&it));
        it = hm_iter_end(map);
        cmc_assert(hm_iter_at_end(&it));

        // Non-empty checks
        cmc_assert(hm_insert(map, 1, 1));
        cmc_assert(hm_insert(map, 2, 2));
        it = hm_iter_end(map);
        cmc_assert(hm_iter_at_end(&it));
        it = hm_iter_start(map);
        cmc_assert(!hm_iter_at_end(&it));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey_numhash, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_start(map);

        cmc_assert(!hm_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
            hm_insert(map, i, i);

        cmc_assert_equals(size_t, 100, map->count);

        it = hm_iter_end(map);

        cmc_assert(!hm_iter_at_start(&it));
        cmc_assert(hm_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, hm_iter_value(&it));

        cmc_assert(hm_iter_to_start(&it));

        cmc_assert(hm_iter_at_start(&it));
        cmc_assert(!hm_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, hm_iter_value(&it));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey_numhash, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_end(map);

        cmc_assert(!hm_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
            hm_insert(map, i, i);

        it = hm_iter_start(map);

        cmc_assert(hm_iter_at_start(&it));
        cmc_assert(!hm_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, hm_iter_value(&it));

        cmc_assert(hm_iter_to_end(&it));

        cmc_assert(!hm_iter_at_start(&it));
        cmc_assert(hm_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, hm_iter_value(&it));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_start(map);

        cmc_assert(!hm_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
            hm_insert(map, i, i);

        size_t sum = 0;
        for (it = hm_iter_start(map); !hm_iter_at_end(&it); hm_iter_next(&it))
        {
            sum += hm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        hm_iter_to_start(&it);
        do
        {
            sum += hm_iter_value(&it);
        } while (hm_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_end(map);

        cmc_assert(!hm_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
            hm_insert(map, i, i);

        size_t sum = 0;
        for (it = hm_iter_end(map); !hm_iter_at_start(&it); hm_iter_prev(&it))
        {
            sum += hm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        hm_iter_to_end(&it);
        do
        {
            sum += hm_iter_value(&it);
        } while (hm_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey_numhash, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_start(map);

        cmc_assert(!hm_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            hm_insert(map, i, i);

        it = hm_iter_start(map);

        cmc_assert(!hm_iter_advance(&it, 0));
        cmc_assert(!hm_iter_advance(&it, map->count));

        size_t sum = 0;
        for (it = hm_iter_start(map);;)
        {
            sum += hm_iter_value(&it);

            if (!hm_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        hm_iter_to_start(&it);
        cmc_assert(hm_iter_advance(&it, map->count - 1));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey_numhash, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_end(map);

        cmc_assert(!hm_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            hm_insert(map, i, i);

        it = hm_iter_end(map);

        cmc_assert(!hm_iter_rewind(&it, 0));
        cmc_assert(!hm_iter_rewind(&it, map->count));

        size_t sum = 0;
        for (it = hm_iter_end(map);;)
        {
            sum += hm_iter_value(&it);

            if (!hm_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        hm_iter_to_end(&it);
        cmc_assert(hm_iter_rewind(&it, map->count - 1));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey_numhash, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_end(map);
        cmc_assert(!hm_iter_go_to(&it, 0));

        it = hm_iter_start(map);
        cmc_assert(!hm_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            hm_insert(map, i, i);

        it = hm_iter_start(map);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            hm_iter_go_to(&it, i);

            sum += hm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(hm_iter_go_to(&it, i - 1));

            sum += hm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 0; i < 1001; i += 100)
        {
            cmc_assert(hm_iter_go_to(&it, i));
            cmc_assert_equals(size_t, i, hm_iter_index(&it));

            sum += hm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_key(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_end(map);

        cmc_assert_equals(size_t, (size_t){ 0 }, hm_iter_key(&it));

        cmc_assert(hm_insert(map, 10, 10));

        it = hm_iter_start(map);

        cmc_assert_equals(size_t, 10, hm_iter_key(&it));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_end(map);

        cmc_assert_equals(size_t, (size_t){ 0 }, hm_iter_value(&it));

        cmc_assert(hm_insert(map, 10, 10));

        it = hm_iter_start(map);

        cmc_assert_equals(size_t, 10, hm_iter_value(&it));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_rvalue(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmap_iter it = hm_iter_end(map);

        cmc_assert_equals(ptr, NULL, hm_iter_rvalue(&it));

        cmc_assert(hm_insert(map, 10, 10));

        it = hm_iter_start(map);

        cmc_assert_not_equals(ptr, NULL, hm_iter_rvalue(&it));
        cmc_assert_equals(size_t, 10, *hm_iter_rvalue(&it));

        hm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct hashmap *map = hm_new(100, 0.6, hm_fkey, hm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i <= 1000; i++)
            hm_insert(map, i, i);

        struct hashmap_iter it = hm_iter_start(map);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, hm_iter_index(&it));
            hm_iter_next(&it);
        }

        it = hm_iter_end(map);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, hm_iter_index(&it));
            hm_iter_prev(&it);
        }

        hm_free(map);
    });
});

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = CMCHashMap() + CMCHashMapIter();

    printf(" +---------------------------------------------------------------+");
    printf("\n");
    printf(" | CMCHashMap Suit : %-46s |\n", result == 0 ? "PASSED" : "FAILED");
    printf(" +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif

#endif /* CMC_TESTS_UNT_CMC_ASHMAP_H */
