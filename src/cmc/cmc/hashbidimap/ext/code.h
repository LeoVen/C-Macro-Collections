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
 * INIT
 *
 * The part 'INIT' gives a new way of initializing a collection. The collection
 * struct is not heap allocated, only its internal structure (nodes, buffers).
 */
#ifdef CMC_EXT_INIT

struct SNAME CMC_(PFX, _init)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
                              struct CMC_DEF_FVAL(SNAME) * f_val)
{
    return CMC_(PFX, _init_custom)(capacity, load, f_key, f_val, NULL, NULL);
}

struct SNAME CMC_(PFX, _init_custom)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
                                     struct CMC_DEF_FVAL(SNAME) * f_val, struct CMC_ALLOC_NODE_NAME *alloc,
                                     struct CMC_CALLBACKS_NAME *callbacks)
{
    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    struct SNAME _map_ = { 0 };

    if (capacity == 0 || load <= 0 || load >= 1)
        return _map_;

    /* Prevent integer overflow */
    if (capacity >= (double)UINTMAX_MAX * load)
        return _map_;

    if (!f_key || !f_val)
        return _map_;

    size_t real_capacity = CMC_(PFX, _impl_calculate_size)(capacity / load);

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    _map_.buffer = alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME) *[2]));

    if (!_map_.buffer)
        return _map_;

    _map_.count = 0;
    _map_.capacity = real_capacity;
    _map_.load = load;
    _map_.flag = CMC_FLAG_OK;
    _map_.f_key = f_key;
    _map_.f_val = f_val;
    _map_.flag = CMC_FLAG_OK;
    _map_.alloc = alloc;
    CMC_CALLBACKS_ASSIGN(&_map_, callbacks);

    return _map_;
}

void CMC_(PFX, _release)(struct SNAME _map_)
{
    CMC_(PFX, _clear)(&_map_);

    _map_.alloc->free(_map_.buffer);
}

#endif /* CMC_EXT_INIT */

/**
 * ITER
 *
 * Hashbidimap bi-directional iterator.
 */
#ifdef CMC_EXT_ITER

struct CMC_DEF_ITER(SNAME) * CMC_(PFX, _iter_new)(struct SNAME *target)
{
    struct CMC_DEF_ITER(SNAME) *iter = target->alloc->malloc(sizeof(struct CMC_DEF_ITER(SNAME)));

    if (!iter)
        return NULL;

    CMC_(PFX, _iter_init)(iter, target);

    return iter;
}

void CMC_(PFX, _iter_free)(struct CMC_DEF_ITER(SNAME) * iter)
{
    iter->target->alloc->free(iter);
}

void CMC_(PFX, _iter_init)(struct CMC_DEF_ITER(SNAME) * iter, struct SNAME *target)
{
    memset(iter, 0, sizeof(struct CMC_DEF_ITER(SNAME)));

    iter->target = target;
    iter->start = true;
    iter->end = CMC_(PFX, _empty)(target);

    if (!CMC_(PFX, _empty)(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i][0];

            if (entry && entry != CMC_ENTRY_DELETED)
            {
                iter->first = i;
                break;
            }
        }

        iter->cursor = iter->first;

        for (size_t i = target->capacity; i > 0; i--)
        {
            struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i - 1][0];

            if (entry && entry != CMC_ENTRY_DELETED)
            {
                iter->last = i - 1;
                break;
            }
        }
    }
}

bool CMC_(PFX, _iter_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
    return CMC_(PFX, _empty)(iter->target) || iter->start;
}

bool CMC_(PFX, _iter_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
    return CMC_(PFX, _empty)(iter->target) || iter->end;
}

void CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = true;
        iter->end = false;
    }
}

void CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = false;
        iter->end = true;
    }
}

bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (iter->end)
        return false;

    if (iter->index + 1 == iter->target->count)
    {
        iter->end = true;
        return false;
    }

    iter->start = CMC_(PFX, _empty)(iter->target);

    struct CMC_DEF_ENTRY(SNAME) *scan = iter->target->buffer[iter->cursor][0];

    iter->index++;

    while (1)
    {
        iter->cursor++;
        scan = iter->target->buffer[iter->cursor][0];

        if (scan != NULL && scan != CMC_ENTRY_DELETED)
            break;
    }

    return true;
}

bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (iter->start)
        return false;

    if (iter->index == 0)
    {
        iter->start = true;
        return false;
    }

    iter->end = CMC_(PFX, _empty)(iter->target);

    struct CMC_DEF_ENTRY(SNAME) *scan = iter->target->buffer[iter->cursor][0];

    iter->index--;

    while (1)
    {
        iter->cursor--;
        scan = iter->target->buffer[iter->cursor][0];

        if (scan != NULL && scan != CMC_ENTRY_DELETED)
            break;
    }

    return true;
}

/* Returns true only if the iterator moved */
bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps)
{
    if (iter->end)
        return false;

    if (iter->index + 1 == iter->target->count)
    {
        iter->end = true;
        return false;
    }

    if (steps == 0 || iter->index + steps >= iter->target->count)
        return false;

    for (size_t i = 0; i < steps; i++)
        CMC_(PFX, _iter_next)(iter);

    return true;
}

/* Returns true only if the iterator moved */
bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps)
{
    if (iter->start)
        return false;

    if (iter->index == 0)
    {
        iter->start = true;
        return false;
    }

    if (steps == 0 || iter->index < steps)
        return false;

    for (size_t i = 0; i < steps; i++)
        CMC_(PFX, _iter_prev)(iter);

    return true;
}

/* Returns true only if the iterator was able to be positioned at */
/* the given index */
bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter, size_t index)
{
    if (index >= iter->target->count)
        return false;

    if (iter->index > index)
        return CMC_(PFX, _iter_rewind)(iter, iter->index - index);
    else if (iter->index < index)
        return CMC_(PFX, _iter_advance)(iter, index - iter->index);

    return true;
}

K CMC_(PFX, _iter_key)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (CMC_(PFX, _empty)(iter->target))
    {
        iter->target->flag = CMC_FLAG_EMPTY;
        return (K){ 0 };
    }

    return iter->target->buffer[iter->cursor][0]->key;
}

V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (CMC_(PFX, _empty)(iter->target))
    {
        iter->target->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    return iter->target->buffer[iter->cursor][0]->value;
}

size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)
{
    return iter->index;
}

#endif /* CMC_EXT_ITER */

/**
 * STR
 *
 * Print helper functions.
 */
#ifdef CMC_EXT_STR

bool CMC_(PFX, _to_string)(struct SNAME *_map_, FILE *fptr)
{
    struct SNAME *m_ = _map_;

    return 0 <= fprintf(fptr,
                        "struct %s<%s, %s> "
                        "at %p { "
                        "buffer:%p, "
                        "capacity:%" PRIuMAX ", "
                        "count:%" PRIuMAX ", "
                        "load:%lf, "
                        "flag:%d, "
                        "f_key:%p, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        CMC_TO_STRING(SNAME), CMC_TO_STRING(K), CMC_TO_STRING(V), m_, m_->buffer, m_->capacity,
                        m_->count, m_->load, m_->flag, m_->f_key, m_->f_val, m_->alloc, CMC_CALLBACKS_GET(m_));
}

bool CMC_(PFX, _print)(struct SNAME *_map_, FILE *fptr, const char *start, const char *separator, const char *end,
                       const char *key_val_sep)
{
    fprintf(fptr, "%s", start);

    size_t last = 0;
    for (size_t i = _map_->capacity; i > 0; i--)
    {
        struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[i - 1][0];

        if (entry && entry != CMC_ENTRY_DELETED)
        {
            last = i - 1;
            break;
        }
    }

    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[i][0];

        if (entry && entry != CMC_ENTRY_DELETED)
        {
            if (!_map_->f_key->str(fptr, entry->key))
                return false;

            fprintf(fptr, "%s", key_val_sep);

            if (!_map_->f_val->str(fptr, entry->value))
                return false;

            if (i + 1 < last)
                fprintf(fptr, "%s", separator);
        }
    }

    fprintf(fptr, "%s", end);

    return true;
}

#endif /* CMC_EXT_STR */
