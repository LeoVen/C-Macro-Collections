#include "utl/assert.h"
#include "utl/test.h"
#include "../src/treemap.c"

#include "utl.c"

CMC_CREATE_UNIT(treemap_test, true, {
    CMC_CREATE_TEST(new, {
        treemap *map = tm_new(cmp);

        cmc_assert_not_equals(ptr, NULL, map);

        CMC_TEST_PASS_ELSE_FAIL(map);

        tm_free(map);
    });

    CMC_CREATE_TEST(clear[count], {
        treemap *map = tm_new(cmp);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 50; i++)
            tm_insert(map, i, i);

        cmc_assert_equals(size_t, 50, tm_count(map));

        tm_clear(map);

        cmc_assert_equals(size_t, 0, tm_count(map));
        cmc_assert_equals(ptr, NULL, map->root);

        CMC_TEST_PASS_ELSE_FAIL(tm_count(map) == 0);

        tm_free(map);
    });

});
