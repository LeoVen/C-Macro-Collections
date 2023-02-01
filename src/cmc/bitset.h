/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Libray.
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
 * Creation Date: 30/04/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * BitSet
 *
 * A Bit Set is an array where each bit can be individually modified and queried
 * by using bitwise operators such as |, &, ^, ~, >> and << (or, and, xor, not,
 * right shift, left shift respectively).
 *
 * Implementation
 *
 * This BitSet implementation uses an array of type cmc_bitset_word which can be
 * typedefed to any unsigned type such as uint8_t, uint16_t, uint32_t, uint64_t,
 * size_t, etc. The BitSet does not make use of K or V. Because of that, it also
 * doesn't have Functions Tables.
 *
 * The BitSet is initialized with a custom capacity but, if a bit index accessed
 * is greater than the total capacity, the BitSet will resize. This means that
 * the BitSet will try to guarantee that every bit index is accessible, as long
 * as there is enough memory.
 */

#include "cor/bitset.h"
#include "cor/core.h"

#ifdef CMC_DEV
#include "utl/log.h"
#endif

/**
 * Used values
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/bitset/struct.h"

/* Function declaration */
#include "cmc/bitset/header.h"

/* Function implementation */
#include "cmc/bitset/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - bitset iterator
 * STR - Print helper functions
 */
#define CMC_EXT_BITSET_PARTS INIT, ITER, STR
/**/
#include "cmc/bitset/ext/struct.h"
/**/
#include "cmc/bitset/ext/header.h"
/**/
#include "cmc/bitset/ext/code.h"

#include "cor/undef.h"
