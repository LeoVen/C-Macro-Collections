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
 * prelude.h
 *
 * Creation Date: 12/01/2023
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Common undefs
 */

#ifndef CMC_ARGS_FALLTHROUGH

#undef V
#undef K
#undef PFX
#undef SNAME
#undef SIZE

/* Deque */
#undef CMC_EXT_DEQUE_INIT
#undef CMC_EXT_DEQUE_ITER
#undef CMC_EXT_DEQUE_STR

/* Hashbidimap */
#undef CMC_EXT_HASHBIDIMAP_INIT
#undef CMC_EXT_HASHBIDIMAP_ITER
#undef CMC_EXT_HASHBIDIMAP_STR

/* List */
#undef CMC_EXT_LIST_INIT
#undef CMC_EXT_LIST_ITER
#undef CMC_EXT_LIST_SEQ
#undef CMC_EXT_LIST_STR

/* Queue */
#undef CMC_EXT_QUEUE_INIT
#undef CMC_EXT_QUEUE_ITER
#undef CMC_EXT_QUEUE_STR

/* Stack */
#undef CMC_EXT_STACK_INIT
#undef CMC_EXT_STACK_ITER
#undef CMC_EXT_STACK_STR

#endif
