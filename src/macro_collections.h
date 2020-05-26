/**
 * macro_collections.h
 *
 * Creation Date: 11/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_MACRO_COLLECTIONS_H
#define CMC_MACRO_COLLECTIONS_H

#include "cmc/bitset.h"       /* Added in 30/04/2020 */
#include "cmc/deque.h"        /* Added in 20/03/2019 */
#include "cmc/hashbidimap.h"  /* Added in 26/09/2019 */
#include "cmc/hashmap.h"      /* Added in 03/04/2019 */
#include "cmc/hashmultimap.h" /* Added in 26/04/2019 */
#include "cmc/hashmultiset.h" /* Added in 10/04/2019 */
#include "cmc/hashset.h"      /* Added in 01/04/2019 */
#include "cmc/heap.h"         /* Added in 25/03/2019 */
#include "cmc/intervalheap.h" /* Added in 06/07/2019 */
#include "cmc/linkedlist.h"   /* Added in 22/03/2019 */
#include "cmc/list.h"         /* Added in 12/02/2019 */
#include "cmc/queue.h"        /* Added in 15/02/2019 */
#include "cmc/sortedlist.h"   /* Added in 17/09/2019 */
#include "cmc/stack.h"        /* Added in 14/02/2019 */
#include "cmc/treemap.h"      /* Added in 28/03/2019 */
#include "cmc/treeset.h"      /* Added in 27/03/2019 */
#include "cor/alloc.h"        /* Added in 24/05/2020 */
#include "cor/bitset.h"       /* Added in 20/05/2020 */
#include "cor/callbacks.h"    /* Added in 24/05/2020 */
#include "cor/core.h"         /* Added in 17/03/2020 */
#include "cor/flags.h"        /* Added in 14/05/2020 */
#include "cor/hashtable.h"    /* Added in 17/03/2020 */
#include "ext/cmc/deque.h"    /* Added in 25/05/2020 */
#include "utl/assert.h"       /* Added in 27/06/2019 */
#include "utl/foreach.h"      /* Added in 25/02/2019 */
#include "utl/futils.h"       /* Added in 15/04/2020 */
#include "utl/log.h"          /* Added in 21/06/2019 */
#include "utl/mutex.h"        /* Added in 14/05/2020 */
#include "utl/test.h"         /* Added in 26/06/2019 */
#include "utl/thread.h"       /* Added in 14/05/2020 */
#include "utl/timer.h"        /* Added in 12/04/2019 */

/**
 * Generate a specific part of a collection
 */
#define C_MACRO_COLLECTIONS(LIB, COLLECTION, PART, FILE, PARAMS) \
    C_MACRO_COLLECTIONS_(LIB, COLLECTION, PART, FILE)(PARAMS)
#define C_MACRO_COLLECTIONS_(LIB, COLLECTION, PART, FILE) \
    CMC_##LIB##_##COLLECTION##_##PART##_##FILE

/**
 * Generate everything provided by a collection included in ext
 */
#define C_MACRO_COLLECTIONS_ALL(LIB, COLLECTION, PARAMS)    \
    C_MACRO_COLLECTIONS_ALL_HEADER(LIB, COLLECTION, PARAMS) \
    C_MACRO_COLLECTIONS_ALL_SOURCE(LIB, COLLECTION, PARAMS)

#define C_MACRO_COLLECTIONS_ALL_HEADER(LIB, COLLECTION, PARAMS) \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, CORE, HEADER, PARAMS)  \
    CMC_MAP(CMC_(EXT_, LIB), COLLECTION, HEADER, PARAMS,        \
            C_MACRO_COLLECTIONS_PARTS(LIB, COLLECTION))

#define C_MACRO_COLLECTIONS_ALL_SOURCE(LIB, COLLECTION, PARAMS) \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, CORE, SOURCE, PARAMS)  \
    CMC_MAP(CMC_(EXT_, LIB), COLLECTION, SOURCE, PARAMS,        \
            C_MACRO_COLLECTIONS_PARTS(LIB, COLLECTION))

/* Gets the macro with all EXT parts of a collection */
#define C_MACRO_COLLECTIONS_PARTS(LIB, COLLECTION) \
    CMC_EXT_##LIB##_##COLLECTION##_PARTS

#define CMC_MAP(LIB, COLLECTION, FILE, PARAMS, ...) \
    CMC_(CMC_MAP_, CMC_NARG(__VA_ARGS__))           \
    (LIB, COLLECTION, FILE, PARAMS, __VA_ARGS__)

#define CMC_MAP_0(LIB, COLLECTION, FILE, PARAMS, PART, ...)
#define CMC_MAP_1(LIB, COLLECTION, FILE, PARAMS, PART, ...)  \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, PART, FILE, PARAMS) \
    CMC_MAP_0(LIB, COLLECTION, FILE, PARAMS, __VA_ARGS__)
#define CMC_MAP_2(LIB, COLLECTION, FILE, PARAMS, PART, ...)  \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, PART, FILE, PARAMS) \
    CMC_MAP_1(LIB, COLLECTION, FILE, PARAMS, __VA_ARGS__)
#define CMC_MAP_3(LIB, COLLECTION, FILE, PARAMS, PART, ...)  \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, PART, FILE, PARAMS) \
    CMC_MAP_2(LIB, COLLECTION, FILE, PARAMS, __VA_ARGS__)
#define CMC_MAP_4(LIB, COLLECTION, FILE, PARAMS, PART, ...)  \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, PART, FILE, PARAMS) \
    CMC_MAP_3(LIB, COLLECTION, FILE, PARAMS, __VA_ARGS__)
#define CMC_MAP_5(LIB, COLLECTION, FILE, PARAMS, PART, ...)  \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, PART, FILE, PARAMS) \
    CMC_MAP_4(LIB, COLLECTION, FILE, PARAMS, __VA_ARGS__)
#define CMC_MAP_6(LIB, COLLECTION, FILE, PARAMS, PART, ...)  \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, PART, FILE, PARAMS) \
    CMC_MAP_5(LIB, COLLECTION, FILE, PARAMS, __VA_ARGS__)
#define CMC_MAP_7(LIB, COLLECTION, FILE, PARAMS, PART, ...)  \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, PART, FILE, PARAMS) \
    CMC_MAP_6(LIB, COLLECTION, FILE, PARAMS, __VA_ARGS__)
#define CMC_MAP_8(LIB, COLLECTION, FILE, PARAMS, PART, ...)  \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, PART, FILE, PARAMS) \
    CMC_MAP_7(LIB, COLLECTION, FILE, PARAMS, __VA_ARGS__)
#define CMC_MAP_9(LIB, COLLECTION, FILE, PARAMS, PART, ...)  \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, PART, FILE, PARAMS) \
    CMC_MAP_8(LIB, COLLECTION, FILE, PARAMS, __VA_ARGS__)
#define CMC_MAP_10(LIB, COLLECTION, FILE, PARAMS, PART, ...) \
    C_MACRO_COLLECTIONS(LIB, COLLECTION, PART, FILE, PARAMS) \
    CMC_MAP_9(LIB, COLLECTION, FILE, PARAMS, __VA_ARGS__)

#endif /* CMC_MACRO_COLLECTIONS_H */
