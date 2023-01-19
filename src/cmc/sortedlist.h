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
 * sortedlist.h
 *
 * Creation Date: 17/09/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * SortedList
 *
 * A sorted list is a dynamic array, meaning that you can store as many elements
 * as you like and when its capacity is full, the buffer is reallocated. The
 * elements are only sorted when a certain action requires that the array is
 * sorted like accessing min() or max(). This prevents the array from being
 * sorted after every insertion or removal. The array is sorted using a
 * variation of quick sort that uses insertion sort for small partitions.
 */

#include "cor/core.h"

/**
 * Used values
 * V - sortedlist data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/sortedlist/struct.h"

/* Function declaration */
#include "cmc/sortedlist/header.h"

/* Function implementation */
#include "cmc/sortedlist/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - List iterator
 * STR - Print helper functions
 */
#define CMC_EXT_SORTEDLIST_PARTS INIT, ITER, STR
/**/
#include "cmc/sortedlist/ext/struct.h"
/**/
#include "cmc/sortedlist/ext/header.h"
/**/
#include "cmc/sortedlist/ext/code.h"

#include "cor/undef.h"
