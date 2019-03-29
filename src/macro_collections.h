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

#define CONCATH_(C, P) C##_GENERATE_HEADER##_##P
#define CONCATC_(C, P) C##_GENERATE_SOURCE##_##P

#define CONCATH(C, P) CONCATH_(C, P)
#define CONCATC(C, P) CONCATC_(C, P)

#define COLLECTION_GENERATE(C, P, PFX, SNAME, FMOD, K, V)    \
    COLLECTION_GENERATE_HEADER(C, P, PFX, SNAME, FMOD, K, V) \
    COLLECTION_GENERATE_SOURCE(C, P, PFX, SNAME, FMOD, K, V)

#define COLLECTION_GENERATE_HEADER(C, P, PFX, SNAME, FMOD, K, V) \
    CONCATH(C, P)                                                \
    (PFX, SNAME, FMOD, K, V)

#define COLLECTION_GENERATE_SOURCE(C, P, PFX, SNAME, FMOD, K, V) \
    CONCATC(C, P)                                                \
    (PFX, SNAME, FMOD, K, V)

#include "list.h"
#include "linkedlist.h"
#include "stack.h"
#include "queue.h"
#include "deque.h"
#include "heap.h"
#include "treeset.h"
#include "treemap.h"

#include "foreach.h"

#endif /* CMC_MACRO_COLLECTIONS */