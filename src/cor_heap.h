/**
 * cor_heap.h
 *
 * Creation Date: 01/06/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Things commonly used by heap collections.
 */

#ifndef CMC_COR_HEAP_H
#define CMC_COR_HEAP_H

/**
 * enum cmc_heap_order
 *
 * Defines the two possible heaps:
 * - Max Heap has the greatest element at the top
 * - Min Heap has the smallest element at the top
 */
enum cmc_heap_order
{
    CMC_MAX_HEAP = 1,
    CMC_MIN_HEAP = -1
};

#endif /* CMC_COR_HEAP_H */
