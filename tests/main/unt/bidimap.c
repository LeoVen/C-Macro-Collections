#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/bidimap.h>

CMC_GENERATE_BIDIMAP(bm, bidimap, size_t, size_t)

struct bidimap_ftab_key *bm_ftab_key =
    &(struct bidimap_ftab_key){ .cmp = cmp,
                                .cpy = copy,
                                .str = str,
                                .free = custom_free,
                                .hash = hash,
                                .pri = pri };

struct bidimap_ftab_val *bm_ftab_val =
    &(struct bidimap_ftab_val){ .cmp = cmp,
                                .cpy = copy,
                                .str = str,
                                .free = custom_free,
                                .hash = hash,
                                .pri = pri };

struct bidimap_ftab_key *bm_ftab_key_counter =
    &(struct bidimap_ftab_key){ .cmp = k_c_cmp,
                                .cpy = k_c_cpy,
                                .str = k_c_str,
                                .free = k_c_free,
                                .hash = k_c_hash,
                                .pri = k_c_pri };

struct bidimap_ftab_val *bm_ftab_val_counter =
    &(struct bidimap_ftab_val){ .cmp = v_c_cmp,
                                .cpy = v_c_cpy,
                                .str = v_c_str,
                                .free = v_c_free,
                                .hash = v_c_hash,
                                .pri = v_c_pri };

CMC_CREATE_UNIT(bidimap_test, true, {
    CMC_CREATE_TEST(new, {
        struct bidimap *map = bm_new(100, 0.6, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);

        bm_free(map);
    });

    CMC_CREATE_TEST(new[null buffer], {
        struct bidimap *map = bm_new(100, 0.6, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < map->capacity; i++)
        {
            cmc_assert_equals(ptr, NULL, map->buffer[i][0]);
            cmc_assert_equals(ptr, NULL, map->buffer[i][1]);
        }

        bm_free(map);
    });

    CMC_CREATE_TEST(new_custom, {
        struct bidimap *map =
            bm_new_custom(100, 0.6, bm_ftab_key, bm_ftab_val,
                          &(struct cmc_alloc_node){ .malloc = malloc,
                                                    .calloc = calloc,
                                                    .realloc = realloc,
                                                    .free = free },
                          &(struct cmc_callbacks){ 0 });

        cmc_assert_not_equals(ptr, NULL, map);

        bm_free(map);
    });

    CMC_CREATE_TEST(new_custom[insert remove], {
        struct bidimap *map =
            bm_new_custom(500, 0.6, bm_ftab_key, bm_ftab_val,
                          &(struct cmc_alloc_node){ .malloc = malloc,
                                                    .calloc = calloc,
                                                    .realloc = realloc,
                                                    .free = free },
                          &(struct cmc_callbacks){ 0 });

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 500; i++)
            cmc_assert(bm_insert(map, i, i));

        cmc_assert_equals(size_t, 500, bm_count(map));

        for (size_t i = 0; i < 500; i++)
        {
            if (i % 2 == 0)
            {
                cmc_assert(bm_remove_by_key(map, i, NULL, NULL));
            }
            else
            {
                cmc_assert(bm_remove_by_val(map, i, NULL, NULL));
            }
        }

        cmc_assert_equals(size_t, 0, bm_count(map));

        bm_free(map);
    });

    CMC_CREATE_TEST(clear[ftab free calls], {
        struct bidimap *map =
            bm_new(1000, 0.6, bm_ftab_key_counter, bm_ftab_val_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        k_total_free = 0;
        v_total_free = 0;

        bm_clear(map);

        cmc_assert_equals(int32_t, 0, k_total_free);
        cmc_assert_equals(int32_t, 0, v_total_free);

        for (size_t i = 0; i < 1000; i++)
            cmc_assert(bm_insert(map, i, i));

        bm_clear(map);

        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        bm_ftab_val_counter->free = NULL;

        for (size_t i = 0; i < 1000; i++)
            cmc_assert(bm_insert(map, i, i));

        bm_clear(map);

        cmc_assert_equals(int32_t, 2000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        bm_ftab_val_counter->free = v_c_free;
        bm_ftab_key_counter->free = NULL;

        for (size_t i = 0; i < 1000; i++)
            cmc_assert(bm_insert(map, i, i));

        bm_clear(map);

        cmc_assert_equals(int32_t, 2000, k_total_free);
        cmc_assert_equals(int32_t, 2000, v_total_free);

        bm_ftab_key_counter->free = k_c_free;

        bm_free(map);

        k_total_free = 0;
        v_total_free = 0;
    });

    CMC_CREATE_TEST(clear[count], {
        struct bidimap *map = bm_new(100, 0.6, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 500; i++)
            cmc_assert(bm_insert(map, i, i));

        cmc_assert_equals(size_t, 500, bm_count(map));

        bm_clear(map);

        cmc_assert_equals(size_t, 0, bm_count(map));

        bm_free(map);
    });

    CMC_CREATE_TEST(customize, {
        struct bidimap *map =
            bm_new_custom(100, 0.6, bm_ftab_key, bm_ftab_val, NULL, NULL);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(ptr, &cmc_alloc_node_default, map->alloc);
        cmc_assert_equals(ptr, NULL, map->callbacks);

        bm_free(map);

        struct cmc_alloc_node node;
        node.malloc = malloc;
        node.realloc = realloc;
        node.free = free;
        node.calloc = calloc;

        map =
            bm_new_custom(100, 0.6, bm_ftab_key, bm_ftab_val, &node, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(ptr, &node, map->alloc);
        cmc_assert_equals(ptr, callbacks, map->callbacks);

        bm_free(map);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct bidimap *map = bm_new(1, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 10000; i++)
            cmc_assert(bm_insert(map, i, i));

        size_t sum = 0;

        for (size_t i = 0; i < bm_capacity(map); i++)
        {
            if (map->buffer[i][0] && map->buffer[i][0] != CMC_ENTRY_DELETED)
            {
                sum += map->buffer[i][0]->key;
            }
        }

        cmc_assert_equals(size_t, 50005000, sum);

        bm_free(map);
    });

    CMC_CREATE_TEST(insert, {
        struct bidimap *map = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(bm_insert(map, 1, 1));
        cmc_assert(bm_insert(map, 2, 2));
        cmc_assert(bm_insert(map, 3, 3));

        cmc_assert_equals(size_t, 3, bm_count(map));

        bm_free(map);
    });

    CMC_CREATE_TEST(insert[duplicate], {
        struct bidimap *map = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(bm_insert(map, 1, 1));
        cmc_assert(!bm_insert(map, 1, 1));
        cmc_assert(!bm_insert(map, 2, 1));
        cmc_assert(!bm_insert(map, 1, 2));
        cmc_assert(bm_insert(map, 2, 2));

        cmc_assert_equals(size_t, 2, bm_count(map));

        bm_free(map);
    });

    CMC_CREATE_TEST(insert[references], {
        struct bidimap *map = bm_new(500, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 500; i++)
            cmc_assert(bm_insert(map, i, i));

        for (size_t i = 0; i < map->capacity; i++)
        {
            struct bidimap_entry *kentry = map->buffer[i][0];
            struct bidimap_entry *ventry = map->buffer[i][1];

            if (kentry && kentry != CMC_ENTRY_DELETED)
                cmc_assert_equals(ptr, kentry, *(kentry->ref[0]));
            if (ventry && ventry != CMC_ENTRY_DELETED)
                cmc_assert_equals(ptr, ventry, *(ventry->ref[1]));
        }

        bm_free(map);
    });

    CMC_CREATE_TEST(insert[growth remove clear], {
        struct bidimap *map = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            cmc_assert(bm_insert(map, i, i));

        cmc_assert_equals(size_t, 1000, bm_count(map));

        size_t j = 1;
        while (!bm_empty(map))
        {
            if (bm_count(map) % 2 == 0)
                cmc_assert(bm_remove_by_key(map, j++, NULL, NULL));
            else
                cmc_assert(bm_remove_by_val(map, j++, NULL, NULL));
        }

        cmc_assert_equals(size_t, 0, bm_count(map));

        for (size_t i = 1; i <= 1000; i++)
            cmc_assert(bm_insert(map, i, i));

        cmc_assert_equals(size_t, 1000, bm_count(map));

        bm_clear(map);

        cmc_assert_equals(size_t, 0, bm_count(map));

        bm_free(map);
    });

    CMC_CREATE_TEST(insert[ftab hash calls], {
        struct bidimap *map =
            bm_new(100, 0.7, bm_ftab_key_counter, bm_ftab_val_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        k_total_hash = 0;
        v_total_hash = 0;

        for (size_t i = 1; i <= 1000; i++)
            cmc_assert(bm_insert(map, i, i));

        cmc_assert_greater_equals(int32_t, 1000, k_total_hash);
        cmc_assert_greater_equals(int32_t, 1000, v_total_hash);

        bm_free(map);

        k_total_hash = 0;
        v_total_hash = 0;
    });

    CMC_CREATE_TEST(update_key, {
        struct bidimap *map = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(bm_insert(map, i, i));

        cmc_assert_equals(size_t, 100, bm_count(map));

        for (size_t i = 100; i > 0; i--)
            cmc_assert(bm_update_key(map, i, i + 10));

        cmc_assert_equals(size_t, 100, bm_count(map));

        for (size_t i = 1; i <= 100; i++)
            cmc_assert_equals(size_t, i + 10, bm_get_key(map, i));

        cmc_assert_equals(size_t, 100, bm_count(map));

        bm_free(map);
    });

    CMC_CREATE_TEST(update_key[empty], {
        struct bidimap *map = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, 0, bm_count(map));

        cmc_assert(!bm_update_key(map, 1, 1));

        cmc_assert_equals(int32_t, cmc_flags.EMPTY, bm_flag(map));

        bm_free(map);
    });

    CMC_CREATE_TEST(update_key[not_found duplicate], {
        struct bidimap *map = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(bm_insert(map, 1, 1));

        cmc_assert_equals(size_t, 1, bm_count(map));

        map->flag = cmc_flags.ERROR;
        cmc_assert(bm_update_key(map, 1, 1));

        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        cmc_assert(!bm_update_key(map, 2, 2));

        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, bm_flag(map));

        cmc_assert(bm_insert(map, 2, 2));

        cmc_assert(!bm_update_key(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, bm_flag(map));
        cmc_assert(!bm_update_key(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, bm_flag(map));

        cmc_assert(bm_update_key(map, 2, 2));

        bm_free(map);
    });

    CMC_CREATE_TEST(update_val, {
        struct bidimap *map = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 100; i++)
            cmc_assert(bm_insert(map, i, i));

        cmc_assert_equals(size_t, 100, bm_count(map));

        for (size_t i = 100; i > 0; i--)
            cmc_assert(bm_update_val(map, i, i + 10));

        cmc_assert_equals(size_t, 100, bm_count(map));

        for (size_t i = 1; i <= 100; i++)
            cmc_assert_equals(size_t, i + 10, bm_get_val(map, i));

        cmc_assert_equals(size_t, 100, bm_count(map));

        bm_free(map);
    });

    CMC_CREATE_TEST(update_val[empty], {
        struct bidimap *map = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert_equals(size_t, 0, bm_count(map));

        cmc_assert(!bm_update_val(map, 1, 1));

        cmc_assert_equals(int32_t, cmc_flags.EMPTY, bm_flag(map));

        bm_free(map);
    });

    CMC_CREATE_TEST(update_val[not_found duplicate], {
        struct bidimap *map = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        cmc_assert(bm_insert(map, 1, 1));

        cmc_assert_equals(size_t, 1, bm_count(map));

        map->flag = cmc_flags.ERROR;
        cmc_assert(bm_update_val(map, 1, 1));

        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        cmc_assert(!bm_update_val(map, 2, 2));

        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, bm_flag(map));

        cmc_assert(bm_insert(map, 2, 2));

        cmc_assert(!bm_update_val(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, bm_flag(map));
        cmc_assert(!bm_update_val(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, bm_flag(map));

        cmc_assert(bm_update_val(map, 2, 2));

        bm_free(map);
    });

    CMC_CREATE_TEST(remove_by_key[cleanup custom], {
        struct bidimap *map =
            bm_new_custom(10000, 0.6, bm_ftab_key, bm_ftab_val,
                          &(struct cmc_alloc_node){ .malloc = malloc,
                                                    .calloc = calloc,
                                                    .realloc = realloc,
                                                    .free = free },
                          &(struct cmc_callbacks){ 0 });

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 10000; i++)
        {
            cmc_assert(bm_insert(map, i, i));
        }

        for (size_t i = 0; i < 10000; i++)
        {
            cmc_assert(bm_remove_by_key(map, i, NULL, NULL));
        }

        for (size_t i = 0; i < map->capacity; i++)
        {
            struct bidimap_entry *k = map->buffer[i][0];
            struct bidimap_entry *v = map->buffer[i][1];

            cmc_assert((k == NULL || k == CMC_ENTRY_DELETED) &&
                       (v == NULL || v == CMC_ENTRY_DELETED));
        }

        bm_free(map);
    });

    CMC_CREATE_TEST(remove_by_val[cleanup custom], {
        struct bidimap *map =
            bm_new_custom(10000, 0.6, bm_ftab_key, bm_ftab_val,
                          &(struct cmc_alloc_node){ .malloc = malloc,
                                                    .calloc = calloc,
                                                    .realloc = realloc,
                                                    .free = free },
                          &(struct cmc_callbacks){ 0 });

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 10000; i++)
        {
            cmc_assert(bm_insert(map, i, i));
        }

        for (size_t i = 0; i < 10000; i++)
        {
            cmc_assert(bm_remove_by_val(map, i, NULL, NULL));
        }

        for (size_t i = 0; i < map->capacity; i++)
        {
            struct bidimap_entry *k = map->buffer[i][0];
            struct bidimap_entry *v = map->buffer[i][1];

            cmc_assert((k == NULL || k == CMC_ENTRY_DELETED) &&
                       (v == NULL || v == CMC_ENTRY_DELETED));
        }

        bm_free(map);
    });

    CMC_CREATE_TEST(copy_of, {
        struct bidimap *map1 = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(bm_insert(map1, i, i));

        struct bidimap *map2 = bm_copy_of(map1);

        cmc_assert_not_equals(ptr, NULL, map2);
        cmc_assert_equals(size_t, bm_count(map1), bm_count(map2));

        for (size_t i = 0; i < 100; i++)
        {
            cmc_assert(bm_contains_key(map2, i) && bm_contains_key(map1, i));
            cmc_assert(bm_contains_val(map2, i) && bm_contains_val(map1, i));
        }

        bm_free(map1);
        bm_free(map2);
    });

    CMC_CREATE_TEST(equals, {
        struct bidimap *map1 = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);
        struct bidimap *map2 = bm_new(1000, 0.9, bm_ftab_key, bm_ftab_val);

        for (size_t i = 0; i < 100; i++)
        {
            cmc_assert(bm_insert(map1, i, i));
            cmc_assert(bm_insert(map2, 99 - i, 99 - i));
        }

        cmc_assert_not_equals(ptr, NULL, map1);
        cmc_assert_not_equals(ptr, NULL, map2);
        cmc_assert_equals(size_t, bm_count(map1), bm_count(map2));

        for (size_t i = 0; i < 100; i++)
        {
            cmc_assert(bm_contains_key(map2, i) && bm_contains_key(map1, i));
            cmc_assert(bm_contains_val(map2, i) && bm_contains_val(map1, i));
        }

        bm_free(map1);
        bm_free(map2);
    });

    CMC_CREATE_TEST(equals[from copy], {
        struct bidimap *map1 = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(bm_insert(map1, i, i));

        struct bidimap *map2 = bm_copy_of(map1);

        cmc_assert_not_equals(ptr, NULL, map2);

        cmc_assert(bm_equals(map1, map2));

        bm_free(map1);
        bm_free(map2);
    });

    CMC_CREATE_TEST(flags, {
        struct bidimap *map = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        // customize
        bm_customize(map, &cmc_alloc_node_default,
                     &(struct cmc_callbacks){ 0 });
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        // Insert
        cmc_assert(bm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        cmc_assert(!bm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, bm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(!bm_insert(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, bm_flag(map));

        // clear
        bm_clear(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        // bm_update_key
        cmc_assert(!bm_update_key(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, bm_flag(map));

        cmc_assert(bm_insert(map, 1, 1));
        cmc_assert(!bm_update_key(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, bm_flag(map));

        cmc_assert(bm_insert(map, 2, 2));
        cmc_assert(!bm_update_key(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, bm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(!bm_update_key(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, bm_flag(map));

        // clear
        bm_clear(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        // bm_update_val
        cmc_assert(!bm_update_val(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, bm_flag(map));

        cmc_assert(bm_insert(map, 1, 1));
        cmc_assert(!bm_update_val(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, bm_flag(map));

        cmc_assert(bm_insert(map, 2, 2));
        cmc_assert(!bm_update_val(map, 2, 1));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, bm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(!bm_update_val(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, bm_flag(map));

        // remove_by_key
        cmc_assert(bm_remove_by_key(map, 1, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        cmc_assert(!bm_remove_by_key(map, 1, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, bm_flag(map));

        cmc_assert(bm_remove_by_key(map, 2, NULL, NULL));
        cmc_assert(!bm_remove_by_key(map, 2, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, bm_flag(map));

        // remove_by_val
        cmc_assert(bm_insert(map, 1, 1) && bm_insert(map, 2, 2));

        cmc_assert(bm_remove_by_val(map, 1, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        cmc_assert(!bm_remove_by_val(map, 1, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, bm_flag(map));

        cmc_assert(bm_remove_by_val(map, 2, NULL, NULL));
        cmc_assert(!bm_remove_by_val(map, 2, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, bm_flag(map));

        // get_key and get_val
        cmc_assert(bm_insert(map, 2, 1));

        cmc_assert_equals(size_t, 2, bm_get_key(map, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 1, bm_get_val(map, 2));
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        cmc_assert_equals(size_t, 0, bm_get_key(map, 2));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, bm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 0, bm_get_val(map, 1));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, bm_flag(map));

        // contains_key
        cmc_assert(bm_contains_key(map, 2));
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        // contains_val
        map->flag = cmc_flags.ERROR;
        cmc_assert(bm_contains_val(map, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));

        // copy_of
        map->flag = cmc_flags.ERROR;
        struct bidimap *map2 = bm_copy_of(map);

        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map2));

        size_t tmp = map->capacity;
        map->capacity = 0;

        struct bidimap *map3 = bm_copy_of(map);
        cmc_assert_equals(ptr, NULL, map3);
        cmc_assert_equals(int32_t, cmc_flags.ERROR, bm_flag(map));

        map->capacity = tmp;

        // equals
        bm_get_key(map, 100);
        bm_get_key(map2, 100);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, bm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, bm_flag(map2));
        map->flag = cmc_flags.ERROR;
        cmc_assert(bm_equals(map, map2));
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, bm_flag(map2));

        bm_free(map);
        bm_free(map2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct bidimap *map =
            bm_new_custom(100, 0.7, bm_ftab_key, bm_ftab_val, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_equals(ptr, callbacks, map->callbacks);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(bm_insert(map, 10, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(bm_update_key(map, 10, 5));
        cmc_assert_equals(int32_t, 1, total_update);

        cmc_assert(bm_update_val(map, 5, 5));
        cmc_assert_equals(int32_t, 2, total_update);

        cmc_assert(bm_insert(map, 10, 10));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(bm_remove_by_key(map, 5, NULL, NULL));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(bm_remove_by_val(map, 10, NULL, NULL));
        cmc_assert_equals(int32_t, 2, total_delete);

        cmc_assert(bm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, 3, total_create);

        cmc_assert_equals(size_t, 1, bm_get_key(map, 2));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert_equals(size_t, 2, bm_get_val(map, 1));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert(bm_resize(map, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(bm_resize(map, 200));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert(bm_contains_key(map, 1));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert(bm_contains_val(map, 2));
        cmc_assert_equals(int32_t, 4, total_read);

        cmc_assert_equals(int32_t, 3, total_create);
        cmc_assert_equals(int32_t, 4, total_read);
        cmc_assert_equals(int32_t, 2, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        bm_customize(map, NULL, NULL);

        cmc_assert_equals(ptr, NULL, map->callbacks);

        bm_clear(map);
        cmc_assert(bm_insert(map, 10, 10));
        cmc_assert(bm_update_key(map, 10, 5));
        cmc_assert(bm_update_val(map, 5, 5));
        cmc_assert(bm_insert(map, 10, 10));
        cmc_assert(bm_remove_by_key(map, 5, NULL, NULL));
        cmc_assert(bm_remove_by_val(map, 10, NULL, NULL));
        cmc_assert(bm_insert(map, 1, 2));
        cmc_assert_equals(size_t, 1, bm_get_key(map, 2));
        cmc_assert_equals(size_t, 2, bm_get_val(map, 1));
        cmc_assert(bm_resize(map, 1000));
        cmc_assert(bm_resize(map, 200));
        cmc_assert(bm_contains_key(map, 1));
        cmc_assert(bm_contains_val(map, 2));

        cmc_assert_equals(int32_t, 3, total_create);
        cmc_assert_equals(int32_t, 4, total_read);
        cmc_assert_equals(int32_t, 2, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, map->callbacks);

        bm_free(map);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});
