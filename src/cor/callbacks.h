/**
 * cor/callbacks.h
 *
 * Creation Date: 24/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_COR_CALLBACK_H
#define CMC_COR_CALLBACK_H

/**
 * struct cmc_callbacks
 *
 * Callback node. Each collection will call one of these functions when:
 * - create : an element was successfully added to the collection
 * - read   : the collection was successfully queried about an element
 * - update : an element in the collection was successfully updated
 * - delete : an element was successfully removed from the collection
 * - resize : the collection was full and successfully resized
 */
struct cmc_callbacks
{
    void (*create)(void);
    void (*read)(void);
    void (*update)(void);
    void (*delete)(void);
    void (*resize)(void);
};

#endif /* CMC_COR_CALLBACK_H */
