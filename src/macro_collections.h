/**
 * macro_collections.h
 *
 * Creation Date: 11/02/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_MACRO_COLLECTIONS
#define CMC_MACRO_COLLECTIONS

#include <stdlib.h>
#include <stdbool.h>

#define CONCATH_(C) C##_WRAPGEN_HEADER
#define CONCATC_(C) C##_WRAPGEN_SOURCE

#define CONCATH(C) CONCATH_(C)
#define CONCATC(C) CONCATC_(C)

#define COLLECTION_GENERATE(C, PFX, SNAME, FMOD, K, V)    \
    COLLECTION_GENERATE_HEADER(C, PFX, SNAME, FMOD, K, V) \
    COLLECTION_GENERATE_SOURCE(C, PFX, SNAME, FMOD, K, V)

#define COLLECTION_GENERATE_HEADER(C, PFX, SNAME, FMOD, K, V) \
    CONCATH(C)                                                \
    (PFX, SNAME, FMOD, K, V)

#define COLLECTION_GENERATE_SOURCE(C, PFX, SNAME, FMOD, K, V) \
    CONCATC(C)                                                \
    (PFX, SNAME, FMOD, K, V)

#include "list.h"
#include "linkedlist.h"
#include "stack.h"
#include "queue.h"
#include "deque.h"
#include "heap.h"
#include "treeset.h"
#include "treemap.h"
#include "hashset.h"
#include "hashmap.h"

#include "foreach.h"

#endif /* CMC_MACRO_COLLECTIONS */