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
 * stack.h
 *
 * Creation Date: 14/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Stack
 *
 * A Stack is a Last-in First-out (or First-in Last-out) data structure used in
 * a variety of algorithms. It is a Dynamic Array that can only add or remove
 * its elements at the end of the buffer, in this case, it represents the top of
 * the stack.
 *
 * It has three main functions: `push` which adds an element at the top of the
 * stack; `pop` which removes the top element from the stack; and `top` which
 * returns the top element without removing it (it is also sometimes called
 * `peek`).
 *
 * A Stack is used in algorithms like backtracking, depth-first search,
 * expression evaluation, syntax parsing and many more.
 */

#include "cor/core.h"

#ifdef CMC_DEV
#include "utl/log.h"
#endif

/**
 * Used values
 * V - stack data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/stack/struct.h"

/* Function declaration */
#include "cmc/stack/header.h"

/* Function implementation */
#include "cmc/stack/code.h"

/**
 * Extensions
 *
 * ITER - Stack iterator
 * STR - Print helper functions
 */
#define CMC_EXT_STACK_PARTS INIT, ITER, STR
/**/
#include "cmc/stack/ext/struct.h"
/**/
#include "cmc/stack/ext/header.h"
/**/
#include "cmc/stack/ext/code.h"

#include "cor/undef.h"
