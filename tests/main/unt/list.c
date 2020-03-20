#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/list.h>

CMC_GENERATE_LIST(l, list, size_t)

struct list_ftab_val *l_ftab_val = &(struct list_ftab_val){ .cmp = cmp,
                                                            .cpy = copy,
                                                            .str = str,
                                                            .free = custom_free,
                                                            .hash = hash,
                                                            .pri = pri };

CMC_CREATE_UNIT(list_test, true, {
    CMC_CREATE_TEST(new, {
        struct list *l = l_new(1000000, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert_not_equals(ptr, NULL, l->buffer);
        cmc_assert_equals(size_t, 1000000, l_capacity(l));
        cmc_assert_equals(size_t, 0, l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(new[capacity = 0], {
        struct list *l = l_new(0, l_ftab_val);

        cmc_assert_equals(ptr, NULL, l);
    });

    CMC_CREATE_TEST(new[capacity = UINT64_MAX], {
        struct list *l = l_new(UINT64_MAX, l_ftab_val);

        cmc_assert_equals(ptr, NULL, l);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 50; i++)
            l_push_back(l, i);

        cmc_assert_equals(size_t, 50, l_count(l));

        l_clear(l);

        cmc_assert_equals(size_t, 0, l_count(l));
        cmc_assert_equals(size_t, 100, l_capacity(l));

        l_free(l);
    });

    CMC_CREATE_TEST(buffer_growth[capacity = 1], {
        struct list *l = l_new(1, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 100; i++)
            l_push_back(l, i);

        cmc_assert_equals(size_t, 100, l_count(l));

        for (size_t i = 0; i < 100; i++)
            cmc_assert_equals(size_t, i, l_get(l, i));

        l_free(l);
    });

    CMC_CREATE_TEST(push_front[count], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_front(l, i));

        cmc_assert_equals(size_t, 250, l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_front[capacity], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_front(l, i));

        cmc_assert_lesser_equals(size_t, l_capacity(l), l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_front[item preservation], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_front(l, i));

        for (size_t i = 0; i < l_count(l); i++)
            cmc_assert_equals(size_t, l_get(l, i), l_count(l) - i - 1);

        l_free(l);
    });

    CMC_CREATE_TEST(push_at[count], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        cmc_assert(l_push_back(l, 0));
        cmc_assert(l_push_back(l, 249));

        for (size_t i = 1; i < 249; i++)
            cmc_assert(l_push_at(l, i, l_count(l) - 1));

        cmc_assert_equals(size_t, 250, l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_at[capacity], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        cmc_assert(l_push_back(l, 0));
        cmc_assert(l_push_back(l, 249));

        for (size_t i = 1; i < 249; i++)
            cmc_assert(l_push_at(l, i, l_count(l) - 1));

        cmc_assert_lesser_equals(size_t, l_capacity(l), l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_at[item preservation], {
        struct list *l = l_new(100, l_ftab_val);

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
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_back(l, i));

        cmc_assert_equals(size_t, 250, l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_back[capacity], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_back(l, i));

        cmc_assert_lesser_equals(size_t, l_capacity(l), l_count(l));

        l_free(l);
    });

    CMC_CREATE_TEST(push_back[item preservation], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            cmc_assert(l_push_back(l, i));

        for (size_t i = 0; i < l_count(l); i++)
            cmc_assert_equals(size_t, l_get(l, i), i);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[count], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            l_push_back(l, i);

        cmc_assert(l_pop_front(l));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[item preservation], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);

        for (size_t i = 0; i < 250; i++)
            l_push_back(l, i);

        cmc_assert(l_pop_front(l));

        for (size_t i = 0; i < l_count(l); i++)
            cmc_assert_equals(size_t, l_get(l, i), i + 1);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[count = 0], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop_front(l));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[count = 1], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 10));
        cmc_assert(l_pop_front(l));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_front[buffer_clear], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_push_front(l, 2));

        cmc_assert(l_pop_front(l));

        cmc_assert_equals(size_t, 0, l->buffer[1]);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_at[count = 0], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop_at(l, 1));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_at[count = 1], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_pop_at(l, 0));

        cmc_assert_equals(size_t, 0, l->buffer[0]);

        l_free(l);
    });

    CMC_CREATE_TEST(pop_at[buffer_clear], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 1));
        cmc_assert(l_push_back(l, 2));
        cmc_assert(l_push_back(l, 3));
        cmc_assert(l_pop_at(l, 1));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_back[count = 0], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(!l_pop_back(l));

        l_free(l);
    });

    CMC_CREATE_TEST(pop_back[count = 1], {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert(l_push_back(l, 10));
        cmc_assert(l_pop_back(l));

        l_free(l);
    });

    CMC_CREATE_TEST(flags, {
        struct list *l = l_new(100, l_ftab_val);

        cmc_assert_not_equals(ptr, NULL, l);
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // clear
        l->flag = cmc_flags.ERROR;
        l_clear(l);
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // customize
        l->flag = cmc_flags.ERROR;
        l_customize(l, NULL, NULL);
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // push_front
        l->flag = cmc_flags.ERROR;
        cmc_assert(l_push_front(l, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // push_at
        cmc_assert(!l_push_at(l, 5, 2));
        cmc_assert_equals(int32_t, cmc_flags.RANGE, l_flag(l));
        cmc_assert(l_push_at(l, 5, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // push_back
        l->flag = cmc_flags.ERROR;
        cmc_assert(l_push_back(l, 0));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // pop_front
        l->flag = cmc_flags.ERROR;
        cmc_assert(l_pop_front(l));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        l_clear(l);
        cmc_assert(!l_pop_front(l));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, l_flag(l));

        // pop_back
        cmc_assert(l_push_front(l, 10));
        l->flag = cmc_flags.ERROR;
        cmc_assert(l_pop_back(l));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        cmc_assert(!l_pop_front(l));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, l_flag(l));

        // pop_at
        l->flag = cmc_flags.ERROR;
        cmc_assert(!l_pop_at(l, 0));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, l_flag(l));

        cmc_assert(l_push_back(l, 10));
        cmc_assert(!l_pop_at(l, 1));
        cmc_assert_equals(int32_t, cmc_flags.RANGE, l_flag(l));

        cmc_assert(l_pop_at(l, 0));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // seq_push_front
        cmc_assert(!l_seq_push_front(l, NULL, 0));
        cmc_assert_equals(int32_t, cmc_flags.INVALID, l_flag(l));

        cmc_assert(l_seq_push_front(l, (size_t[2]){ 1, 2 }, 2));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // seq_push_at
        cmc_assert(!l_seq_push_at(l, NULL, 0, 0));
        cmc_assert_equals(int32_t, cmc_flags.INVALID, l_flag(l));

        cmc_assert(!l_seq_push_at(l, (size_t[2]){ 1, 2 }, 2, 10));
        cmc_assert_equals(int32_t, cmc_flags.RANGE, l_flag(l));

        cmc_assert(l_seq_push_at(l, (size_t[2]){ 1, 2 }, 2, l_count(l) - 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // seq_push_back
        cmc_assert(!l_seq_push_back(l, NULL, 0));
        cmc_assert_equals(int32_t, cmc_flags.INVALID, l_flag(l));

        cmc_assert(l_seq_push_back(l, (size_t[2]){ 1, 2 }, 2));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // seq_pop_at
        cmc_assert(!l_seq_pop_at(l, 10, 1));
        cmc_assert_equals(int32_t, cmc_flags.INVALID, l_flag(l));

        cmc_assert(!l_seq_pop_at(l, 100, 200));
        cmc_assert_equals(int32_t, cmc_flags.RANGE, l_flag(l));

        cmc_assert(l_seq_pop_at(l, 1, l_count(l) - 2));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // seq_sublist
        struct list *l2 = l_seq_sublist(l, 10, 1);
        cmc_assert_equals(ptr, NULL, l2);
        cmc_assert_equals(int32_t, cmc_flags.INVALID, l_flag(l));

        l2 = l_seq_sublist(l, 100, 200);
        cmc_assert_equals(ptr, NULL, l2);
        cmc_assert_equals(int32_t, cmc_flags.RANGE, l_flag(l));

        l2 = l_seq_sublist(l, 0, l_count(l) - 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l2));

        l_clear(l);

        // l_front
        l_front(l);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, l_flag(l));

        cmc_assert(l_push_back(l, 10));
        l->flag = cmc_flags.ERROR;
        l_front(l);
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        l_clear(l);

        // l_back
        l_back(l);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, l_flag(l));

        cmc_assert(l_push_back(l, 10));
        l->flag = cmc_flags.ERROR;
        l_back(l);
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        l_clear(l);

        // get, get_ref
        l_get(l, 0);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, l_flag(l));
        l->flag = cmc_flags.ERROR;
        l_get_ref(l, 0);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, l_flag(l));

        cmc_assert(l_push_back(l, 10));

        l_get(l, 1);
        cmc_assert_equals(int32_t, cmc_flags.RANGE, l_flag(l));
        l->flag = cmc_flags.ERROR;
        l_get_ref(l, 1);
        cmc_assert_equals(int32_t, cmc_flags.RANGE, l_flag(l));

        l_get(l, 0);
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));
        l->flag = cmc_flags.ERROR;
        l_get_ref(l, 0);
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // index_of
        l->flag = cmc_flags.ERROR;
        l_index_of(l, 10, false);
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // contains
        l->flag = cmc_flags.ERROR;
        cmc_assert(l_contains(l, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));

        // copy_of
        l->flag = cmc_flags.ERROR;
        struct list *l3 = l_copy_of(l);
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l3));

        // equals
        l->flag = cmc_flags.ERROR;
        l3->flag = cmc_flags.ERROR;
        cmc_assert(l_equals(l, l3));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l));
        cmc_assert_equals(int32_t, cmc_flags.OK, l_flag(l3));

        l_free(l);
        l_free(l2);
        l_free(l3);
    });

    CMC_CREATE_TEST(callbacks, {
        struct list *l = l_new_custom(100, l_ftab_val, NULL, callbacks);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(l_push_front(l, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(l_push_back(l, 10));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(l_push_at(l, 10, 1));
        cmc_assert_equals(int32_t, 3, total_create);

        cmc_assert(l_pop_at(l, 1));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(l_pop_front(l));
        cmc_assert_equals(int32_t, 2, total_delete);

        cmc_assert(l_pop_back(l));
        cmc_assert_equals(int32_t, 3, total_delete);

        cmc_assert(l_push_back(l, 10));
        cmc_assert_equals(int32_t, 4, total_create);

        cmc_assert_equals(size_t, 10, l_front(l));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert_equals(size_t, 10, l_back(l));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert_equals(size_t, 10, l_get(l, 0));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert_not_equals(ptr, NULL, l_get_ref(l, 0));
        cmc_assert_equals(int32_t, 4, total_read);

        cmc_assert(l_contains(l, 10));
        cmc_assert_equals(int32_t, 5, total_read);

        cmc_assert(l_seq_push_front(l, (size_t[]) {7, 8, 9}, 3));
        cmc_assert_equals(int32_t, 5, total_create);

        cmc_assert(l_seq_push_back(l, (size_t[]) {14, 15, 16}, 3));
        cmc_assert_equals(int32_t, 6, total_create);

        cmc_assert(l_seq_push_at(l, (size_t[]) {11, 12, 13}, 3, 4));
        cmc_assert_equals(int32_t, 7, total_create);

        cmc_assert(l_seq_pop_at(l, 1, 4));
        cmc_assert_equals(int32_t, 4, total_delete);

        struct list *l2 = l_seq_sublist(l, 0, 4);
        cmc_assert_equals(int32_t, 5, total_delete);

        l_free(l);
        l_free(l2);
    });
})
