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
 * alloc.h
 *
 * Creation Date: 24/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_COR_ALLOC_H
#define CMC_COR_ALLOC_H

#ifndef CMC_NO_ALLOC

#ifdef CMC_CAMEL_CASE
#define CMC_ALLOC_NODE_NAME CMCAllocNode
#else
#define CMC_ALLOC_NODE_NAME cmc_alloc_node
#endif

#include "core.h"
/**
 * struct cmc_alloc_node
 *
 * Custom allocation node. Allows collections to use custom allocation
 * functions. The cmc_alloc_node_default is the default heap allocator provided
 * by the standard library.
 */

#define CMC_ALLOC_TYPE struct CMC_ALLOC_NODE_NAME *

static struct CMC_ALLOC_NODE_NAME
{
    void *(*malloc)(size_t);
    void *(*calloc)(size_t, size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
} CMC_UNUSED cmc_alloc_node_default = { malloc, calloc, realloc, free };

#endif /* CMC_NO_ALLOC */

#endif /* CMC_COR_ALLOC_H */
