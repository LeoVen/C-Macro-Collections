/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Libray.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * ext_cmc_hashbidimap.h
 *
 * Creation Date: 26/05/2020
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

#ifndef CMC_EXT_CMC_HASHBIDIMAP_H
#define CMC_EXT_CMC_HASHBIDIMAP_H

#include "cor_core.h"

/**
 * All the EXT parts of CMC HashBidiMap.
 */
#define CMC_EXT_CMC_HASHBIDIMAP_PARTS STR

/**
 * STR
 *
 * \param ACCESS Either PUBLIC or PRIVATE
 * \param FILE   Either HEADER or SOURCE
 * \param PARAMS A tuple of form (PFX, SNAME, SIZE, K, V)
 */
#define CMC_EXT_CMC_HASHBIDIMAP_STR(ACCESS, FILE, PARAMS) \
    CMC_(CMC_(CMC_EXT_CMC_HASHBIDIMAP_STR_, ACCESS), CMC_(_, FILE))(PARAMS)

#define CMC_EXT_CMC_HASHBIDIMAP_STR_PUBLIC_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHBIDIMAP_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                        CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHBIDIMAP_STR_PUBLIC_SOURCE(PARAMS) \
    CMC_EXT_CMC_HASHBIDIMAP_STR_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                        CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHBIDIMAP_STR_PRIVATE_HEADER(PARAMS) \
    CMC_EXT_CMC_HASHBIDIMAP_STR_HEADER_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                        CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHBIDIMAP_STR_PRIVATE_SOURCE(PARAMS) \
    CMC_EXT_CMC_HASHBIDIMAP_STR_SOURCE_(CMC_PARAM_PFX(PARAMS), CMC_PARAM_SNAME(PARAMS), CMC_PARAM_K(PARAMS), \
                                        CMC_PARAM_V(PARAMS))

#define CMC_EXT_CMC_HASHBIDIMAP_STR_HEADER_(PFX, SNAME, K, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _map_, FILE * fptr); \
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr, const char *start, const char *separator, \
                           const char *end, const char *key_val_sep);

#define CMC_EXT_CMC_HASHBIDIMAP_STR_SOURCE_(PFX, SNAME, K, V) \
\
    bool CMC_(PFX, _to_string)(struct SNAME * _map_, FILE * fptr) \
    { \
        struct SNAME *m_ = _map_; \
\
        return 0 <= fprintf(fptr, \
                            "struct %s<%s, %s> " \
                            "at %p { " \
                            "buffer:%p, " \
                            "capacity:%" PRIuMAX ", " \
                            "count:%" PRIuMAX ", " \
                            "load:%lf, " \
                            "flag:%d, " \
                            "f_key:%p, " \
                            "f_val:%p, " \
                            "alloc:%p, " \
                            "callbacks:%p }", \
                            CMC_TO_STRING(SNAME), CMC_TO_STRING(K), CMC_TO_STRING(V), m_, m_->buffer, m_->capacity, \
                            m_->count, m_->load, m_->flag, m_->f_key, m_->f_val, m_->alloc, CMC_CALLBACKS_GET(m_)); \
    } \
\
    bool CMC_(PFX, _print)(struct SNAME * _map_, FILE * fptr, const char *start, const char *separator, \
                           const char *end, const char *key_val_sep) \
    { \
        fprintf(fptr, "%s", start); \
\
        size_t last = 0; \
        for (size_t i = _map_->capacity; i > 0; i--) \
        { \
            struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[i - 1][0]; \
\
            if (entry && entry != CMC_ENTRY_DELETED) \
            { \
                last = i - 1; \
                break; \
            } \
        } \
\
        for (size_t i = 0; i < _map_->capacity; i++) \
        { \
            struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[i][0]; \
\
            if (entry && entry != CMC_ENTRY_DELETED) \
            { \
                if (!_map_->f_key->str(fptr, entry->key)) \
                    return false; \
\
                fprintf(fptr, "%s", key_val_sep); \
\
                if (!_map_->f_val->str(fptr, entry->value)) \
                    return false; \
\
                if (i + 1 < last) \
                    fprintf(fptr, "%s", separator); \
            } \
        } \
\
        fprintf(fptr, "%s", end); \
\
        return true; \
    }

#endif /* CMC_EXT_CMC_HASHBIDIMAP_H */
