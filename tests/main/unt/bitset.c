#include "utl.c"
#include "utl/assert.h"
#include "utl/test.h"

#include "../src/bitset.c"

struct cmc_alloc_node *bs_alloc_node = &(struct cmc_alloc_node){
    .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free
};

CMC_CREATE_UNIT(BitSet, true, {
    const size_t word_bytes = sizeof(cmc_bitset_word);
    const size_t word_bits = sizeof(cmc_bitset_word) * CHAR_BIT;

    printf("%s", cmc_test_color(5));
    printf("    BitSet<"CMC_TO_STRING(CMC_BITSET_WORD_TYPE)">\n");
    printf("%s", cmc_test_color(0));

    CMC_CREATE_TEST(PFX##_new(), {
        struct bitset *bs = bs_new(64);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_not_equals(ptr, NULL, bs->buffer);
        cmc_assert_equals(size_t, bs_bit_to_index(63) + 1, bs->capacity);
        cmc_assert_equals(ptr, &cmc_alloc_node_default, bs->alloc);
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
        cmc_assert_equals(ptr, &cmc_alloc_node_default, bs.alloc);
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

        bs->flag = cmc_flags.ERROR;
        bs_customize(bs, bs_alloc_node, callbacks);

        cmc_assert_equals(ptr, bs_alloc_node, bs->alloc);
        cmc_assert_equals(ptr, callbacks, bs->callbacks);
        cmc_assert_equals(int32_t, cmc_flags.OK, bs->flag);

        bs->flag = cmc_flags.ERROR;
        bs_customize(bs, NULL, NULL);

        cmc_assert_equals(ptr, &cmc_alloc_node_default, bs->alloc);
        cmc_assert_equals(ptr, NULL, bs->callbacks);
        cmc_assert_equals(int32_t, cmc_flags.OK, bs->flag);

        bs_free(bs);
    });

    CMC_CREATE_TEST(PFX##_set(), {
        struct bitset *bs = bs_new(word_bits);

        cmc_assert_not_equals(ptr, NULL, bs);
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 0, bs->buffer[0]);

        bs->flag = cmc_flags.ERROR;
        cmc_assert(bs_set(bs, 0));
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, 1, bs->buffer[0]);
        cmc_assert_equals(int32_t, cmc_flags.OK, bs->flag);

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

        bs->flag = cmc_flags.ERROR;
        cmc_assert(bs_set_range(bs, 0, 0));
        cmc_assert_equals(uint8_t, 1, bs->buffer[0]);
        cmc_assert_equals(int32_t, cmc_flags.OK, bs->flag);

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
        cmc_assert_equals(CMC_BITSET_WORD_TYPE, (~(cmc_bitset_word)0),
                   bs->buffer[bs_bit_to_index(word_bits * 20)]);

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

        cmc_assert(bs_clear_range(bs, 0, word_bits * 100 - 1));
        cmc_assert_equals(size_t, 100, bs->capacity);

        cmc_assert(bs_set_range(bs, word_bits * 20, word_bits * 21 - 1));
        cmc_assert(bs_clear_range(bs, word_bits * 20, word_bits * 21 - 1));

        cmc_assert_equals(uint32_t, 0,
                          bs->buffer[bs_bit_to_index(word_bits * 20)]);

        for (size_t i = 0; i < bs->capacity; i++)
            bs->buffer[i] = ~((cmc_bitset_word)0);

        cmc_assert(bs_clear_range(bs, 0, word_bits * 200 - 1));

        cmc_assert_equals(size_t, 200, bs->capacity);

        for (size_t i = 0; i < bs->capacity; i++)
            cmc_assert_equals(size_t, 0, bs->buffer[0]);

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

CMC_CREATE_UNIT(BitSetIter, true, {
    const size_t word_bytes = sizeof(cmc_bitset_word);
    const size_t word_bits = sizeof(cmc_bitset_word) * CHAR_BIT;

    printf("%s", cmc_test_color(5));
    printf("    BitSet<"CMC_TO_STRING(CMC_BITSET_WORD_TYPE)">\n");
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
    int result = BitSet() + BitSetIter();

    printf(" +---------------------------------------------------------------+");
    printf("\n");
    printf(" | BitSet Suit : %-47s |\n", result == 0 ? "PASSED" : "FAILED");
    printf(" +---------------------------------------------------------------+");
    printf("\n\n\n");

    return result;
}
#endif
