#include "utl.c"
#include <utl/assert.h>
#include <utl/log.h>
#include <utl/test.h>

#include <cmc/linkedlist.h>

CMC_GENERATE_LINKEDLIST(ll, linkedlist, size_t)

CMC_CREATE_UNIT(linkedlist_test, true, {
    CMC_CREATE_TEST(new, {
        struct linkedlist *ll = ll_new();

        cmc_assert_not_equals(ptr, NULL, ll);
        cmc_assert_equals(size_t, 0, ll_count(ll));
        cmc_assert_equals(ptr, NULL, ll->head);
        cmc_assert_equals(ptr, NULL, ll->tail);

        ll_free(ll, NULL);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        struct linkedlist *ll = ll_new();

        cmc_assert_not_equals(ptr, NULL, ll);

        for (size_t i = 0; i < 50; i++)
            ll_push_back(ll, i);

        cmc_assert_equals(size_t, 50, ll_count(ll));

        ll_clear(ll, NULL);

        cmc_assert_equals(size_t, 0, ll_count(ll));

        ll_free(ll, NULL);
    });

    CMC_CREATE_TEST(clear[count = 0], {
        struct linkedlist *ll = ll_new();

        cmc_assert_not_equals(ptr, NULL, ll);

        ll_clear(ll, NULL);

        cmc_assert_equals(size_t, 0, ll_count(ll));

        ll_free(ll, NULL);
    });

    CMC_CREATE_TEST(push_front[count], {
        struct linkedlist *ll = ll_new();

        cmc_assert_not_equals(ptr, NULL, ll);

        ll_push_front(ll, 1);

        cmc_assert_not_equals(ptr, NULL, ll->head);
        cmc_assert_not_equals(ptr, NULL, ll->tail);
        cmc_assert_equals(size_t, 1, ll_count(ll));

        ll_free(ll, NULL);
    });

    CMC_CREATE_TEST(push_front[item preservation], {
        struct linkedlist *ll = ll_new();

        cmc_assert_not_equals(ptr, NULL, ll);

        for (size_t i = 0; i < 200; i++)
            ll_push_front(ll, i);

        for (size_t i = 0; i < ll_count(ll); i++)
            cmc_assert_equals(size_t, ll_get(ll, i), ll_count(ll) - i - 1);

        ll_free(ll, NULL);
    });
});
