/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Library.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * bitset.h
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

#include "core.h"

/* Defines the underlaying data type for the bitset array */
/* Possible values: uint64_t, uint32_t, uint16_t, uint8_t, size_t */
#ifndef CMC_BITSET_WORD_TYPE
#define CMC_BITSET_WORD_TYPE uint32_t
#endif /* CMC_BITSET_WORD_TYPE */

typedef CMC_BITSET_WORD_TYPE cmc_bitset_word;

/**
 * cmc_bidx_to_widx
 *
 * Translates a bit index to a word index
 */
static inline size_t cmc_bidx_to_widx(size_t idx)
{
    /* Calculate how many shifts based on the size of cmc_bitset_word */
    static const size_t shift = ((sizeof(cmc_bitset_word) * 8) >> 6) > 0   ? 6
                                : ((sizeof(cmc_bitset_word) * 8) >> 5) > 0 ? 5
                                : ((sizeof(cmc_bitset_word) * 8) >> 4) > 0 ? 4
                                                                           : 3;

    return idx >> shift;
}

#endif /* CMC_COR_BITSET_H */
