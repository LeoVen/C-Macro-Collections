#ifndef CMC_TESTS_UNT_LIST_H
#define CMC_TESTS_UNT_LIST_H

#include "utl.h"

#define V size_t
#define PFX l
#define SNAME list
#include "cmc/list.h"

struct list_fval *l_fval = &(struct list_fval){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

CMC_CREATE_UNIT(CMCList, true, {
    CMC_CREATE_TEST(PFX##_new, {
        struct list *l = l_new(1000000, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert_not_equals(ptr, NULL, l->buffer);
        cmc_assert_equals(ptr, l->f_val, l_fval);
        cmc_assert_equals(size_t, 1000000, l_capacity(l));
        cmc_assert_equals(size_t, 0, l_count(l));

        l_free(l);

        // cap == 0
        l = l_new(0, l_fval);
        cmc_assert_equals(ptr, NULL, l);

        // cap == UINT64_MAX
        l = l_new(UINT64_MAX, l_fval);
        cmc_assert_equals(ptr, NULL, l);
    });

    CMC_CREATE_TEST(PFX##_new_custom, {
        // All NULL
        struct list *l = l_new_custom(100, NULL, NULL, NULL);
        cmc_assert_not_equals(ptr, NULL, l);
        l_free(l);

        l = l_new_custom(100, l_fval, custom_alloc, callbacks);
        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert_not_equals(ptr, NULL, l->buffer);
        cmc_assert_equals(ptr, l_fval, l->f_val);
        cmc_assert_equals(ptr, custom_alloc, l->alloc);
        cmc_assert_equals(ptr, callbacks, l->callbacks);
        cmc_assert_equals(size_t, 100, l_capacity(l));
        cmc_assert_equals(size_t, 0, l_count(l));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));
    });

    CMC_CREATE_TEST(PFX##_clear, {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 50; i++)
            l_push_back(l, i);

        cmc_assert_equals(size_t, 50, l_count(l));

        l_clear(l);

        cmc_assert_equals(size_t, 0, l_count(l));
        cmc_assert_equals(size_t, 100, l_capacity(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_front[count], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_front(l, i));

        cmc_assert_equals(size_t, 250, l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_front[capacity], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_front(l, i));

        cmc_assert_lesser_equals(size_t, l_capacity(l), l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_front[item preservation], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_front(l, i));

        for (size_t i = 0; i < l_count(l); i++)
            cmc_assert_equals(size_t, l_get(l, i), l_count(l) - i - 1);

        l_free(l);
    });

    CMC_CREATE_TEST(push_at[count], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        cmc_assert(l_push_back(l, 0));
        cmc_assert(l_push_back(l, 249));

        for (size_t i = 1; i < 249; i++)
            cmc_assert(l_push_at(l, i, l_count(l) - 1));

        cmc_assert_equals(size_t, 250, l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_at[capacity], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        cmc_assert(l_push_back(l, 0));
        cmc_assert(l_push_back(l, 249));

        for (size_t i = 1; i < 249; i++)
            cmc_assert(l_push_at(l, i, l_count(l) - 1));

        cmc_assert_lesser_equals(size_t, l_capacity(l), l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_at[item preservation], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        cmc_assert(l_push_back(l, 0));
        cmc_assert(l_push_back(l, 249));

        for (size_t i = 1; i < 249; i++)
            cmc_assert(l_push_at(l, i, l_count(l) - 1));

        for (size_t i = 0; i < l_count(l); i++)
            cmc_assert_equals(size_t, l_get(l, i), i);

        l_free(l);
    });

    CMC_CREATE_TEST(push_back[count], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_back(l, i));

        cmc_assert_equals(size_t, 250, l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_back[capacity], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_back(l, i));

        cmc_assert_lesser_equals(size_t, l_capacity(l), l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_back[item preservation], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_back(l, i));

        for (size_t i = 0; i < l_count(l); i++)
            cmc_assert_equals(size_t, l_get(l, i), i);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[count], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            l_push_back(l, i);

        cmc_assert(l_pop_front(l));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[item preservation], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            l_push_back(l, i);

        cmc_assert(l_pop_front(l));

        for (size_t i = 0; i < l_count(l); i++)
            cmc_assert_equals(size_t, l_get(l, i), i + 1);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[count = 0], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop_front(l));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[count = 1], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 10));
        cmc_assert(l_pop_front(l));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[buffer_clear], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_push_front(l, 2));

        cmc_assert(l_pop_front(l));

        cmc_assert_equals(size_t, 0, l->buffer[1]);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_at[count = 0], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop_at(l, 1));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_at[count = 1], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_pop_at(l, 0));

        cmc_assert_equals(size_t, 0, l->buffer[0]);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_at[buffer_clear], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_push_back(l, 2));
        cmc_assert(l_push_back(l, 3));
        cmc_assert(l_pop_at(l, 1));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_back[count = 0], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop_back(l));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_back[count = 1], {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 10));
        cmc_assert(l_pop_back(l));

        l_free(l);
    });

    CMC_CREATE_TEST(flags, {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // clear
        l->flag = CMC_FLAG_ERROR;
        l_clear(l);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // customize
        l->flag = CMC_FLAG_ERROR;
        l_customize(l, NULL, NULL);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // push_front
        l->flag = CMC_FLAG_ERROR;
        cmc_assert(l_push_front(l, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // push_at
        cmc_assert(!l_push_at(l, 5, 2));
        cmc_assert_equals(int32_t, CMC_FLAG_RANGE, l_flag(l));
        cmc_assert(l_push_at(l, 5, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // push_back
        l->flag = CMC_FLAG_ERROR;
        cmc_assert(l_push_back(l, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // pop_front
        l->flag = CMC_FLAG_ERROR;
        cmc_assert(l_pop_front(l));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        l_clear(l);
        cmc_assert(!l_pop_front(l));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, l_flag(l));

        // pop_back
        cmc_assert(l_push_front(l, 10));
        l->flag = CMC_FLAG_ERROR;
        cmc_assert(l_pop_back(l));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        cmc_assert(!l_pop_front(l));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, l_flag(l));

        // pop_at
        l->flag = CMC_FLAG_ERROR;
        cmc_assert(!l_pop_at(l, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, l_flag(l));

        cmc_assert(l_push_back(l, 10));
        cmc_assert(!l_pop_at(l, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_RANGE, l_flag(l));

        cmc_assert(l_pop_at(l, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // seq_push_front
        cmc_assert(!l_seq_push_front(l, NULL, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_INVALID, l_flag(l));

        cmc_assert(l_seq_push_front(l, (size_t[2]){ 1, 2 }, 2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // seq_push_at
        cmc_assert(!l_seq_push_at(l, NULL, 0, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_INVALID, l_flag(l));

        cmc_assert(!l_seq_push_at(l, (size_t[2]){ 1, 2 }, 2, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_RANGE, l_flag(l));

        cmc_assert(l_seq_push_at(l, (size_t[2]){ 1, 2 }, 2, l_count(l) - 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // seq_push_back
        cmc_assert(!l_seq_push_back(l, NULL, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_INVALID, l_flag(l));

        cmc_assert(l_seq_push_back(l, (size_t[2]){ 1, 2 }, 2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // seq_pop_at
        cmc_assert(!l_seq_pop_at(l, 10, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_INVALID, l_flag(l));

        cmc_assert(!l_seq_pop_at(l, 100, 200));
        cmc_assert_equals(int32_t, CMC_FLAG_RANGE, l_flag(l));

        cmc_assert(l_seq_pop_at(l, 1, l_count(l) - 2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // seq_sublist
        struct list *l2 = l_seq_sublist(l, 10, 1);
        cmc_assert_equals(ptr, NULL, l2);
        cmc_assert_equals(int32_t, CMC_FLAG_INVALID, l_flag(l));

        l2 = l_seq_sublist(l, 100, 200);
        cmc_assert_equals(ptr, NULL, l2);
        cmc_assert_equals(int32_t, CMC_FLAG_RANGE, l_flag(l));

        l2 = l_seq_sublist(l, 0, l_count(l) - 1);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l2));

        l_clear(l);

        // l_front
        l_front(l);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, l_flag(l));

        cmc_assert(l_push_back(l, 10));
        l->flag = CMC_FLAG_ERROR;
        l_front(l);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        l_clear(l);

        // l_back
        l_back(l);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, l_flag(l));

        cmc_assert(l_push_back(l, 10));
        l->flag = CMC_FLAG_ERROR;
        l_back(l);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        l_clear(l);

        // get, get_ref
        l_get(l, 0);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, l_flag(l));
        l->flag = CMC_FLAG_ERROR;
        l_get_ref(l, 0);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, l_flag(l));

        cmc_assert(l_push_back(l, 10));

        l_get(l, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_RANGE, l_flag(l));
        l->flag = CMC_FLAG_ERROR;
        l_get_ref(l, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_RANGE, l_flag(l));

        l_get(l, 0);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));
        l->flag = CMC_FLAG_ERROR;
        l_get_ref(l, 0);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // index_of
        l->flag = CMC_FLAG_ERROR;
        l_index_of(l, 10, false);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // contains
        l->flag = CMC_FLAG_ERROR;
        cmc_assert(l_contains(l, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));

        // copy_of
        l->flag = CMC_FLAG_ERROR;
        struct list *l3 = l_copy_of(l);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l3));

        // equals
        l->flag = CMC_FLAG_ERROR;
        l3->flag = CMC_FLAG_ERROR;
        cmc_assert(l_equals(l, l3));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, l_flag(l3));

        l_free(l);
        l_free(l2);
        l_free(l3);
    });
})

CMC_CREATE_UNIT(CMCListIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_start(l);

        cmc_assert_equals(ptr, l, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(l_iter_at_start(&it));
        cmc_assert(l_iter_at_end(&it));

        cmc_assert(l_push_front(l, 1));
        cmc_assert(l_push_front(l, 2));

        it = l_iter_start(l);

        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, false, it.end);

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_end(l);

        cmc_assert_equals(ptr, l, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(l_iter_at_start(&it));
        cmc_assert(l_iter_at_end(&it));

        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_push_back(l, 2));
        cmc_assert(l_push_back(l, 3));

        it = l_iter_end(l);

        cmc_assert_equals(size_t, l->count - 1, it.cursor);
        cmc_assert_equals(bool, false, it.start);

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_start(l);

        // Empty checks
        cmc_assert(l_iter_at_start(&it));
        it = l_iter_end(l);
        cmc_assert(l_iter_at_start(&it));

        // Non-empty checks
        cmc_assert(l_push_back(l, 1));
        it = l_iter_end(l);
        cmc_assert(!l_iter_at_start(&it));
        it = l_iter_start(l);
        cmc_assert(l_iter_at_start(&it));

        l_clear(l);

        cmc_assert(l_push_front(l, 1));
        it = l_iter_end(l);
        cmc_assert(!l_iter_at_start(&it));
        it = l_iter_start(l);
        cmc_assert(l_iter_at_start(&it));

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_start(l);

        // Empty check
        cmc_assert(l_iter_at_end(&it));
        it = l_iter_end(l);
        cmc_assert(l_iter_at_end(&it));

        // Non-empty checks
        cmc_assert(l_push_back(l, 1));
        it = l_iter_end(l);
        cmc_assert(l_iter_at_end(&it));
        it = l_iter_start(l);
        cmc_assert(!l_iter_at_end(&it));

        l_clear(l);

        cmc_assert(l_push_front(l, 1));
        it = l_iter_end(l);
        cmc_assert(l_iter_at_end(&it));
        it = l_iter_start(l);
        cmc_assert(!l_iter_at_end(&it));

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_start(l);

        cmc_assert(!l_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
        {
            if (i % 2 == 0)
                l_push_back(l, i);
            else
                l_push_front(l, i);
        }

        cmc_assert_equals(size_t, 100, l->count);

        it = l_iter_end(l);

        cmc_assert(!l_iter_at_start(&it));
        cmc_assert(l_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, l_iter_value(&it));

        cmc_assert(l_iter_to_start(&it));

        cmc_assert(l_iter_at_start(&it));
        cmc_assert(!l_iter_at_end(&it));

        cmc_assert_equals(size_t, 99, l_iter_value(&it));

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_end(l);

        cmc_assert(!l_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
        {
            if (i % 2 == 0)
                l_push_back(l, i);
            else
                l_push_front(l, i);
        }

        it = l_iter_start(l);

        cmc_assert(l_iter_at_start(&it));
        cmc_assert(!l_iter_at_end(&it));

        cmc_assert_equals(size_t, 99, l_iter_value(&it));

        cmc_assert(l_iter_to_end(&it));

        cmc_assert(!l_iter_at_start(&it));
        cmc_assert(l_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, l_iter_value(&it));

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_start(l);

        cmc_assert(!l_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
                l_push_back(l, i);
            else
                l_push_front(l, i);
        }

        size_t sum = 0;
        for (it = l_iter_start(l); !l_iter_at_end(&it); l_iter_next(&it))
        {
            sum += l_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        l_iter_to_start(&it);
        do
        {
            sum += l_iter_value(&it);
        } while (l_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_end(l);

        cmc_assert(!l_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
                l_push_back(l, i);
            else
                l_push_front(l, i);
        }

        size_t sum = 0;
        for (it = l_iter_end(l); !l_iter_at_start(&it); l_iter_prev(&it))
        {
            sum += l_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        l_iter_to_end(&it);
        do
        {
            sum += l_iter_value(&it);
        } while (l_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_start(l);

        cmc_assert(!l_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
        {
            if (i % 2 == 0)
                l_push_back(l, i); // will sum all even numbers
            else
                l_push_back(l, 0);
        }

        it = l_iter_start(l);

        cmc_assert(!l_iter_advance(&it, 0));
        cmc_assert(!l_iter_advance(&it, l->count));

        size_t sum = 0;
        for (it = l_iter_start(l);;)
        {
            sum += l_iter_value(&it);

            if (!l_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        l_iter_to_start(&it);
        cmc_assert(l_iter_advance(&it, l->count - 1));

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_end(l);

        cmc_assert(!l_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
        {
            if (i % 2 == 0)
                l_push_front(l, i); // will sum all even numbers
            else
                l_push_front(l, 0);
        }

        it = l_iter_end(l);

        cmc_assert(!l_iter_rewind(&it, 0));
        cmc_assert(!l_iter_rewind(&it, l->count));

        size_t sum = 0;
        for (it = l_iter_end(l);;)
        {
            sum += l_iter_value(&it);

            if (!l_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        l_iter_to_end(&it);
        cmc_assert(l_iter_rewind(&it, l->count - 1));

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_end(l);
        cmc_assert(!l_iter_go_to(&it, 0));

        it = l_iter_start(l);
        cmc_assert(!l_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            l_push_back(l, i);

        it = l_iter_start(l);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            l_iter_go_to(&it, i);

            sum += l_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(l_iter_go_to(&it, i - 1));

            sum += l_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 0; i < 1001; i += 100)
        {
            cmc_assert(l_iter_go_to(&it, i));
            cmc_assert_equals(size_t, i, l_iter_index(&it));

            sum += l_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_end(l);

        cmc_assert_equals(size_t, (size_t){ 0 }, l_iter_value(&it));

        cmc_assert(l_push_back(l, 10));

        it = l_iter_start(l);

        cmc_assert_equals(size_t, 10, l_iter_value(&it));

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_rvalue(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        struct list_iter it = l_iter_end(l);

        cmc_assert_equals(ptr, NULL, l_iter_rvalue(&it));

        cmc_assert(l_push_back(l, 10));

        it = l_iter_start(l);

        cmc_assert_not_equals(ptr, NULL, l_iter_rvalue(&it));
        cmc_assert_equals(size_t, 10, *l_iter_rvalue(&it));

        l_free(l);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct list *l = l_new(100, l_fval);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i <= 1000; i++)
            l_push_back(l, i);

        struct list_iter it = l_iter_start(l);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, l_iter_index(&it));
            l_iter_next(&it);
        }

        it = l_iter_end(l);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, l_iter_index(&it));
            l_iter_prev(&it);
        }

        l_free(l);
    });
});

#endif /* CMC_TESTS_UNT_LIST_H */
