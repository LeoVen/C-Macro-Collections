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

struct SNAME CMC_(PFX, _init)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _init_custom)(capacity, f_val, NULL, NULL);
}

struct SNAME CMC_(PFX, _init_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val,
                                     struct CMC_ALLOC_NODE_NAME *alloc, struct CMC_CALLBACKS_NAME *callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    struct SNAME _queue_ = { 0 };

    if (capacity < 1)
        return _queue_;

    if (!f_val)
        return _queue_;

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    _queue_.buffer = alloc->calloc(capacity, sizeof(V));

    if (!_queue_.buffer)
        return _queue_;

    _queue_.capacity = capacity;
    _queue_.count = 0;
    _queue_.front = 0;
    _queue_.back = 0;
    _queue_.flag = CMC_FLAG_OK;
    _queue_.f_val = f_val;
    _queue_.alloc = alloc;
    CMC_CALLBACKS_ASSIGN(&_queue_, callbacks);

    return _queue_;
}

void CMC_(PFX, _release)(struct SNAME _queue_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_queue_.f_val->free)
    {
        for (size_t i = _queue_.front, j = 0; j < _queue_.count; j++)
        {
            _queue_.f_val->free(_queue_.buffer[i]);

            i = (i + 1) % _queue_.capacity;
        }
    }

    _queue_.alloc->free(_queue_.buffer);
}

#endif /* CMC_EXT_INIT */

/**
 * ITER
 *
 * Queue bi-directional iterator.
 */
#ifdef CMC_EXT_ITER

struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME *target)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ITER(SNAME) iter;

    iter.target = target;
    iter.cursor = target->front;
    iter.index = 0;
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

    if (!CMC_(PFX, _empty)(target))
    {
        if (iter.target->back == 0)
            iter.cursor = iter.target->capacity - 1;
        else
            iter.cursor = iter.target->back - 1;

        iter.index = iter.target->count - 1;
    }
    else
    {
        iter.cursor = 0;
        iter.index = 0;
    }

    iter.start = CMC_(PFX, _empty)(target);
    iter.end = true;

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
        iter->cursor = iter->target->front;
        iter->index = 0;
        iter->start = true;
        iter->end = false;

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
        if (iter->target->back == 0)
            iter->cursor = iter->target->capacity - 1;
        else
            iter->cursor = iter->target->back - 1;

        iter->index = iter->target->count - 1;

        iter->start = false;
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

    if (iter->index + 1 == iter->target->count)
    {
        iter->end = true;
        return false;
    }

    iter->start = CMC_(PFX, _empty)(iter->target);

    iter->cursor = (iter->cursor + 1) % (iter->target->capacity);
    iter->index++;

    return true;
}

bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->start)
        return false;

    if (iter->index == 0)
    {
        iter->start = true;
        return false;
    }

    iter->end = CMC_(PFX, _empty)(iter->target);

    iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1 : iter->cursor - 1;
    iter->index--;

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

    if (iter->index + 1 == iter->target->count)
    {
        iter->end = true;
        return false;
    }

    if (steps == 0 || iter->index + steps >= iter->target->count)
        return false;

    iter->start = CMC_(PFX, _empty)(iter->target);

    iter->index += steps;
    iter->cursor = (iter->cursor + steps) % iter->target->capacity;

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

    if (iter->index == 0)
    {
        iter->start = true;
        return false;
    }

    if (steps == 0 || iter->index < steps)
        return false;

    iter->end = CMC_(PFX, _empty)(iter->target);

    iter->index -= steps;

    /* Prevent underflow */
    if (iter->cursor < steps)
        iter->cursor += iter->target->capacity;

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

    if (iter->index > index)
        return CMC_(PFX, _iter_rewind)(iter, iter->index - index);
    else if (iter->index < index)
        return CMC_(PFX, _iter_advance)(iter, index - iter->index);

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

    return iter->index;
}

#endif /* CMC_EXT_ITER */

/**
 * STR
 *
 * Print helper functions.
 */
#ifdef CMC_EXT_STR

bool CMC_(PFX, _to_string)(struct SNAME *_queue_, FILE *fptr)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct SNAME *q_ = _queue_;

    return 0 <= fprintf(fptr,
                        "struct %s<%s> "
                        "at %p { "
                        "buffer:%p, "
                        "capacity:%" PRIuMAX ", "
                        "count:%" PRIuMAX ", "
                        "front:%" PRIuMAX ", "
                        "back:%" PRIuMAX ", "
                        "flag:%d, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        CMC_TO_STRING(SNAME), CMC_TO_STRING(V), q_, q_->buffer, q_->capacity, q_->count, q_->front,
                        q_->back, q_->flag, q_->f_val, q_->alloc, CMC_CALLBACKS_GET(q_));
}

bool CMC_(PFX, _print)(struct SNAME *_queue_, FILE *fptr, const char *start, const char *separator, const char *end)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    fprintf(fptr, "%s", start);

    for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
    {
        if (!_queue_->f_val->str(fptr, _queue_->buffer[i]))
            return false;

        i = (i + 1) % _queue_->capacity;

        if (j + 1 < _queue_->count)
            fprintf(fptr, "%s", separator);
    }

    fprintf(fptr, "%s", end);

    return true;
}

#endif /* CMC_EXT_STR */
