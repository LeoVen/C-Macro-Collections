#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <ext/multiset.h>

CMC_GENERATE_MULTISET(ms, multiset, size_t)

CMC_CREATE_UNIT(multiset_test, true, {
    CMC_CREATE_TEST(new, {
        multiset *set = ms_new(943722, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_not_equals(ptr, NULL, set->buffer);
        cmc_assert_equals(size_t, 0, ms_count(set));
        cmc_assert_greater_equals(size_t, ((size_t)(943722 / 0.6)), ms_capacity(set));

        ms_free(set, NULL);
    });

    CMC_CREATE_TEST(new[edge_case capacity = 0], {
        multiset *set = ms_new(0, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(new[edge_case capacity = UINT64_MAX], {
        multiset *set = ms_new(UINT64_MAX, 0.99, cmp, hash);

        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        multiset *set = ms_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(ms_insert(set, i));

        cmc_assert_equals(size_t, 50, ms_count(set));

        ms_clear(set, NULL);

        cmc_assert_equals(size_t, 0, ms_count(set));

        ms_free(set, NULL);
    });

    CMC_CREATE_TEST(insert[count cardinality multiplicity], {
        multiset *set = ms_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(ms_insert(set, 1));
        cmc_assert(ms_insert(set, 2));
        cmc_assert(ms_insert(set, 2));
        cmc_assert(ms_insert(set, 3));
        cmc_assert(ms_insert(set, 3));
        cmc_assert(ms_insert(set, 3));

        cmc_assert_equals(size_t, 3, ms_count(set));
        cmc_assert_equals(size_t, 6, ms_cardinality(set));

        for (size_t i = 0; i <= 3; i++)
            cmc_assert_equals(size_t, i, ms_multiplicity_of(set, i));

        ms_free(set, NULL);
    });

    CMC_CREATE_TEST(multiplicity, {
        multiset *set = ms_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(ms_insert(set, i % 20));

        cmc_assert_equals(size_t, 20, ms_count(set));
        cmc_assert_equals(size_t, 100, ms_cardinality(set));

        for (size_t i = 0; i < 20; i++)
            cmc_assert_equals(size_t, 5, ms_multiplicity_of(set, i));

        ms_free(set, NULL);
    });
});
