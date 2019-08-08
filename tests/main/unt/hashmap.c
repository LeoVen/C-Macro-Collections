#include "utl/assert.h"
#include "utl/test.h"
#include "../src/hashmap.c"

#include "utl.c"

CMC_CREATE_UNIT(hashmap_test, true, {
    CMC_CREATE_TEST(new, {
        hashmap *map = hm_new(943722, 0.6, cmp, hash);

        bool passed = (map->capacity >= (943722 / 0.6)) && map->count == 0 && map->buffer;

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_greater_equals(size_t, (943722 / 0.6), hm_capacity(map));

        CMC_TEST_PASS_ELSE_FAIL(passed);

        hm_free(map);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = 0], {
        hashmap *map = hm_new(0, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, map);

        CMC_TEST_PASS_ELSE_FAIL(map == NULL);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = UINT64_MAX], {
        hashmap *map = hm_new(UINT64_MAX, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, map);

        CMC_TEST_PASS_ELSE_FAIL(map == NULL);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 50; i++)
            hm_insert(map, i, i);

        cmc_assert_equals(size_t, 50, hm_count(map));

        hm_clear(map);

        cmc_assert_equals(size_t, 0, hm_count(map));

        CMC_TEST_PASS_ELSE_FAIL(hm_count(map) == 0);

        hm_free(map);
    });

});
