/**
 * core.h
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

#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloc.h"
#include "callbacks.h"
#include "flags.h"

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
 * PARAM := (PFX, SNAME, SIZE, K, V)
 */
#define CMC_PARAM_PFX CMC_TUP_0
#define CMC_PARAM_SNAME CMC_TUP_1
#define CMC_PARAM_SIZE CMC_TUP_2
#define CMC_PARAM_K CMC_TUP_3
#define CMC_PARAM_V CMC_TUP_4

/**
 * A __VA_ARGS__ argument counter. This argument counter is slightly shifted:
 *
 * CMC_NARG(HELLO)         -> 0
 * CMC_NARG(HELLO,)        -> 1
 * CMC_NARG(HELLO, WORLD,) -> 2
 */
#define CMC_NARG(...) CMC_NARG_(__VA_ARGS__, CMC_SEQ_N())
#define CMC_NARG_(...) CMC_ARG_N(__VA_ARGS__)
#define CMC_ARG_N(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, \
                  _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, \
                  _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, \
                  _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, \
                  _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, \
                  _62, _63, N, ...)                                           \
    N
#define CMC_SEQ_N()                                                         \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, \
        45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, \
        28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, \
        11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#endif /* CMC_COR_CORE_H */
