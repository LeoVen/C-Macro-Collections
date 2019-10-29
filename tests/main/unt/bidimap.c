#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/bidimap.h>

CMC_GENERATE_BIDIMAP(bm, bidimap, size_t, size_t)

CMC_CREATE_UNIT(bidimap_test, true, {
    CMC_CREATE_TEST(new, {
        struct bidimap *map = bm_new(100, 0.6, cmp, hash, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        bm_free(map, NULL);
    });

    CMC_CREATE_TEST(clear[count], {
        struct bidimap *map = bm_new(100, 0.6, cmp, hash, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(bm_insert(map, i, i));

        cmc_assert_equals(size_t, 50, bm_count(map));

        bm_clear(map, NULL);

        cmc_assert_equals(size_t, 0, bm_count(map));

        bm_free(map, NULL);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct bidimap *map = bm_new(1, 0.7, cmp, hash, cmp, hash);

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

        bm_free(map, NULL);
    });

    CMC_CREATE_TEST(copy_of, {
        struct bidimap *map1 = bm_new(100, 0.7, cmp, hash, cmp, hash);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(bm_insert(map1, i, i));

        struct bidimap *map2 = bm_copy_of(map1, NULL, NULL);

        cmc_assert_not_equals(ptr, NULL, map2);
        cmc_assert_equals(size_t, bm_count(map1), bm_count(map2));

        for (size_t i = 0; i < 100; i++)
        {
            cmc_assert(bm_contains_key(map2, i) && bm_contains_key(map1, i));
            cmc_assert(bm_contains_val(map2, i) && bm_contains_val(map1, i));
        }

        bm_free(map1, NULL);
        bm_free(map2, NULL);
    });

    CMC_CREATE_TEST(equals, {
        struct bidimap *map1 = bm_new(100, 0.7, cmp, hash, cmp, hash);
        struct bidimap *map2 = bm_new(1000, 0.9, cmp, hash, cmp, hash);

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

        bm_free(map1, NULL);
        bm_free(map2, NULL);
    });

    CMC_CREATE_TEST(equals[from copy], {
        struct bidimap *map1 = bm_new(100, 0.7, cmp, hash, cmp, hash);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(bm_insert(map1, i, i));

        struct bidimap *map2 = bm_copy_of(map1, NULL, NULL);

        cmc_assert_not_equals(ptr, NULL, map2);

        cmc_assert(bm_equals(map1, map2));

        bm_free(map1, NULL);
        bm_free(map2, NULL);
    });
});
