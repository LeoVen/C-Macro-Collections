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

#endif /* CMC_EXT_INIT */

/**
 * ITER
 *
 * List bi-directional iterator.
 */
#ifdef CMC_EXT_ITER

struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME *target)
{
    struct CMC_DEF_ITER(SNAME) iter;

    iter.target = target;
    iter.cursor = 0;
    iter.index = 0;
    iter.first = 0;
    iter.last = 0;
    iter.start = true;
    iter.end = CMC_(PFX, _empty)(target);

    if (!CMC_(PFX, _empty)(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            if (target->buffer[i].state == CMC_ES_FILLED)
            {
                iter.first = i;
                break;
            }
        }

        iter.cursor = iter.first;

        for (size_t i = target->capacity; i > 0; i--)
        {
            if (target->buffer[i - 1].state == CMC_ES_FILLED)
            {
                iter.last = i - 1;
                break;
            }
        }
    }

    return iter;
}

struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME *target)
{
    struct CMC_DEF_ITER(SNAME) iter;

    iter.target = target;
    iter.cursor = 0;
    iter.index = 0;
    iter.first = 0;
    iter.last = 0;
    iter.start = CMC_(PFX, _empty)(target);
    iter.end = true;

    if (!CMC_(PFX, _empty)(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            if (target->buffer[i].state == CMC_ES_FILLED)
            {
                iter.first = i;
                break;
            }
        }

        for (size_t i = target->capacity; i > 0; i--)
        {
            if (target->buffer[i - 1].state == CMC_ES_FILLED)
            {
                iter.last = i - 1;
                break;
            }
        }

        iter.cursor = iter.last;
        iter.index = target->count - 1;
    }

    return iter;
}

bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
    return CMC_(PFX, _empty)(iter->target) || iter->start;
}

bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
    return CMC_(PFX, _empty)(iter->target) || iter->end;
}

bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = true;
        iter->end = CMC_(PFX, _empty)(iter->target);

        return true;
    }

    return false;
}

bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = CMC_(PFX, _empty)(iter->target);
        iter->end = true;

        return true;
    }

    return false;
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

    struct CMC_DEF_ENTRY(SNAME) *scan = &(iter->target->buffer[iter->cursor]);

    iter->index++;

    while (1)
    {
        iter->cursor++;
        scan = &(iter->target->buffer[iter->cursor]);

        if (scan->state == CMC_ES_FILLED)
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

    struct CMC_DEF_ENTRY(SNAME) *scan = &(iter->target->buffer[iter->cursor]);

    iter->index--;

    while (1)
    {
        iter->cursor--;
        scan = &(iter->target->buffer[iter->cursor]);

        if (scan->state == CMC_ES_FILLED)
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

/* Returns true only if the iterator was able to be positioned at the */
/* given index */
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

V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (CMC_(PFX, _empty)(iter->target))
        return (V){ 0 };

    return iter->target->buffer[iter->cursor].value;
}

size_t CMC_(PFX, _iter_multiplicity)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (CMC_(PFX, _empty)(iter->target))
        return 0;

    return iter->target->buffer[iter->cursor].multiplicity;
}

size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)
{
    return iter->index;
}

#endif /* CMC_EXT_ITER */

/**
 * SETF
 *
 * Set functions
 */
#ifdef CMC_EXT_SETF

struct SNAME *CMC_(PFX, _union)(struct SNAME *_set1_, struct SNAME *_set2_)
{
    /* Callbacks are added later */
    struct SNAME *_set_r_ = CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, NULL);

    if (!_set_r_)
        return NULL;

    struct CMC_DEF_ITER(SNAME) iter1 = CMC_(PFX, _iter_start)(_set1_);
    struct CMC_DEF_ITER(SNAME) iter2 = CMC_(PFX, _iter_start)(_set2_);

    for (; !CMC_(PFX, _iter_at_end)(&iter1); CMC_(PFX, _iter_next)(&iter1))
    {
        V value = CMC_(PFX, _iter_value)(&iter1);

        size_t m1 = CMC_(PFX, _iter_multiplicity)(&iter1);
        size_t m2 = CMC_(PFX, _impl_multiplicity_of)(_set2_, value);
        size_t max_ = m1 > m2 ? m1 : m2;

        CMC_(PFX, _update)(_set_r_, value, max_);
    }

    for (; !CMC_(PFX, _iter_at_end)(&iter2); CMC_(PFX, _iter_next)(&iter2))
    {
        V value = CMC_(PFX, _iter_value)(&iter2);

        size_t m1 = CMC_(PFX, _impl_multiplicity_of)(_set1_, value);
        size_t m2 = CMC_(PFX, _iter_multiplicity)(&iter2);
        size_t max_ = m1 > m2 ? m1 : m2;

        CMC_(PFX, _update)(_set_r_, value, max_);
    }

    CMC_CALLBACKS_ASSIGN(_set_r_, _set1_->callbacks);

    return _set_r_;
}

struct SNAME *CMC_(PFX, _intersection)(struct SNAME *_set1_, struct SNAME *_set2_)
{
    /* Callbacks are added later */
    struct SNAME *_set_r_ = CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, NULL);

    if (!_set_r_)
        return NULL;

    struct SNAME *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;
    struct SNAME *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;

    struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set_A_);

    for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))
    {
        V value = CMC_(PFX, _iter_value)(&iter);

        size_t m1 = CMC_(PFX, _iter_multiplicity)(&iter);
        size_t m2 = CMC_(PFX, _impl_multiplicity_of)(_set_B_, value);
        size_t max_ = m1 < m2 ? m1 : m2;

        CMC_(PFX, _update)(_set_r_, value, max_);
    }

    CMC_CALLBACKS_ASSIGN(_set_r_, _set1_->callbacks);

    return _set_r_;
}

struct SNAME *CMC_(PFX, _difference)(struct SNAME *_set1_, struct SNAME *_set2_)
{
    /* Callbacks are added later */
    struct SNAME *_set_r_ = CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, NULL);

    if (!_set_r_)
        return NULL;

    struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_);

    for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))
    {
        V value = CMC_(PFX, _iter_value)(&iter);

        size_t m1 = CMC_(PFX, _iter_multiplicity)(&iter);
        size_t m2 = CMC_(PFX, _impl_multiplicity_of)(_set2_, value);

        if (m1 > m2)
            CMC_(PFX, _update)(_set_r_, value, m1 - m2);
    }

    CMC_CALLBACKS_ASSIGN(_set_r_, _set1_->callbacks);

    return _set_r_;
}

struct SNAME *CMC_(PFX, _summation)(struct SNAME *_set1_, struct SNAME *_set2_)
{
    /* Callbacks are added later */
    struct SNAME *_set_r_ = CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, NULL);

    if (!_set_r_)
        return NULL;

    struct CMC_DEF_ITER(SNAME) iter1 = CMC_(PFX, _iter_start)(_set1_);
    struct CMC_DEF_ITER(SNAME) iter2 = CMC_(PFX, _iter_start)(_set2_);

    for (; !CMC_(PFX, _iter_at_end)(&iter1); CMC_(PFX, _iter_next)(&iter1))
    {
        CMC_(PFX, _insert_many)
        (_set_r_, CMC_(PFX, _iter_value)(&iter1), CMC_(PFX, _iter_multiplicity)(&iter1));
    }

    for (; !CMC_(PFX, _iter_at_end)(&iter2); CMC_(PFX, _iter_next)(&iter2))
    {
        CMC_(PFX, _insert_many)
        (_set_r_, CMC_(PFX, _iter_value)(&iter2), CMC_(PFX, _iter_multiplicity)(&iter2));
    }

    CMC_CALLBACKS_ASSIGN(_set_r_, _set1_->callbacks);

    return _set_r_;
}

struct SNAME *CMC_(PFX, _symmetric_difference)(struct SNAME *_set1_, struct SNAME *_set2_)
{
    /* Callbacks are added later */
    struct SNAME *_set_r_ = CMC_(PFX, _new_custom)(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, NULL);

    if (!_set_r_)
        return NULL;
    struct CMC_DEF_ITER(SNAME) iter1 = CMC_(PFX, _iter_start)(_set1_);
    struct CMC_DEF_ITER(SNAME) iter2 = CMC_(PFX, _iter_start)(_set2_);

    for (; !CMC_(PFX, _iter_at_end)(&iter1); CMC_(PFX, _iter_next)(&iter1))
    {
        V value = CMC_(PFX, _iter_value)(&iter1);

        size_t m1 = CMC_(PFX, _iter_multiplicity)(&iter1);
        size_t m2 = CMC_(PFX, _impl_multiplicity_of)(_set2_, value);

        if (m1 != m2)
        {
            if (m1 > m2)
                CMC_(PFX, _update)(_set_r_, value, m1 - m2);
            else
                CMC_(PFX, _update)(_set_r_, value, m2 - m1);
        }
    }

    for (; !CMC_(PFX, _iter_at_end)(&iter2); CMC_(PFX, _iter_next)(&iter2))
    {
        V value = CMC_(PFX, _iter_value)(&iter2);

        size_t m1 = CMC_(PFX, _impl_multiplicity_of)(_set1_, value);
        size_t m2 = CMC_(PFX, _iter_multiplicity)(&iter2);

        if (m1 != m2)
        {
            if (m1 > m2)
                CMC_(PFX, _update)(_set_r_, value, m1 - m2);
            else
                CMC_(PFX, _update)(_set_r_, value, m2 - m1);
        }
    }

    CMC_CALLBACKS_ASSIGN(_set_r_, _set1_->callbacks);

    return _set_r_;
}

bool CMC_(PFX, _is_subset)(struct SNAME *_set1_, struct SNAME *_set2_)
{
    if (_set1_->count > _set2_->count)
        return false;

    if (CMC_(PFX, _empty)(_set1_))
        return true;

    struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_);

    for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))
    {
        V value = CMC_(PFX, _iter_value)(&iter);

        size_t m1 = CMC_(PFX, _iter_multiplicity)(&iter);
        size_t m2 = CMC_(PFX, _impl_multiplicity_of)(_set2_, value);

        if (m1 > m2)
            return false;
    }

    return true;
}

bool CMC_(PFX, _is_superset)(struct SNAME *_set1_, struct SNAME *_set2_)
{
    return CMC_(PFX, _is_subset)(_set2_, _set1_);
}

bool CMC_(PFX, _is_proper_subset)(struct SNAME *_set1_, struct SNAME *_set2_)
{
    if (_set1_->count >= _set2_->count)
        return false;

    if (CMC_(PFX, _empty)(_set1_))
    {
        if (!CMC_(PFX, _empty)(_set2_))
            return true;
        else
            return false;
    }

    struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_);

    for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))
    {
        V value = CMC_(PFX, _iter_value)(&iter);

        size_t m1 = CMC_(PFX, _iter_multiplicity)(&iter);
        size_t m2 = CMC_(PFX, _impl_multiplicity_of)(_set2_, value);

        if (m1 >= m2)
            return false;
    }

    return true;
}

bool CMC_(PFX, _is_proper_superset)(struct SNAME *_set1_, struct SNAME *_set2_)
{
    return CMC_(PFX, _is_proper_subset)(_set2_, _set1_);
}

bool CMC_(PFX, _is_disjointset)(struct SNAME *_set1_, struct SNAME *_set2_)
{
    if (CMC_(PFX, _empty)(_set1_))
        return true;

    struct CMC_DEF_ITER(SNAME) iter = CMC_(PFX, _iter_start)(_set1_);

    for (; !CMC_(PFX, _iter_at_end)(&iter); CMC_(PFX, _iter_next)(&iter))
    {
        V value = CMC_(PFX, _iter_value)(&iter);

        if (CMC_(PFX, _impl_get_entry)(_set2_, value) != NULL)
            return false;
    }

    return true;
}

#endif /* CMC_EXT_SETF */

/**
 * STR
 *
 * Print helper functions.
 */
#ifdef CMC_EXT_STR

bool CMC_(PFX, _to_string)(struct SNAME *_set_, FILE *fptr)
{
    struct SNAME *s_ = _set_;

    return 0 <= fprintf(fptr,
                        "struct %s<%s> "
                        "at %p { "
                        "buffer:%p, "
                        "capacity:%" PRIuMAX ", "
                        "count:%" PRIuMAX ", "
                        "cardinality:%" PRIuMAX ", "
                        "load:%lf, "
                        "flag:%d, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        CMC_TO_STRING(SNAME), CMC_TO_STRING(V), s_, s_->buffer, s_->capacity, s_->count,
                        s_->cardinality, s_->load, s_->flag, s_->f_val, s_->alloc, CMC_CALLBACKS_GET(s_));
}

bool CMC_(PFX, _print)(struct SNAME *_set_, FILE *fptr, const char *start, const char *separator, const char *end,
                       const char *val_mul_sep)
{
    fprintf(fptr, "%s", start);

    size_t last = 0;
    for (size_t i = _set_->capacity; i > 0; i--)
    {
        if ((_set_->buffer[i - 1]).state == CMC_ES_FILLED)
        {
            last = i - 1;
            break;
        }
    }

    for (size_t i = 0; i < _set_->capacity; i++)
    {
        struct CMC_DEF_ENTRY(SNAME) *entry = &(_set_->buffer[i]);

        if (entry->state == CMC_ES_FILLED)
        {
            if (!_set_->f_val->str(fptr, entry->value))
                return false;

            fprintf(fptr, "%s", val_mul_sep);

            if (fprintf(fptr, "%" PRIuMAX "", entry->multiplicity) < 0)
                return false;

            if (i + 1 < last)
                fprintf(fptr, "%s", separator);
        }
    }

    fprintf(fptr, "%s", end);

    return true;
}

#endif /* CMC_EXT_STR */
