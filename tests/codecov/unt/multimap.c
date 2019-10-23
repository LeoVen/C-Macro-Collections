#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include "../src/multimap.c"

CMC_CREATE_UNIT(multimap_test, false, {
    CMC_CREATE_TEST(new, {
        multimap *map = mm_new(943722, 0.8, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, mm_count(map));
        cmc_assert_greater_equals(size_t, (943722 / 0.8), mm_capacity(map));

        mm_free(map, NULL);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        multimap *map = mm_new(0, 0.8, cmp, hash);

        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        multimap *map = mm_new(UINT64_MAX, 0.99, cmp, hash);

        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        multimap *map = mm_new(100, 0.8, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(mm_insert(map, i, i));

        cmc_assert_equals(size_t, 50, mm_count(map));

        mm_clear(map, NULL);

        cmc_assert_equals(size_t, 0, mm_count(map));

        mm_free(map, NULL);
    });

    CMC_CREATE_TEST(insert[count], {
        multimap *map = mm_new(100, 0.8, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(mm_insert(map, i % 20, i % 20));

        cmc_assert_equals(size_t, 100, mm_count(map));

        mm_free(map, NULL);
    });

    CMC_CREATE_TEST(remove[count], {
        multimap *map = mm_new(100, 0.8, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 200; i++)
            cmc_assert(mm_insert(map, i % 20, i % 20));

        size_t r;

        for (size_t i = 0; i < 60; i++)
            cmc_assert(mm_remove(map, i % 20, &r));

        cmc_assert_equals(size_t, 140, mm_count(map));

        mm_free(map, NULL);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        multimap *map = mm_new(100, 0.8, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        size_t r;

        cmc_assert(!mm_remove(map, 10, &r));

        mm_free(map, NULL);
    });

    CMC_CREATE_TEST(remove[count = 1], {
        multimap *map = mm_new(100, 0.8, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        size_t r;

        cmc_assert(mm_insert(map, 10, 11));
        cmc_assert(mm_remove(map, 10, &r));

        mm_free(map, NULL);
    });

    CMC_CREATE_TEST(get[key ordering], {
        multimap *map = mm_new(100, 0.8, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        /* Assert that the value 100 is accessed before the value 101 for the
         * same key */
        cmc_assert(mm_insert(map, 10, 100));
        cmc_assert(mm_insert(map, 20, 102));
        cmc_assert(mm_insert(map, 10, 101));

        cmc_assert_equals(size_t, 100, mm_get(map, 10));

        mm_free(map, NULL);
    });

    CMC_CREATE_TEST(key_count, {
        multimap *map = mm_new(50, 0.8, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 1000; i++)
            cmc_assert(mm_insert(map, i % 20, i % 20));

        cmc_assert_equals(size_t, 1000, mm_count(map));

        for (size_t i = 0; i < 20; i++)
            cmc_assert_equals(size_t, 50, mm_key_count(map, i));

        mm_free(map, NULL);
    });
});
