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
 * ext_sac_list.h
 *
 * Creation Date: 06/10/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_SAC_LIST_H
#define CMC_EXT_SAC_LIST_H

#include "cor_core.h"

/**
 * All the EXT parts of SAC List.
 */
#define CMC_EXT_SAC_LIST_PARTS PLACEHOLDER

#define CMC_EXT_SAC_LIST_PLACEHOLDER(ACCESS, FILE, PARAMS) \
    CMC_(CMC_(CMC_EXT_SAC_LIST_PLACEHOLDER_, ACCESS), CMC_(_, FILE))(PARAMS)

#define CMC_EXT_SAC_LIST_PLACEHOLDER_PUBLIC_HEADER(PARAMS)

#define CMC_EXT_SAC_LIST_PLACEHOLDER_PUBLIC_SOURCE(PARAMS)

#define CMC_EXT_SAC_LIST_PLACEHOLDER_PRIVATE_HEADER(PARAMS)

#define CMC_EXT_SAC_LIST_PLACEHOLDER_PRIVATE_SOURCE(PARAMS)

#endif /* CMC_EXT_SAC_LIST_H */
