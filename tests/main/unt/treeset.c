#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/treeset.h>

CMC_GENERATE_TREESET(ts, treeset, size_t)

struct treeset_ftab_val *ts_ftab_val =
    &(struct treeset_ftab_val){ .cmp = cmp,
                                .cpy = copy,
                                .str = str,
                                .free = custom_free,
                                .hash = hash,
                                .pri = pri };

CMC_CREATE_UNIT(treeset_test, true, {
    CMC_CREATE_TEST(new, {
        struct treeset *set = ts_new(ts_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        ts_free(set);
    });

    CMC_CREATE_TEST(clear[count], {
        struct treeset *set = ts_new(ts_ftab_val);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 50; i++)
            ts_insert(set, i);

        cmc_assert_equals(size_t, 50, ts_count(set));

        ts_clear(set);

        cmc_assert_equals(size_t, 0, ts_count(set));
        cmc_assert_equals(ptr, NULL, set->root);

        ts_free(set);
    });
});
