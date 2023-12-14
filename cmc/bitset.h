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

#if !defined(CMC_STRUCT) && !defined(CMC_HEADER) && !defined(CMC_CODE)
#include "bitset/struct.h"
#include "bitset/header.h"
#include "bitset/code.h"
#endif

#ifdef CMC_STRUCT
#include "bitset/struct.h"
#endif /* CMC_STRUCT */

#ifdef CMC_HEADER
#include "bitset/header.h"
#endif /* CMC_HEADER */

#ifdef CMC_CODE
#include "bitset/code.h"
#endif /* CMC_CODE */

#include "cor/undef.h"
