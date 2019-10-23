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

#define CMC_CONCATH_(C) CMC_WRAPGEN_##C##_HEADER
#define CMC_CONCATC_(C) CMC_WRAPGEN_##C##_SOURCE

#define CMC_CONCATH(C) CMC_CONCATH_(C)
#define CMC_CONCATC(C) CMC_CONCATC_(C)

#define CMC_COLLECTION_GENERATE(C, PFX, SNAME, K, V)    \
    CMC_COLLECTION_GENERATE_HEADER(C, PFX, SNAME, K, V) \
    CMC_COLLECTION_GENERATE_SOURCE(C, PFX, SNAME, K, V)

#define CMC_COLLECTION_GENERATE_HEADER(C, PFX, SNAME, K, V) \
    CMC_CONCATH(C)(PFX, SNAME, K, V)

#define CMC_COLLECTION_GENERATE_SOURCE(C, PFX, SNAME, K, V) \
    CMC_CONCATC(C)(PFX, SNAME, K, V)

#include "cmc/bidimap.h"      /* Added in 26/09/2019 */
#include "cmc/deque.h"        /* Added in 20/03/2019 */
#include "cmc/hashmap.h"      /* Added in 03/04/2019 */
#include "cmc/hashset.h"      /* Added in 01/04/2019 */
#include "cmc/heap.h"         /* Added in 25/03/2019 */
#include "cmc/intervalheap.h" /* Added in 06/07/2019 */
#include "cmc/linkedlist.h"   /* Added in 22/03/2019 */
#include "cmc/list.h"         /* Added in 12/02/2019 */
#include "cmc/multimap.h"     /* Added in 26/04/2019 */
#include "cmc/multiset.h"     /* Added in 10/04/2019 */
#include "cmc/queue.h"        /* Added in 15/02/2019 */
#include "cmc/sortedlist.h"   /* Added in 17/09/2019 */
#include "cmc/stack.h"        /* Added in 14/02/2019 */
#include "cmc/treemap.h"      /* Added in 28/03/2019 */
#include "cmc/treeset.h"      /* Added in 27/03/2019 */

#include "sac/queue.h"
#include "sac/stack.h"

#include "utl/assert.h"       /* Added in 27/06/2019 */
#include "utl/foreach.h"      /* Added in 25/02/2019 */
#include "utl/log.h"          /* Added in 21/06/2109 */
#include "utl/test.h"         /* Added in 26/06/2019 */
#include "utl/timer.h"        /* Added in 12/04/2019 */

#endif /* CMC_MACRO_COLLECTIONS_H */
