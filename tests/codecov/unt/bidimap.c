#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include "../src/bidimap.c"

CMC_CREATE_UNIT(bidimap_test, false, {
    CMC_CREATE_TEST(new, {
        bidimap *map = bm_new(100, 0.6, cmp, hash, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        bm_free(map, NULL);
    });

    CMC_CREATE_TEST(clear[count], {
        bidimap *map = bm_new(100, 0.6, cmp, hash, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(bm_insert(map, i, i));

        cmc_assert_equals(size_t, 50, bm_count(map));

        bm_clear(map, NULL);

        cmc_assert_equals(size_t, 0, bm_count(map));

        bm_free(map, NULL);
    });
});
