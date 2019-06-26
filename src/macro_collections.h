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

#include "cmc/deque.h"
#include "cmc/hashmap.h"
#include "cmc/hashset.h"
#include "cmc/heap.h"
#include "cmc/linkedlist.h"
#include "cmc/list.h"
#include "cmc/queue.h"
#include "cmc/stack.h"
#include "cmc/treemap.h"
#include "cmc/treeset.h"

#include "ext/linkedqueue.h"

#include "sac/queue.h"
#include "sac/stack.h"

#include "utl/foreach.h"
#include "utl/log.h"
#include "utl/test.h"
#include "utl/timer.h"

#endif /* CMC_MACRO_COLLECTIONS */
