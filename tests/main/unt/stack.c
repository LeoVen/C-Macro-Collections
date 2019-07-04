#include "utl/assert.h"
#include "utl/test.h"
#include "../src/stack.c"

CMC_CREATE_UNIT(stack_test, true, {
    CMC_CREATE_TEST(new, {
        stack *s = s_new(1000000);

        cmc_assert_not_equals(ptr, NULL, s);

        bool passed = s->capacity == 1000000 && s->count == 0 && s->buffer;

        cmc_assert_equals(size_t, 1000000, s->capacity);
        cmc_assert_equals(size_t, 0, s_count(s));
        cmc_assert_not_equals(ptr, NULL, s->buffer);

        CMC_TEST_PASS_ELSE_FAIL(passed);

        s_free(s);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = 0], {
        stack *s = s_new(0);

        cmc_assert_equals(ptr, NULL, s);

        CMC_TEST_PASS_ELSE_FAIL(s == NULL);
    });

    CMC_CREATE_TEST(new[edge_case:capacity = UINT64_MAX], {
        stack *s = s_new(UINT64_MAX);

        cmc_assert_equals(ptr, NULL, s);

        CMC_TEST_PASS_ELSE_FAIL(s == NULL);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_equals(size_t, 50, s_count(s));

        s_clear(s);

        cmc_assert_equals(size_t, 0, s_count(s));
        cmc_assert_equals(size_t, 100, s_capacity(s));

        CMC_TEST_PASS_ELSE_FAIL(s_count(s) == 0 && s_capacity(s) == 100);

        s_free(s);
    });

    CMC_CREATE_TEST(buffer_growth[edge_case:capacity = 1], {
        stack *s = s_new(1);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_equals(size_t, 50, s_count(s));
        cmc_assert_lesser_equals(size_t, s_capacity(s), s_count(s));

        CMC_TEST_PASS_ELSE_FAIL(s_count(s) == 50 && s_count(s) <= s_capacity(s));

        s_free(s);
    });

    CMC_CREATE_TEST(push[count], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_equals(size_t, 250, s_count(s));

        CMC_TEST_PASS_ELSE_FAIL(s_count(s) == 250);

        s_free(s);
    });

    CMC_CREATE_TEST(push[capacity], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_lesser_equals(size_t, s_capacity(s), s_count(s));

        CMC_TEST_PASS_ELSE_FAIL(s_count(s) <= s_capacity(s));

        s_free(s);
    });

    CMC_CREATE_TEST(push[item_preservation], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        bool passed = true;

        for (size_t i = 0; i < s_count(s); i++)
        {
            passed = s->buffer[i] == i;

            cmc_assert_equals(size_t, i, s->buffer[i]);
        }

        CMC_TEST_PASS_ELSE_FAIL(passed);

        s_free(s);
    });

    CMC_CREATE_TEST(pop[count], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));

        cmc_assert_equals(size_t, 249, s_count(s));

        CMC_TEST_PASS_ELSE_FAIL(s_count(s) == 249);

        s_free(s);
    });

    CMC_CREATE_TEST(pop[capacity], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));

        cmc_assert_lesser_equals(size_t, s_capacity(s), s_count(s));

        CMC_TEST_PASS_ELSE_FAIL(s_count(s) <= s_capacity(s));

        s_free(s);
    });

    CMC_CREATE_TEST(pop[item_preservation], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));
        cmc_assert(s_pop(s));

        bool passed = true;

        for (size_t i = 0; i < s_count(s); i++)
        {
            passed = s->buffer[i] == i;

            cmc_assert_equals(size_t, i, s->buffer[i]);
        }

        CMC_TEST_PASS_ELSE_FAIL(passed);

        s_free(s);
    });

    CMC_CREATE_TEST(push_if, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            s_push_if(s, i, i % 2 == 0);

        bool passed = true;

        for (size_t i = 0; i < s_count(s); i++)
        {
            passed = passed && s->buffer[i] % 2 == 0;

            cmc_assert(s->buffer[i] % 2 == 0);
        }

        CMC_TEST_PASS_ELSE_FAIL(passed);

        s_free(s);
    });

    CMC_CREATE_TEST(pop_if, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
        {
            s_push(s, i);
            s_pop_if(s, s_top(s) % 2 == 0);
        }

        bool passed = true;

        for (size_t i = 0; i < s_count(s); i++)
        {
            passed = passed && !(s->buffer[i] % 2 == 0);

            cmc_assert(!(s->buffer[i] % 2 == 0));
        }

        CMC_TEST_PASS_ELSE_FAIL(passed);

        s_free(s);
    });

    CMC_CREATE_TEST(top, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));
        cmc_assert(s_pop(s));

        cmc_assert_equals(size_t, 247, s_top(s));

        CMC_TEST_PASS_ELSE_FAIL(247 == s_top(s));

        s_free(s);
    });

    CMC_CREATE_TEST(top[edge_case:count = 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 1));

        cmc_assert(s_pop(s));

        cmc_assert_equals(size_t, s_impl_default_value(), s_top(s));

        CMC_TEST_PASS_ELSE_FAIL(s_impl_default_value() == s_top(s));

        s_free(s);
    });

    CMC_CREATE_TEST(top[edge_case:count = 1], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 10));
        cmc_assert_equals(size_t, 1, s_count(s));
        cmc_assert_equals(size_t, 10, s_top(s));

        CMC_TEST_PASS_ELSE_FAIL(10 == s_top(s));

        s_free(s);
    });

    CMC_CREATE_TEST(empty, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_empty(s));

        CMC_TEST_PASS_ELSE_FAIL(s_empty(s));

        s_free(s);
    });

    CMC_CREATE_TEST(empty[after_io], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 10));
        cmc_assert(s_pop(s));
        cmc_assert(s_empty(s));

        CMC_TEST_PASS_ELSE_FAIL(s_empty(s));

        s_free(s);
    });

    CMC_CREATE_TEST(full, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < s_capacity(s); i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_full(s));

        CMC_TEST_PASS_ELSE_FAIL(s_full(s));

        s_free(s);
    });

    CMC_CREATE_TEST(full[edge_case:capacity = 1], {
        stack *s = s_new(1);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 10));

        cmc_assert(s_full(s));

        CMC_TEST_PASS_ELSE_FAIL(s_full(s));

        s_free(s);
    });

});
