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
 * cor_callbacks.h
 *
 * Creation Date: 24/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_COR_CALLBACKS_H
#define CMC_COR_CALLBACKS_H

#ifdef CMC_CAMEL_CASE
#define CMC_CALLBACKS_NAME CMCCallbacks
#else
#define CMC_CALLBACKS_NAME cmc_callbacks
#endif

#ifdef CMC_NO_CALLBACKS

#define CMC_CALLBACKS_DECL
#define CMC_CALLBACKS_MAYBE_UNUSED(param) ((void)param) /* Prevent -Wunused-parameter */
#define CMC_CALLBACKS_GET(ds) NULL                      /* Helps with compatibility */
#define CMC_CALLBACKS_ASSIGN(ds, cb)
#define CMC_CALLBACKS_CALL(ds, cb)

#else

#define CMC_CALLBACKS_DECL struct CMC_CALLBACKS_NAME *callbacks
#define CMC_CALLBACKS_MAYBE_UNUSED(param)
#define CMC_CALLBACKS_GET(ds) (ds)->callbacks
#define CMC_CALLBACKS_ASSIGN(ds, cb) CMC_CALLBACKS_GET(ds) = cb
#define CMC_CALLBACKS_CALL(ds, cb) \
    if (CMC_CALLBACKS_GET(ds) && CMC_CALLBACKS_GET(ds)->cb) \
        CMC_CALLBACKS_GET(ds)->cb();

#endif

/**
 * struct cmc_callbacks
 *
 * Callback node. Each collection will call one of these functions when:
 * - create : an element was successfully added to the collection
 * - read   : the collection was successfully queried about an element
 * - update : an element in the collection was successfully updated
 * - delete : an element was successfully removed from the collection
 * - resize : the collection was full and successfully resized
 */
struct CMC_CALLBACKS_NAME
{
    void (*create)(void);
    void (*read)(void);
    void (*update)(void);
    void (*delete)(void);
    void (*resize)(void);
};

#endif /* CMC_COR_CALLBACKS_H */
