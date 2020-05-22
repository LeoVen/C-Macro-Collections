#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "../src/linkedlist.c"

struct linkedlist_fval *ll_fval =
    &(struct linkedlist_fval){ .cmp = cmc_size_cmp,
                               .cpy = NULL,
                               .str = cmc_size_str,
                               .free = NULL,
                               .hash = cmc_size_hash,
                               .pri = cmc_size_cmp };

CMC_CREATE_UNIT(LinkedList, true, {
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

CMC_CREATE_UNIT(LinkedListIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_start(ll);

        cmc_assert_equals(ptr, ll, it.target);
        cmc_assert_equals(ptr, NULL, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(ll_iter_at_start(&it));
        cmc_assert(ll_iter_at_end(&it));

        cmc_assert(ll_push_front(ll, 1));
        cmc_assert(ll_push_front(ll, 2));

        it = ll_iter_start(ll);

        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, false, it.end);
        cmc_assert_equals(ptr, ll->head, it.cursor);

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_end(ll);

        cmc_assert_equals(ptr, ll, it.target);
        cmc_assert_equals(ptr, NULL, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(ll_iter_at_start(&it));
        cmc_assert(ll_iter_at_end(&it));

        cmc_assert(ll_push_back(ll, 1));
        cmc_assert(ll_push_back(ll, 2));
        cmc_assert(ll_push_back(ll, 3));

        it = ll_iter_end(ll);

        cmc_assert_equals(size_t, ll->count - 1, it.index);
        cmc_assert_equals(ptr, ll->tail, it.cursor);
        cmc_assert_equals(bool, false, it.start);

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_start(ll);

        // Empty checks
        cmc_assert(ll_iter_at_start(&it));
        it = ll_iter_end(ll);
        cmc_assert(ll_iter_at_start(&it));

        // Non-empty checks
        cmc_assert(ll_push_back(ll, 1));
        it = ll_iter_end(ll);
        cmc_assert(!ll_iter_at_start(&it));
        it = ll_iter_start(ll);
        cmc_assert(ll_iter_at_start(&it));

        ll_clear(ll);

        cmc_assert(ll_push_front(ll, 1));
        it = ll_iter_end(ll);
        cmc_assert(!ll_iter_at_start(&it));
        it = ll_iter_start(ll);
        cmc_assert(ll_iter_at_start(&it));

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_start(ll);

        // Empty check
        cmc_assert(ll_iter_at_end(&it));
        it = ll_iter_end(ll);
        cmc_assert(ll_iter_at_end(&it));

        // Non-empty checks
        cmc_assert(ll_push_back(ll, 1));
        it = ll_iter_end(ll);
        cmc_assert(ll_iter_at_end(&it));
        it = ll_iter_start(ll);
        cmc_assert(!ll_iter_at_end(&it));

        ll_clear(ll);

        cmc_assert(ll_push_front(ll, 1));
        it = ll_iter_end(ll);
        cmc_assert(ll_iter_at_end(&it));
        it = ll_iter_start(ll);
        cmc_assert(!ll_iter_at_end(&it));

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_start(ll);

        cmc_assert(!ll_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
        {
            if (i % 2 == 0)
                ll_push_back(ll, i);
            else
                ll_push_front(ll, i);
        }

        cmc_assert_equals(size_t, 100, ll->count);

        it = ll_iter_end(ll);

        cmc_assert(!ll_iter_at_start(&it));
        cmc_assert(ll_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, ll_iter_value(&it));

        cmc_assert(ll_iter_to_start(&it));

        cmc_assert(ll_iter_at_start(&it));
        cmc_assert(!ll_iter_at_end(&it));

        cmc_assert_equals(size_t, 99, ll_iter_value(&it));

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_end(ll);

        cmc_assert(!ll_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
        {
            if (i % 2 == 0)
                ll_push_back(ll, i);
            else
                ll_push_front(ll, i);
        }

        it = ll_iter_start(ll);

        cmc_assert(ll_iter_at_start(&it));
        cmc_assert(!ll_iter_at_end(&it));

        cmc_assert_equals(size_t, 99, ll_iter_value(&it));

        cmc_assert(ll_iter_to_end(&it));

        cmc_assert(!ll_iter_at_start(&it));
        cmc_assert(ll_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, ll_iter_value(&it));

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_start(ll);

        cmc_assert(!ll_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
                ll_push_back(ll, i);
            else
                ll_push_front(ll, i);
        }

        size_t sum = 0;
        for (it = ll_iter_start(ll); !ll_iter_at_end(&it); ll_iter_next(&it))
        {
            sum += ll_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        ll_iter_to_start(&it);
        do
        {
            sum += ll_iter_value(&it);
        } while (ll_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_end(ll);

        cmc_assert(!ll_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
        {
            if (i % 2 == 0)
                ll_push_back(ll, i);
            else
                ll_push_front(ll, i);
        }

        size_t sum = 0;
        for (it = ll_iter_end(ll); !ll_iter_at_start(&it); ll_iter_prev(&it))
        {
            sum += ll_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        ll_iter_to_end(&it);
        do
        {
            sum += ll_iter_value(&it);
        } while (ll_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_start(ll);

        cmc_assert(!ll_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
        {
            if (i % 2 == 0)
                ll_push_back(ll, i); // will sum all even numbers
            else
                ll_push_back(ll, 0);
        }

        it = ll_iter_start(ll);

        cmc_assert(!ll_iter_advance(&it, 0));
        cmc_assert(!ll_iter_advance(&it, ll->count));

        size_t sum = 0;
        for (it = ll_iter_start(ll);;)
        {
            sum += ll_iter_value(&it);

            if (!ll_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        ll_iter_to_start(&it);
        cmc_assert(ll_iter_advance(&it, ll->count - 1));

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_end(ll);

        cmc_assert(!ll_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
        {
            if (i % 2 == 0)
                ll_push_front(ll, i); // will sum all even numbers
            else
                ll_push_front(ll, 0);
        }

        it = ll_iter_end(ll);

        cmc_assert(!ll_iter_rewind(&it, 0));
        cmc_assert(!ll_iter_rewind(&it, ll->count));

        size_t sum = 0;
        for (it = ll_iter_end(ll);;)
        {
            sum += ll_iter_value(&it);

            if (!ll_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        ll_iter_to_end(&it);
        cmc_assert(ll_iter_rewind(&it, ll->count - 1));

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_end(ll);
        cmc_assert(!ll_iter_go_to(&it, 0));

        it = ll_iter_start(ll);
        cmc_assert(!ll_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            ll_push_back(ll, i);

        it = ll_iter_start(ll);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            ll_iter_go_to(&it, i);

            sum += ll_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(ll_iter_go_to(&it, i - 1));

            sum += ll_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 0; i < 1001; i += 100)
        {
            cmc_assert(ll_iter_go_to(&it, i));
            cmc_assert_equals(size_t, i, ll_iter_index(&it));

            sum += ll_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_end(ll);

        cmc_assert_equals(size_t, (size_t){ 0 }, ll_iter_value(&it));

        cmc_assert(ll_push_back(ll, 10));

        it = ll_iter_start(ll);

        cmc_assert_equals(size_t, 10, ll_iter_value(&it));

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_rvalue(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        struct linkedlist_iter it = ll_iter_end(ll);

        cmc_assert_equals(ptr, NULL, ll_iter_rvalue(&it));

        cmc_assert(ll_push_back(ll, 10));

        it = ll_iter_start(ll);

        cmc_assert_not_equals(ptr, NULL, ll_iter_rvalue(&it));
        cmc_assert_equals(size_t, 10, *ll_iter_rvalue(&it));

        ll_free(ll);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct linkedlist *ll = ll_new(ll_fval);

        cmc_assert_not_equals(ptr, NULL, ll);

        for (size_t i = 0; i <= 1000; i++)
            ll_push_back(ll, i);

        struct linkedlist_iter it = ll_iter_start(ll);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, ll_iter_index(&it));
            ll_iter_next(&it);
        }

        it = ll_iter_end(ll);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, ll_iter_index(&it));
            ll_iter_prev(&it);
        }

        ll_free(ll);
    });
});

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = LinkedList() + LinkedListIter();

    printf(
        " +---------------------------------------------------------------+");
    printf("\n");
    printf(" | LinkedList Suit : %-43s |\n", result == 0 ? "PASSED" : "FAILED");
    printf(
        " +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif
