#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "../src/stack.c"

struct stack_fval *s_fval = &(struct stack_fval){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

CMC_CREATE_UNIT(Stack, true, {
    CMC_CREATE_TEST(new, {
        struct stack *s = s_new(1000000, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);
        cmc_assert_not_equals(ptr, NULL, s->buffer);
        cmc_assert_equals(size_t, 1000000, s_capacity(s));
        cmc_assert_equals(size_t, 0, s_count(s));
        cmc_assert_not_equals(ptr, NULL, s->buffer);

        s_free(s);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        struct stack *s = s_new(0, s_fval);

        cmc_assert_equals(ptr, NULL, s);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        struct stack *s = s_new(UINT64_MAX, s_fval);

        cmc_assert_equals(ptr, NULL, s);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_equals(size_t, 50, s_count(s));

        s_clear(s);

        cmc_assert_equals(size_t, 0, s_count(s));
        cmc_assert_equals(size_t, 100, s_capacity(s));

        s_free(s);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct stack *s = s_new(1, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 50; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_equals(size_t, 50, s_count(s));
        cmc_assert_lesser_equals(size_t, s_capacity(s), s_count(s));

        s_free(s);
    });

    CMC_CREATE_TEST(push[count], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_equals(size_t, 250, s_count(s));

        s_free(s);
    });

    CMC_CREATE_TEST(push[capacity], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert_lesser_equals(size_t, s_capacity(s), s_count(s));

        s_free(s);
    });

    CMC_CREATE_TEST(push[item preservation], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        for (size_t i = 0; i < s_count(s); i++)
            cmc_assert_equals(size_t, i, s->buffer[i]);

        s_free(s);
    });

    CMC_CREATE_TEST(pop[count], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));

        cmc_assert_equals(size_t, 249, s_count(s));

        s_free(s);
    });

    CMC_CREATE_TEST(pop[capacity], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));

        cmc_assert_lesser_equals(size_t, s_capacity(s), s_count(s));

        s_free(s);
    });

    CMC_CREATE_TEST(pop[item preservation], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));
        cmc_assert(s_pop(s));

        for (size_t i = 0; i < s_count(s); i++)
            cmc_assert_equals(size_t, i, s->buffer[i]);

        s_free(s);
    });

    CMC_CREATE_TEST(pop[count = 0], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert_equals(bool, false, s_pop(s));

        s_free(s);
    });

    CMC_CREATE_TEST(top, {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_pop(s));
        cmc_assert(s_pop(s));

        cmc_assert_equals(size_t, 247, s_top(s));

        s_free(s);
    });

    CMC_CREATE_TEST(top[count = 0], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 1));

        cmc_assert(s_pop(s));

        cmc_assert_equals(size_t, (size_t){ 0 }, s_top(s));

        s_free(s);
    });

    CMC_CREATE_TEST(top[count = 1], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 10));
        cmc_assert_equals(size_t, 1, s_count(s));
        cmc_assert_equals(size_t, 10, s_top(s));

        s_free(s);
    });

    CMC_CREATE_TEST(contains, {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < 150; i++)
            cmc_assert(s_push(s, i));

        for (size_t i = 0; i < 150; i++)
            cmc_assert(s_contains(s, i));

        s_free(s);
    });

    CMC_CREATE_TEST(contains[count = 0], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert_equals(bool, false, s_contains(s, 10));

        s_free(s);
    });

    CMC_CREATE_TEST(empty, {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_empty(s));

        s_free(s);
    });

    CMC_CREATE_TEST(empty[after_io], {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 10));
        cmc_assert(s_pop(s));
        cmc_assert(s_empty(s));

        s_free(s);
    });

    CMC_CREATE_TEST(full, {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i < s_capacity(s); i++)
            cmc_assert(s_push(s, i));

        cmc_assert(s_full(s));

        s_free(s);
    });

    CMC_CREATE_TEST(full[capacity = 1], {
        struct stack *s = s_new(1, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        cmc_assert(s_push(s, 10));

        cmc_assert(s_full(s));

        s_free(s);
    });

    CMC_CREATE_TEST(flags, {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, s_flag(s));

        // clear
        s->flag = CMC_FLAG_ERROR;
        s_clear(s);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, s_flag(s));

        // customize
        s->flag = CMC_FLAG_ERROR;
        s_customize(s, NULL, NULL);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, s_flag(s));

        // push
        s->flag = CMC_FLAG_ERROR;
        cmc_assert(s_push(s, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, s_flag(s));

        // pop
        s->flag = CMC_FLAG_ERROR;
        cmc_assert(s_pop(s));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, s_flag(s));

        cmc_assert(!s_pop(s));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, s_flag(s));

        // top
        s->flag = CMC_FLAG_ERROR;
        s_top(s);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, s_flag(s));

        cmc_assert(s_push(s, 1));
        s_top(s);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, s_flag(s));

        // contains
        s->flag = CMC_FLAG_ERROR;
        cmc_assert(s_contains(s, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, s_flag(s));

        // copy_of
        s->flag = CMC_FLAG_ERROR;
        struct stack *s2 = s_copy_of(s);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, s_flag(s));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, s_flag(s2));

        // equals
        s->flag = CMC_FLAG_ERROR;
        s2->flag = CMC_FLAG_ERROR;
        cmc_assert(s_equals(s, s2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, s_flag(s));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, s_flag(s2));

        s_free(s);
        s_free(s2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct stack *s = s_new_custom(100, s_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, s);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(s_push(s, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(s_pop(s));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(s_push(s, 1));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert_equals(size_t, 1, s_top(s));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert(s_contains(s, 1));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert(s_resize(s, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(s_resize(s, 10));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 2, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        s_customize(s, NULL, NULL);

        s_clear(s);
        cmc_assert(s_push(s, 10));
        cmc_assert(s_pop(s));
        cmc_assert(s_push(s, 1));
        cmc_assert_equals(size_t, 1, s_top(s));
        cmc_assert(s_contains(s, 1));
        cmc_assert(s_resize(s, 1000));
        cmc_assert(s_resize(s, 10));

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 2, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, s->callbacks);

        s_free(s);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});

CMC_CREATE_UNIT(StackIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_start(s);

        cmc_assert_equals(ptr, s, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(s_iter_at_start(&it));
        cmc_assert(s_iter_at_end(&it));

        cmc_assert(s_push(s, 1));
        cmc_assert(s_push(s, 2));

        it = s_iter_start(s);

        cmc_assert_equals(size_t, s->count - 1, it.cursor);
        cmc_assert_equals(bool, false, it.end);

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_end(s);

        cmc_assert_equals(ptr, s, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(s_iter_at_start(&it));
        cmc_assert(s_iter_at_end(&it));

        cmc_assert(s_push(s, 1));
        cmc_assert(s_push(s, 2));
        cmc_assert(s_push(s, 3));

        it = s_iter_end(s);

        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, false, it.start);

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_start(s);

        // Empty checks
        cmc_assert(s_iter_at_start(&it));
        it = s_iter_end(s);
        cmc_assert(s_iter_at_start(&it));

        // Non-empty checks
        cmc_assert(s_push(s, 1));
        it = s_iter_end(s);
        cmc_assert(!s_iter_at_start(&it));
        it = s_iter_start(s);
        cmc_assert(s_iter_at_start(&it));

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_start(s);

        // Empty check
        cmc_assert(s_iter_at_end(&it));
        it = s_iter_end(s);
        cmc_assert(s_iter_at_end(&it));

        // Non-empty checks
        cmc_assert(s_push(s, 1));
        it = s_iter_end(s);
        cmc_assert(s_iter_at_end(&it));
        it = s_iter_start(s);
        cmc_assert(!s_iter_at_end(&it));

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_start(s);

        cmc_assert(!s_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
            s_push(s, i);

        cmc_assert_equals(size_t, 100, s->count);

        it = s_iter_end(s);

        cmc_assert(!s_iter_at_start(&it));
        cmc_assert(s_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, s_iter_value(&it));

        cmc_assert(s_iter_to_start(&it));

        cmc_assert(s_iter_at_start(&it));
        cmc_assert(!s_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, s_iter_value(&it));

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_end(s);

        cmc_assert(!s_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
            s_push(s, i);

        it = s_iter_start(s);

        cmc_assert(s_iter_at_start(&it));
        cmc_assert(!s_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, s_iter_value(&it));

        cmc_assert(s_iter_to_end(&it));

        cmc_assert(!s_iter_at_start(&it));
        cmc_assert(s_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, s_iter_value(&it));

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_start(s);

        cmc_assert(!s_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
            s_push(s, i);

        size_t sum = 0;
        for (it = s_iter_start(s); !s_iter_at_end(&it); s_iter_next(&it))
        {
            sum += s_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        s_iter_to_start(&it);
        do
        {
            sum += s_iter_value(&it);
        } while (s_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_end(s);

        cmc_assert(!s_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
            s_push(s, i);

        size_t sum = 0;
        for (it = s_iter_end(s); !s_iter_at_start(&it); s_iter_prev(&it))
        {
            sum += s_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        s_iter_to_end(&it);
        do
        {
            sum += s_iter_value(&it);
        } while (s_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_start(s);

        cmc_assert(!s_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            s_push(s, i);

        it = s_iter_start(s);

        cmc_assert(!s_iter_advance(&it, 0));
        cmc_assert(!s_iter_advance(&it, s->count));

        size_t sum = 0;
        for (it = s_iter_start(s);;)
        {
            sum += s_iter_value(&it);

            if (!s_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        s_iter_to_start(&it);
        cmc_assert(s_iter_advance(&it, s->count - 1));

        // sum (1000, 990, 980, 970, ..., 910, 900)
        sum = 0;
        it = s_iter_start(s);
        for (size_t val = s_iter_value(&it); val > 900; s_iter_advance(&it, 10))
        {
            val = s_iter_value(&it);
            sum += val;
        }

        cmc_assert_equals(size_t, 10450, sum);

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_end(s);

        cmc_assert(!s_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            s_push(s, i);

        it = s_iter_end(s);

        cmc_assert(!s_iter_rewind(&it, 0));
        cmc_assert(!s_iter_rewind(&it, s->count));

        size_t sum = 0;
        for (it = s_iter_end(s);;)
        {
            sum += s_iter_value(&it);

            if (!s_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        s_iter_to_end(&it);
        cmc_assert(s_iter_rewind(&it, s->count - 1));

        // sum (0, 10, 20, ..., 90, 100)
        sum = 0;
        it = s_iter_end(s);
        for (size_t val = s_iter_value(&it); val < 100; s_iter_rewind(&it, 10))
        {
            val = s_iter_value(&it);
            sum += val;
        }

        cmc_assert_equals(size_t, 550, sum);

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_end(s);
        cmc_assert(!s_iter_go_to(&it, 0));

        it = s_iter_start(s);
        cmc_assert(!s_iter_go_to(&it, 0));

        for (size_t i = 1001; i > 0; i--)
            s_push(s, i - 1);

        it = s_iter_start(s);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            s_iter_go_to(&it, i);

            sum += s_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(s_iter_go_to(&it, i - 1));

            sum += s_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1000; i > 0; i -= 100)
        {
            cmc_assert(s_iter_go_to(&it, 1000 - i));
            cmc_assert_equals(size_t, i, s_iter_index(&it));

            sum += s_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_end(s);

        cmc_assert_equals(size_t, (size_t){ 0 }, s_iter_value(&it));

        cmc_assert(s_push(s, 10));

        it = s_iter_start(s);

        cmc_assert_equals(size_t, 10, s_iter_value(&it));

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_rvalue(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        struct stack_iter it = s_iter_end(s);

        cmc_assert_equals(ptr, NULL, s_iter_rvalue(&it));

        cmc_assert(s_push(s, 10));

        it = s_iter_start(s);

        cmc_assert_not_equals(ptr, NULL, s_iter_rvalue(&it));
        cmc_assert_equals(size_t, 10, *s_iter_rvalue(&it));

        s_free(s);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct stack *s = s_new(100, s_fval);

        cmc_assert_not_equals(ptr, NULL, s);

        for (size_t i = 0; i <= 1000; i++)
            s_push(s, i);

        struct stack_iter it = s_iter_start(s);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, s_iter_index(&it));
            s_iter_next(&it);
        }

        it = s_iter_end(s);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, s_iter_index(&it));
            s_iter_prev(&it);
        }

        s_free(s);
    });
});

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = Stack() + StackIter();

    printf(" +---------------------------------------------------------------+");
    printf("\n");
    printf(" | Stack Suit : %-48s |\n", result == 0 ? "PASSED" : "FAILED");
    printf(" +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif
