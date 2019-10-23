#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include "../src/list.c"

CMC_CREATE_UNIT(list_test, false, {
    CMC_CREATE_TEST(new, {
        list *l = l_new(1000000);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert_not_equals(ptr, NULL, l->buffer);
        cmc_assert_equals(size_t, 1000000, l_capacity(l));
        cmc_assert_equals(size_t, 0, l_count(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        list *l = l_new(0);

        cmc_assert_equals(ptr, NULL, l);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        list *l = l_new(UINT64_MAX);

        cmc_assert_equals(ptr, NULL, l);
    });

    CMC_CREATE_TEST(new_from[count and item preservation], {
        size_t v[5];

        for (size_t i = 0; i < 5; i++)
        {
            v[i] = i;
        }

        list *l = l_new_from(v, 5);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 5; i++)
            cmc_assert_equals(size_t, i, l_get(l, i));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(new_from[size = 1], {
        size_t a = 10;

        size_t *values = &a;

        list *l = l_new_from(values, 1);

        cmc_assert_not_equals(ptr, NULL, l);

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 50; i++)
            l_push_back(l, i);

        cmc_assert_equals(size_t, 50, l_count(l));

        l_clear(l, NULL);

        cmc_assert_equals(size_t, 0, l_count(l));
        cmc_assert_equals(size_t, 100, l_capacity(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        list *l = l_new(1);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 100; i++)
            l_push_back(l, i);

        cmc_assert_equals(size_t, 100, l_count(l));

        for (size_t i = 0; i < 100; i++)
            cmc_assert_equals(size_t, i, l_get(l, i));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(push_front[count], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_front(l, i));

        cmc_assert_equals(size_t, 250, l_count(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(push_front[capacity], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_front(l, i));

        cmc_assert_lesser_equals(size_t, l_capacity(l), l_count(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(push_front[item preservation], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_front(l, i));

        for (size_t i = 0; i < l_count(l); i++)
            cmc_assert_equals(size_t, l_get(l, i), l_count(l) - i - 1);

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(push_at[count], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        cmc_assert(l_push_back(l, 0));
        cmc_assert(l_push_back(l, 249));

        for (size_t i = 1; i < 249; i++)
            cmc_assert(l_push_at(l, i, l_count(l) - 1));

        cmc_assert_equals(size_t, 250, l_count(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(push_at[capacity], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        cmc_assert(l_push_back(l, 0));
        cmc_assert(l_push_back(l, 249));

        for (size_t i = 1; i < 249; i++)
            cmc_assert(l_push_at(l, i, l_count(l) - 1));

        cmc_assert_lesser_equals(size_t, l_capacity(l), l_count(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(push_at[item preservation], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        cmc_assert(l_push_back(l, 0));
        cmc_assert(l_push_back(l, 249));

        for (size_t i = 1; i < 249; i++)
            cmc_assert(l_push_at(l, i, l_count(l) - 1));

        for (size_t i = 0; i < l_count(l); i++)
            cmc_assert_equals(size_t, l_get(l, i), i);

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(push_back[count], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_back(l, i));

        cmc_assert_equals(size_t, 250, l_count(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(push_back[capacity], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_back(l, i));

        cmc_assert_lesser_equals(size_t, l_capacity(l), l_count(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(push_back[item preservation], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_back(l, i));

        for (size_t i = 0; i < l_count(l); i++)
            cmc_assert_equals(size_t, l_get(l, i), i);

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(pop_front[count], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            l_push_back(l, i);

        cmc_assert(l_pop_front(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(pop_front[item preservation], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            l_push_back(l, i);

        cmc_assert(l_pop_front(l));

        for (size_t i = 0; i < l_count(l); i++)
            cmc_assert_equals(size_t, l_get(l, i), i + 1);

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(pop_front[count = 0], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop_front(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(pop_front[count = 1], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 10));
        cmc_assert(l_pop_front(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(pop_front[buffer_clear], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_push_front(l, 2));

        cmc_assert(l_pop_front(l));

        cmc_assert_equals(size_t, 0, l->buffer[1]);

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(pop_at[count = 0], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop_at(l, 1));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(pop_at[count = 1], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_pop_at(l, 0));

        cmc_assert_equals(size_t, 0, l->buffer[0]);

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(pop_at[buffer_clear], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_push_back(l, 2));
        cmc_assert(l_push_back(l, 3));
        cmc_assert(l_pop_at(l, 1));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(pop_back[count = 0], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop_back(l));

        l_free(l, NULL);
    });

    CMC_CREATE_TEST(pop_back[count = 1], {
        list *l = l_new(100);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 10));
        cmc_assert(l_pop_back(l));

        l_free(l, NULL);
    });
})
