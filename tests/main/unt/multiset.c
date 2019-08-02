#include "utl/assert.h"
#include "utl/test.h"
#include "../src/multiset.c"

#include "utl.c"

CMC_CREATE_UNIT(multiset_test, true, {
    CMC_CREATE_TEST(new, {
        multiset *set = ms_new(943722, 0.6, cmp, hash);

        bool passed = (set->capacity >= (943722 / 0.6)) && set->count == 0 && set->buffer;

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert(ms_capacity(set) > (943722 / 0.6));

        CMC_TEST_PASS_ELSE_FAIL(passed);

        ms_free(set);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = 0], {
        multiset *set = ms_new(0, 0.6, cmp, hash);

        cmc_assert_equals(ptr, NULL, set);

        CMC_TEST_PASS_ELSE_FAIL(set == NULL);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = UINTMAX_MAX], {
        multiset *set = ms_new(UINTMAX_MAX, 0.99, cmp, hash);

        cmc_assert_equals(ptr, NULL, set);

        CMC_TEST_PASS_ELSE_FAIL(set == NULL);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        multiset *set = ms_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(ms_insert(set, i));

        cmc_assert_equals(size_t, 50, ms_count(set));

        ms_clear(set);

        cmc_assert_equals(size_t, 0, ms_count(set));

        CMC_TEST_PASS_ELSE_FAIL(ms_count(set) == 0);

        ms_free(set);
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

        bool passed = ms_count(set) == 3 && ms_cardinality(set) == 6;

        for (size_t i = 0; i <= 3; i++)
            passed = passed && ms_multiplicity_of(set, i) == i;

        CMC_TEST_PASS_ELSE_FAIL(passed);

        ms_free(set);
    });

    CMC_CREATE_TEST(multiplicity, {
        multiset *set = ms_new(100, 0.6, cmp, hash);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(ms_insert(set, i % 20));

        cmc_assert_equals(size_t, 20, ms_count(set));
        cmc_assert_equals(size_t, 100, ms_cardinality(set));

        bool passed = ms_count(set) == 20 && ms_cardinality(set) == 100;

        for (size_t i = 0; i < 20; i++)
            passed = passed && ms_multiplicity_of(set, i) == 5;

        CMC_TEST_PASS_ELSE_FAIL(passed);

        ms_free(set);
    });

});
