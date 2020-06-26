/**
 * cor/bitset.h
 *
 * Creation Date: 20/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Things commonly used by bitset collections.
 */

#ifndef CMC_COR_BITSET_H
#define CMC_COR_BITSET_H

#include <stdint.h>

/* Defines the underlaying data type for the bitset array */
/* Possible values: uint64_t, uint32_t, uint16_t, uint8_t, size_t */
#ifndef CMC_BITSET_WORD_TYPE
#define CMC_BITSET_WORD_TYPE uint32_t
#endif /* CMC_BITSET_WORD_TYPE */

typedef CMC_BITSET_WORD_TYPE cmc_bitset_word;

/**
 * cmc_bit_to_index
 *
 * Translates a bit index to a word index
 */
static inline size_t cmc_bidx_to_widx(size_t idx)
{
    /* Calculate how many shifts based on the size of cmc_bitset_word */
    static const size_t shift =
        ((sizeof(cmc_bitset_word) * 8) >> 6) > 0
            ? 6
            : ((sizeof(cmc_bitset_word) * 8) >> 5) > 0 ? 5 : ((sizeof(cmc_bitset_word) * 8) >> 4) > 0 ? 4 : 3;

    return idx >> shift;
}

#endif /* CMC_COR_BITSET_H */
