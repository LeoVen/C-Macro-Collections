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
 * hashbidimap.h
 *
 * Creation Date: 26/09/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * HashBidiMap
 *
 * A bidirectional map is a map that allows you to create a bijection in both
 * directions between two sets of elements (K <-> V). It is possible to retrieve
 * the value using a key or retrieve a key using a value. The naming (key and
 * value) is simply used to differentiate between one set of elements and the
 * other set of elements.
 *
 * Implementation
 *
 * This implementation uses two arrays of pointers to an entry containing both
 * the key and the value.
 */

#include "cor/core.h"
#include "cor/hashtable.h"

#ifdef CMC_DEV
#include "utl/log.h"
#endif

/**
 * Used values
 * K - hashbidimap key data type
 * V - hashbidimap value data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

#if !defined(CMC_STRUCT) && !defined(CMC_HEADER) && !defined(CMC_CODE)
#include "hashbidimap/struct.h"
#include "hashbidimap/header.h"
#include "hashbidimap/code.h"
#endif

#ifdef CMC_STRUCT
#include "hashbidimap/struct.h"
#endif /* CMC_STRUCT */

#ifdef CMC_HEADER
#include "hashbidimap/header.h"
#endif /* CMC_HEADER */

#ifdef CMC_CODE
#include "hashbidimap/code.h"
#endif /* CMC_CODE */

#include "cor/undef.h"
