#include "utl.c"
#include "utl/assert.h"
#include "utl/log.h"
#include "utl/test.h"

#include "../src/linkedlist.c"

struct linkedlist_fval *ll_fval =
    &(struct linkedlist_fval){ .cmp = cmp,
                                   .cpy = copy,
                                   .str = str,
                                   .free = custom_free,
                                   .hash = hash,
                                   .pri = pri };

CMC_CREATE_UNIT(linkedlist_test, true, {
    CMC_CREATE_TEST(new, {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);
        cmc_assert_equals(size_t, 0, ll_count(ll));
        cmc_assert_equals(ptr, NULL, ll->head);
        cmc_assert_equals(ptr, NULL, ll->tail);

        ll_free(ll);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        for (size_t i = 0; i < 50; i++)
            ll_push_back(ll, i);

        cmc_assert_equals(size_t, 50, ll_count(ll));

        ll_clear(ll);

        cmc_assert_equals(size_t, 0, ll_count(ll));

        ll_free(ll);
    });

    CMC_CREATE_TEST(clear[count = 0], {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        ll_clear(ll);

        cmc_assert_equals(size_t, 0, ll_count(ll));

        ll_free(ll);
    });

    CMC_CREATE_TEST(push_front[count], {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        ll_push_front(ll, 1);

        cmc_assert_not_equals(ptr, NULL, ll->head);
        cmc_assert_not_equals(ptr, NULL, ll->tail);
        cmc_assert_equals(size_t, 1, ll_count(ll));

        ll_free(ll);
    });

    CMC_CREATE_TEST(push_front[item preservation], {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        for (size_t i = 0; i < 200; i++)
            ll_push_front(ll, i);

        for (size_t i = 0; i < ll_count(ll); i++)
            cmc_assert_equals(size_t, ll_get(ll, i), ll_count(ll) - i - 1);

        ll_free(ll);
    });

    CMC_CREATE_TEST(flags, {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        // clear
        ll->flag = cmc_flags.ERROR;
        ll_clear(ll);
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        // customize
        ll->flag = cmc_flags.ERROR;
        ll_customize(ll, NULL, NULL);
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        // push_front
        ll->flag = cmc_flags.ERROR;
        cmc_assert(ll_push_front(ll, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        // push_at
        ll->flag = cmc_flags.ERROR;
        cmc_assert(ll_push_at(ll, 10, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));
        cmc_assert(!ll_push_at(ll, 10, 300));
        cmc_assert_equals(int32_t, cmc_flags.RANGE, ll_flag(ll));

        // push_back
        ll->flag = cmc_flags.ERROR;
        cmc_assert(ll_push_back(ll, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        ll_clear(ll);

        // pop_front
        ll->flag = cmc_flags.ERROR;
        cmc_assert(!ll_pop_front(ll));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ll_flag(ll));

        cmc_assert(ll_push_back(ll, 10));
        cmc_assert(ll_pop_front(ll));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        // pop_at
        cmc_assert(!ll_pop_at(ll, 0));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ll_flag(ll));
        cmc_assert(ll_push_front(ll, 10));
        cmc_assert(!ll_pop_at(ll, 1));
        cmc_assert_equals(int32_t, cmc_flags.RANGE, ll_flag(ll));
        cmc_assert(ll_pop_at(ll, 0));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        // pop_back
        ll->flag = cmc_flags.ERROR;
        cmc_assert(!ll_pop_front(ll));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ll_flag(ll));

        cmc_assert(ll_push_back(ll, 10));
        cmc_assert(ll_pop_front(ll));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        ll_clear(ll);

        // front
        ll_front(ll);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ll_flag(ll));
        cmc_assert(ll_push_front(ll, 10));
        ll->flag = cmc_flags.ERROR;
        ll_front(ll);
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        ll_clear(ll);

        // back
        ll_back(ll);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ll_flag(ll));
        cmc_assert(ll_push_front(ll, 10));
        ll->flag = cmc_flags.ERROR;
        ll_back(ll);
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        ll_clear(ll);

        // get get_ref
        ll_get(ll, 0);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ll_flag(ll));
        ll->flag = cmc_flags.ERROR;
        ll_get_ref(ll, 0);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ll_flag(ll));

        cmc_assert(ll_push_at(ll, 10, 0));
        ll->flag = cmc_flags.ERROR;
        ll_get(ll, 0);
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));
        ll_get(ll, 1);
        cmc_assert_equals(int32_t, cmc_flags.RANGE, ll_flag(ll));
        ll_get_ref(ll, 0);
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));
        ll_get_ref(ll, 1);
        cmc_assert_equals(int32_t, cmc_flags.RANGE, ll_flag(ll));

        // contains
        cmc_assert(ll_contains(ll, 10));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        // copy_of
        ll->flag = cmc_flags.ERROR;
        struct linkedlist *ll2 = ll_copy_of(ll);
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll2));

        // equals
        ll->flag = cmc_flags.ERROR;
        ll2->flag = cmc_flags.ERROR;
        cmc_assert(ll_equals(ll, ll2));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll2));

        ll_clear(ll);

        // get_node
        struct linkedlist_node *n = ll_get_node(ll, 0);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, ll_flag(ll));

        cmc_assert(ll_push_at(ll, 10, 0));
        n = ll_get_node(ll, 0);
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        n = ll_get_node(ll, 1);
        cmc_assert_equals(int32_t, cmc_flags.RANGE, ll_flag(ll));

        // functions relative to node
        ll_clear(ll);
        cmc_assert(ll_push_front(ll, 10));
        n = ll_get_node(ll, 0);
        cmc_assert_not_equals(ptr, NULL, n);

        // add_next
        ll->flag = cmc_flags.ERROR;
        cmc_assert(ll_add_next(ll, n, 11));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        // add_prev
        ll->flag = cmc_flags.ERROR;
        cmc_assert(ll_add_prev(ll, n, 9));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        // del_next
        n = ll_next_node(n);
        cmc_assert(!ll_del_next(ll, n));
        cmc_assert_equals(int32_t, cmc_flags.INVALID, ll_flag(ll));
        n = ll_prev_node(n);
        cmc_assert(ll_del_next(ll, n));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        // del_prev
        n = ll_prev_node(n);
        cmc_assert(!ll_del_prev(ll, n));
        cmc_assert_equals(int32_t, cmc_flags.INVALID, ll_flag(ll));
        n = ll_next_node(n);
        cmc_assert(ll_del_prev(ll, n));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        // del_curr
        ll->flag = cmc_flags.ERROR;
        cmc_assert(ll_del_curr(ll, n));
        cmc_assert_equals(int32_t, cmc_flags.OK, ll_flag(ll));

        ll_free(ll);
        ll_free(ll2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct linkedlist *ll = ll_new_custom(ll_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, ll);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(ll_push_front(ll, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(ll_push_back(ll, 10));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(ll_push_at(ll, 10, 1));
        cmc_assert_equals(int32_t, 3, total_create);

        cmc_assert(ll_pop_at(ll, 1));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(ll_pop_front(ll));
        cmc_assert_equals(int32_t, 2, total_delete);

        cmc_assert(ll_pop_back(ll));
        cmc_assert_equals(int32_t, 3, total_delete);

        cmc_assert(ll_push_back(ll, 10));
        cmc_assert_equals(int32_t, 4, total_create);

        cmc_assert_equals(size_t, 10, ll_front(ll));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert_equals(size_t, 10, ll_back(ll));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert_equals(size_t, 10, ll_get(ll, 0));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert_not_equals(ptr, NULL, ll_get_ref(ll, 0));
        cmc_assert_equals(int32_t, 4, total_read);

        cmc_assert(ll_contains(ll, 10));
        cmc_assert_equals(int32_t, 5, total_read);

        cmc_assert_equals(int32_t, 4, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 3, total_delete);
        cmc_assert_equals(int32_t, 0, total_resize);

        ll_customize(ll, NULL, NULL);

        cmc_assert_equals(ptr, NULL, ll->callbacks);

        ll_clear(ll);
        cmc_assert(ll_push_front(ll, 10));
        cmc_assert(ll_push_back(ll, 10));
        cmc_assert(ll_push_at(ll, 10, 1));
        cmc_assert(ll_pop_at(ll, 1));
        cmc_assert(ll_pop_front(ll));
        cmc_assert(ll_pop_back(ll));
        cmc_assert(ll_push_back(ll, 10));
        cmc_assert_equals(size_t, 10, ll_front(ll));
        cmc_assert_equals(size_t, 10, ll_back(ll));
        cmc_assert_equals(size_t, 10, ll_get(ll, 0));
        cmc_assert_not_equals(ptr, NULL, ll_get_ref(ll, 0));
        cmc_assert(ll_contains(ll, 10));

        cmc_assert_equals(int32_t, 4, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 0, total_update);
        cmc_assert_equals(int32_t, 3, total_delete);
        cmc_assert_equals(int32_t, 0, total_resize);

        cmc_assert_equals(ptr, NULL, ll->callbacks);

        ll_free(ll);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});
