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
 * flags.h
 *
 * Creation Date: 14/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_COR_FLAGS_H
#define CMC_COR_FLAGS_H

#include "core.h"

/**
 * enum cmc_flags
 *
 * Defines common error codes used by the entire library.
 */
// clang-format off
enum cmc_flags
{
    CMC_FLAG_OK        =  0, // No errors
    CMC_FLAG_ALLOC     =  1, // Allocation failed
    CMC_FLAG_EMPTY     =  2, // The collection is empty when it should not
    CMC_FLAG_NOT_FOUND =  3, // Key or value not found
    CMC_FLAG_INVALID   =  4, // Invalid argument or operation
    CMC_FLAG_RANGE     =  5, // Index out of range
    CMC_FLAG_DUPLICATE =  6, // Duplicate key or value
    CMC_FLAG_ERROR     =  7, // Generic error, usually caused by algorithm error
    CMC_FLAG_THREAD    =  8, // Generic error regarding threads
    CMC_FLAG_MUTEX     =  9, // Generic error regarding mutexes
    CMC_FLAG_FULL      = 10, // When a collection that doesn't resize is full
};
// clang-format on

/**
 * cmc_flags_to_str
 *
 * Maps the error codes to their character representation.
 */
CMC_UNUSED static const char *cmc_flags_to_str[11] = { "OK",        "ALLOC", "EMPTY",  "NOT_FOUND", "INVALID", "RANGE",
                                                       "DUPLICATE", "ERROR", "THREAD", "MUTEX",     "FULL" };

#endif /* CMC_COR_FLAGS_H */
