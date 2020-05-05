#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "../src/hashmultimap.c"

struct hashmultimap_fkey *hmm_fkey =
    &(struct hashmultimap_fkey){ .cmp = cmc_size_cmp,
                                 .cpy = NULL,
                                 .str = cmc_size_str,
                                 .free = NULL,
                                 .hash = cmc_size_hash,
                                 .pri = cmc_size_cmp };

struct hashmultimap_fval *hmm_fval =
    &(struct hashmultimap_fval){ .cmp = cmc_size_cmp,
                                 .cpy = NULL,
                                 .str = cmc_size_str,
                                 .free = NULL,
                                 .hash = cmc_size_hash,
                                 .pri = cmc_size_cmp };

struct hashmultimap_fkey *hmm_fkey_counter =
    &(struct hashmultimap_fkey){ .cmp = k_c_cmp,
                                 .cpy = k_c_cpy,
                                 .str = k_c_str,
                                 .free = k_c_free,
                                 .hash = k_c_hash,
                                 .pri = k_c_pri };

struct hashmultimap_fval *hmm_fval_counter =
    &(struct hashmultimap_fval){ .cmp = v_c_cmp,
                                 .cpy = v_c_cpy,
                                 .str = v_c_str,
                                 .free = v_c_free,
                                 .hash = v_c_hash,
                                 .pri = v_c_pri };

struct cmc_alloc_node *hmm_alloc_node = &(struct cmc_alloc_node){
    .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free
};

CMC_CREATE_UNIT(HashMultiMap, true, {
    CMC_CREATE_TEST(PFX##_new(), {
        struct hashmultimap *map = hmm_new(943722, 0.6, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
        cmc_assert_equals(ptr, hmm_fkey, map->f_key);
        cmc_assert_equals(ptr, hmm_fval, map->f_val);
        cmc_assert_equals(ptr, &cmc_alloc_node_default, map->alloc);
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
        struct hashmultimap *map = hmm_new_custom(
            943722, 0.6, hmm_fkey, hmm_fval, hmm_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
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
        struct hashmultimap *map =
            hmm_new(100, 0.6, hmm_fkey_counter, hmm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            hmm_insert(map, i, i);

        cmc_assert_equals(size_t, 1000, map->count);

        map->flag = cmc_flags.ERROR;
        hmm_clear(map);

        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        hmm_free(map);
        k_total_free = 0;
        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_free(), {
        k_total_free = 0;
        v_total_free = 0;
        struct hashmultimap *map =
            hmm_new(100, 0.6, hmm_fkey_counter, hmm_fval_counter);

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

    CMC_CREATE_TEST(remove[count], {
        struct hashmultimap *map = hmm_new(100, 0.8, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 200; i++)
            cmc_assert(hmm_insert(map, i % 20, i % 20));

        size_t r;

        for (size_t i = 0; i < 60; i++)
            cmc_assert(hmm_remove(map, i % 20, &r));

        cmc_assert_equals(size_t, 140, hmm_count(map));

        hmm_free(map);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        struct hashmultimap *map = hmm_new(100, 0.8, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        size_t r;

        cmc_assert(!hmm_remove(map, 10, &r));

        hmm_free(map);
    });

    CMC_CREATE_TEST(remove[count = 1], {
        struct hashmultimap *map = hmm_new(100, 0.8, hmm_fkey, hmm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        size_t r;

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
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));

        // clear
        map->flag = cmc_flags.ERROR;
        hmm_clear(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));

        // customize
        map->flag = cmc_flags.ERROR;
        hmm_customize(map, NULL, NULL);
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));

        // insert
        map->flag = cmc_flags.ERROR;
        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));

        // update
        cmc_assert(!hmm_update(map, 9, 10, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hmm_flag(map));

        cmc_assert(hmm_update(map, 1, 20, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));

        // update_all
        map->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 1, hmm_update_all(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));

        map->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 0, hmm_update_all(map, 2, 4, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hmm_flag(map));

        hmm_clear(map);
        cmc_assert_equals(size_t, 0, hmm_update_all(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hmm_flag(map));

        // remove
        map->flag = cmc_flags.ERROR;
        cmc_assert(!hmm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hmm_flag(map));

        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert(!hmm_remove(map, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hmm_flag(map));

        cmc_assert(hmm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));

        // remove_all
        cmc_assert(!hmm_remove_all(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hmm_flag(map));

        cmc_assert(hmm_insert(map, 1, 1) && hmm_insert(map, 1, 1));
        cmc_assert_equals(size_t, 0, hmm_remove_all(map, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hmm_flag(map));

        cmc_assert_equals(size_t, 2, hmm_remove_all(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));

        // max min
        cmc_assert(!hmm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hmm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(!hmm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hmm_flag(map));

        cmc_assert(hmm_insert(map, 1, 1));
        cmc_assert(hmm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(hmm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));

        // get get_ref
        hmm_get(map, 2);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hmm_flag(map));
        map->flag = cmc_flags.ERROR;
        hmm_get_ref(map, 2);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hmm_flag(map));

        hmm_get(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));
        map->flag = cmc_flags.ERROR;
        hmm_get_ref(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));

        hmm_clear(map);
        hmm_get(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hmm_flag(map));
        map->flag = cmc_flags.ERROR;
        hmm_get_ref(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hmm_flag(map));

        // contains
        map->flag = cmc_flags.ERROR;
        cmc_assert(!hmm_contains(map, 1000));
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));

        // copy_of
        map->flag = cmc_flags.ERROR;
        struct hashmultimap *map2 = hmm_copy_of(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map2));

        // equals
        map->flag = cmc_flags.ERROR;
        map2->flag = cmc_flags.ERROR;
        cmc_assert(hmm_equals(map, map2));
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, hmm_flag(map2));

        hmm_free(map);
        hmm_free(map2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct hashmultimap *map =
            hmm_new_custom(100, 0.8, hmm_fkey, hmm_fval, NULL, callbacks);

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
