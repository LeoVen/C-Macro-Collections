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
 * treeset.h
 *
 * Creation Date: 27/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * TreeSet
 *
 * A TreeSet is an implementation of a Set that keeps its elements sorted. Like
 * a Set it has only unique keys. This implementation uses a balanced binary
 * tree called AVL Tree that uses the height of nodes to keep its keys balanced.
 */

#include "cor/core.h"

#ifdef CMC_DEV
#include "utl/log.h"
#endif

/**
 * Used values
 * V - treeset value data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/treeset/struct.h"

/* Function declaration */
#include "cmc/treeset/header.h"

/* Function implementation */
#include "cmc/treeset/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - treeset iterator
 * SETF - Set functions
 * STR - Print helper functions
 */
#define CMC_EXT_TREESET_PARTS INIT, ITER, SETF, STR
/**/
#include "cmc/treeset/ext/struct.h"
/**/
#include "cmc/treeset/ext/header.h"
/**/
#include "cmc/treeset/ext/code.h"

#include "cor/undef.h"
