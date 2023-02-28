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
 * hashmultiset.h
 *
 * Creation Date: 10/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * HashMultiSet
 *
 * In mathematics, a multiset is a modification of the concept of a set that,
 * unlike a set, allows for multiple instances for each of its elements. The
 * positive integer number of instances, given for each element is called the
 * multiplicity of this element in the multiset. A multiSet also has a
 * cardinality which equals the sum of the multiplicities of its elements.
 *
 * Implementation
 *
 * In this implementation, the values are stored in a flat hashtable and are
 * mapped to their multiplicity.
 */

#include "cor/core.h"
#include "cor/hashtable.h"

#ifdef CMC_DEV
#include "utl/log.h"
#endif

/**
 * Used values
 * V - hashmultiset value data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/hashmultiset/struct.h"

/* Function declaration */
#include "cmc/hashmultiset/header.h"

/* Function implementation */
#include "cmc/hashmultiset/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - hashmultiset iterator
 * STR - Print helper functions
 */
#define CMC_EXT_HASHMULTISET_PARTS INIT, ITER, SETF, STR
/**/
#include "cmc/hashmultiset/ext/struct.h"
/**/
#include "cmc/hashmultiset/ext/header.h"
/**/
#include "cmc/hashmultiset/ext/code.h"

#include "cor/undef.h"
