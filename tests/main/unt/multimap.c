#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/multimap.h>

CMC_GENERATE_MULTIMAP(mm, multimap, size_t, size_t)

struct multimap_ftab_key *mm_ftab_key =
    &(struct multimap_ftab_key){ .cmp = cmp,
                                 .cpy = copy,
                                 .str = str,
                                 .free = custom_free,
                                 .hash = hash,
                                 .pri = pri };

struct multimap_ftab_val *mm_ftab_val =
    &(struct multimap_ftab_val){ .cmp = cmp,
                                 .cpy = copy,
                                 .str = str,
                                 .free = custom_free,
                                 .hash = hash,
                                 .pri = pri };

CMC_CREATE_UNIT(multimap_test, true, {
    CMC_CREATE_TEST(new, {
        struct multimap *map = mm_new(943722, 0.8, mm_ftab_key, mm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, mm_count(map));
        cmc_assert_greater_equals(size_t, (943722 / 0.8), mm_capacity(map));

        mm_free(map);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        struct multimap *map = mm_new(0, 0.8, mm_ftab_key, mm_ftab_val);

        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        struct multimap *map =
            mm_new(UINT64_MAX, 0.99, mm_ftab_key, mm_ftab_val);

        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct multimap *map = mm_new(100, 0.8, mm_ftab_key, mm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(mm_insert(map, i, i));

        cmc_assert_equals(size_t, 50, mm_count(map));

        mm_clear(map);

        cmc_assert_equals(size_t, 0, mm_count(map));

        mm_free(map);
    });

    CMC_CREATE_TEST(insert[count], {
        struct multimap *map = mm_new(100, 0.8, mm_ftab_key, mm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(mm_insert(map, i % 20, i % 20));

        cmc_assert_equals(size_t, 100, mm_count(map));

        mm_free(map);
    });

    CMC_CREATE_TEST(remove[count], {
        struct multimap *map = mm_new(100, 0.8, mm_ftab_key, mm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 200; i++)
            cmc_assert(mm_insert(map, i % 20, i % 20));

        size_t r;

        for (size_t i = 0; i < 60; i++)
            cmc_assert(mm_remove(map, i % 20, &r));

        cmc_assert_equals(size_t, 140, mm_count(map));

        mm_free(map);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        struct multimap *map = mm_new(100, 0.8, mm_ftab_key, mm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        size_t r;

        cmc_assert(!mm_remove(map, 10, &r));

        mm_free(map);
    });

    CMC_CREATE_TEST(remove[count = 1], {
        struct multimap *map = mm_new(100, 0.8, mm_ftab_key, mm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        size_t r;

        cmc_assert(mm_insert(map, 10, 11));
        cmc_assert(mm_remove(map, 10, &r));

        mm_free(map);
    });

    CMC_CREATE_TEST(get[key ordering], {
        struct multimap *map = mm_new(100, 0.8, mm_ftab_key, mm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        /* Assert that the value 100 is accessed before the value 101 for */
        /* the same key */
        cmc_assert(mm_insert(map, 10, 100));
        cmc_assert(mm_insert(map, 20, 102));
        cmc_assert(mm_insert(map, 10, 101));

        cmc_assert_equals(size_t, 100, mm_get(map, 10));

        mm_free(map);
    });

    CMC_CREATE_TEST(key_count, {
        struct multimap *map = mm_new(50, 0.8, mm_ftab_key, mm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 1000; i++)
            cmc_assert(mm_insert(map, i % 20, i % 20));

        cmc_assert_equals(size_t, 1000, mm_count(map));

        for (size_t i = 0; i < 20; i++)
            cmc_assert_equals(size_t, 50, mm_key_count(map, i));

        mm_free(map);
    });

    CMC_CREATE_TEST(flags, {
        struct multimap *map = mm_new(100, 0.8, mm_ftab_key, mm_ftab_val);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // clear
        map->flag = cmc_flags.ERROR;
        mm_clear(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // customize
        map->flag = cmc_flags.ERROR;
        mm_customize(map, NULL, NULL);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // insert
        map->flag = cmc_flags.ERROR;
        cmc_assert(mm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // update
        cmc_assert(!mm_update(map, 9, 10, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));

        cmc_assert(mm_update(map, 1, 20, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // update_all
        map->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 1, mm_update_all(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        map->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 0, mm_update_all(map, 2, 4, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));

        mm_clear(map);
        cmc_assert_equals(size_t, 0, mm_update_all(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));

        // remove
        map->flag = cmc_flags.ERROR;
        cmc_assert(!mm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));

        cmc_assert(mm_insert(map, 1, 1));
        cmc_assert(!mm_remove(map, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));

        cmc_assert(mm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // remove_all
        cmc_assert(!mm_remove_all(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));

        cmc_assert(mm_insert(map, 1, 1) && mm_insert(map, 1, 1));
        cmc_assert_equals(size_t, 0, mm_remove_all(map, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));

        cmc_assert_equals(size_t, 2, mm_remove_all(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // max min
        cmc_assert(!mm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(!mm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));

        cmc_assert(mm_insert(map, 1, 1));
        cmc_assert(mm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(mm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // get get_ref
        mm_get(map, 2);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));
        map->flag = cmc_flags.ERROR;
        mm_get_ref(map, 2);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));

        mm_get(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));
        map->flag = cmc_flags.ERROR;
        mm_get_ref(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        mm_clear(map);
        mm_get(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));
        map->flag = cmc_flags.ERROR;
        mm_get_ref(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));

        // contains
        map->flag = cmc_flags.ERROR;
        cmc_assert(!mm_contains(map, 1000));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // copy_of
        map->flag = cmc_flags.ERROR;
        struct multimap *map2 = mm_copy_of(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map2));

        // equals
        map->flag = cmc_flags.ERROR;
        map2->flag = cmc_flags.ERROR;
        cmc_assert(mm_equals(map, map2));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map2));

        mm_free(map);
        mm_free(map2);
    });
});
