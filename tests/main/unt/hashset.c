#include "utl/assert.h"
#include "utl/test.h"
#include "../src/hashset.c"

#include "../src/utl.c"

CMC_CREATE_UNIT(hashset_test, true, {
    CMC_CREATE_TEST(new, {
        hashset *set = hs_new(943722, 0.6, cmp, hash);

        bool passed = (set->capacity >= (943722 / 0.6)) && set->count == 0 && set->buffer;

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert(hs_capacity(set) > (943722 / 0.6));

        CMC_TEST_PASS_ELSE_FAIL(passed);

        hs_free(set);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = 0], {
        hashset *set = hs_new(0, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, set);

        CMC_TEST_PASS_ELSE_FAIL(set == NULL);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = UINT64_MAX], {
        hashset *set = hs_new(UINT64_MAX, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, set);

        CMC_TEST_PASS_ELSE_FAIL(set == NULL);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        hashset *set = hs_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 50; i++)
            hs_insert(set, i);

        cmc_assert_equals(size_t, 50, hs_count(set));

        hs_clear(set);

        cmc_assert_equals(size_t, 0, hs_count(set));

        CMC_TEST_PASS_ELSE_FAIL(hs_count(set) == 0);

        hs_free(set);
    });

});
