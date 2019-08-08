#include "utl/assert.h"
#include "utl/test.h"
#include "../src/linkedlist.c"

CMC_CREATE_UNIT(linkedlist_test, true, {
    CMC_CREATE_TEST(new, {
        linkedlist *ll = ll_new();

        cmc_assert_not_equals(ptr, NULL, ll);

        bool passed = ll->count == 0 && ll->head == NULL && ll->tail == NULL;

        cmc_assert_equals(size_t, 0, ll_count(ll));
        cmc_assert_equals(ptr, NULL, ll->head);
        cmc_assert_equals(ptr, NULL, ll->tail);

        CMC_TEST_PASS_ELSE_FAIL(passed);

        ll_free(ll);
    });

    CMC_CREATE_TEST(clear[count capacity], {
        linkedlist *ll = ll_new();

        cmc_assert_not_equals(ptr, NULL, ll);

        for (size_t i = 0; i < 50; i++)
            ll_push_back(ll, i);

        cmc_assert_equals(size_t, 50, ll_count(ll));

        ll_clear(ll);

        cmc_assert_equals(size_t, 0, ll_count(ll));

        CMC_TEST_PASS_ELSE_FAIL(ll_count(ll) == 0);

        ll_free(ll);
    });

    CMC_CREATE_TEST(clear[edge_case:count = 0], {
        linkedlist *ll = ll_new();

        cmc_assert_not_equals(ptr, NULL, ll);

        ll_clear(ll);

        cmc_assert_equals(size_t, 0, ll_count(ll));

        CMC_TEST_PASS_ELSE_FAIL(ll_count(ll) == 0);

        ll_free(ll);
    });

    CMC_CREATE_TEST(push_front[count], {
        linkedlist *ll = ll_new();

        cmc_assert_not_equals(ptr, NULL, ll);

        ll_push_front(ll, 1);

        cmc_assert_not_equals(ptr, NULL, ll->head);
        cmc_assert_not_equals(ptr, NULL, ll->tail);
        cmc_assert_equals(size_t, 1, ll_count(ll));

        CMC_TEST_PASS_ELSE_FAIL(ll->head && ll->tail && ll_count(ll) == 1);

        ll_free(ll);
    });

    CMC_CREATE_TEST(push_front[item_preservation], {
        linkedlist *ll = ll_new();

        cmc_assert_not_equals(ptr, NULL, ll);

        for (size_t i = 0; i < 200; i++)
            ll_push_front(ll, i);

        bool passed = true;

        for (size_t i = 0; i < ll_count(ll); i++)
        {
            passed = passed && ll_get(ll, i) == (ll_count(ll) - i - 1);

            cmc_assert_equals(size_t, ll_get(ll, i), ll_count(ll) - i - 1);
        }

        CMC_TEST_PASS_ELSE_FAIL(passed && ll_count(ll) == 200);

        ll_free(ll);
    });

});
