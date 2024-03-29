/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Library.
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

#ifdef CMC_SAC
struct SNAME CMC_(PFX, _init)(struct CMC_DEF_FVAL(SNAME) * f_val)
#else
struct SNAME CMC_(PFX, _init)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val)
#endif
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

#ifdef CMC_SAC
    return CMC_(PFX, _init_custom)(f_val, NULL, NULL);
#else
    return CMC_(PFX, _init_custom)(capacity, f_val, NULL, NULL);
#endif
}

#ifdef CMC_SAC
struct SNAME CMC_(PFX, _init_custom)(struct CMC_DEF_FVAL(SNAME) * f_val, CMC_ALLOC_TYPE alloc,
                                     CMC_CALLBACK_TYPE callbacks)
#else
struct SNAME CMC_(PFX, _init_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val, CMC_ALLOC_TYPE alloc,
                                     CMC_CALLBACK_TYPE callbacks)
#endif
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    struct SNAME _list_ = { 0 };

#ifndef CMC_SAC
    if (capacity < 1)
        return _list_;
#endif

    if (!f_val)
        return _list_;

    if (!alloc)
        alloc = &cmc_alloc_node_default;

#ifdef CMC_SAC
    memset(_list_.buffer, 0, sizeof(V) * SIZE);
#else
    _list_.buffer = alloc->calloc(capacity, sizeof(V));
#endif

    if (!_list_.buffer)
        return _list_;

#ifndef CMC_SAC
    _list_.capacity = capacity;
#endif
    _list_.count = 0;
    _list_.flag = CMC_FLAG_OK;
    _list_.f_val = f_val;
    _list_.alloc = alloc;
    CMC_CALLBACKS_ASSIGN(&_list_, callbacks);

    return _list_;
}

void CMC_(PFX, _release)(struct SNAME _list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_list_.f_val->free)
    {
        for (size_t i = 0; i < _list_.count; i++)
            _list_.f_val->free(_list_.buffer[i]);
    }

#ifndef CMC_SAC
    _list_.alloc->free(_list_.buffer);
#endif
}

#endif /* CMC_EXT_INIT */

/**
 * ITER
 *
 * List bi-directional iterator.
 */
#ifdef CMC_EXT_ITER

struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME *target)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ITER(SNAME) iter;

    iter.target = target;
    iter.cursor = 0;
    iter.start = true;
    iter.end = CMC_(PFX, _empty)(target);

    return iter;
}

struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME *target)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ITER(SNAME) iter;

    iter.target = target;
    iter.cursor = 0;
    iter.start = CMC_(PFX, _empty)(target);
    iter.end = true;

    if (!CMC_(PFX, _empty)(target))
        iter.cursor = target->count - 1;

    return iter;
}

bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _empty)(iter->target) || iter->start;
}

bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _empty)(iter->target) || iter->end;
}

bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->cursor = 0;
        iter->start = true;
        iter->end = CMC_(PFX, _empty)(iter->target);

        return true;
    }

    return false;
}

bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->start = CMC_(PFX, _empty)(iter->target);
        iter->cursor = iter->target->count - 1;
        iter->end = true;

        return true;
    }

    return false;
}

bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->end)
        return false;

    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = true;
        return false;
    }

    iter->start = CMC_(PFX, _empty)(iter->target);

    iter->cursor++;

    return true;
}

bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->start)
        return false;

    if (iter->cursor == 0)
    {
        iter->start = true;
        return false;
    }

    iter->end = CMC_(PFX, _empty)(iter->target);

    iter->cursor--;

    return true;
}

/* Returns true only if the iterator moved */
bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->end)
        return false;

    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = true;
        return false;
    }

    if (steps == 0 || iter->cursor + steps >= iter->target->count)
        return false;

    iter->start = CMC_(PFX, _empty)(iter->target);

    iter->cursor += steps;

    return true;
}

/* Returns true only if the iterator moved */
bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->start)
        return false;

    if (iter->cursor == 0)
    {
        iter->start = true;
        return false;
    }

    if (steps == 0 || iter->cursor < steps)
        return false;

    iter->end = CMC_(PFX, _empty)(iter->target);

    iter->cursor -= steps;

    return true;
}

/* Returns true only if the iterator was able to be positioned at the */
/* given index */
bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter, size_t index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (index >= iter->target->count)
        return false;

    if (iter->cursor > index)
        return CMC_(PFX, _iter_rewind)(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return CMC_(PFX, _iter_advance)(iter, index - iter->cursor);

    return true;
}

V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(iter->target))
        return (V){ 0 };

    return iter->target->buffer[iter->cursor];
}

V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(iter->target))
        return NULL;

    return &(iter->target->buffer[iter->cursor]);
}

size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return iter->cursor;
}

#endif /* CMC_EXT_ITER */

/**
 * SEQ
 *
 * Push and pop sequence of items.
 */
#ifdef CMC_EXT_SEQ

bool CMC_(PFX, _seq_push_front)(struct SNAME *_list_, V *values, size_t size)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (size == 0)
    {
        _list_->flag = CMC_FLAG_INVALID;
        return false;
    }

    if (!CMC_(PFX, _fits)(_list_, size))
    {
#ifdef CMC_SAC
        _list_->flag = CMC_FLAG_FULL;
        return false;
#else
        if (!CMC_(PFX, _resize)(_list_, _list_->count + size))
            return false;
#endif
    }

    memmove(_list_->buffer + size, _list_->buffer, _list_->count * sizeof(V));

    memcpy(_list_->buffer, values, size * sizeof(V));

    _list_->count += size;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return true;
}

bool CMC_(PFX, _seq_push_at)(struct SNAME *_list_, V *values, size_t size, size_t index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (size == 0)
    {
        _list_->flag = CMC_FLAG_INVALID;
        return false;
    }

    if (index > _list_->count)
    {
        _list_->flag = CMC_FLAG_RANGE;
        return false;
    }

    if (index == 0)
        return CMC_(PFX, _seq_push_front)(_list_, values, size);
    else if (index == _list_->count)
        return CMC_(PFX, _seq_push_back)(_list_, values, size);

    if (!CMC_(PFX, _fits)(_list_, size))
    {
#ifdef CMC_SAC
        _list_->flag = CMC_FLAG_FULL;
        return false;
#else
        if (!CMC_(PFX, _resize)(_list_, _list_->count + size))
            return false;
#endif
    }

    memmove(_list_->buffer + index + size, _list_->buffer + index, (_list_->count - index) * sizeof(V));

    memcpy(_list_->buffer + index, values, size * sizeof(V));

    _list_->count += size;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return true;
}

bool CMC_(PFX, _seq_push_back)(struct SNAME *_list_, V *values, size_t size)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (size == 0)
    {
        _list_->flag = CMC_FLAG_INVALID;
        return false;
    }

    if (!CMC_(PFX, _fits)(_list_, size))
    {
#ifdef CMC_SAC
        _list_->flag = CMC_FLAG_FULL;
        return false;
#else
        if (!CMC_(PFX, _resize)(_list_, _list_->count + size))
            return false;
#endif
    }

    memcpy(_list_->buffer + _list_->count, values, size * sizeof(V));

    _list_->count += size;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return true;
}

bool CMC_(PFX, _seq_pop_at)(struct SNAME *_list_, size_t from, size_t to)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (from > to)
    {
        _list_->flag = CMC_FLAG_INVALID;
        return false;
    }

    if (to >= _list_->count)
    {
        _list_->flag = CMC_FLAG_RANGE;
        return false;
    }

    size_t length = (to - from + 1);

    memmove(_list_->buffer + from, _list_->buffer + to + 1, (_list_->count - to - 1) * sizeof(V));

    memset(_list_->buffer + _list_->count - length, 0, length * sizeof(V));

    _list_->count -= to - from + 1;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return true;
}

struct SNAME *CMC_(PFX, _seq_sublist)(struct SNAME *_list_, size_t from, size_t to)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (from > to)
    {
        _list_->flag = CMC_FLAG_INVALID;
        return NULL;
    }

    if (to >= _list_->count)
    {
        _list_->flag = CMC_FLAG_RANGE;
        return NULL;
    }

    size_t length = to - from + 1;

#ifdef CMC_SAC
    struct SNAME *result = CMC_(PFX, _new_custom)(_list_->f_val, _list_->alloc, CMC_CALLBACKS_GET(_list_));
#else
    struct SNAME *result = CMC_(PFX, _new_custom)(length, _list_->f_val, _list_->alloc, CMC_CALLBACKS_GET(_list_));
#endif

    if (!result)
    {
        _list_->flag = CMC_FLAG_ALLOC;
        return NULL;
    }

    memcpy(result->buffer, _list_->buffer, length * sizeof(V));

    memmove(_list_->buffer + from, _list_->buffer + to + 1, (_list_->count - to - 1) * sizeof(V));

    memset(_list_->buffer + _list_->count - length, 0, length * sizeof(V));

    _list_->count -= length;
    result->count = length;

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return result;
}

#endif /* CMC_EXT_SEQ */

/**
 * STR
 *
 * Print helper functions.
 */
#ifdef CMC_EXT_STR

bool CMC_(PFX, _to_string)(struct SNAME *_list_, FILE *fptr)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct SNAME *l_ = _list_;

    return 0 <= fprintf(fptr,
                        "struct %s<%s> "
                        "at %p { "
                        "buffer:%p, "
                        "capacity:%" PRIuMAX ", "
                        "count:%" PRIuMAX ", "
                        "flag:%d, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        CMC_TO_STRING(SNAME), CMC_TO_STRING(V), l_, l_->buffer,
#ifdef CMC_SAC
                        (size_t)SIZE
#else
                        l_->capacity
#endif
                        ,
                        l_->count, l_->flag, l_->f_val, l_->alloc, CMC_CALLBACKS_GET(l_));
}

bool CMC_(PFX, _print)(struct SNAME *_list_, FILE *fptr, const char *start, const char *separator, const char *end)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    fprintf(fptr, "%s", start);

    for (size_t i = 0; i < _list_->count; i++)
    {
        if (!_list_->f_val->str(fptr, _list_->buffer[i]))
            return false;

        if (i + 1 < _list_->count)
            fprintf(fptr, "%s", separator);
    }

    fprintf(fptr, "%s", end);

    return true;
}

#endif /* CMC_EXT_STR */
