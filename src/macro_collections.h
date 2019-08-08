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

#define CMC_CONCATH_(C) C##_WRAPGEN_HEADER
#define CMC_CONCATC_(C) C##_WRAPGEN_SOURCE

#define CMC_CONCATH(C) CMC_CONCATH_(C)
#define CMC_CONCATC(C) CMC_CONCATC_(C)

#define COLLECTION_GENERATE(C, PFX, SNAME, FMOD, K, V)    \
    COLLECTION_GENERATE_HEADER(C, PFX, SNAME, FMOD, K, V) \
    COLLECTION_GENERATE_SOURCE(C, PFX, SNAME, FMOD, K, V)

#define COLLECTION_GENERATE_HEADER(C, PFX, SNAME, FMOD, K, V) \
    CMC_CONCATH(C)                                            \
    (PFX, SNAME, FMOD, K, V)

#define COLLECTION_GENERATE_SOURCE(C, PFX, SNAME, FMOD, K, V) \
    CMC_CONCATC(C)                                            \
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

#include "ext/intervalheap.h"
#include "ext/linkedqueue.h"
#include "ext/multimap.h"
#include "ext/multiset.h"

#include "sac/queue.h"
#include "sac/stack.h"

#include "utl/assert.h"
#include "utl/foreach.h"
#include "utl/log.h"
#include "utl/test.h"
#include "utl/timer.h"

#endif /* CMC_MACRO_COLLECTIONS */
