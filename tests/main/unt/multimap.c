#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "../src/multimap.c"

struct multimap_fkey *mm_fkey = &(struct multimap_fkey){ .cmp = cmc_size_cmp,
                                                         .cpy = NULL,
                                                         .str = cmc_size_str,
                                                         .free = NULL,
                                                         .hash = cmc_size_hash,
                                                         .pri = cmc_size_cmp };

struct multimap_fval *mm_fval = &(struct multimap_fval){ .cmp = cmc_size_cmp,
                                                         .cpy = NULL,
                                                         .str = cmc_size_str,
                                                         .free = NULL,
                                                         .hash = cmc_size_hash,
                                                         .pri = cmc_size_cmp };

struct multimap_fkey *mm_fkey_counter =
    &(struct multimap_fkey){ .cmp = k_c_cmp,
                             .cpy = k_c_cpy,
                             .str = k_c_str,
                             .free = k_c_free,
                             .hash = k_c_hash,
                             .pri = k_c_pri };

struct multimap_fval *mm_fval_counter =
    &(struct multimap_fval){ .cmp = v_c_cmp,
                             .cpy = v_c_cpy,
                             .str = v_c_str,
                             .free = v_c_free,
                             .hash = v_c_hash,
                             .pri = v_c_pri };

struct cmc_alloc_node *mm_alloc_node = &(struct cmc_alloc_node){
    .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free
};

CMC_CREATE_UNIT(MultiMap, true, {
    CMC_CREATE_TEST(PFX##_new(), {
        struct multimap *map = mm_new(943722, 0.6, mm_fkey, mm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
        cmc_assert_equals(ptr, mm_fkey, map->f_key);
        cmc_assert_equals(ptr, mm_fval, map->f_val);
        cmc_assert_equals(ptr, &cmc_alloc_node_default, map->alloc);
        cmc_assert_equals(ptr, NULL, map->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), mm_capacity(map));

        mm_free(map);

        map = mm_new(100, 0.6, mm_fkey, mm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < map->capacity; i++)
        {
            cmc_assert_equals(ptr, NULL, map->buffer[i][0]);
            cmc_assert_equals(ptr, NULL, map->buffer[i][1]);
        }

        mm_free(map);

        map = mm_new(0, 0.6, mm_fkey, mm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = mm_new(UINT64_MAX, 0.99, mm_fkey, mm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = mm_new(1000, 0.6, mm_fkey, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = mm_new(1000, 0.6, NULL, mm_fval);
        cmc_assert_equals(ptr, NULL, map);

        map = mm_new(1000, 0.6, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(PFX##_new_custom(), {
        struct multimap *map = mm_new_custom(943722, 0.6, mm_fkey, mm_fval,
                                             mm_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);
        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(double, 0.6, map->load);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
        cmc_assert_equals(ptr, mm_fkey, map->f_key);
        cmc_assert_equals(ptr, mm_fval, map->f_val);
        cmc_assert_equals(ptr, mm_alloc_node, map->alloc);
        cmc_assert_equals(ptr, callbacks, map->callbacks);

        cmc_assert_greater_equals(size_t, (943722 / 0.6), mm_capacity(map));

        mm_free(map);

        map = mm_new_custom(0, 0.6, mm_fkey, mm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = mm_new_custom(UINT64_MAX, 0.99, mm_fkey, mm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = mm_new_custom(1000, 0.6, mm_fkey, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = mm_new_custom(1000, 0.6, NULL, mm_fval, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);

        map = mm_new_custom(1000, 0.6, NULL, NULL, NULL, NULL);
        cmc_assert_equals(ptr, NULL, map);
    });

    CMC_CREATE_TEST(PFX##_clear(), {
        k_total_free = 0;
        v_total_free = 0;
        struct multimap *map =
            mm_new(100, 0.6, mm_fkey_counter, mm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            mm_insert(map, i, i);

        cmc_assert_equals(size_t, 1000, map->count);

        map->flag = cmc_flags.ERROR;
        mm_clear(map);

        cmc_assert_equals(size_t, 0, map->count);
        cmc_assert_equals(int32_t, cmc_flags.OK, map->flag);
        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        mm_free(map);
        k_total_free = 0;
        v_total_free = 0;
    });

    CMC_CREATE_TEST(PFX##_free(), {
        k_total_free = 0;
        v_total_free = 0;
        struct multimap *map =
            mm_new(100, 0.6, mm_fkey_counter, mm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 1; i <= 1000; i++)
            mm_insert(map, i, i);

        cmc_assert_equals(size_t, 1000, map->count);

        mm_free(map);

        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);

        map = mm_new(1000, 0.6, mm_fkey_counter, mm_fval_counter);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_not_equals(ptr, NULL, map->buffer);

        mm_free(map);

        cmc_assert_equals(int32_t, 1000, k_total_free);
        cmc_assert_equals(int32_t, 1000, v_total_free);
        k_total_free = 0;
        v_total_free = 0;
    });

    CMC_CREATE_TEST(insert[count], {
        struct multimap *map = mm_new(100, 0.8, mm_fkey, mm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 100; i++)
            cmc_assert(mm_insert(map, i % 20, i % 20));

        cmc_assert_equals(size_t, 100, mm_count(map));

        mm_free(map);
    });

    CMC_CREATE_TEST(remove[count], {
        struct multimap *map = mm_new(100, 0.8, mm_fkey, mm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 200; i++)
            cmc_assert(mm_insert(map, i % 20, i % 20));

        size_t r;

        for (size_t i = 0; i < 60; i++)
            cmc_assert(mm_remove(map, i % 20, &r));

        cmc_assert_equals(size_t, 140, mm_count(map));

        mm_free(map);
    });

    CMC_CREATE_TEST(remove[count = 0], {
        struct multimap *map = mm_new(100, 0.8, mm_fkey, mm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        size_t r;

        cmc_assert(!mm_remove(map, 10, &r));

        mm_free(map);
    });

    CMC_CREATE_TEST(remove[count = 1], {
        struct multimap *map = mm_new(100, 0.8, mm_fkey, mm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        size_t r;

        cmc_assert(mm_insert(map, 10, 11));
        cmc_assert(mm_remove(map, 10, &r));

        mm_free(map);
    });

    CMC_CREATE_TEST(get[key ordering], {
        struct multimap *map = mm_new(100, 0.8, mm_fkey, mm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        /* Assert that the value 100 is accessed before the value 101 for */
        /* the same key */
        cmc_assert(mm_insert(map, 10, 100));
        cmc_assert(mm_insert(map, 20, 102));
        cmc_assert(mm_insert(map, 10, 101));

        cmc_assert_equals(size_t, 100, mm_get(map, 10));

        mm_free(map);
    });

    CMC_CREATE_TEST(key_count, {
        struct multimap *map = mm_new(50, 0.8, mm_fkey, mm_fval);

        cmc_assert_not_equals(ptr, NULL, map);

        for (size_t i = 0; i < 1000; i++)
            cmc_assert(mm_insert(map, i % 20, i % 20));

        cmc_assert_equals(size_t, 1000, mm_count(map));

        for (size_t i = 0; i < 20; i++)
            cmc_assert_equals(size_t, 50, mm_key_count(map, i));

        mm_free(map);
    });

    CMC_CREATE_TEST(flags, {
        struct multimap *map = mm_new(100, 0.8, mm_fkey, mm_fval);

        cmc_assert_not_equals(ptr, NULL, map);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // clear
        map->flag = cmc_flags.ERROR;
        mm_clear(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // customize
        map->flag = cmc_flags.ERROR;
        mm_customize(map, NULL, NULL);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // insert
        map->flag = cmc_flags.ERROR;
        cmc_assert(mm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // update
        cmc_assert(!mm_update(map, 9, 10, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));

        cmc_assert(mm_update(map, 1, 20, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // update_all
        map->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 1, mm_update_all(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        map->flag = cmc_flags.ERROR;
        cmc_assert_equals(size_t, 0, mm_update_all(map, 2, 4, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));

        mm_clear(map);
        cmc_assert_equals(size_t, 0, mm_update_all(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));

        // remove
        map->flag = cmc_flags.ERROR;
        cmc_assert(!mm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));

        cmc_assert(mm_insert(map, 1, 1));
        cmc_assert(!mm_remove(map, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));

        cmc_assert(mm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // remove_all
        cmc_assert(!mm_remove_all(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));

        cmc_assert(mm_insert(map, 1, 1) && mm_insert(map, 1, 1));
        cmc_assert_equals(size_t, 0, mm_remove_all(map, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));

        cmc_assert_equals(size_t, 2, mm_remove_all(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // max min
        cmc_assert(!mm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(!mm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));

        cmc_assert(mm_insert(map, 1, 1));
        cmc_assert(mm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));
        map->flag = cmc_flags.ERROR;
        cmc_assert(mm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // get get_ref
        mm_get(map, 2);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));
        map->flag = cmc_flags.ERROR;
        mm_get_ref(map, 2);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, mm_flag(map));

        mm_get(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));
        map->flag = cmc_flags.ERROR;
        mm_get_ref(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        mm_clear(map);
        mm_get(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));
        map->flag = cmc_flags.ERROR;
        mm_get_ref(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, mm_flag(map));

        // contains
        map->flag = cmc_flags.ERROR;
        cmc_assert(!mm_contains(map, 1000));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));

        // copy_of
        map->flag = cmc_flags.ERROR;
        struct multimap *map2 = mm_copy_of(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map2));

        // equals
        map->flag = cmc_flags.ERROR;
        map2->flag = cmc_flags.ERROR;
        cmc_assert(mm_equals(map, map2));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, mm_flag(map2));

        mm_free(map);
        mm_free(map2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct multimap *map =
            mm_new_custom(100, 0.8, mm_fkey, mm_fval, NULL, callbacks);

        cmc_assert_not_equals(ptr, NULL, map);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;

        cmc_assert(mm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, 1, total_create);

        cmc_assert(mm_update(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, 1, total_update);

        cmc_assert(mm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, 2, total_create);

        cmc_assert_equals(size_t, 2, mm_update_all(map, 1, 3, NULL));
        cmc_assert_equals(int32_t, 2, total_update);

        cmc_assert(mm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, 1, total_delete);

        cmc_assert(mm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, 3, total_create);

        cmc_assert_equals(size_t, 2, mm_remove_all(map, 1, NULL));
        cmc_assert_equals(int32_t, 2, total_delete);

        cmc_assert(mm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, 4, total_create);

        cmc_assert(mm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, 1, total_read);

        cmc_assert(mm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, 2, total_read);

        cmc_assert_equals(size_t, 2, mm_get(map, 1));
        cmc_assert_equals(int32_t, 3, total_read);

        cmc_assert_not_equals(ptr, NULL, mm_get_ref(map, 1));
        cmc_assert_equals(int32_t, 4, total_read);

        cmc_assert(mm_contains(map, 1));
        cmc_assert_equals(int32_t, 5, total_read);

        cmc_assert(mm_resize(map, 1000));
        cmc_assert_equals(int32_t, 1, total_resize);

        cmc_assert(mm_resize(map, 100));
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(int32_t, 4, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 2, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        mm_customize(map, NULL, NULL);

        cmc_assert_equals(ptr, NULL, map->callbacks);

        mm_clear(map);
        cmc_assert(mm_insert(map, 1, 1));
        cmc_assert(mm_update(map, 1, 2, NULL));
        cmc_assert(mm_insert(map, 1, 1));
        cmc_assert_equals(size_t, 2, mm_update_all(map, 1, 3, NULL));
        cmc_assert(mm_remove(map, 1, NULL));
        cmc_assert(mm_insert(map, 1, 2));
        cmc_assert_equals(size_t, 2, mm_remove_all(map, 1, NULL));
        cmc_assert(mm_insert(map, 1, 2));
        cmc_assert(mm_max(map, NULL, NULL));
        cmc_assert(mm_min(map, NULL, NULL));
        cmc_assert_equals(size_t, 2, mm_get(map, 1));
        cmc_assert_not_equals(ptr, NULL, mm_get_ref(map, 1));
        cmc_assert(mm_contains(map, 1));
        cmc_assert(mm_resize(map, 1000));
        cmc_assert(mm_resize(map, 100));

        cmc_assert_equals(int32_t, 4, total_create);
        cmc_assert_equals(int32_t, 5, total_read);
        cmc_assert_equals(int32_t, 2, total_update);
        cmc_assert_equals(int32_t, 2, total_delete);
        cmc_assert_equals(int32_t, 2, total_resize);

        cmc_assert_equals(ptr, NULL, map->callbacks);

        mm_free(map);

        total_create = 0;
        total_read = 0;
        total_update = 0;
        total_delete = 0;
        total_resize = 0;
    });
});
