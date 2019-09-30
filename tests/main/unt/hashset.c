#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/hashset.h>

CMC_GENERATE_HASHSET(hs, hashset, size_t)

CMC_CREATE_UNIT(hashset_test, true, {
    CMC_CREATE_TEST(new, {
        hashset *set = hs_new(943722, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_not_equals(ptr, NULL, set->buffer);
        cmc_assert_equals(size_t, 0, hs_count(set));
        cmc_assert_greater_equals(size_t, (943722 / 0.6), hs_capacity(set));

        hs_free(set, NULL);
    });

    CMC_CREATE_TEST(new [edge_case:capacity = 0], {
        hashset *set = hs_new(0, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(new [edge_case:capacity = UINT64_MAX], {
        hashset *set = hs_new(UINT64_MAX, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 50; i++)
            hs_insert(set, i);

        cmc_assert_equals(size_t, 50, hs_count(set));

        hs_clear(set, NULL);

        cmc_assert_equals(size_t, 0, hs_count(set));

        hs_free(set, NULL);
    });
});
