#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "../src/hashmultiset.c"

struct hashmultiset_fval *hms_fval =
    &(struct hashmultiset_fval){ .cmp = cmc_size_cmp,
                                 .cpy = NULL,
                                 .str = cmc_size_str,
                                 .free = NULL,
                                 .hash = cmc_size_hash,
                                 .pri = cmc_size_cmp };

struct hashmultiset_fval *hms_fval_counter =
    &(struct hashmultiset_fval){ .cmp = v_c_cmp,
                                 .cpy = v_c_cpy,
                                 .str = v_c_str,
                                 .free = v_c_free,
                                 .hash = v_c_hash,
                                 .pri = v_c_pri };

struct cmc_alloc_node *hms_alloc_node = &(struct cmc_alloc_node){
    .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free
};

CMC_CREATE_UNIT(HashMultiSet, true, {
    CMC_CREATE_TEST(PFX##_new(), {
        struct hashmultiset *set = hms_new(943722, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_not_equals(ptr, NULL, set->buffer);
        cmc_assert_equals(size_t, 0, set->count);
        cmc_assert_equals(double, 0.6, set->load);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, set->flag);
        cmc_assert_equals(ptr, hms_fval, set->f_val);
        cmc_assert_equals(ptr, &cmc_alloc_node_default, set->alloc);
        cmc_assert_equals(ptr, NULL, set->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hms_capacity(set));

        hms_free(set);

        set = hms_new(0, 0.6, hms_fval);
        cmc_assert_equals(ptr, NULL, set);

        set = hms_new(UINT64_MAX, 0.99, hms_fval);
        cmc_assert_equals(ptr, NULL, set);

        set = hms_new(1000, 0.6, NULL);
        cmc_assert_equals(ptr, NULL, set);

        set = hms_new(1000, 0.6, NULL);
        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(PFX##_new_custom(), {
        struct hashmultiset *set =
            hms_new_custom(943722, 0.6, hms_fval, hms_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_not_equals(ptr, NULL, set->buffer);
        cmc_assert_equals(size_t, 0, set->count);
        cmc_assert_equals(double, 0.6, set->load);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, set->flag);
        cmc_assert_equals(ptr, hms_fval, set->f_val);
        cmc_assert_equals(ptr, hms_alloc_node, set->alloc);
        cmc_assert_equals(ptr, callbacks, set->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), hms_capacity(set));

        hms_free(set);

        set = hms_new_custom(0, 0.6, hms_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, set);

        set = hms_new_custom(UINT64_MAX, 0.99, hms_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, set);

        set = hms_new_custom(1000, 0.6, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, set);
    });

    CMC_CREATE_TEST(PFX##_clear(), {
        v_total_free = 0;
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval_counter);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i <= 1000; i++)
            hms_insert(set, i);

        cmc_assert_equals(size_t, 1000, set->count);

        set->flag = CMC_FLAG_ERROR;
        hms_clear(set);

        cmc_assert_equals(size_t, 0, set->count);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, set->flag);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        hms_free(set);
        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_free(), {
        v_total_free = 0;
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval_counter);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 1; i <= 1000; i++)
            hms_insert(set, i);

        cmc_assert_equals(size_t, 1000, set->count);

        hms_free(set);

        cmc_assert_equals(int32_t, 1000, v_total_free);

        set = hms_new(1000, 0.6, hms_fval_counter);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_not_equals(ptr, NULL, set->buffer);

        hms_free(set);

        cmc_assert_equals(int32_t, 1000, v_total_free);
        v_total_free = 0;
    });

    CMC_CREATE_TEST(insert[count cardinality multiplicity], {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hms_insert(set, 1));
        cmc_assert(hms_insert(set, 2));
        cmc_assert(hms_insert(set, 2));
        cmc_assert(hms_insert(set, 3));
        cmc_assert(hms_insert(set, 3));
        cmc_assert(hms_insert(set, 3));

        cmc_assert_equals(size_t, 3, hms_count(set));
        cmc_assert_equals(size_t, 6, hms_cardinality(set));

        for (size_t i = 0; i <= 3; i++)
            cmc_assert_equals(size_t, i, hms_multiplicity_of(set, i));

        hms_free(set);
    });

    CMC_CREATE_TEST(remove[count cardinality multiplicity], {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hms_insert(set, 1));
        cmc_assert(hms_insert(set, 2));
        cmc_assert(hms_insert(set, 2));
        cmc_assert(hms_insert(set, 3));
        cmc_assert(hms_insert(set, 3));
        cmc_assert(hms_insert(set, 3));

        cmc_assert_equals(size_t, 3, hms_count(set));
        cmc_assert_equals(size_t, 6, hms_cardinality(set));

        for (size_t i = 0; i <= 3; i++)
            cmc_assert_equals(size_t, i, hms_multiplicity_of(set, i));

        cmc_assert(hms_remove(set, 1));
        cmc_assert(hms_remove(set, 2));
        cmc_assert(hms_remove(set, 3));

        cmc_assert_equals(size_t, 2, hms_count(set));
        cmc_assert_equals(size_t, 3, hms_cardinality(set));

        cmc_assert_equals(size_t, 0, hms_multiplicity_of(set, 0));
        cmc_assert_equals(size_t, 0, hms_multiplicity_of(set, 1));
        cmc_assert_equals(size_t, 1, hms_multiplicity_of(set, 2));
        cmc_assert_equals(size_t, 2, hms_multiplicity_of(set, 3));

        hms_free(set);
    });

    CMC_CREATE_TEST(remove_all[count cardinality multiplicity], {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        cmc_assert(hms_insert(set, 1));
        cmc_assert(hms_insert(set, 2));
        cmc_assert(hms_insert(set, 2));
        cmc_assert(hms_insert(set, 3));
        cmc_assert(hms_insert(set, 3));
        cmc_assert(hms_insert(set, 3));

        cmc_assert_equals(size_t, 3, hms_count(set));
        cmc_assert_equals(size_t, 6, hms_cardinality(set));

        for (size_t i = 0; i <= 3; i++)
            cmc_assert_equals(size_t, i, hms_multiplicity_of(set, i));

        cmc_assert(hms_remove_all(set, 1));
        cmc_assert(hms_remove_all(set, 3));

        cmc_assert_equals(size_t, 1, hms_count(set));
        cmc_assert_equals(size_t, 2, hms_cardinality(set));

        cmc_assert_equals(size_t, 0, hms_multiplicity_of(set, 0));
        cmc_assert_equals(size_t, 0, hms_multiplicity_of(set, 1));
        cmc_assert_equals(size_t, 2, hms_multiplicity_of(set, 2));
        cmc_assert_equals(size_t, 0, hms_multiplicity_of(set, 3));

        hms_free(set);
    });

    CMC_CREATE_TEST(multiplicity, {
        struct hashmultiset *set = hms_new(50, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i < 1000; i++)
            cmc_assert(hms_insert(set, i % 20));

        cmc_assert_equals(size_t, 20, hms_count(set));
        cmc_assert_equals(size_t, 1000, hms_cardinality(set));

        for (size_t i = 0; i < 20; i++)
            cmc_assert_equals(size_t, 50, hms_multiplicity_of(set, i));

        hms_free(set);
    });

    CMC_CREATE_TEST(flags, {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        // clear
        set->flag = CMC_FLAG_ERROR;
        hms_clear(set);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        // customize
        set->flag = CMC_FLAG_ERROR;
        hms_customize(set, NULL, NULL);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        // insert
        set->flag = CMC_FLAG_ERROR;
        cmc_assert(hms_insert(set, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        set->flag = CMC_FLAG_ERROR;
        cmc_assert(hms_insert(set, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        // insert_many
        set->flag = CMC_FLAG_ERROR;
        cmc_assert(hms_insert_many(set, 1, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        set->flag = CMC_FLAG_ERROR;
        cmc_assert(hms_insert_many(set, 1, 2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        // update
        set->flag = CMC_FLAG_ERROR;
        cmc_assert(hms_update(set, 1, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        set->flag = CMC_FLAG_ERROR;
        cmc_assert(hms_update(set, 10, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        // remove
        cmc_assert(!hms_remove(set, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hms_flag(set));

        cmc_assert(hms_remove(set, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        // remove_all
        cmc_assert_equals(size_t, 0, hms_remove_all(set, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hms_flag(set));

        cmc_assert(hms_insert_many(set, 1, 10));
        cmc_assert_equals(size_t, 0, hms_remove_all(set, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, hms_flag(set));

        cmc_assert_equals(size_t, 10, hms_remove_all(set, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        // max min
        cmc_assert(!hms_max(set, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hms_flag(set));
        set->flag = CMC_FLAG_ERROR;
        cmc_assert(!hms_min(set, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, hms_flag(set));

        cmc_assert(hms_insert(set, 1));
        set->flag = CMC_FLAG_ERROR;
        cmc_assert(hms_max(set, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));
        set->flag = CMC_FLAG_ERROR;
        cmc_assert(hms_min(set, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        // multiplicity_of
        set->flag = CMC_FLAG_ERROR;
        cmc_assert_equals(size_t, 0, hms_multiplicity_of(set, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        // contains
        set->flag = CMC_FLAG_ERROR;
        cmc_assert(!hms_contains(set, 10));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));

        // copy_of
        set->flag = CMC_FLAG_ERROR;
        struct hashmultiset *set2 = hms_copy_of(set);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set2));

        // equals
        set->flag = CMC_FLAG_ERROR;
        set2->flag = CMC_FLAG_ERROR;
        cmc_assert(hms_equals(set, set2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, hms_flag(set2));

        hms_free(set);
        hms_free(set2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct hashmultiset *set =
            hms_new_custom(100, 0.8, hms_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, set);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(hms_insert(set, 1));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(hms_insert_many(set, 1, 3));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(hms_update(set, 1, 0));
        cmc_assert_equals(int32_t, 1, total_update);

        cmc_assert(hms_update(set, 1, 10));
        cmc_assert_equals(int32_t, 2, total_update);

        cmc_assert(hms_remove(set, 1));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(hms_remove_all(set, 1));
        cmc_assert_equals(int32_t, 2, total_delete);

        cmc_assert(hms_update(set, 1, 10));
        cmc_assert_equals(int32_t, 3, total_update);

        cmc_assert(hms_max(set, NULL));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert(hms_min(set, NULL));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert(hms_contains(set, 1));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert(hms_resize(set, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(hms_resize(set, 100));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 3, total_read);
        cmc_assert_equals(int32_t, 3, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        hms_customize(set, NULL, NULL);

        cmc_assert_equals(ptr, NULL, set->callbacks);

        hms_clear(set);
        cmc_assert(hms_insert(set, 1));
        cmc_assert(hms_insert_many(set, 1, 3));
        cmc_assert(hms_update(set, 1, 0));
        cmc_assert(hms_update(set, 1, 10));
        cmc_assert(hms_remove(set, 1));
        cmc_assert(hms_remove_all(set, 1));
        cmc_assert(hms_update(set, 1, 10));
        cmc_assert(hms_max(set, NULL));
        cmc_assert(hms_min(set, NULL));
        cmc_assert(hms_contains(set, 1));
        cmc_assert(hms_resize(set, 1000));
        cmc_assert(hms_resize(set, 100));

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 3, total_read);
        cmc_assert_equals(int32_t, 3, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, set->callbacks);

        hms_free(set);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});

struct hashmultiset_fval *hms_fval_numhash =
    &(struct hashmultiset_fval){ .cmp = cmc_size_cmp,
                                 .cpy = NULL,
                                 .str = cmc_size_str,
                                 .free = NULL,
                                 .hash = numhash,
                                 .pri = cmc_size_cmp };

CMC_CREATE_UNIT(HashMultiSetIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_start(set);

        cmc_assert_equals(ptr, set, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(size_t, 0, it.first);
        cmc_assert_equals(size_t, 0, it.last);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(hms_iter_at_start(&it));
        cmc_assert(hms_iter_at_end(&it));

        cmc_assert(hms_insert(set, 1));
        cmc_assert(hms_insert(set, 2));
        cmc_assert(hms_insert(set, 3));

        it = hms_iter_start(set);

        cmc_assert_equals(size_t, 0, it.index);

        cmc_assert_equals(size_t, 1, it.cursor);
        cmc_assert_equals(size_t, 1, it.first);
        cmc_assert_equals(size_t, 3, it.last);
        cmc_assert_equals(bool, false, it.end);

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_end(set);

        cmc_assert_equals(ptr, set, it.target);
        cmc_assert_equals(size_t, 0, it.cursor);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(size_t, 0, it.first);
        cmc_assert_equals(size_t, 0, it.last);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(hms_iter_at_start(&it));
        cmc_assert(hms_iter_at_end(&it));

        cmc_assert(hms_insert(set, 1));
        cmc_assert(hms_insert(set, 2));
        cmc_assert(hms_insert(set, 3));

        it = hms_iter_end(set);

        cmc_assert_equals(size_t, set->count - 1, it.index);

        cmc_assert_equals(size_t, 3, it.cursor);
        cmc_assert_equals(size_t, 1, it.first);
        cmc_assert_equals(size_t, 3, it.last);
        cmc_assert_equals(bool, false, it.start);

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_start(set);

        // Empty checks
        cmc_assert(hms_iter_at_start(&it));
        it = hms_iter_end(set);
        cmc_assert(hms_iter_at_start(&it));

        // Non-empty checks
        cmc_assert(hms_insert(set, 1));
        cmc_assert(hms_insert(set, 2));
        it = hms_iter_end(set);
        cmc_assert(!hms_iter_at_start(&it));
        it = hms_iter_start(set);
        cmc_assert(hms_iter_at_start(&it));

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_start(set);

        // Empty check
        cmc_assert(hms_iter_at_end(&it));
        it = hms_iter_end(set);
        cmc_assert(hms_iter_at_end(&it));

        // Non-empty checks
        cmc_assert(hms_insert(set, 1));
        cmc_assert(hms_insert(set, 2));
        it = hms_iter_end(set);
        cmc_assert(hms_iter_at_end(&it));
        it = hms_iter_start(set);
        cmc_assert(!hms_iter_at_end(&it));

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_start(set);

        cmc_assert(!hms_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
            hms_insert(set, i);

        cmc_assert_equals(size_t, 100, set->count);

        it = hms_iter_end(set);

        cmc_assert(!hms_iter_at_start(&it));
        cmc_assert(hms_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, hms_iter_value(&it));

        cmc_assert(hms_iter_to_start(&it));

        cmc_assert(hms_iter_at_start(&it));
        cmc_assert(!hms_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, hms_iter_value(&it));

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_end(set);

        cmc_assert(!hms_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
            hms_insert(set, i);

        it = hms_iter_start(set);

        cmc_assert(hms_iter_at_start(&it));
        cmc_assert(!hms_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, hms_iter_value(&it));

        cmc_assert(hms_iter_to_end(&it));

        cmc_assert(!hms_iter_at_start(&it));
        cmc_assert(hms_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, hms_iter_value(&it));

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_start(set);

        cmc_assert(!hms_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
            hms_insert(set, i);

        size_t sum = 0;
        for (it = hms_iter_start(set); !hms_iter_at_end(&it);
             hms_iter_next(&it))
        {
            sum += hms_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        hms_iter_to_start(&it);
        do
        {
            sum += hms_iter_value(&it);
        } while (hms_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_end(set);

        cmc_assert(!hms_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
            hms_insert(set, i);

        size_t sum = 0;
        for (it = hms_iter_end(set); !hms_iter_at_start(&it);
             hms_iter_prev(&it))
        {
            sum += hms_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        hms_iter_to_end(&it);
        do
        {
            sum += hms_iter_value(&it);
        } while (hms_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_start(set);

        cmc_assert(!hms_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            hms_insert(set, i);

        it = hms_iter_start(set);

        cmc_assert(!hms_iter_advance(&it, 0));
        cmc_assert(!hms_iter_advance(&it, set->count));

        size_t sum = 0;
        for (it = hms_iter_start(set);;)
        {
            sum += hms_iter_value(&it);

            if (!hms_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        hms_iter_to_start(&it);
        cmc_assert(hms_iter_advance(&it, set->count - 1));

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_end(set);

        cmc_assert(!hms_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            hms_insert(set, i);

        it = hms_iter_end(set);

        cmc_assert(!hms_iter_rewind(&it, 0));
        cmc_assert(!hms_iter_rewind(&it, set->count));

        size_t sum = 0;
        for (it = hms_iter_end(set);;)
        {
            sum += hms_iter_value(&it);

            if (!hms_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        hms_iter_to_end(&it);
        cmc_assert(hms_iter_rewind(&it, set->count - 1));

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval_numhash);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_end(set);
        cmc_assert(!hms_iter_go_to(&it, 0));

        it = hms_iter_start(set);
        cmc_assert(!hms_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            hms_insert(set, i);

        it = hms_iter_start(set);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            hms_iter_go_to(&it, i);

            sum += hms_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(hms_iter_go_to(&it, i - 1));

            sum += hms_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 0; i < 1001; i += 100)
        {
            cmc_assert(hms_iter_go_to(&it, i));
            cmc_assert_equals(size_t, i, hms_iter_index(&it));

            sum += hms_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_end(set);

        cmc_assert_equals(size_t, (size_t){ 0 }, hms_iter_value(&it));

        cmc_assert(hms_insert(set, 10));

        it = hms_iter_start(set);

        cmc_assert_equals(size_t, 10, hms_iter_value(&it));

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_multiplicity(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        struct hashmultiset_iter it = hms_iter_end(set);

        cmc_assert_equals(size_t, (size_t){ 0 }, hms_iter_multiplicity(&it));

        cmc_assert(hms_insert_many(set, 10, 5));

        it = hms_iter_start(set);
        cmc_assert_equals(size_t, 5, hms_iter_multiplicity(&it));

        it = hms_iter_end(set);
        cmc_assert_equals(size_t, 5, hms_iter_multiplicity(&it));

        hms_free(set);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct hashmultiset *set = hms_new(100, 0.6, hms_fval);

        cmc_assert_not_equals(ptr, NULL, set);

        for (size_t i = 0; i <= 1000; i++)
            hms_insert(set, i);

        struct hashmultiset_iter it = hms_iter_start(set);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, hms_iter_index(&it));
            hms_iter_next(&it);
        }

        it = hms_iter_end(set);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, hms_iter_index(&it));
            hms_iter_prev(&it);
        }

        hms_free(set);
    });
});

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = HashMultiSet() + HashMultiSetIter();

    printf(
        " +---------------------------------------------------------------+");
    printf("\n");
    printf(" | HashMultiSet Suit : %-41s |\n",
           result == 0 ? "PASSED" : "FAILED");
    printf(
        " +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif
