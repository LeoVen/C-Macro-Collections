#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include "../src/stack.c"

CMC_CREATE_UNIT(stack_test, false, {
    CMC_CREATE_TEST(new, {
        stack *s = s_new(1000000);

        cmc_assert_not_equals(ptr, NULL, s);
        cmc_assert_not_equals(ptr, NULL, s->buffer);
        cmc_assert_equals(size_t, 1000000, s_capacity(s));
        cmc_assert_equals(size_t, 0, s_count(s));
        cmc_assert_not_equals(ptr, NULL, s->buffer);

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        stack *s = s_new(0);

        cmc_assert_equals(ptr, NULL, s);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        stack *s = s_new(UINT64_MAX);

        cmc_assert_equals(ptr, NULL, s);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_equals(size_t, 50, s_count(s));

        s_clear(s, NULL);

        cmc_assert_equals(size_t, 0, s_count(s));
        cmc_assert_equals(size_t, 100, s_capacity(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        stack *s = s_new(1);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_equals(size_t, 50, s_count(s));
        cmc_assert_lesser_equals(size_t, s_capacity(s), s_count(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(push[count], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_equals(size_t, 250, s_count(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(push[capacity], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_lesser_equals(size_t, s_capacity(s), s_count(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(push[item preservation], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        for (size_t i = 0; i < s_count(s); i++)
            cmc_assert_equals(size_t, i, s->buffer[i]);

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(pop[count], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));

        cmc_assert_equals(size_t, 249, s_count(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(pop[capacity], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));

        cmc_assert_lesser_equals(size_t, s_capacity(s), s_count(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(pop[item preservation], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));
        cmc_assert(s_pop(s));

        for (size_t i = 0; i < s_count(s); i++)
            cmc_assert_equals(size_t, i, s->buffer[i]);

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(pop[count = 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert_equals(bool, false, s_pop(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(top, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));
        cmc_assert(s_pop(s));

        cmc_assert_equals(size_t, 247, s_top(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(top[count = 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 1));

        cmc_assert(s_pop(s));

        cmc_assert_equals(size_t, (size_t){0}, s_top(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(top[count = 1], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 10));
        cmc_assert_equals(size_t, 1, s_count(s));
        cmc_assert_equals(size_t, 10, s_top(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(contains, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 150; i++)
            cmc_assert(s_push(s, i));

        for (size_t i = 0; i < 150; i++)
            cmc_assert(s_contains(s, i, cmp));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(contains[count = 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert_equals(bool, false, s_contains(s, 10, cmp));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(empty, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_empty(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(empty[after_io], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 10));
        cmc_assert(s_pop(s));
        cmc_assert(s_empty(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(full, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < s_capacity(s); i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_full(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(full[capacity = 1], {
        stack *s = s_new(1);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 10));

        cmc_assert(s_full(s));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iter_alloc, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        stack_iter *iter = s_iter_new(s);

        cmc_assert_not_equals(ptr, NULL, iter);

        s_iter_free(iter);
        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iter_init[count = 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        stack_iter iter;

        s_iter_init(&iter, s);

        cmc_assert_equals(ptr, iter.target, s);
        cmc_assert_equals(size_t, iter.cursor, 0);
        cmc_assert(iter.start);
        cmc_assert(iter.end);

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iter_start[count = 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        stack_iter iter;

        s_iter_init(&iter, s);

        cmc_assert(s_iter_start(&iter));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iter_end[count = 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        stack_iter iter;

        s_iter_init(&iter, s);

        cmc_assert(s_iter_end(&iter));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iteration, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 1; i <= 200; i++)
            cmc_assert(s_push(s, i));

        size_t total = 0;

        for (stack_iter it = s->it_start(s); !s_iter_end(&it); s_iter_next(&it))
        {
            total += s_iter_value(&it);
        }

        cmc_assert_equals(size_t, 20100, total);

        for (stack_iter it = s->it_end(s); !s_iter_start(&it); s_iter_prev(&it))
        {
            total += s_iter_value(&it);
        }

        cmc_assert_equals(size_t, 40200, total);

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iteration[while loop], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 1; i <= 200; i++)
            cmc_assert(s_push(s, i));

        size_t total = 0;
        stack_iter it = s->it_start(s);

        do
        {
            total += s_iter_value(&it);
        } while (s_iter_next(&it));

        cmc_assert_equals(size_t, 20100, total);

        it = s->it_end(s);

        do
        {
            total += s_iter_value(&it);
        } while (s_iter_prev(&it));

        cmc_assert_equals(size_t, 40200, total);

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iteration[count = 1], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);
        cmc_assert(s_push(s, 10));

        size_t total = 0;

        for (stack_iter it = s->it_start(s); !s_iter_end(&it); s_iter_next(&it))
        {
            total += s_iter_value(&it);
        }

        for (stack_iter it = s->it_end(s); !s_iter_start(&it); s_iter_prev(&it))
        {
            total += s_iter_value(&it);
        }

        cmc_assert_equals(size_t, 20, total);

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iteration[count = capacity], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        while (!s_full(s))
            cmc_assert(s_push(s, 10));

        cmc_assert_equals(size_t, s_capacity(s), s_count(s));

        size_t total = 0;

        for (stack_iter it = s->it_start(s); !s_iter_end(&it); s_iter_next(&it))
        {
            total += s_iter_value(&it);
        }

        cmc_assert_equals(size_t, 1000, total);

        for (stack_iter it = s->it_end(s); !s_iter_start(&it); s_iter_prev(&it))
        {
            total += *s_iter_rvalue(&it);
        }

        cmc_assert_equals(size_t, 2000, total);

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iteration[to_start = it_start and to_end = it_end], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(s_push(s, i));

        stack_iter it1;
        stack_iter it2;

        memset(&it1, 0, sizeof(stack_iter));
        memset(&it2, 0, sizeof(stack_iter));

        s_iter_init(&it2, s);

        it1 = s->it_start(s);
        s_iter_to_start(&it2);

        cmc_assert_equals(ptr, it1.target, it2.target);
        cmc_assert_equals(size_t, it1.cursor, it2.cursor);
        cmc_assert_equals(bool, it1.start, it2.start);
        cmc_assert_equals(bool, it1.end, it2.end);

        it1 = s->it_end(s);
        s_iter_to_end(&it2);

        cmc_assert_equals(ptr, it1.target, it2.target);
        cmc_assert_equals(size_t, it1.cursor, it2.cursor);
        cmc_assert_equals(bool, it1.start, it2.start);
        cmc_assert_equals(bool, it1.end, it2.end);

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iteration[boundaries count = 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        stack_iter iter;
        s_iter_init(&iter, s);

        s_iter_to_end(&iter);

        cmc_assert_equals(bool, false, s_iter_next(&iter));

        s_iter_to_start(&iter);

        cmc_assert_equals(bool, false, s_iter_prev(&iter));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iteration[boundaries count > 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 200; i++)
            cmc_assert(s_push(s, i));

        stack_iter iter;
        s_iter_init(&iter, s);

        s_iter_to_end(&iter);

        cmc_assert_equals(bool, false, s_iter_next(&iter));

        s_iter_to_start(&iter);

        cmc_assert_equals(bool, false, s_iter_prev(&iter));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iter_value[count = 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        stack_iter iter;

        s_iter_init(&iter, s);

        cmc_assert_equals(size_t, (size_t){0}, s_iter_value(&iter));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(iter_rvalue[count = 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        stack_iter iter;

        s_iter_init(&iter, s);

        cmc_assert_equals(ptr, NULL, s_iter_rvalue(&iter));

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(index, {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 10; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_equals(size_t, 10, s_count(s));

        size_t total = 0;

        for (stack_iter it = s->it_start(s); false;)
        {
            cmc_assert_equals(size_t, 0, s_iter_index(&it));
        }

        for (stack_iter it = s->it_start(s); false;)
        {
            cmc_assert_equals(size_t, s_count(s) - 1, s_iter_index(&it));
        }

        for (stack_iter it = s->it_start(s); !s_iter_end(&it); s_iter_next(&it))
        {
            total += s_iter_index(&it);
        }

        cmc_assert_equals(size_t, 45, total);

        for (stack_iter it = s->it_end(s); !s_iter_start(&it); s_iter_prev(&it))
        {
            total += s_iter_index(&it);
        }

        cmc_assert_equals(size_t, 90, total);

        s_free(s, NULL);
    });

    CMC_CREATE_TEST(index[count = 0], {
        stack *s = s_new(100);

        cmc_assert_not_equals(ptr, NULL, s);

        stack_iter iter;
        s_iter_init(&iter, s);

        cmc_assert_equals(size_t, 0, s_iter_index(&iter));

        s_free(s, NULL);
    });
});
