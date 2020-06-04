/**
 * cor/callbacks.h
 *
 * Creation Date: 24/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_COR_CALLBACKS_H
#define CMC_COR_CALLBACKS_H

#ifdef CMC_CAMEL_CASE
#define CMC_CALLBACKS_NAME CMCCallbacks
#else
#define CMC_CALLBACKS_NAME cmc_callbacks
#endif

#ifdef CMC_NO_CALLBACKS

#define CMC_CALLBACKS_DECL
#define CMC_CALLBACKS_GET(ds) NULL /* Placeholder for STR */
#define CMC_CALLBACKS_ASSIGN(ds, cb)
#define CMC_CALLBACKS_CALL(ds, cb)

#else

#define CMC_CALLBACKS_DECL struct CMC_CALLBACKS_NAME *callbacks
#define CMC_CALLBACKS_GET(ds) (ds)->callbacks
#define CMC_CALLBACKS_ASSIGN(ds, cb) CMC_CALLBACKS_GET(ds) = cb
#define CMC_CALLBACKS_CALL(ds, cb) \
    if (CMC_CALLBACKS_GET(ds) && CMC_CALLBACKS_GET(ds)->cb) \
        CMC_CALLBACKS_GET(ds)->cb();

#endif

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
struct CMC_CALLBACKS_NAME
{
    void (*create)(void);
    void (*read)(void);
    void (*update)(void);
    void (*delete)(void);
    void (*resize)(void);
};

#endif /* CMC_COR_CALLBACKS_H */
