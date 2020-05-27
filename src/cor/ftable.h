/**
 * ftable.h
 *
 * Creation Date: 27/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Type definitions for Functions Tables.
 */

#ifndef CMC_COR_FTABLE_H
#define CMC_COR_FTABLE_H

/**
 * Where T can either be K or V
 */
#define CMC_DEF_FTAB_CMP(T) int (*cmp)(T, T)
#define CMC_DEF_FTAB_CPY(T) T (*cpy)(T)
#define CMC_DEF_FTAB_STR(T) bool (*str)(FILE *, T)
#define CMC_DEF_FTAB_FREE(T) void (*free)(T)
#define CMC_DEF_FTAB_HASH(T) size_t (*hash)(T)
#define CMC_DEF_FTAB_PRI(T) int (*pri)(T, T)

#endif /* CMC_COR_FTABLE_H */
