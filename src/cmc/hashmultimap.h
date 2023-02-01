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
 * hashmultimap.h
 *
 * Creation Date: 26/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * HashMultiMap
 *
 * The HashMultiMap is a Map that allows for multiple keys. This is a data
 * structure that has a very narrow usage. A Map (either TreeMap or HashMap) can
 * also work like a HashMultiMap if a certain key is mapped to another
 * collection.
 *
 * Map<K = int, V = List<int>> maps an integer to a list of integer.
 * HashMultiMap<K = int, V = int> maps many integer keys to integer values.
 *
 * The difference is that in a HashMultiMap you can store keys that are the same
 * but might be different instances of the same value. This is also relevant if
 * your data type is more complex like a struct where its ID is the same but
 * some other members of this data type are different.
 *
 * Implementation
 *
 * The map uses separate chaining and robin hood hashing. Its internal buffer
 * is made of a pair of pointers to entries that represent a linked list. So
 * accessing map->buffer[0] would give the first pair of pointers. Each entry
 * has both a pointer to a previous entry and a next entry. This design choice
 * was made so that every collection in this library has a two-way iterator
 * with a very few exceptions.
 *
 * Each entry is composed of a Key and a Value. Entries with the same key should
 * always hash to the same linked list. Also, keys that hash to the same bucket
 * will also be in the same linked list.
 *
 * The order of inserting and removing the same keys will behave like a FIFO. So
 * the first key added will be the first to be removed.
 */

#include "cor/core.h"
#include "cor/hashtable.h"

#ifdef CMC_DEV
#include "utl/log.h"
#endif

/**
 * Used values
 * K - hashmultimap key data type
 * V - hashmultimap value data type
 * SNAME - struct name and prefix of other related structs
 * PFX - functions prefix
 */

/* Structs definition */
#include "cmc/hashmultimap/struct.h"

/* Function declaration */
#include "cmc/hashmultimap/header.h"

/* Function implementation */
#include "cmc/hashmultimap/code.h"

/**
 * Extensions
 *
 * INIT - Initializes the struct on the stack
 * ITER - hashmultimap iterator
 * STR - Print helper functions
 */
#define CMC_EXT_HASHMULTIMAP_PARTS INIT, ITER, STR
/**/
#include "cmc/hashmultimap/ext/struct.h"
/**/
#include "cmc/hashmultimap/ext/header.h"
/**/
#include "cmc/hashmultimap/ext/code.h"

#include "cor/undef.h"
