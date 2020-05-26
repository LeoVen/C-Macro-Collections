/**
 * params.h
 *
 * Creation Date: 25/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

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

#ifndef CMC_COR_PARAMS_H
#define CMC_COR_PARAMS_H

// clang-format off
/*
for i in range(5):
    print(f"#define CMC_TUP_{i}_({', '.join(chr(x) for x in range(65, 65 + i + 1))}, ...) {chr(65 + i)}\n#define CMC_TUP_{i}(TUP) CMC_TUP_{i}_ TUP")
*/
// clang-format on

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

#define CMC_PARAM_PFX CMC_TUP_0
#define CMC_PARAM_SNAME CMC_TUP_1
#define CMC_PARAM_SIZE CMC_TUP_2
#define CMC_PARAM_K CMC_TUP_3
#define CMC_PARAM_V CMC_TUP_4

#endif /* CMC_COR_PARAMS_H */
