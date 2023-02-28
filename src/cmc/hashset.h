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
 * hashset.h
 *
 * Creation Date: 01/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * HashSet
 *
 * A HashSet is an implementation of a Set with unique keys. The keys are not
 * sorted. It is implemented as a flat hashtable with linear probing and robin
 * hood hashing.
 */

#include "cor/core.h"
#include "cor/hashtable.h"

#ifdef CMC_DEV
#include "utl/log.h"
#endif

/**
 * Used values
 * V - hashset value data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/hashset/struct.h"

/* Function declaration */
#include "cmc/hashset/header.h"

/* Function implementation */
#include "cmc/hashset/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - hashset iterator
 * STR - Print helper functions
 */
#define CMC_EXT_HASHSET_PARTS ITER, SETF, STR
/**/
#include "cmc/hashset/ext/struct.h"
/**/
#include "cmc/hashset/ext/header.h"
/**/
#include "cmc/hashset/ext/code.h"

#include "cor/undef.h"
