#include "utl/assert.h"
#include "utl/test.h"
#include "../src/list.c"

CMC_CREATE_UNIT(list_test, true, {
    CMC_CREATE_TEST(new, {
        list *l = l_new(1000000);

        cmc_assert_not_equals(ptr, NULL, l);

        bool passed = l->capacity == 1000000 && l->count == 0 && l->buffer;

        cmc_assert_equals(size_t, 1000000, l->capacity);
        cmc_assert_equals(size_t, 0, l_count(l));
        cmc_assert_not_equals(ptr, NULL, l->buffer);

        CMC_TEST_PASS_ELSE_FAIL(passed);

        l_free(l);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = 0], {
        list *l = l_new(0);

        cmc_assert_equals(ptr, NULL, l);

        CMC_TEST_PASS_ELSE_FAIL(l == NULL);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = UINT64_MAX], {
        list *l = l_new(UINT64_MAX);

        cmc_assert_equals(ptr, NULL, l);

        CMC_TEST_PASS_ELSE_FAIL(l == NULL);
    });

    CMC_CREATE_TEST(new_from[count item_preservation], {
        size_t v[5];

        for (size_t i = 0; i < 5; i++)
        {
            v[i] = i;
        }

        list *l = l_new_from(v, 5);

        cmc_assert_not_equals(ptr, NULL, l);

        bool passed = true;

        for (size_t i = 0; i < 5; i++)
        {
            cmc_assert_equals(size_t, i, l_get(l, i));

            passed = passed && l_get(l, i) == i;
        }

        CMC_TEST_PASS_ELSE_FAIL(passed && l_count(l) == 5);

        l_free(l);
    });

    CMC_CREATE_TEST(new_from[edge_case:size = 1], {
        size_t a = 10;

        size_t *values = &a;

        list *l = l_new_from(values, 1);

        cmc_assert_not_equals(ptr, NULL, l);

        CMC_TEST_PASS_ELSE_FAIL(l_count(l) == 1);

        l_free(l);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 50; i++)
            l_push_back(l, i);

        cmc_assert_equals(size_t, 50, l_count(l));

        l_clear(l);

        cmc_assert_equals(size_t, 0, l_count(l));
        cmc_assert_equals(size_t, 100, l_capacity(l));

        CMC_TEST_PASS_ELSE_FAIL(l_count(l) == 0 && l_capacity(l) == 100);

        l_free(l);
    });

    CMC_CREATE_TEST(buffer_growth[edge_case:capacity = 1], {
        list *l = l_new(1);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 100; i++)
            l_push_back(l, i);

        cmc_assert_equals(size_t, 100, l_count(l));

        bool passed = true;

        for (size_t i = 0; i < 100; i++)
        {
            passed = passed && l_get(l, i) == i;
            cmc_assert_equals(size_t, i, l_get(l, i));
        }

        CMC_TEST_PASS_ELSE_FAIL(passed);

        l_free(l);
    });

    CMC_CREATE_TEST(push_front[count capacity item_preservation grow], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 200; i++)
            l_push_front(l, i);

        bool passed = true;

        for (size_t i = 0; i < l_count(l); i++)
        {
            passed = passed && l_get(l, i) == (l_count(l) - i - 1);

            cmc_assert_equals(size_t, l_get(l, i), l_count(l) - i - 1);
        }

        CMC_TEST_PASS_ELSE_FAIL(passed && l_count(l) == 200);

        l_free(l);
    });

    CMC_CREATE_TEST(push[count capacity item_preservation grow], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 200; i++)
            l_push(l, i, l_count(l));

        bool passed = true;

        for (size_t i = 0; i < l_count(l); i++)
        {
            passed = passed && l_get(l, i) == i;

            cmc_assert_equals(size_t, l_get(l, i), i);
        }

        CMC_TEST_PASS_ELSE_FAIL(passed && l_count(l) == 200);

        l_free(l);
    });

    CMC_CREATE_TEST(push_back[count capacity item_preservation grow], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 200; i++)
            l_push_back(l, i);

        bool passed = true;

        for (size_t i = 0; i < l_count(l); i++)
        {
            passed = passed && l_get(l, i) == i;

            cmc_assert_equals(size_t, l_get(l, i), i);
        }

        CMC_TEST_PASS_ELSE_FAIL(passed && l_count(l) == 200);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[count item_preservation], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 200; i++)
            l_push_back(l, i);

        bool passed = l_pop_front(l);

        for (size_t i = 0; i < l_count(l); i++)
        {
            passed = passed && l_get(l, i) == i + 1;

            cmc_assert_equals(size_t, l_get(l, i), i + 1);
        }

        CMC_TEST_PASS_ELSE_FAIL(passed && l_count(l) == 199);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[edge_case:count = 0], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop_front(l));
        CMC_TEST_PASS_ELSE_FAIL(l_count(l) == 0);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[edge_case:count = 1], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 10));
        cmc_assert(l_pop_front(l));

        CMC_TEST_PASS_ELSE_FAIL(l_count(l) == 0);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[buffer_clear], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_push_front(l, 2));

        bool passed = l_pop_front(l) && l->buffer[1] == 0;

        cmc_assert_equals(size_t, 0, l->buffer[1]);

        CMC_TEST_PASS_ELSE_FAIL(passed && l_count(l) == 1);

        l_free(l);
    });

    CMC_CREATE_TEST(pop[edge_case:count = 0], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop(l, 1));

        CMC_TEST_PASS_ELSE_FAIL(!l_pop(l, 1));

        l_free(l);
    });

    CMC_CREATE_TEST(pop[edge_case:count = 1], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));

        CMC_TEST_PASS_ELSE_FAIL(l_pop(l, 0));

        cmc_assert_equals(size_t, 0, l->buffer[0]);

        l_free(l);
    });

    CMC_CREATE_TEST(pop[buffer_clear], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_push_back(l, 2));
        cmc_assert(l_push_back(l, 3));
        cmc_assert(l_pop(l, 1));

        CMC_TEST_PASS_ELSE_FAIL(0 == l->buffer[2]);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_back[edge_case:count = 0], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop_back(l));
        CMC_TEST_PASS_ELSE_FAIL(l_count(l) == 0);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_back[edge_case:count = 1], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 10));
        cmc_assert(l_pop_back(l));

        CMC_TEST_PASS_ELSE_FAIL(l_count(l) == 0);

        l_free(l);
    });
})
