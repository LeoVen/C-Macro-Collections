#ifndef CMC_TESTS_UNT_CMC_TREEMAP_H
#define CMC_TESTS_UNT_CMC_TREEMAP_H

#include "../utl.h"

#include "cmc/treemap.h"

struct treemap_fkey *tm_fkey = &(struct treemap_fkey){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

struct treemap_fval *tm_fval = &(struct treemap_fval){
    .cmp = cmc_size_cmp, .cpy = NULL, .str = cmc_size_str, .free = NULL, .hash = cmc_size_hash, .pri = cmc_size_cmp
};

CMC_CREATE_UNIT(CMCTreeMap, true, {
    CMC_CREATE_TEST(new, {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        tm_free(map);
    });

    CMC_CREATE_TEST(clear[count], {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 50; i++)
            tm_insert(map, i, i);

        cmc_assert_equals(size_t, 50, tm_count(map));

        tm_clear(map);

        cmc_assert_equals(size_t, 0, tm_count(map));
        cmc_assert_equals(ptr, NULL, map->root);

        tm_free(map);
    });

    CMC_CREATE_TEST(flags, {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map));

        // clear
        map->flag = CMC_FLAG_ERROR;
        tm_clear(map);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map));

        // insert
        map->flag = CMC_FLAG_ERROR;
        cmc_assert(tm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map));

        cmc_assert(!tm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, CMC_FLAG_DUPLICATE, tm_flag(map));

        // update
        cmc_assert(!tm_update(map, 2, 2, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, tm_flag(map));

        cmc_assert(tm_update(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map));

        // remove
        cmc_assert(!tm_remove(map, 2, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, tm_flag(map));

        cmc_assert(tm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map));

        cmc_assert(!tm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, tm_flag(map));

        // max min
        map->flag = CMC_FLAG_ERROR;
        cmc_assert(!tm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, tm_flag(map));

        map->flag = CMC_FLAG_ERROR;
        cmc_assert(!tm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, tm_flag(map));

        cmc_assert(tm_insert(map, 1, 1));
        map->flag = CMC_FLAG_ERROR;
        cmc_assert(tm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map));

        map->flag = CMC_FLAG_ERROR;
        cmc_assert(tm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map));

        // get get_ref
        tm_get(map, 2);
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, tm_flag(map));

        tm_get(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map));

        tm_get_ref(map, 2);
        cmc_assert_equals(int32_t, CMC_FLAG_NOT_FOUND, tm_flag(map));

        tm_get_ref(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map));

        cmc_assert(tm_remove(map, 1, NULL));
        map->flag = CMC_FLAG_ERROR;
        tm_get(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, tm_flag(map));

        map->flag = CMC_FLAG_ERROR;
        tm_get_ref(map, 1);
        cmc_assert_equals(int32_t, CMC_FLAG_EMPTY, tm_flag(map));

        // copy_of
        map->flag = CMC_FLAG_ERROR;
        struct treemap *map2 = tm_copy_of(map);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map2));

        // equals
        map->flag = CMC_FLAG_ERROR;
        map2->flag = CMC_FLAG_ERROR;
        cmc_assert(tm_equals(map, map2));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map));
        cmc_assert_equals(int32_t, CMC_FLAG_OK, tm_flag(map2));

        tm_free(map);
        tm_free(map2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct treemap *map = tm_new_custom(tm_fkey, tm_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(tm_insert(map, 1, 10));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(tm_update(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, 1, total_update);

        cmc_assert(tm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(tm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert(tm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert(tm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert_equals(size_t, 2, tm_get(map, 1));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert_not_equals(ptr, NULL, tm_get_ref(map, 1));
        cmc_assert_equals(int32_t, 4, total_read);

        cmc_assert(tm_contains(map, 1));
        cmc_assert_equals(int32_t, 5, total_read);

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 1, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 0, total_resize);

        tm_customize(map, NULL, NULL);

        tm_clear(map);
        cmc_assert(tm_insert(map, 1, 10));
        cmc_assert(tm_update(map, 1, 2, NULL));
        cmc_assert(tm_remove(map, 1, NULL));
        cmc_assert(tm_insert(map, 1, 2));
        cmc_assert(tm_max(map, NULL, NULL));
        cmc_assert(tm_min(map, NULL, NULL));
        cmc_assert_equals(size_t, 2, tm_get(map, 1));
        cmc_assert_not_equals(ptr, NULL, tm_get_ref(map, 1));
        cmc_assert(tm_contains(map, 1));

        cmc_assert_equals(int32_t, 2, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 1, total_update);
        cmc_assert_equals(int32_t, 1, total_delete);
        cmc_assert_equals(int32_t, 0, total_resize);

        cmc_assert_equals(ptr, NULL, map->callbacks);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        tm_free(map);
    });
});

CMC_CREATE_UNIT(CMCTreeMapIter, true, {
    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_start(map);

        cmc_assert_equals(ptr, map, it.target);
        cmc_assert_equals(ptr, NULL, it.cursor);
        cmc_assert_equals(ptr, NULL, it.first);
        cmc_assert_equals(ptr, NULL, it.last);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(tm_iter_at_start(&it));
        cmc_assert(tm_iter_at_end(&it));

        cmc_assert(tm_insert(map, 1, 1));
        cmc_assert(tm_insert(map, 2, 2));
        cmc_assert(tm_insert(map, 3, 3));

        it = tm_iter_start(map);

        cmc_assert_equals(size_t, 0, it.index);

        cmc_assert_equals(size_t, 1, it.cursor->key);
        cmc_assert_equals(size_t, 1, it.first->key);
        cmc_assert_equals(size_t, 3, it.last->key);
        cmc_assert_equals(bool, false, it.end);

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_end(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_end(map);

        cmc_assert_equals(ptr, map, it.target);
        cmc_assert_equals(ptr, NULL, it.cursor);
        cmc_assert_equals(ptr, NULL, it.first);
        cmc_assert_equals(ptr, NULL, it.last);
        cmc_assert_equals(size_t, 0, it.index);
        cmc_assert_equals(bool, true, it.start);
        cmc_assert_equals(bool, true, it.end);

        cmc_assert(tm_iter_at_start(&it));
        cmc_assert(tm_iter_at_end(&it));

        cmc_assert(tm_insert(map, 1, 1));
        cmc_assert(tm_insert(map, 2, 2));
        cmc_assert(tm_insert(map, 3, 3));

        it = tm_iter_end(map);

        cmc_assert_equals(size_t, map->count - 1, it.index);

        cmc_assert_equals(size_t, 3, it.cursor->key);
        cmc_assert_equals(size_t, 1, it.first->key);
        cmc_assert_equals(size_t, 3, it.last->key);
        cmc_assert_equals(bool, false, it.start);

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_at_start(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_start(map);

        // Empty checks
        cmc_assert(tm_iter_at_start(&it));
        it = tm_iter_end(map);
        cmc_assert(tm_iter_at_start(&it));

        // Non-empty checks
        cmc_assert(tm_insert(map, 1, 1));
        cmc_assert(tm_insert(map, 2, 2));
        it = tm_iter_end(map);
        cmc_assert(!tm_iter_at_start(&it));
        it = tm_iter_start(map);
        cmc_assert(tm_iter_at_start(&it));

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_at_end(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_start(map);

        // Empty check
        cmc_assert(tm_iter_at_end(&it));
        it = tm_iter_end(map);
        cmc_assert(tm_iter_at_end(&it));

        // Non-empty checks
        cmc_assert(tm_insert(map, 1, 1));
        cmc_assert(tm_insert(map, 2, 2));
        it = tm_iter_end(map);
        cmc_assert(tm_iter_at_end(&it));
        it = tm_iter_start(map);
        cmc_assert(!tm_iter_at_end(&it));

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_to_start(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_start(map);

        cmc_assert(!tm_iter_to_start(&it));

        for (size_t i = 1; i <= 100; i++)
            tm_insert(map, i, i);

        cmc_assert_equals(size_t, 100, map->count);

        it = tm_iter_end(map);

        cmc_assert(!tm_iter_at_start(&it));
        cmc_assert(tm_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, tm_iter_value(&it));

        cmc_assert(tm_iter_to_start(&it));

        cmc_assert(tm_iter_at_start(&it));
        cmc_assert(!tm_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, tm_iter_value(&it));

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_to_end(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_end(map);

        cmc_assert(!tm_iter_to_end(&it));

        for (size_t i = 1; i <= 100; i++)
            tm_insert(map, i, i);

        it = tm_iter_start(map);

        cmc_assert(tm_iter_at_start(&it));
        cmc_assert(!tm_iter_at_end(&it));

        cmc_assert_equals(size_t, 1, tm_iter_value(&it));

        cmc_assert(tm_iter_to_end(&it));

        cmc_assert(!tm_iter_at_start(&it));
        cmc_assert(tm_iter_at_end(&it));

        cmc_assert_equals(size_t, 100, tm_iter_value(&it));

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_next(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_start(map);

        cmc_assert(!tm_iter_next(&it));

        for (size_t i = 1; i <= 1000; i++)
            tm_insert(map, i, i);

        size_t sum = 0;
        for (it = tm_iter_start(map); !tm_iter_at_end(&it); tm_iter_next(&it))
        {
            sum += tm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        tm_iter_to_start(&it);
        do
        {
            sum += tm_iter_value(&it);
        } while (tm_iter_next(&it));

        cmc_assert_equals(size_t, 500500, sum);

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_prev(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_end(map);

        cmc_assert(!tm_iter_prev(&it));

        for (size_t i = 1; i <= 1000; i++)
            tm_insert(map, i, i);

        size_t sum = 0;
        for (it = tm_iter_end(map); !tm_iter_at_start(&it); tm_iter_prev(&it))
        {
            sum += tm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;

        tm_iter_to_end(&it);
        do
        {
            sum += tm_iter_value(&it);
        } while (tm_iter_prev(&it));

        cmc_assert_equals(size_t, 500500, sum);

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_advance(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_start(map);

        cmc_assert(!tm_iter_advance(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            tm_insert(map, i, i);

        it = tm_iter_start(map);

        cmc_assert(!tm_iter_advance(&it, 0));
        cmc_assert(!tm_iter_advance(&it, map->count));

        size_t sum = 0;
        for (it = tm_iter_start(map);;)
        {
            sum += tm_iter_value(&it);

            if (!tm_iter_advance(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        tm_iter_to_start(&it);
        cmc_assert(tm_iter_advance(&it, map->count - 1));

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_rewind(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_end(map);

        cmc_assert(!tm_iter_rewind(&it, 1));

        for (size_t i = 0; i <= 1000; i++)
            tm_insert(map, i, i);

        it = tm_iter_end(map);

        cmc_assert(!tm_iter_rewind(&it, 0));
        cmc_assert(!tm_iter_rewind(&it, map->count));

        size_t sum = 0;
        for (it = tm_iter_end(map);;)
        {
            sum += tm_iter_value(&it);

            if (!tm_iter_rewind(&it, 2))
                break;
        }

        cmc_assert_equals(size_t, 250500, sum);

        tm_iter_to_end(&it);
        cmc_assert(tm_iter_rewind(&it, map->count - 1));

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_go_to(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_end(map);
        cmc_assert(!tm_iter_go_to(&it, 0));

        it = tm_iter_start(map);
        cmc_assert(!tm_iter_go_to(&it, 0));

        for (size_t i = 0; i <= 1000; i++)
            tm_insert(map, i, i);

        it = tm_iter_start(map);

        size_t sum = 0;
        for (size_t i = 0; i < 1001; i++)
        {
            tm_iter_go_to(&it, i);

            sum += tm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert(tm_iter_go_to(&it, i - 1));

            sum += tm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 500500, sum);

        sum = 0;
        for (size_t i = 0; i < 1001; i += 100)
        {
            cmc_assert(tm_iter_go_to(&it, i));
            cmc_assert_equals(size_t, i, tm_iter_index(&it));

            sum += tm_iter_value(&it);
        }

        cmc_assert_equals(size_t, 5500, sum);

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_key(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_end(map);

        cmc_assert_equals(size_t, (size_t){ 0 }, tm_iter_key(&it));

        cmc_assert(tm_insert(map, 10, 10));

        it = tm_iter_start(map);

        cmc_assert_equals(size_t, 10, tm_iter_key(&it));

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_value(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_end(map);

        cmc_assert_equals(size_t, (size_t){ 0 }, tm_iter_value(&it));

        cmc_assert(tm_insert(map, 10, 10));

        it = tm_iter_start(map);

        cmc_assert_equals(size_t, 10, tm_iter_value(&it));

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_rvalue(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        struct treemap_iter it = tm_iter_end(map);

        cmc_assert_equals(ptr, NULL, tm_iter_rvalue(&it));

        cmc_assert(tm_insert(map, 10, 10));

        it = tm_iter_start(map);

        cmc_assert_not_equals(ptr, NULL, tm_iter_rvalue(&it));
        cmc_assert_equals(size_t, 10, *tm_iter_rvalue(&it));

        tm_free(map);
    });

    CMC_CREATE_TEST(PFX##_iter_index(), {
        struct treemap *map = tm_new(tm_fkey, tm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i <= 1000; i++)
            tm_insert(map, i, i);

        struct treemap_iter it = tm_iter_start(map);

        for (size_t i = 0; i < 1001; i++)
        {
            cmc_assert_equals(size_t, i, tm_iter_index(&it));
            tm_iter_next(&it);
        }

        it = tm_iter_end(map);
        for (size_t i = 1001; i > 0; i--)
        {
            cmc_assert_equals(size_t, i - 1, tm_iter_index(&it));
            tm_iter_prev(&it);
        }

        tm_free(map);
    });
});

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = CMCTreeMap() + CMCTreeMapIter();

    printf(" +---------------------------------------------------------------+");
    printf("\n");
    printf(" | CMCTreeMap Suit : %-46s |\n", result == 0 ? "PASSED" : "FAILED");
    printf(" +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif

#endif /* CMC_TESTS_UNT_CMC_TREEMAP_H */
