/**
 * alloc.h
 *
 * Creation Date: 24/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_COR_ALLOC_H
#define CMC_COR_ALLOC_H

#include <stdlib.h>

/**
 * struct cmc_alloc_node
 *
 * Custom allocation node. Allows collections to use custom allocation
 * functions. The cmc_alloc_node_default is the default heap allocator provided
 * by the standard library.
 */
static struct cmc_alloc_node
{
    void *(*malloc)(size_t);
    void *(*calloc)(size_t, size_t);
    void *(*realloc)(void *, size_t);
    void (*free)(void *);
} cmc_alloc_node_default = { malloc, calloc, realloc, free };

#endif /* CMC_COR_ALLOC_H */
