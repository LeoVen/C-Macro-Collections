/**
 * cor/core.h
 *
 * Creation Date: 17/03/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * Core includes and macros for the entire library.
 *
 * Here you will find:
 *  - CMC_
 *  - CMC_TO_STRING
 *  - CMC_INTERNAL_PREFIX_[ITER|NODE|ENTRY|FKEY|FVAL]
 *  - CMC_DEF_[ITER|NODE|ENTRY|FKEY|FVAL]
 *  - CMC_PARAM_[PFX|SNAME|SIZE|K|V]
 */

#ifndef CMC_COR_CORE_H
#define CMC_COR_CORE_H

#if defined(__GNUC__) || defined(__clang__)
#define CMC_UNUSED __attribute__((__unused__))
#else
#define CMC_UNUSED
#endif

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "callbacks.h"
#include "flags.h"
#include "ftable.h"

#define CMC__(A, B) A##B
#define CMC_(A, B) CMC__(A, B)

#define CMC_TO_STRING_(X) #X
#define CMC_TO_STRING(X) CMC_TO_STRING_(X)

#if defined(CMC_CAMEL_CASE)
#define CMC_INTERNAL_PREFIX_ITER Iter
#define CMC_INTERNAL_PREFIX_NODE Node
#define CMC_INTERNAL_PREFIX_ENTRY Entry
#define CMC_INTERNAL_PREFIX_FVAL FVal
#define CMC_INTERNAL_PREFIX_FKEY FKey
#else
#define CMC_INTERNAL_PREFIX_ITER _iter
#define CMC_INTERNAL_PREFIX_NODE _node
#define CMC_INTERNAL_PREFIX_ENTRY _entry
#define CMC_INTERNAL_PREFIX_FVAL _fval
#define CMC_INTERNAL_PREFIX_FKEY _fkey
#endif

#define CMC_DEF_ITER(SNAME) CMC_(SNAME, CMC_INTERNAL_PREFIX_ITER)
#define CMC_DEF_NODE(SNAME) CMC_(SNAME, CMC_INTERNAL_PREFIX_NODE)
#define CMC_DEF_ENTRY(SNAME) CMC_(SNAME, CMC_INTERNAL_PREFIX_ENTRY)
#define CMC_DEF_FVAL(SNAME) CMC_(SNAME, CMC_INTERNAL_PREFIX_FVAL)
#define CMC_DEF_FKEY(SNAME) CMC_(SNAME, CMC_INTERNAL_PREFIX_FKEY)

/**
 * Code to get specific arguments from tuples.
 */
#define CMC_TUP_0_(A, ...) A
#define CMC_TUP_0(TUP) CMC_TUP_0_ TUP
#define CMC_TUP_1_(A, B, ...) B
#define CMC_TUP_1(TUP) CMC_TUP_1_ TUP
#define CMC_TUP_2_(A, B, C, ...) C
#define CMC_TUP_2(TUP) CMC_TUP_2_ TUP
#define CMC_TUP_3_(A, B, C, D, ...) D
#define CMC_TUP_3(TUP) CMC_TUP_3_ TUP
#define CMC_TUP_4_(A, B, C, D, E, ...) E
#define CMC_TUP_4(TUP) CMC_TUP_4_ TUP

/**
 * A PARAM is a unified way to pass arguments to all macros that generate code.
 * It is a tuple that contains the following parameters in order:
 *
 *   PFX - Functions prefix
 * SNAME - `struct` name
 *  SIZE - Size for SAC library
 *     K - Key type
 *     V - Value type
 *
 * PARAMS := (PFX, SNAME, SIZE, K, V)
 */
#define CMC_PARAM_PFX CMC_TUP_0
#define CMC_PARAM_SNAME CMC_TUP_1
#define CMC_PARAM_SIZE CMC_TUP_2
#define CMC_PARAM_K CMC_TUP_3
#define CMC_PARAM_V CMC_TUP_4

#endif /* CMC_COR_CORE_H */
