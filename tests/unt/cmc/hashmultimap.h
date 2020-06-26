#ifndef CMC_TESTS_UNT_CMC_ASHMULTIMAP_H
#define CMC_TESTS_UNT_CMC_ASHMULTIMAP_H

#include "../utl.h"

#include "cmc/hashmultimap.h"

struct hashmultimap_fkey *hmm_fkey = &(struct hashmultimap_fkey){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

struct hashmultimap_fval *hmm_fval = &(struct hashmultimap_fval){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

struct hashmultimap_fkey *hmm_fkey_counter = &(struct hashmultimap_fkey){
    .cmp = k_c_cmp, .cpy = k_c_cpy, .str = k_c_str, .free = k_c_free, .hash = k_c_hash, .pri = k_c_pri
};

struct hashmultimap_fval *hmm_fval_counter = &(struct hashmultimap_fval){
    .cmp = v_c_cmp, .cpy = v_c_cpy, .str = v_c_str, .free = v_c_free, .hash = v_c_hash, .pri = v_c_pri
};

struct cmc_alloc_node *hmm_alloc_node =
    &(struct cmc_alloc_node){ .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free };

CMC_CREATE_UNIT(CMCHashMultiMap, true, {
    CMC_CREATE_TEST(PFX##_new(), {
        struct hashmultimap *map = hmm_new(943722, 0.6, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, map->flag);
        cmc_assert_equals(ptr, hmm_fkey, map->f_key);
        cmc_assert_equals(ptr, hmm_fval, map->f_val);
        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, map->alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, map->alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, map->alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, map->alloc->free);
        cmc_assert_equals(ptr, NULL, map->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hmm_capacity(map));

        hmm_free(map);

        map = hmm_new(100, 0.6, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < map->capacity; i++)
        {
            cmc_assert_equals(ptr, NULL, map->buffer[i][0]);
            cmc_assert_equals(ptr, NULL, map->buffer[i][1]);
        }

        hmm_free(map);

        map = hmm_new(0, 0.6, hmm_fkey, hmm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = hmm_new(UINT64_MAX, 0.99, hmm_fkey, hmm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = hmm_new(1000, 0.6, hmm_fkey, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hmm_new(1000, 0.6, NULL, hmm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = hmm_new(1000, 0.6, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(PFX##_new_custom(), {
        struct hashmultimap *map = hmm_new_custom(943722, 0.6, hmm_fkey, hmm_fval, hmm_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, map->flag);
        cmc_assert_equals(ptr, hmm_fkey, map->f_key);
        cmc_assert_equals(ptr, hmm_fval, map->f_val);
        cmc_assert_equals(ptr, hmm_alloc_node, map->alloc);
        cmc_assert_equals(ptr, callbacks, map->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hmm_capacity(map));

        hmm_free(map);

        map = hmm_new_custom(0, 0.6, hmm_fkey, hmm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hmm_new_custom(UINT64_MAX, 0.99, hmm_fkey, hmm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hmm_new_custom(1000, 0.6, hmm_fkey, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hmm_new_custom(1000, 0.6, NULL, hmm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hmm_new_custom(1000, 0.6, NULL, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(PFX##_clear(), {
        k_total_free = 0;
        v_total_free = 0;
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey_counter, hmm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            hmm_insert(map, i, i);

        cmc_assert_equals(size_t, 1000, map->count);

        map->flag = CMC_FLAG_ERROR;
        hmm_clear(map);

        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, map->flag);
        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        hmm_free(map);
        k_total_free = 0;
        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_free(), {
        k_total_free = 0;
        v_total_free = 0;
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey_counter, hmm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            hmm_insert(map, i, i);

        cmc_assert_equals(size_t, 1000, map->count);

        hmm_free(map);

        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        map = hmm_new(1000, 0.6, hmm_fkey_counter, hmm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);

        hmm_free(map);

        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);
        k_total_free = 0;
        v_total_free = 0;
    });

    CMC_CREATE_TEST(insert[count], {
        struct hashmultimap *map = hmm_new(100, 0.8, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hmm_insert(map, i % 20, i % 20));

        cmc_assert_equals(size_t, 100, hmm_count(map));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_remove(), {
        struct hashmultimap *map = hmm_new(100, 0.8, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 200; i++)
            cmc_assert(hmm_insert(map, i % 20, i % 20));

        size_t r;

        for (size_t i = 0; i < 60; i++)
            cmc_assert(hmm_remove(map, i % 20, &r));

        cmc_assert_equals(size_t, 140, hmm_count(map));

        hmm_free(map);

        // count = 0
        map = hmm_new(100, 0.8, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(!hmm_remove(map, 10, &r));

        hmm_free(map);

        // count = 1
        map = hmm_new(100, 0.8, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hmm_insert(map, 10, 11));
        cmc_assert(hmm_remove(map, 10, &r));

        hmm_free(map);
    });

    CMC_CREATE_TEST(get[key ordering], {
        struct hashmultimap *map = hmm_new(100, 0.8, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        /* Assert that the value 100 is accessed before the value 101 for */
        /* the same key */
        cmc_assert(hmm_insert(map, 10, 100));
        cmc_assert(hmm_insert(map, 20, 102));
        cmc_assert(hmm_insert(map, 10, 101));

        cmc_assert_equals(size_t, 100, hmm_get(map, 10));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_max(), {
        struct hashmultimap *map = hmm_new(50, 0.8, hmm_fkey, hmm_fval);

        size_t max_key;
        size_t max_val;
        cmc_assert(!hmm_max(map, &max_key, &max_val));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, map->flag);

        cmc_assert(hmm_insert(map, 1, 2));
        cmc_assert(hmm_max(map, &max_key, &max_val));
        cmc_assert_equals(size_t, 1, max_key);
        cmc_assert_equals(size_t, 2, max_val);

        cmc_assert(hmm_insert(map, 2, 3));
        cmc_assert(hmm_insert(map, 0, 1));
        cmc_assert(hmm_max(map, &max_key, &max_val));
        cmc_assert_equals(size_t, 2, max_key);
        cmc_assert_equals(size_t, 3, max_val);

        for (size_t i = 3; i <= 100; i++)
        {
            cmc_assert(hmm_insert(map, i, i));
        }

        cmc_assert(hmm_max(map, &max_key, &max_val));
        cmc_assert_equals(size_t, 100, max_key);
        cmc_assert_equals(size_t, 100, max_val);

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_min(), {
        struct hashmultimap *map = hmm_new(50, 0.8, hmm_fkey, hmm_fval);

        size_t min_key;
        size_t min_val;
        cmc_assert(!hmm_min(map, &min_key, &min_val));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, map->flag);

        cmc_assert(hmm_insert(map, 1, 2));
        cmc_assert(hmm_min(map, &min_key, &min_val));
        cmc_assert_equals(size_t, 1, min_key);
        cmc_assert_equals(size_t, 2, min_val);

        cmc_assert(hmm_insert(map, 2, 3));
        cmc_assert(hmm_insert(map, 0, 1));
        cmc_assert(hmm_min(map, &min_key, &min_val));
        cmc_assert_equals(size_t, 0, min_key);
        cmc_assert_equals(size_t, 1, min_val);

        for (size_t i = 3; i <= 100; i++)
        {
            cmc_assert(hmm_insert(map, i, i));
        }

        cmc_assert(hmm_min(map, &min_key, &min_val));
        cmc_assert_equals(size_t, 0, min_key);
        cmc_assert_equals(size_t, 1, min_val);

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_copy_of(), {
        struct hashmultimap *map = hmm_new(100, 0.8, hmm_fkey, hmm_fval);

        for (size_t i = 0; i < 1000; i++)
            hmm_insert(map, i, i);

        struct hashmultimap *map2 = hmm_copy_of(map);

        cmc_assert_equals(size_t, map->count, map2->count);
        cmc_assert(hmm_equals(map, map2));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_equals(), {
        struct hashmultimap *map1 = hmm_new(100, 0.8, hmm_fkey, hmm_fval);
        struct hashmultimap *map2 = hmm_new(100, 0.8, hmm_fkey, hmm_fval);

        cmc_assert(hmm_equals(map1, map2));

        cmc_assert(hmm_insert(map1, 1, 1));
        cmc_assert(hmm_insert(map2, 1, 1));
        cmc_assert(hmm_equals(map1, map2));

        for (size_t i = 2; i < 200; i++)
        {
            cmc_assert(hmm_insert(map1, i, i) && hmm_insert(map2, i, i));
        }

        cmc_assert(hmm_equals(map1, map2));

        hmm_free(map1);
        hmm_free(map2);
    });

    CMC_CREATE_TEST(key_count, {
        struct hashmultimap *map = hmm_new(50, 0.8, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 1000; i++)
            cmc_assert(hmm_insert(map, i % 20, i % 20));

        cmc_assert_equals(size_t, 1000, hmm_count(map));

        for (size_t i = 0; i < 20; i++)
            cmc_assert_equals(size_t, 50, hmm_key_count(map, i));

        hmm_free(map);
    });

    CMC_CREATE_TEST(flags, {
        struct hashmultimap *map = hmm_new(100, 0.8, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));

        // clear
        map->flag = CMC_FLAG_ERROR;
        hmm_clear(map);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));

        // customize
        map->flag = CMC_FLAG_ERROR;
        hmm_customize(map, NULL, NULL);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));

        // insert
        map->flag = CMC_FLAG_ERROR;
        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));

        // update
        cmc_assert(!hmm_update(map, 9, 10, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hmm_flag(map));

        cmc_assert(hmm_update(map, 1, 20, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));

        // update_all
        map->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, 1, hmm_update_all(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));

        map->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, 0, hmm_update_all(map, 2, 4, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hmm_flag(map));

        hmm_clear(map);
        cmc_assert_equals(size_t, 0, hmm_update_all(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hmm_flag(map));

        // remove
        map->flag = CMC_FLAG_ERROR;
        cmc_assert(!hmm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hmm_flag(map));

        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert(!hmm_remove(map, 2, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hmm_flag(map));

        cmc_assert(hmm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));

        // remove_all
        cmc_assert(!hmm_remove_all(map, 1, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hmm_flag(map));

        cmc_assert(hmm_insert(map, 1, 1) && hmm_insert(map, 1, 1));
        cmc_assert_equals(size_t, 0, hmm_remove_all(map, 2, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hmm_flag(map));

        cmc_assert_equals(size_t, 2, hmm_remove_all(map, 1, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));

        // max min
        cmc_assert(!hmm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hmm_flag(map));
        map->flag = CMC_FLAG_ERROR;
        cmc_assert(!hmm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hmm_flag(map));

        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert(hmm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));
        map->flag = CMC_FLAG_ERROR;
        cmc_assert(hmm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));

        // get get_ref
        hmm_get(map, 2);
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hmm_flag(map));
        map->flag = CMC_FLAG_ERROR;
        hmm_get_ref(map, 2);
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hmm_flag(map));

        hmm_get(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));
        map->flag = CMC_FLAG_ERROR;
        hmm_get_ref(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));

        hmm_clear(map);
        hmm_get(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hmm_flag(map));
        map->flag = CMC_FLAG_ERROR;
        hmm_get_ref(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hmm_flag(map));

        // contains
        map->flag = CMC_FLAG_ERROR;
        cmc_assert(!hmm_contains(map, 1000));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));

        // copy_of
        map->flag = CMC_FLAG_ERROR;
        struct hashmultimap *map2 = hmm_copy_of(map);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map2));

        // equals
        map->flag = CMC_FLAG_ERROR;
        map2->flag = CMC_FLAG_ERROR;
        cmc_assert(hmm_equals(map, map2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hmm_flag(map2));

        hmm_free(map);
        hmm_free(map2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct hashmultimap *map = hmm_new_custom(100, 0.8, hmm_fkey, hmm_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(hmm_update(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, 1, total_update);

        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert_equals(size_t, 2, hmm_update_all(map, 1, 3, NULL));
        cmc_assert_equals(int32_t, 2, total_update);

        cmc_assert(hmm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(hmm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, 3, total_create);

        cmc_assert_equals(size_t, 2, hmm_remove_all(map, 1, NULL));
        cmc_assert_equals(int32_t, 2, total_delete);

        cmc_assert(hmm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, 4, total_create);

        cmc_assert(hmm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert(hmm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert_equals(size_t, 2, hmm_get(map, 1));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert_not_equals(ptr, NULL, hmm_get_ref(map, 1));
        cmc_assert_equals(int32_t, 4, total_read);

        cmc_assert(hmm_contains(map, 1));
        cmc_assert_equals(int32_t, 5, total_read);

        cmc_assert(hmm_resize(map, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(hmm_resize(map, 100));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 4, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 2, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        hmm_customize(map, NULL, NULL);

        cmc_assert_equals(ptr, NULL, map->callbacks);

        hmm_clear(map);
        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert(hmm_update(map, 1, 2, NULL));
        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert_equals(size_t, 2, hmm_update_all(map, 1, 3, NULL));
        cmc_assert(hmm_remove(map, 1, NULL));
        cmc_assert(hmm_insert(map, 1, 2));
        cmc_assert_equals(size_t, 2, hmm_remove_all(map, 1, NULL));
        cmc_assert(hmm_insert(map, 1, 2));
        cmc_assert(hmm_max(map, NULL, NULL));
        cmc_assert(hmm_min(map, NULL, NULL));
        cmc_assert_equals(size_t, 2, hmm_get(map, 1));
        cmc_assert_not_equals(ptr, NULL, hmm_get_ref(map, 1));
        cmc_assert(hmm_contains(map, 1));
        cmc_assert(hmm_resize(map, 1000));
        cmc_assert(hmm_resize(map, 100));

        cmc_assert_equals(int32_t, 4, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 2, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, map->callbacks);

        hmm_free(map);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});

struct hashmultimap_fkey *hmm_fkey_numhash = &(struct hashmultimap_fkey){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = numhash, .pri = cmc_size_cmp
};

CMC_CREATE_UNIT(CMCHashMultiMapIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey_numhash, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_start(map);

        cmc_assert_equals(ptr, map, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(size_t, 0, it.first);
        cmc_assert_equals(size_t, 0, it.last);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(hmm_iter_at_start(&it));
        cmc_assert(hmm_iter_at_end(&it));

        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert(hmm_insert(map, 2, 2));
        cmc_assert(hmm_insert(map, 3, 3));

        it = hmm_iter_start(map);

        cmc_assert_equals(size_t, 0, it.index);

        cmc_assert_equals(size_t, 1, it.cursor);
        cmc_assert_equals(size_t, 1, it.first);
        cmc_assert_equals(size_t, 3, it.last);
        cmc_assert_equals(bool, false, it.end);

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey_numhash, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_end(map);

        cmc_assert_equals(ptr, map, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(size_t, 0, it.first);
        cmc_assert_equals(size_t, 0, it.last);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(hmm_iter_at_start(&it));
        cmc_assert(hmm_iter_at_end(&it));

        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert(hmm_insert(map, 2, 2));
        cmc_assert(hmm_insert(map, 3, 3));

        it = hmm_iter_end(map);

        cmc_assert_equals(size_t, map->count - 1, it.index);

        cmc_assert_equals(size_t, 3, it.cursor);
        cmc_assert_equals(size_t, 1, it.first);
        cmc_assert_equals(size_t, 3, it.last);
        cmc_assert_equals(bool, false, it.start);

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_start(map);

        // Empty checks
        cmc_assert(hmm_iter_at_start(&it));
        it = hmm_iter_end(map);
        cmc_assert(hmm_iter_at_start(&it));

        // Non-empty checks
        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert(hmm_insert(map, 2, 2));
        it = hmm_iter_end(map);
        cmc_assert(!hmm_iter_at_start(&it));
        it = hmm_iter_start(map);
        cmc_assert(hmm_iter_at_start(&it));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_start(map);

        // Empty check
        cmc_assert(hmm_iter_at_end(&it));
        it = hmm_iter_end(map);
        cmc_assert(hmm_iter_at_end(&it));

        // Non-empty checks
        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert(hmm_insert(map, 2, 2));
        it = hmm_iter_end(map);
        cmc_assert(hmm_iter_at_end(&it));
        it = hmm_iter_start(map);
        cmc_assert(!hmm_iter_at_end(&it));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey_numhash, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_start(map);

        cmc_assert(!hmm_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
            hmm_insert(map, i, i);

        cmc_assert_equals(size_t, 100, map->count);

        it = hmm_iter_end(map);

        cmc_assert(!hmm_iter_at_start(&it));
        cmc_assert(hmm_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, hmm_iter_value(&it));

        cmc_assert(hmm_iter_to_start(&it));

        cmc_assert(hmm_iter_at_start(&it));
        cmc_assert(!hmm_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, hmm_iter_value(&it));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey_numhash, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_end(map);

        cmc_assert(!hmm_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
            hmm_insert(map, i, i);

        it = hmm_iter_start(map);

        cmc_assert(hmm_iter_at_start(&it));
        cmc_assert(!hmm_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, hmm_iter_value(&it));

        cmc_assert(hmm_iter_to_end(&it));

        cmc_assert(!hmm_iter_at_start(&it));
        cmc_assert(hmm_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, hmm_iter_value(&it));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_start(map);

        cmc_assert(!hmm_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
            hmm_insert(map, i, i);

        size_t sum = 0;
        for (it = hmm_iter_start(map); !hmm_iter_at_end(&it); hmm_iter_next(&it))
        {
            sum += hmm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        hmm_iter_to_start(&it);
        do
        {
            sum += hmm_iter_value(&it);
        } while (hmm_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_end(map);

        cmc_assert(!hmm_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
            hmm_insert(map, i, i);

        size_t sum = 0;
        for (it = hmm_iter_end(map); !hmm_iter_at_start(&it); hmm_iter_prev(&it))
        {
            sum += hmm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        hmm_iter_to_end(&it);
        do
        {
            sum += hmm_iter_value(&it);
        } while (hmm_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey_numhash, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_start(map);

        cmc_assert(!hmm_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            hmm_insert(map, i, i);

        it = hmm_iter_start(map);

        cmc_assert(!hmm_iter_advance(&it, 0));
        cmc_assert(!hmm_iter_advance(&it, map->count));

        size_t sum = 0;
        for (it = hmm_iter_start(map);;)
        {
            sum += hmm_iter_value(&it);

            if (!hmm_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        hmm_iter_to_start(&it);
        cmc_assert(hmm_iter_advance(&it, map->count - 1));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey_numhash, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_end(map);

        cmc_assert(!hmm_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            hmm_insert(map, i, i);

        it = hmm_iter_end(map);

        cmc_assert(!hmm_iter_rewind(&it, 0));
        cmc_assert(!hmm_iter_rewind(&it, map->count));

        size_t sum = 0;
        for (it = hmm_iter_end(map);;)
        {
            sum += hmm_iter_value(&it);

            if (!hmm_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        hmm_iter_to_end(&it);
        cmc_assert(hmm_iter_rewind(&it, map->count - 1));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey_numhash, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_end(map);
        cmc_assert(!hmm_iter_go_to(&it, 0));

        it = hmm_iter_start(map);
        cmc_assert(!hmm_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            hmm_insert(map, i, i);

        it = hmm_iter_start(map);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            hmm_iter_go_to(&it, i);

            sum += hmm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(hmm_iter_go_to(&it, i - 1));

            sum += hmm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 0; i < 1001; i += 100)
        {
            cmc_assert(hmm_iter_go_to(&it, i));
            cmc_assert_equals(size_t, i, hmm_iter_index(&it));

            sum += hmm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_key(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_end(map);

        cmc_assert_equals(size_t, (size_t){ 0 }, hmm_iter_key(&it));

        cmc_assert(hmm_insert(map, 10, 10));

        it = hmm_iter_start(map);

        cmc_assert_equals(size_t, 10, hmm_iter_key(&it));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_end(map);

        cmc_assert_equals(size_t, (size_t){ 0 }, hmm_iter_value(&it));

        cmc_assert(hmm_insert(map, 10, 10));

        it = hmm_iter_start(map);

        cmc_assert_equals(size_t, 10, hmm_iter_value(&it));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_rvalue(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct hashmultimap_iter it = hmm_iter_end(map);

        cmc_assert_equals(ptr, NULL, hmm_iter_rvalue(&it));

        cmc_assert(hmm_insert(map, 10, 10));

        it = hmm_iter_start(map);

        cmc_assert_not_equals(ptr, NULL, hmm_iter_rvalue(&it));
        cmc_assert_equals(size_t, 10, *hmm_iter_rvalue(&it));

        hmm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct hashmultimap *map = hmm_new(100, 0.6, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i <= 1000; i++)
            hmm_insert(map, i, i);

        struct hashmultimap_iter it = hmm_iter_start(map);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, hmm_iter_index(&it));
            hmm_iter_next(&it);
        }

        it = hmm_iter_end(map);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, hmm_iter_index(&it));
            hmm_iter_prev(&it);
        }

        hmm_free(map);
    });
});

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = CMCHashMultiMap() + CMCHashMultiMapIter();

    printf(" +---------------------------------------------------------------+");
    printf("\n");
    printf(" | CMCHashMultiMap Suit : %-41s |\n", result == 0 ? "PASSED" : "FAILED");
    printf(" +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif

#endif /* CMC_TESTS_UNT_CMC_ASHMULTIMAP_H */
