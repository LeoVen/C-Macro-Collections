#ifndef CMC_TESTS_UNT_CMC_ITSET_H
#define CMC_TESTS_UNT_CMC_ITSET_H

#include "../utl.h"

#include "cmc/bitset.h"

struct cmc_alloc_node *bs_alloc_node =
    &(struct cmc_alloc_node){ .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free };

CMC_CREATE_UNIT(CMCBitSet, true, {
    const size_t word_bits = sizeof(cmc_bitset_word) * CHAR_BIT;

    printf("%s", cmc_test_color(5));
    printf("    BitSet<" CMC_TO_STRING(CMC_BITSET_WORD_TYPE) ">\n");
    printf("%s", cmc_test_color(0));

    CMC_CREATE_TEST(PFX##_new(), {
        struct bitset *bs = bs_new(64);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_not_equals(ptr, NULL, bs->buffer);
        cmc_assert_equals(size_t, bs_bit_to_index(63) + 1, bs->capacity);
        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, bs->alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, bs->alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, bs->alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, bs->alloc->free);
        cmc_assert_equals(ptr, NULL, bs->callbacks);

        bs_free(bs);

        bs = bs_new(129);

        cmc_assert_equals(size_t, bs_bit_to_index(128) + 1, bs->capacity);

        bs_free(bs);

        bs = bs_new(0);

        cmc_assert_equals(ptr, NULL, bs);
    });

    CMC_CREATE_TEST(PFX##_new_custom(), {
        struct bitset *bs = bs_new_custom(64, bs_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_not_equals(ptr, NULL, bs->buffer);
        cmc_assert_equals(size_t, bs_bit_to_index(63) + 1, bs->capacity);
        cmc_assert_equals(ptr, bs_alloc_node, bs->alloc);
        cmc_assert_equals(ptr, callbacks, bs->callbacks);

        bs_free(bs);

        bs = bs_new_custom(129, NULL, NULL);

        cmc_assert_equals(size_t, bs_bit_to_index(128) + 1, bs->capacity);
        bs_free(bs);

        bs = bs_new_custom(0, bs_alloc_node, callbacks);

        cmc_assert_equals(ptr, NULL, bs);
    });

    CMC_CREATE_TEST(PFX##_init(), {
        struct bitset bs = bs_init(64);

        cmc_assert_not_equals(ptr, NULL, bs.buffer);
        cmc_assert_equals(size_t, bs_bit_to_index(63) + 1, bs.capacity);
        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, bs.alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, bs.alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, bs.alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, bs.alloc->free);
        cmc_assert_equals(ptr, NULL, bs.callbacks);

        bs_release(bs);

        bs = bs_init(129);

        cmc_assert_equals(size_t, bs_bit_to_index(128) + 1, bs.capacity);

        bs_release(bs);

        bs = bs_init(0);

        cmc_assert_equals(ptr, NULL, bs.buffer);
    });

    CMC_CREATE_TEST(PFX##_init_custom(), {
        struct bitset bs = bs_init_custom(64, bs_alloc_node, callbacks);

        cmc_assert_not_equals(ptr, NULL, bs.buffer);
        cmc_assert_equals(size_t, bs_bit_to_index(63) + 1, bs.capacity);
        cmc_assert_equals(ptr, bs_alloc_node, bs.alloc);
        cmc_assert_equals(ptr, callbacks, bs.callbacks);

        bs_release(bs);

        bs = bs_init_custom(129, NULL, NULL);

        cmc_assert_equals(size_t, bs_bit_to_index(128) + 1, bs.capacity);

        bs_release(bs);

        bs = bs_init_custom(0, bs_alloc_node, callbacks);

        cmc_assert_equals(ptr, NULL, bs.buffer);
    });

    CMC_CREATE_TEST(PFX##_customize(), {
        struct bitset *bs = bs_new(word_bits);

        cmc_assert_not_equals(ptr, NULL, bs);

        bs->flag = CMC_FLAG_ERROR;
        bs_customize(bs, bs_alloc_node, callbacks);

        cmc_assert_equals(ptr, bs_alloc_node, bs->alloc);
        cmc_assert_equals(ptr, callbacks, bs->callbacks);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, bs->flag);

        bs->flag = CMC_FLAG_ERROR;
        bs_customize(bs, NULL, NULL);

        cmc_assert_equals(ptr, cmc_alloc_node_default.malloc, bs->alloc->malloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.calloc, bs->alloc->calloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.realloc, bs->alloc->realloc);
        cmc_assert_equals(ptr, cmc_alloc_node_default.free, bs->alloc->free);
        cmc_assert_equals(ptr, NULL, bs->callbacks);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, bs->flag);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_set(), {
        struct bitset *bs = bs_new(word_bits);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        bs->flag = CMC_FLAG_ERROR;
        cmc_assert(bs_set(bs, 0));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 1, bs->buffer[0]);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, bs->flag);

        bs->buffer[0] = 0;

        cmc_assert(bs_set(bs, word_bits - 1));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, ((cmc_bitset_word)1 << (word_bits - 1)), bs->buffer[0]);

        cmc_assert(bs_set(bs, word_bits * 8 - 1));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, ((cmc_bitset_word)1 << (word_bits - 1)),
                          bs->buffer[bs_bit_to_index(word_bits * 8 - 1)]);

        cmc_assert(bs_set(bs, word_bits * 18 - 2));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, ((cmc_bitset_word)1 << (word_bits - 2)),
                          bs->buffer[bs_bit_to_index(word_bits * 18 - 2)]);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_set_range(), {
        struct bitset *bs = bs_new(word_bits);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        bs->flag = CMC_FLAG_ERROR;
        cmc_assert(bs_set_range(bs, 0, 0));
        cmc_assert_equals(uint8_t, 1, bs->buffer[0]);
        cmc_assert_equals(int32_t, CMC_FLAG_OK, bs->flag);

        cmc_assert(!bs_set_range(bs, 1, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_INVALID, bs->flag);

        bs->buffer[0] = 0;

        cmc_assert(bs_set_range(bs, 4, 7)); /* 1111 0000 */
        cmc_assert_equals(uint8_t, 240, bs->buffer[0]);

        cmc_assert(bs_set_range(bs, 2, 2)); /* 1111 0100 */
        cmc_assert_equals(uint8_t, 244, bs->buffer[0]);

        cmc_assert(bs_set_range(bs, word_bits * 6 - 2, word_bits * 6 - 2));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, ((cmc_bitset_word)1 << (word_bits - 2)),
                          bs->buffer[bs_bit_to_index(word_bits * 6 - 2)]);

        // Make sure we have at least word_bits * 40 slots
        bs_set(bs, word_bits * 40);

        cmc_assert(bs_set_range(bs, word_bits * 20, word_bits * 21 - 1));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, (~(cmc_bitset_word)0), bs->buffer[bs_bit_to_index(word_bits * 20)]);

        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[bs_bit_to_index(word_bits * 20 - 1)]);
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[bs_bit_to_index(word_bits * 21 + 1)]);

        cmc_assert(bs_set_range(bs, 0, word_bits * 100 - 1));
        cmc_assert_equals(size_t, 100, bs->capacity);

        for (size_t i = 0; i < bs->capacity; i++)
            cmc_assert_equals(CMC_BITSET_WORD_TYPE, ~((cmc_bitset_word)0), bs->buffer[i]);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_clear(), {
        struct bitset *bs = bs_new(word_bits * 10 - 1);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        bs->buffer[0] = 1; // 0001
        cmc_assert(bs_clear(bs, 0));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        bs->buffer[0] = 10; // 1010
        cmc_assert(bs_clear(bs, 1));
        cmc_assert(bs_clear(bs, 3));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        cmc_assert(bs_clear(bs, word_bits * 100 - 1));
        cmc_assert_equals(size_t, 100, bs->capacity);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_clear_range(), {
        struct bitset *bs = bs_new(word_bits * 10 - 1);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        cmc_assert(!bs_clear_range(bs, 1, 0));
        cmc_assert_equals(int32_t, CMC_FLAG_INVALID, bs->flag);

        cmc_assert(bs_clear_range(bs, 0, word_bits * 100 - 1));
        cmc_assert_equals(size_t, 100, bs->capacity);

        cmc_assert(bs_set_range(bs, word_bits * 20, word_bits * 21 - 1));
        cmc_assert(bs_clear_range(bs, word_bits * 20, word_bits * 21 - 1));

        cmc_assert_equals(uint32_t, 0, bs->buffer[bs_bit_to_index(word_bits * 20)]);

        for (size_t i = 0; i < bs->capacity; i++)
            bs->buffer[i] = ~((cmc_bitset_word)0);

        cmc_assert(bs_set_range(bs, 0, word_bits * 100 - 1));
        cmc_assert(bs_clear_range(bs, 0, word_bits * 100 - 1));

        cmc_assert_equals(size_t, 100, bs->capacity);

        for (size_t i = 0; i < bs->capacity; i++)
            cmc_assert_equals(size_t, 0, bs->buffer[0]);

        cmc_assert(bs_clear_range(bs, 0, word_bits * 200 - 1));
        cmc_assert_equals(size_t, 200, bs->capacity);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_flip(), {
        struct bitset *bs = bs_new(word_bits * 10 - 1);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        cmc_assert(bs_flip(bs, 0));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 1, bs->buffer[0]);

        cmc_assert(bs_flip(bs, 0));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        cmc_assert(bs_set_range(bs, 0, 2)); // 0111
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 7, bs->buffer[0]);
        cmc_assert(bs_flip(bs, 1)); // 0101
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 5, bs->buffer[0]);

        cmc_assert(bs_flip(bs, word_bits * 100 - 1));
        cmc_assert_equals(size_t, 100, bs->capacity);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_flip_range(), {
        struct bitset *bs = bs_new(word_bits * 10 - 1);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        cmc_assert(bs_flip_range(bs, 0, 0));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 1, bs->buffer[0]);

        cmc_assert(bs_flip_range(bs, 0, 0));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        cmc_assert(bs_flip_range(bs, 1, 4)); // 0001 1110
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 30, bs->buffer[0]);

        cmc_assert(bs_flip_range(bs, 2, 3)); // 0001 0010
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 18, bs->buffer[0]);

        cmc_assert(bs_flip_range(bs, 0, 2)); // 0001 0101
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 21, bs->buffer[0]);

        bs->buffer[0] = 0;

        cmc_assert(bs_flip_range(bs, 0, word_bits * 100 - 1));
        cmc_assert_equals(size_t, 100, bs->capacity);

        for (size_t i = 0; i < bs->capacity; i++)
            cmc_assert_equals(CMC_BITSET_WORD_TYPE, ~((cmc_bitset_word)0), bs->buffer[i]);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_put(), {
        struct bitset *bs = bs_new(word_bits * 10 - 1);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        cmc_assert(bs_put(bs, 1, true)); // 0010
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 2, bs->buffer[0]);
        cmc_assert(bs_put(bs, 0, false)); // 0010
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 2, bs->buffer[0]);
        cmc_assert(bs_put(bs, 1, false)); // 0000
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);
        cmc_assert(bs_put(bs, 3, true)); // 1000
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 8, bs->buffer[0]);
        cmc_assert(bs_put(bs, 0, true)); // 1001
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 9, bs->buffer[0]);
        cmc_assert(bs_put(bs, 3, false)); // 0001
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 1, bs->buffer[0]);

        cmc_assert(bs_put(bs, word_bits * 100 - 1, true));
        cmc_assert_equals(size_t, 100, bs->capacity);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_put_range(), {
        struct bitset *bs = bs_new(word_bits * 10 - 1);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        cmc_assert(bs_put_range(bs, 1, 2, true)); // 0110
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 6, bs->buffer[0]);
        cmc_assert(bs_put_range(bs, 0, 1, false)); // 0100
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 4, bs->buffer[0]);
        cmc_assert(bs_put_range(bs, 1, 3, true)); // 1110
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 14, bs->buffer[0]);
        cmc_assert(bs_put_range(bs, 2, 3, false)); // 0010
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 2, bs->buffer[0]);

        cmc_assert(bs_put_range(bs, 0, word_bits * 100 - 1, true));
        cmc_assert_equals(size_t, 100, bs->capacity);

        for (size_t i = 0; i < bs->capacity; i++)
            cmc_assert_equals(CMC_BITSET_WORD_TYPE, ~((cmc_bitset_word)0), bs->buffer[i]);

        cmc_assert(bs_put_range(bs, 0, word_bits * 100 - 1, false));
        for (size_t i = 0; i < bs->capacity; i++)
            cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[i]);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_set_all(), {
        struct bitset *bs = bs_new(word_bits * 10 - 1);

        cmc_assert_not_equals(ptr, NULL, bs);

        cmc_assert(bs_set_all(bs));

        for (size_t i = 0; i < bs->capacity; i++)
            cmc_assert_equals(CMC_BITSET_WORD_TYPE, ~((cmc_bitset_word)0), bs->buffer[i]);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_clear_all(), {
        struct bitset *bs = bs_new(word_bits * 10 - 1);

        cmc_assert_not_equals(ptr, NULL, bs);

        cmc_assert(bs_set_all(bs));
        cmc_assert(bs_clear_all(bs));

        for (size_t i = 0; i < bs->capacity; i++)
            cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[i]);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_flip_all(), {
        struct bitset *bs = bs_new(word_bits * 10 - 1);

        cmc_assert_not_equals(ptr, NULL, bs);

        cmc_assert(bs_flip_all(bs));
        for (size_t i = 0; i < bs->capacity; i++)
            cmc_assert_equals(CMC_BITSET_WORD_TYPE, ~((cmc_bitset_word)0), bs->buffer[i]);

        cmc_assert(bs_flip_all(bs));
        for (size_t i = 0; i < bs->capacity; i++)
            cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[i]);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_get(), {
        struct bitset *bs = bs_new(64);

        cmc_assert_not_equals(ptr, NULL, bs);

        cmc_assert(bs_set(bs, 1));

        cmc_assert(bs_get(bs, 1));

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_resize(), {
        struct bitset *bs = bs_new(64);

        // WIP

        bs_free(bs);
    });
});

CMC_CREATE_UNIT(CMCBitSetIter, true, {
    // const size_t word_bytes = sizeof(cmc_bitset_word);
    // const size_t word_bits = sizeof(cmc_bitset_word) * CHAR_BIT;

    printf("%s", cmc_test_color(5));
    printf("    BitSet<" CMC_TO_STRING(CMC_BITSET_WORD_TYPE) ">\n");
    printf("%s", cmc_test_color(0));

    CMC_CREATE_TEST(PFX##_iter_start(), {
        struct bitset *bs = bs_new(64);

        // WIP

        bs_free(bs);
    });
});

#ifdef CMC_TEST_MAIN
int main(void)
{
    int result = CMCBitSet() + CMCBitSetIter();

    printf(" +---------------------------------------------------------------+");
    printf("\n");
    printf(" | CMCBitSet Suit : %-47s |\n", result == 0 ? "PASSED" : "FAILED");
    printf(" +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif

#endif /* CMC_TESTS_UNT_CMC_ITSET_H */
