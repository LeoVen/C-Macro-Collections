/**
 * cor_hashtable.h
 *
 * Creation Date: 17/03/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Things commonly used by hashtable-based collections.
 */

#ifndef CMC_COR_HASHTABLE_H
#define CMC_COR_HASHTABLE_H

#include "cor_core.h"

/**
 * CMC_ENTRY_DELETED
 *
 * Defines a pointer to an entry that has been deleted
 */
#define CMC_ENTRY_DELETED ((void *)1)

/**
 * enum cmc_entry_state
 *
 * Defines the three possible states for a node on a flat hashtable.
 */
enum cmc_entry_state
{
    CMC_ES_DELETED = -1,
    CMC_ES_EMPTY = 0,
    CMC_ES_FILLED = 1
};

/**
 * static const size_t cmc_hashtable_primes[59]
 *
 * Prime numbers used to define a capacity for the hashtables for better
 * hashing.
 */
// clang-format off
static const size_t cmc_hashtable_primes[] = {
    /* < 1e3 */
    53, 97, 191, 383, 769,
    /* < 1e4 */
    1531, 3067, 6143,
    /* < 1e5 */
    12289, 24571, 49157, 98299,
    /* < 1e6 */
    196613, 393209, 786431,
    /* < 1e7 */
    1572869, 3145721, 6291449,
    /* < 1e8 */
    12582917, 25165813, 50331653,
    /* < 1e9 */
    100663291, 201326611, 402653189, 805306357,
    /* < 1e10 */
    1610612741, 3221225473, 6442450939,
    /* < 1e11 */
    12884901893, 25769803799, 51539607551,
    /* < 1e12 */
    103079215111, 206158430209, 412316860441, 824633720831,
    /* < 1e13 */
    1649267441651, 3298534883309, 6597069766657,
    /* < 1e14 */
    13194139533299, 26388279066623, 52776558133303,
    /* < 1e15 */
    105553116266489, 211106232532969, 422212465066001, 844424930131963,
    /* < 1e16 */
    1688849860263953, 3377699720527861, 6755399441055731,
    /* < 1e17 */
    13510798882111483, 27021597764222939, 54043195528445957,
    /* < 1e18 */
    108086391056891903, 216172782113783773, 432345564227567621, 864691128455135207,
    /* < 1e19 */
    1729382256910270481, 3458764513820540933, 6917529027641081903,
    /* < 1e20 */
    13835058055282163729llu,
};
// clang-format on

#endif /* CMC_COR_HASHTABLE_H */
