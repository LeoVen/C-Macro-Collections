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
 * treemap.h
 *
 * Creation Date: 28/03/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * TreeMap
 *
 * A TreeMap is an implementation of a Map that keeps its keys sorted. Like a
 * Map, it has only unique keys. This implementation uses a balanced binary
 * tree called AVL Tree that uses the height of nodes to keep its keys balanced.
 */

#include "cor/core.h"

/**
 * Used values
 * K - treemap key data type
 * V - treemap value data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/treemap/struct.h"

/* Function declaration */
#include "cmc/treemap/header.h"

/* Function implementation */
#include "cmc/treemap/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - treemap iterator
 * STR - Print helper functions
 */
#define CMC_EXT_TREEMAP_PARTS INIT, ITER, STR
/**/
#include "cmc/treemap/ext/struct.h"
/**/
#include "cmc/treemap/ext/header.h"
/**/
#include "cmc/treemap/ext/code.h"

#include "cor/undef.h"
