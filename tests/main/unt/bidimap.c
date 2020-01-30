#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/bidimap.h>

CMC_GENERATE_BIDIMAP(bm, bidimap, size_t, size_t)

struct bidimap_ftab_key *bm_ftab_key = &(struct bidimap_ftab_key) {
    .cmp = cmp,
    .cpy = copy,
    .str = to_string,
    .free = custom_free,
    .hash = hash,
    .pri = priority
};

struct bidimap_ftab_val *bm_ftab_val = &(struct bidimap_ftab_val) {
    .cmp = cmp,
    .cpy = copy,
    .str = to_string,
    .free = custom_free,
    .hash = hash,
    .pri = priority
};

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
            cmc_assert_equals(ptr, NULL, map->key_buffer[i]);
            cmc_assert_equals(ptr, NULL, map->val_buffer[i]);
        }

        bm_free(map);
    });

    CMC_CREATE_TEST(new_custom, {
        struct bidimap *map = bm_new_custom(100, 0.6, bm_ftab_key, bm_ftab_val,
                                            &(struct cmc_alloc_node){
                                                .malloc = malloc,
                                                .calloc = calloc,
                                                .realloc = realloc,
                                                .free = free},
                                            &(struct cmc_callbacks_bidimap){0});

        cmc_assert_not_equals(ptr, NULL, map);

        bm_free(map);
    });

    CMC_CREATE_TEST(new_custom[insert remove], {
        struct bidimap *map = bm_new_custom(500, 0.6, bm_ftab_key, bm_ftab_val,
                                            &(struct cmc_alloc_node){
                                                .malloc = malloc,
                                                .calloc = calloc,
                                                .realloc = realloc,
                                                .free = free},
                                            &(struct cmc_callbacks_bidimap){0});

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

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct bidimap *map = bm_new(1, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 10000; i++)
            cmc_assert(bm_insert(map, i, i));

        size_t sum = 0;

        for (size_t i = 0; i < bm_capacity(map); i++)
        {
            if (map->key_buffer[i] && map->key_buffer[i] != CMC_ENTRY_DELETED)
            {
                sum += map->key_buffer[i]->key;
            }
        }

        cmc_assert_equals(size_t, 50005000, sum);

        bm_free(map);
    });

    CMC_CREATE_TEST(insert, {
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

    CMC_CREATE_TEST(update_key, {
        struct bidimap *map = bm_new(100, 0.7, bm_ftab_key, bm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);

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

    CMC_CREATE_TEST(remove_by_key[cleanup custom], {
        struct bidimap *map = bm_new_custom(10000, 0.6, bm_ftab_key, bm_ftab_val,
                                            &(struct cmc_alloc_node){
                                                .malloc = malloc,
                                                .calloc = calloc,
                                                .realloc = realloc,
                                                .free = free},
                                            &(struct cmc_callbacks_bidimap){0});

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
            struct bidimap_entry *k = map->key_buffer[i];
            struct bidimap_entry *v = map->val_buffer[i];

            cmc_assert((k == NULL || k == CMC_ENTRY_DELETED)
                    && (v == NULL || v == CMC_ENTRY_DELETED));
        }

        bm_free(map);
    });

    CMC_CREATE_TEST(remove_by_val[cleanup custom], {
        struct bidimap *map = bm_new_custom(10000, 0.6, bm_ftab_key, bm_ftab_val,
                                            &(struct cmc_alloc_node){
                                                .malloc = malloc,
                                                .calloc = calloc,
                                                .realloc = realloc,
                                                .free = free},
                                            &(struct cmc_callbacks_bidimap){0});

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
            struct bidimap_entry *k = map->key_buffer[i];
            struct bidimap_entry *v = map->val_buffer[i];

            cmc_assert((k == NULL || k == CMC_ENTRY_DELETED) && (v == NULL || v == CMC_ENTRY_DELETED));
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
});
