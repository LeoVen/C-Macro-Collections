#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/hashmap.h>

CMC_GENERATE_HASHMAP(hm, hashmap, size_t, size_t)

CMC_CREATE_UNIT(hashmap_test, true, {
    CMC_CREATE_TEST(new, {
        hashmap *map = hm_new(943722, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, hm_count(map));
        cmc_assert_greater_equals(size_t, (943722 / 0.6), hm_capacity(map));

        hm_free(map, NULL);
    });

    CMC_CREATE_TEST(new [edge_case:capacity = 0], {
        hashmap *map = hm_new(0, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(new [edge_case:capacity = UINT64_MAX], {
        hashmap *map = hm_new(UINT64_MAX, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        hashmap *map = hm_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 50; i++)
            hm_insert(map, i, i);

        cmc_assert_equals(size_t, 50, hm_count(map));

        hm_clear(map, NULL);

        cmc_assert_equals(size_t, 0, hm_count(map));

        hm_free(map, NULL);
    });
});
