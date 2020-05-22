#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "../src/hashbidimap.c"

struct hashbidimap_fkey *hbm_fkey =
    &(struct hashbidimap_fkey){ .cmp = cmc_size_cmp,
                                .cpy = NULL,
                                .str = cmc_size_str,
                                .free = NULL,
                                .hash = cmc_size_hash,
                                .pri = cmc_size_cmp };

struct hashbidimap_fval *hbm_fval =
    &(struct hashbidimap_fval){ .cmp = cmc_size_cmp,
                                .cpy = NULL,
                                .str = cmc_size_str,
                                .free = NULL,
                                .hash = cmc_size_hash,
                                .pri = cmc_size_cmp };

struct hashbidimap_fkey *hbm_fkey_counter =
    &(struct hashbidimap_fkey){ .cmp = k_c_cmp,
                                .cpy = k_c_cpy,
                                .str = k_c_str,
                                .free = k_c_free,
                                .hash = k_c_hash,
                                .pri = k_c_pri };

struct hashbidimap_fval *hbm_fval_counter =
    &(struct hashbidimap_fval){ .cmp = v_c_cmp,
                                .cpy = v_c_cpy,
                                .str = v_c_str,
                                .free = v_c_free,
                                .hash = v_c_hash,
                                .pri = v_c_pri };

struct cmc_alloc_node *hbm_alloc_node = &(struct cmc_alloc_node){
    .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free
};

CMC_CREATE_UNIT(HashBidiMap, true, {
    CMC_CREATE_TEST(PFX##_new(), {
        struct hashbidimap *map = hbm_new(943722, 0.6, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
        cmc_assert_equals(ptr, hbm_fkey, map->f_key);
        cmc_assert_equals(ptr, hbm_fval, map->f_val);
        cmc_assert_equals(ptr, &cmc_alloc_node_default, map->alloc);
        cmc_assert_equals(ptr, NULL, map->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hbm_capacity(map));

        hbm_free(map);

        map = hbm_new(100, 0.6, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < map->capacity; i++)
        {
            cmc_assert_equals(ptr, NULL, map->buffer[i][0]);
            cmc_assert_equals(ptr, NULL, map->buffer[i][1]);
        }

        hbm_free(map);

        map = hbm_new(0, 0.6, hbm_fkey, hbm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = hbm_new(UINT64_MAX, 0.99, hbm_fkey, hbm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = hbm_new(1000, 0.6, hbm_fkey, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hbm_new(1000, 0.6, NULL, hbm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = hbm_new(1000, 0.6, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(PFX##_new_custom(), {
        struct hashbidimap *map = hbm_new_custom(
            943722, 0.6, hbm_fkey, hbm_fval, hbm_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
        cmc_assert_equals(ptr, hbm_fkey, map->f_key);
        cmc_assert_equals(ptr, hbm_fval, map->f_val);
        cmc_assert_equals(ptr, hbm_alloc_node, map->alloc);
        cmc_assert_equals(ptr, callbacks, map->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hbm_capacity(map));

        hbm_free(map);

        map = hbm_new_custom(0, 0.6, hbm_fkey, hbm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hbm_new_custom(UINT64_MAX, 0.99, hbm_fkey, hbm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hbm_new_custom(1000, 0.6, hbm_fkey, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hbm_new_custom(1000, 0.6, NULL, hbm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = hbm_new_custom(1000, 0.6, NULL, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(PFX##_clear(), {
        k_total_free = 0;
        v_total_free = 0;
        struct hashbidimap *map =
            hbm_new(100, 0.6, hbm_fkey_counter, hbm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            hbm_insert(map, i, i);

        cmc_assert_equals(size_t, 1000, map->count);

        map->flag = cmc_flags.ERROR;
        hbm_clear(map);

        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        hbm_free(map);
        k_total_free = 0;
        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_free(), {
        k_total_free = 0;
        v_total_free = 0;
        struct hashbidimap *map =
            hbm_new(100, 0.6, hbm_fkey_counter, hbm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            hbm_insert(map, i, i);

        cmc_assert_equals(size_t, 1000, map->count);

        hbm_free(map);

        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        map = hbm_new(1000, 0.6, hbm_fkey_counter, hbm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);

        hbm_free(map);

        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);
        k_total_free = 0;
        v_total_free = 0;
    });

    CMC_CREATE_TEST(customize, {
        struct hashbidimap *map =
            hbm_new_custom(100, 0.6, hbm_fkey, hbm_fval, NULL, NULL);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(ptr, &cmc_alloc_node_default, map->alloc);
        cmc_assert_equals(ptr, NULL, map->callbacks);

        hbm_free(map);

        struct cmc_alloc_node node;
        node.malloc = malloc;
        node.realloc = realloc;
        node.free = free;
        node.calloc = calloc;

        map = hbm_new_custom(100, 0.6, hbm_fkey, hbm_fval, &node, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(ptr, &node, map->alloc);
        cmc_assert_equals(ptr, callbacks, map->callbacks);

        hbm_free(map);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct hashbidimap *map = hbm_new(1, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 10000; i++)
            cmc_assert(hbm_insert(map, i, i));

        size_t sum = 0;

        for (size_t i = 0; i < hbm_capacity(map); i++)
        {
            if (map->buffer[i][0] && map->buffer[i][0] != CMC_ENTRY_DELETED)
            {
                sum += map->buffer[i][0]->key;
            }
        }

        cmc_assert_equals(size_t, 50005000, sum);

        hbm_free(map);
    });

    CMC_CREATE_TEST(insert, {
        struct hashbidimap *map = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hbm_insert(map, 1, 1));
        cmc_assert(hbm_insert(map, 2, 2));
        cmc_assert(hbm_insert(map, 3, 3));

        cmc_assert_equals(size_t, 3, hbm_count(map));

        hbm_free(map);
    });

    CMC_CREATE_TEST(insert[duplicate], {
        struct hashbidimap *map = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hbm_insert(map, 1, 1));
        cmc_assert(!hbm_insert(map, 1, 1));
        cmc_assert(!hbm_insert(map, 2, 1));
        cmc_assert(!hbm_insert(map, 1, 2));
        cmc_assert(hbm_insert(map, 2, 2));

        cmc_assert_equals(size_t, 2, hbm_count(map));

        hbm_free(map);
    });

    CMC_CREATE_TEST(insert[references], {
        struct hashbidimap *map = hbm_new(500, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 500; i++)
            cmc_assert(hbm_insert(map, i, i));

        for (size_t i = 0; i < map->capacity; i++)
        {
            struct hashbidimap_entry *kentry = map->buffer[i][0];
            struct hashbidimap_entry *ventry = map->buffer[i][1];

            if (kentry && kentry != CMC_ENTRY_DELETED)
                cmc_assert_equals(ptr, kentry, *(kentry->ref[0]));
            if (ventry && ventry != CMC_ENTRY_DELETED)
                cmc_assert_equals(ptr, ventry, *(ventry->ref[1]));
        }

        hbm_free(map);
    });

    CMC_CREATE_TEST(insert[growth remove clear], {
        struct hashbidimap *map = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            cmc_assert(hbm_insert(map, i, i));

        cmc_assert_equals(size_t, 1000, hbm_count(map));

        size_t j = 1;
        while (!hbm_empty(map))
        {
            if (hbm_count(map) % 2 == 0)
                cmc_assert(hbm_remove_by_key(map, j++, NULL, NULL));
            else
                cmc_assert(hbm_remove_by_val(map, j++, NULL, NULL));
        }

        cmc_assert_equals(size_t, 0, hbm_count(map));

        for (size_t i = 1; i <= 1000; i++)
            cmc_assert(hbm_insert(map, i, i));

        cmc_assert_equals(size_t, 1000, hbm_count(map));

        hbm_clear(map);

        cmc_assert_equals(size_t, 0, hbm_count(map));

        hbm_free(map);
    });

    CMC_CREATE_TEST(insert[ftab hash calls], {
        struct hashbidimap *map =
            hbm_new(100, 0.7, hbm_fkey_counter, hbm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        k_total_hash = 0;
        v_total_hash = 0;

        for (size_t i = 1; i <= 1000; i++)
            cmc_assert(hbm_insert(map, i, i));

        cmc_assert_greater_equals(int32_t, 1000, k_total_hash);
        cmc_assert_greater_equals(int32_t, 1000, v_total_hash);

        hbm_free(map);

        k_total_hash = 0;
        v_total_hash = 0;
    });

    CMC_CREATE_TEST(update_key, {
        struct hashbidimap *map = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hbm_insert(map, i, i));

        cmc_assert_equals(size_t, 100, hbm_count(map));

        for (size_t i = 100; i > 0; i--)
            cmc_assert(hbm_update_key(map, i, i + 10));

        cmc_assert_equals(size_t, 100, hbm_count(map));

        for (size_t i = 1; i <= 100; i++)
            cmc_assert_equals(size_t, i + 10, hbm_get_key(map, i));

        cmc_assert_equals(size_t, 100, hbm_count(map));

        hbm_free(map);
    });

    CMC_CREATE_TEST(update_key[empty], {
        struct hashbidimap *map = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, 0, hbm_count(map));

        cmc_assert(!hbm_update_key(map, 1, 1));

        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hbm_flag(map));

        hbm_free(map);
    });

    CMC_CREATE_TEST(update_key[not_found duplicate], {
        struct hashbidimap *map = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hbm_insert(map, 1, 1));

        cmc_assert_equals(size_t, 1, hbm_count(map));

        map->flag = cmc_flags.ERROR;
        cmc_assert(hbm_update_key(map, 1, 1));

        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        cmc_assert(!hbm_update_key(map, 2, 2));

        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hbm_flag(map));

        cmc_assert(hbm_insert(map, 2, 2));

        cmc_assert(!hbm_update_key(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hbm_flag(map));
        cmc_assert(!hbm_update_key(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hbm_flag(map));

        cmc_assert(hbm_update_key(map, 2, 2));

        hbm_free(map);
    });

    CMC_CREATE_TEST(update_val, {
        struct hashbidimap *map = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(hbm_insert(map, i, i));

        cmc_assert_equals(size_t, 100, hbm_count(map));

        for (size_t i = 100; i > 0; i--)
            cmc_assert(hbm_update_val(map, i, i + 10));

        cmc_assert_equals(size_t, 100, hbm_count(map));

        for (size_t i = 1; i <= 100; i++)
            cmc_assert_equals(size_t, i + 10, hbm_get_val(map, i));

        cmc_assert_equals(size_t, 100, hbm_count(map));

        hbm_free(map);
    });

    CMC_CREATE_TEST(update_val[empty], {
        struct hashbidimap *map = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, 0, hbm_count(map));

        cmc_assert(!hbm_update_val(map, 1, 1));

        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hbm_flag(map));

        hbm_free(map);
    });

    CMC_CREATE_TEST(update_val[not_found duplicate], {
        struct hashbidimap *map = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(hbm_insert(map, 1, 1));

        cmc_assert_equals(size_t, 1, hbm_count(map));

        map->flag = cmc_flags.ERROR;
        cmc_assert(hbm_update_val(map, 1, 1));

        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        cmc_assert(!hbm_update_val(map, 2, 2));

        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hbm_flag(map));

        cmc_assert(hbm_insert(map, 2, 2));

        cmc_assert(!hbm_update_val(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hbm_flag(map));
        cmc_assert(!hbm_update_val(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hbm_flag(map));

        cmc_assert(hbm_update_val(map, 2, 2));

        hbm_free(map);
    });

    CMC_CREATE_TEST(remove_by_key[cleanup custom], {
        struct hashbidimap *map =
            hbm_new_custom(10000, 0.6, hbm_fkey, hbm_fval,
                           &(struct cmc_alloc_node){ .malloc = malloc,
                                                     .calloc = calloc,
                                                     .realloc = realloc,
                                                     .free = free },
                           &(struct cmc_callbacks){ 0 });

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 10000; i++)
        {
            cmc_assert(hbm_insert(map, i, i));
        }

        for (size_t i = 0; i < 10000; i++)
        {
            cmc_assert(hbm_remove_by_key(map, i, NULL, NULL));
        }

        for (size_t i = 0; i < map->capacity; i++)
        {
            struct hashbidimap_entry *k = map->buffer[i][0];
            struct hashbidimap_entry *v = map->buffer[i][1];

            cmc_assert((k == NULL || k == CMC_ENTRY_DELETED) &&
                       (v == NULL || v == CMC_ENTRY_DELETED));
        }

        hbm_free(map);
    });

    CMC_CREATE_TEST(remove_by_val[cleanup custom], {
        struct hashbidimap *map =
            hbm_new_custom(10000, 0.6, hbm_fkey, hbm_fval,
                           &(struct cmc_alloc_node){ .malloc = malloc,
                                                     .calloc = calloc,
                                                     .realloc = realloc,
                                                     .free = free },
                           &(struct cmc_callbacks){ 0 });

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 10000; i++)
        {
            cmc_assert(hbm_insert(map, i, i));
        }

        for (size_t i = 0; i < 10000; i++)
        {
            cmc_assert(hbm_remove_by_val(map, i, NULL, NULL));
        }

        for (size_t i = 0; i < map->capacity; i++)
        {
            struct hashbidimap_entry *k = map->buffer[i][0];
            struct hashbidimap_entry *v = map->buffer[i][1];

            cmc_assert((k == NULL || k == CMC_ENTRY_DELETED) &&
                       (v == NULL || v == CMC_ENTRY_DELETED));
        }

        hbm_free(map);
    });

    CMC_CREATE_TEST(copy_of, {
        struct hashbidimap *map1 = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hbm_insert(map1, i, i));

        struct hashbidimap *map2 = hbm_copy_of(map1);

        cmc_assert_not_equals(ptr, NULL, map2);
        cmc_assert_equals(size_t, hbm_count(map1), hbm_count(map2));

        for (size_t i = 0; i < 100; i++)
        {
            cmc_assert(hbm_contains_key(map2, i) && hbm_contains_key(map1, i));
            cmc_assert(hbm_contains_val(map2, i) && hbm_contains_val(map1, i));
        }

        hbm_free(map1);
        hbm_free(map2);
    });

    CMC_CREATE_TEST(equals, {
        struct hashbidimap *map1 = hbm_new(100, 0.7, hbm_fkey, hbm_fval);
        struct hashbidimap *map2 = hbm_new(1000, 0.9, hbm_fkey, hbm_fval);

        for (size_t i = 0; i < 100; i++)
        {
            cmc_assert(hbm_insert(map1, i, i));
            cmc_assert(hbm_insert(map2, 99 - i, 99 - i));
        }

        cmc_assert_not_equals(ptr, NULL, map1);
        cmc_assert_not_equals(ptr, NULL, map2);
        cmc_assert_equals(size_t, hbm_count(map1), hbm_count(map2));

        for (size_t i = 0; i < 100; i++)
        {
            cmc_assert(hbm_contains_key(map2, i) && hbm_contains_key(map1, i));
            cmc_assert(hbm_contains_val(map2, i) && hbm_contains_val(map1, i));
        }

        hbm_free(map1);
        hbm_free(map2);
    });

    CMC_CREATE_TEST(equals[from copy], {
        struct hashbidimap *map1 = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(hbm_insert(map1, i, i));

        struct hashbidimap *map2 = hbm_copy_of(map1);

        cmc_assert_not_equals(ptr, NULL, map2);

        cmc_assert(hbm_equals(map1, map2));

        hbm_free(map1);
        hbm_free(map2);
    });

    CMC_CREATE_TEST(flags, {
        struct hashbidimap *map = hbm_new(100, 0.7, hbm_fkey, hbm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        // customize
        hbm_customize(map, &cmc_alloc_node_default,
                      &(struct cmc_callbacks){ 0 });
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        // Insert
        cmc_assert(hbm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        cmc_assert(!hbm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hbm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(!hbm_insert(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hbm_flag(map));

        // clear
        hbm_clear(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        // hbm_update_key
        cmc_assert(!hbm_update_key(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hbm_flag(map));

        cmc_assert(hbm_insert(map, 1, 1));
        cmc_assert(!hbm_update_key(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hbm_flag(map));

        cmc_assert(hbm_insert(map, 2, 2));
        cmc_assert(!hbm_update_key(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hbm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(!hbm_update_key(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hbm_flag(map));

        // clear
        hbm_clear(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        // hbm_update_val
        cmc_assert(!hbm_update_val(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hbm_flag(map));

        cmc_assert(hbm_insert(map, 1, 1));
        cmc_assert(!hbm_update_val(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hbm_flag(map));

        cmc_assert(hbm_insert(map, 2, 2));
        cmc_assert(!hbm_update_val(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hbm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(!hbm_update_val(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, hbm_flag(map));

        // remove_by_key
        cmc_assert(hbm_remove_by_key(map, 1, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        cmc_assert(!hbm_remove_by_key(map, 1, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hbm_flag(map));

        cmc_assert(hbm_remove_by_key(map, 2, NULL, NULL));
        cmc_assert(!hbm_remove_by_key(map, 2, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hbm_flag(map));

        // remove_by_val
        cmc_assert(hbm_insert(map, 1, 1) && hbm_insert(map, 2, 2));

        cmc_assert(hbm_remove_by_val(map, 1, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        cmc_assert(!hbm_remove_by_val(map, 1, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hbm_flag(map));

        cmc_assert(hbm_remove_by_val(map, 2, NULL, NULL));
        cmc_assert(!hbm_remove_by_val(map, 2, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, hbm_flag(map));

        // get_key and get_val
        cmc_assert(hbm_insert(map, 2, 1));

        cmc_assert_equals(size_t, 2, hbm_get_key(map, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 1, hbm_get_val(map, 2));
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        cmc_assert_equals(size_t, 0, hbm_get_key(map, 2));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hbm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 0, hbm_get_val(map, 1));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hbm_flag(map));

        // contains_key
        cmc_assert(hbm_contains_key(map, 2));
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        // contains_val
        map->flag = cmc_flags.ERROR;
        cmc_assert(hbm_contains_val(map, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));

        // copy_of
        map->flag = cmc_flags.ERROR;
        struct hashbidimap *map2 = hbm_copy_of(map);

        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map2));

        size_t tmp = map->capacity;
        map->capacity = 0;

        struct hashbidimap *map3 = hbm_copy_of(map);
        cmc_assert_equals(ptr, NULL, map3);
        cmc_assert_equals(int32_t, cmc_flags.ERROR, hbm_flag(map));

        map->capacity = tmp;

        // equals
        hbm_get_key(map, 100);
        hbm_get_key(map2, 100);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hbm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, hbm_flag(map2));
        map->flag = cmc_flags.ERROR;
        cmc_assert(hbm_equals(map, map2));
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, hbm_flag(map2));

        hbm_free(map);
        hbm_free(map2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct hashbidimap *map =
            hbm_new_custom(100, 0.7, hbm_fkey, hbm_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_equals(ptr, callbacks, map->callbacks);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(hbm_insert(map, 10, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(hbm_update_key(map, 10, 5));
        cmc_assert_equals(int32_t, 1, total_update);

        cmc_assert(hbm_update_val(map, 5, 5));
        cmc_assert_equals(int32_t, 2, total_update);

        cmc_assert(hbm_insert(map, 10, 10));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(hbm_remove_by_key(map, 5, NULL, NULL));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(hbm_remove_by_val(map, 10, NULL, NULL));
        cmc_assert_equals(int32_t, 2, total_delete);

        cmc_assert(hbm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, 3, total_create);

        cmc_assert_equals(size_t, 1, hbm_get_key(map, 2));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert_equals(size_t, 2, hbm_get_val(map, 1));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert(hbm_resize(map, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(hbm_resize(map, 200));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert(hbm_contains_key(map, 1));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert(hbm_contains_val(map, 2));
        cmc_assert_equals(int32_t, 4, total_read);

        cmc_assert_equals(int32_t, 3, total_create);
        cmc_assert_equals(int32_t, 4, total_read);
        cmc_assert_equals(int32_t, 2, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        hbm_customize(map, NULL, NULL);

        cmc_assert_equals(ptr, NULL, map->callbacks);

        hbm_clear(map);
        cmc_assert(hbm_insert(map, 10, 10));
        cmc_assert(hbm_update_key(map, 10, 5));
        cmc_assert(hbm_update_val(map, 5, 5));
        cmc_assert(hbm_insert(map, 10, 10));
        cmc_assert(hbm_remove_by_key(map, 5, NULL, NULL));
        cmc_assert(hbm_remove_by_val(map, 10, NULL, NULL));
        cmc_assert(hbm_insert(map, 1, 2));
        cmc_assert_equals(size_t, 1, hbm_get_key(map, 2));
        cmc_assert_equals(size_t, 2, hbm_get_val(map, 1));
        cmc_assert(hbm_resize(map, 1000));
        cmc_assert(hbm_resize(map, 200));
        cmc_assert(hbm_contains_key(map, 1));
        cmc_assert(hbm_contains_val(map, 2));

        cmc_assert_equals(int32_t, 3, total_create);
        cmc_assert_equals(int32_t, 4, total_read);
        cmc_assert_equals(int32_t, 2, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, map->callbacks);

        hbm_free(map);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});

CMC_CREATE_UNIT(HashBidiMapIter, true,
                {

                });

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = HashBidiMap() + HashBidiMapIter();

    printf(
        " +---------------------------------------------------------------+");
    printf("\n");
    printf(" | HashBidiMap Suit : %-42s |\n",
           result == 0 ? "PASSED" : "FAILED");
    printf(
        " +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif
