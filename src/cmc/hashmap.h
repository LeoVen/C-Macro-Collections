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
 * hashmap.h
 *
 * Creation Date: 03/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * HashMap
 *
 * A HashMap is an implementation of a Map with unique keys, where every key is
 * mapped to a value. The keys are not sorted. It is implemented as a flat
 * hashtable with linear probing and robin hood hashing.
 */

#include "cor/core.h"
#include "cor/hashtable.h"

/**
 * Used values
 * K - hashmap key data type
 * V - hashmap value data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/hashmap/struct.h"

/* Function declaration */
#include "cmc/hashmap/header.h"

/* Function implementation */
#include "cmc/hashmap/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - hashmap iterator
 * STR - Print helper functions
 */
#define CMC_EXT_HASHMAP_PARTS INIT, ITER, STR
/**/
#include "cmc/hashmap/ext/struct.h"
/**/
#include "cmc/hashmap/ext/header.h"
/**/
#include "cmc/hashmap/ext/code.h"

#include "cor/undef.h"
