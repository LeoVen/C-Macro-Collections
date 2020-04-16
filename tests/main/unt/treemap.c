#include "utl.c"
#include "utl/assert.h"
#include "utl/log.h"
#include "utl/test.h"

#include "../src/treemap.c"

struct treemap_fkey *tm_fkey =
    &(struct treemap_fkey){ .cmp = cmp,
                                .cpy = copy,
                                .str = str,
                                .free = custom_free,
                                .hash = hash,
                                .pri = pri };

struct treemap_fval *tm_fval =
    &(struct treemap_fval){ .cmp = cmp,
                                .cpy = copy,
                                .str = str,
                                .free = custom_free,
                                .hash = hash,
                                .pri = pri };

CMC_CREATE_UNIT(treemap_test, true, {
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
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map));

        // clear
        map->flag = cmc_flags.ERROR;
        tm_clear(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map));

        // insert
        map->flag = cmc_flags.ERROR;
        cmc_assert(tm_insert(map, 1, 1));
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map));

        cmc_assert(!tm_insert(map, 1, 2));
        cmc_assert_equals(int32_t, cmc_flags.DUPLICATE, tm_flag(map));

        // update
        cmc_assert(!tm_update(map, 2, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, tm_flag(map));

        cmc_assert(tm_update(map, 1, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map));

        // remove
        cmc_assert(!tm_remove(map, 2, NULL));
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, tm_flag(map));

        cmc_assert(tm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map));

        cmc_assert(!tm_remove(map, 1, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, tm_flag(map));

        // max min
        map->flag = cmc_flags.ERROR;
        cmc_assert(!tm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, tm_flag(map));

        map->flag = cmc_flags.ERROR;
        cmc_assert(!tm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, tm_flag(map));

        cmc_assert(tm_insert(map, 1, 1));
        map->flag = cmc_flags.ERROR;
        cmc_assert(tm_max(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map));

        map->flag = cmc_flags.ERROR;
        cmc_assert(tm_min(map, NULL, NULL));
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map));

        // get get_ref
        tm_get(map, 2);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, tm_flag(map));

        tm_get(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map));

        tm_get_ref(map, 2);
        cmc_assert_equals(int32_t, cmc_flags.NOT_FOUND, tm_flag(map));

        tm_get_ref(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map));

        cmc_assert(tm_remove(map, 1, NULL));
        map->flag = cmc_flags.ERROR;
        tm_get(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, tm_flag(map));

        map->flag = cmc_flags.ERROR;
        tm_get_ref(map, 1);
        cmc_assert_equals(int32_t, cmc_flags.EMPTY, tm_flag(map));

        // copy_of
        map->flag = cmc_flags.ERROR;
        struct treemap *map2 = tm_copy_of(map);
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map2));

        // equals
        map->flag = cmc_flags.ERROR;
        map2->flag = cmc_flags.ERROR;
        cmc_assert(tm_equals(map, map2));
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map));
        cmc_assert_equals(int32_t, cmc_flags.OK, tm_flag(map2));

        tm_free(map);
        tm_free(map2);
    });

    CMC_CREATE_TEST(callbacks, {
        struct treemap *map =
            tm_new_custom(tm_fkey, tm_fval, NULL, callbacks);

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
