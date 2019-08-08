#include "utl/assert.h"
#include "utl/test.h"
#include "../src/linkedqueue.c"

CMC_CREATE_UNIT(linkedqueue_test, true, {
    CMC_CREATE_TEST(new, {
        linkedqueue *q = lq_new();

        cmc_assert_not_equals(ptr, NULL, q);

        bool passed = q->count == 0 && !q->head && !q->tail;

        cmc_assert_equals(size_t, 0, lq_count(q));
        cmc_assert_equals(ptr, NULL, q->head);
        cmc_assert_equals(ptr, NULL, q->tail);

        CMC_TEST_PASS_ELSE_FAIL(passed);

        lq_free(q);
    });

    CMC_CREATE_TEST(clear[count], {
        linkedqueue *q = lq_new();

        cmc_assert_not_equals(ptr, NULL, q);

        for (size_t i = 0; i < 50; i++)
            lq_enqueue(q, i);

        cmc_assert_equals(size_t, 50, lq_count(q));

        lq_clear(q);

        cmc_assert_equals(size_t, 0, lq_count(q));

        CMC_TEST_PASS_ELSE_FAIL(lq_count(q) == 0);

        lq_free(q);
    });

});
