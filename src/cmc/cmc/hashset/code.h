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

/* Implementation Detail Functions */
static struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_get_entry)(struct SNAME *_set_, V value);
static size_t CMC_(PFX, _impl_calculate_size)(size_t required);

struct SNAME *CMC_(PFX, _new)(size_t capacity, double load, struct CMC_DEF_FVAL(SNAME) * f_val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _new_custom)(capacity, load, f_val, NULL, NULL);
}

struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, double load, struct CMC_DEF_FVAL(SNAME) * f_val,
                                     CMC_ALLOC_TYPE *alloc, CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (capacity == 0 || load <= 0 || load >= 1)
        return NULL;

    /* Prevent integer overflow */
    if (capacity >= (double)UINTMAX_MAX * load)
        return NULL;

    if (!f_val)
        return NULL;

    size_t real_capacity = CMC_(PFX, _impl_calculate_size)(capacity / load);

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    struct SNAME *_set_ = alloc->malloc(sizeof(struct SNAME));

    if (!_set_)
        return NULL;

    _set_->buffer = alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME)));

    if (!_set_->buffer)
    {
        alloc->free(_set_);
        return NULL;
    }

    _set_->count = 0;
    _set_->capacity = real_capacity;
    _set_->load = load;
    _set_->flag = CMC_FLAG_OK;
    _set_->f_val = f_val;
    _set_->alloc = alloc;
    CMC_CALLBACKS_ASSIGN(_set_, callbacks);

    return _set_;
}

void CMC_(PFX, _clear)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_set_->f_val->free)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct CMC_DEF_ENTRY(SNAME) *entry = &(_set_->buffer[i]);

            if (entry->state == CMC_ES_FILLED)
            {
                _set_->f_val->free(entry->value);
            }
        }
    }

    memset(_set_->buffer, 0, sizeof(struct CMC_DEF_ENTRY(SNAME)) * _set_->capacity);

    _set_->count = 0;
    _set_->flag = CMC_FLAG_OK;
}

void CMC_(PFX, _free)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_set_->f_val->free)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct CMC_DEF_ENTRY(SNAME) *entry = &(_set_->buffer[i]);

            if (entry->state == CMC_ES_FILLED)
            {
                _set_->f_val->free(entry->value);
            }
        }
    }

    _set_->alloc->free(_set_->buffer);
    _set_->alloc->free(_set_);
}

void CMC_(PFX, _customize)(struct SNAME *_set_, CMC_ALLOC_TYPE *alloc, CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (!alloc)
        _set_->alloc = &cmc_alloc_node_default;
    else
        _set_->alloc = alloc;

    CMC_CALLBACKS_ASSIGN(_set_, callbacks);

    _set_->flag = CMC_FLAG_OK;
}

bool CMC_(PFX, _insert)(struct SNAME *_set_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _full)(_set_))
    {
        if (!CMC_(PFX, _resize)(_set_, _set_->capacity + 1))
            return false;
    }

    if (CMC_(PFX, _impl_get_entry)(_set_, value) != NULL)
    {
        _set_->flag = CMC_FLAG_DUPLICATE;
        return false;
    }

    size_t hash = _set_->f_val->hash(value);
    size_t original_pos = hash % _set_->capacity;
    size_t pos = original_pos;

    struct CMC_DEF_ENTRY(SNAME) *target = &(_set_->buffer[pos]);

    if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)
    {
        target->value = value;
        target->dist = 0;
        target->state = CMC_ES_FILLED;
    }
    else
    {
        while (true)
        {
            pos++;
            target = &(_set_->buffer[pos % _set_->capacity]);

            if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)
            {
                target->value = value;
                target->dist = pos - original_pos;
                target->state = CMC_ES_FILLED;

                break;
            }
            else if (target->dist < pos - original_pos)
            {
                V tmp = target->value;
                size_t tmp_dist = target->dist;

                target->value = value;
                target->dist = pos - original_pos;

                value = tmp;
                original_pos = pos - tmp_dist;
            }
        }
    }

    _set_->count++;
    _set_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_set_);

    return true;
}

bool CMC_(PFX, _remove)(struct SNAME *_set_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    struct CMC_DEF_ENTRY(SNAME) *result = CMC_(PFX, _impl_get_entry)(_set_, value);

    if (result == NULL)
    {
        _set_->flag = CMC_FLAG_NOT_FOUND;
        return false;
    }

    result->value = (V){ 0 };
    result->dist = 0;
    result->state = CMC_ES_DELETED;

    _set_->count--;
    _set_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_set_);

    return true;
}

bool CMC_(PFX, _max)(struct SNAME *_set_, V *value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    bool first = true;
    V max_val = (V){ 0 };

    for (size_t i = 0; i < _set_->capacity; i++)
    {
        if (_set_->buffer[i].state == CMC_ES_FILLED)
        {
            if (first)
            {
                max_val = _set_->buffer[i].value;
                first = false;
            }
            else if (_set_->f_val->cmp(_set_->buffer[i].value, max_val) > 0)
            {
                max_val = _set_->buffer[i].value;
            }
        }
    }

    if (value)
        *value = max_val;

    _set_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_set_);

    return true;
}

bool CMC_(PFX, _min)(struct SNAME *_set_, V *value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    bool first = true;
    V min_val = (V){ 0 };

    for (size_t i = 0; i < _set_->capacity; i++)
    {
        if (_set_->buffer[i].state == CMC_ES_FILLED)
        {
            if (first)
            {
                min_val = _set_->buffer[i].value;
                first = false;
            }
            else if (_set_->f_val->cmp(_set_->buffer[i].value, min_val) < 0)
            {
                min_val = _set_->buffer[i].value;
            }
        }
    }

    if (value)
        *value = min_val;

    _set_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_set_);

    return true;
}

bool CMC_(PFX, _contains)(struct SNAME *_set_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _set_->flag = CMC_FLAG_OK;

    bool result = CMC_(PFX, _impl_get_entry)(_set_, value) != NULL;

    CMC_CALLBACKS_CALL(_set_);

    return result;
}

bool CMC_(PFX, _empty)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _set_->count == 0;
}

bool CMC_(PFX, _full)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return (double)_set_->capacity * _set_->load <= (double)_set_->count;
}

size_t CMC_(PFX, _count)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _set_->count;
}

size_t CMC_(PFX, _capacity)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _set_->capacity;
}

double CMC_(PFX, _load)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _set_->load;
}

int CMC_(PFX, _flag)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _set_->flag;
}

bool CMC_(PFX, _resize)(struct SNAME *_set_, size_t capacity)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _set_->flag = CMC_FLAG_OK;

    if (_set_->capacity == capacity)
        goto success;

    if (_set_->capacity > capacity / _set_->load)
        goto success;

    /* Prevent integer overflow */
    if (capacity >= (double)UINTMAX_MAX * _set_->load)
    {
        _set_->flag = CMC_FLAG_ERROR;
        return false;
    }

    /* Calculate required capacity based on the prime numbers */
    size_t theoretical_size = CMC_(PFX, _impl_calculate_size)(capacity);

    /* Not possible to shrink with current available prime numbers */
    if (theoretical_size < _set_->count / _set_->load)
    {
        _set_->flag = CMC_FLAG_INVALID;
        return false;
    }

    /* No callbacks since _new_set_ is just a temporary hashtable */
    struct SNAME *_new_set_ = CMC_(PFX, _new_custom)(capacity, _set_->load, _set_->f_val, _set_->alloc, NULL);

    if (!_new_set_)
    {
        _set_->flag = CMC_FLAG_ALLOC;
        return false;
    }

    for (size_t i = 0; i < _set_->capacity; i++)
    {
        if (_set_->buffer[i].state == CMC_ES_FILLED)
        {
            V value = _set_->buffer[i].value;

            /* TODO check this for possible errors */
            CMC_(PFX, _insert)(_new_set_, value);
        }
    }

    /* Unlikely */
    if (_set_->count != _new_set_->count)
    {
        CMC_(PFX, _free)(_new_set_);

        _set_->flag = CMC_FLAG_ERROR;
        return false;
    }

    struct CMC_DEF_ENTRY(SNAME) *tmp_b = _set_->buffer;
    _set_->buffer = _new_set_->buffer;
    _new_set_->buffer = tmp_b;

    size_t tmp_c = _set_->capacity;
    _set_->capacity = _new_set_->capacity;
    _new_set_->capacity = tmp_c;

    /* Prevent the set from freeing the data */
    _new_set_->f_val = &(struct CMC_DEF_FVAL(SNAME)){ 0 };

    CMC_(PFX, _free)(_new_set_);

success:

    CMC_CALLBACKS_CALL(_set_);

    return true;
}

struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct SNAME *result =
        CMC_(PFX, _new_custom)(_set_->capacity * _set_->load, _set_->load, _set_->f_val, _set_->alloc, NULL);

    if (!result)
    {
        _set_->flag = CMC_FLAG_ERROR;
        return NULL;
    }

    CMC_CALLBACKS_ASSIGN(result, _set_->callbacks);

    if (_set_->f_val->cpy)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct CMC_DEF_ENTRY(SNAME) *scan = &(_set_->buffer[i]);

            if (scan->state != CMC_ES_EMPTY)
            {
                struct CMC_DEF_ENTRY(SNAME) *target = &(result->buffer[i]);

                if (scan->state == CMC_ES_DELETED)
                    target->state = CMC_ES_DELETED;
                else
                {
                    target->state = scan->state;
                    target->dist = scan->dist;

                    target->value = _set_->f_val->cpy(scan->value);
                }
            }
        }
    }
    else
        memcpy(result->buffer, _set_->buffer, sizeof(struct CMC_DEF_ENTRY(SNAME)) * _set_->capacity);

    result->count = _set_->count;

    _set_->flag = CMC_FLAG_OK;

    return result;
}

bool CMC_(PFX, _equals)(struct SNAME *_set1_, struct SNAME *_set2_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _set1_->flag = CMC_FLAG_OK;
    _set2_->flag = CMC_FLAG_OK;

    if (_set1_->count != _set2_->count)
        return false;

    /* Optimize loop using the smallest hashtable */
    struct SNAME *_set_a_;
    struct SNAME *_set_b_;

    _set_a_ = _set1_->capacity < _set2_->capacity ? _set1_ : _set2_;
    _set_b_ = _set_a_ == _set1_ ? _set2_ : _set1_;

    for (size_t i = 0; i < _set_a_->capacity; i++)
    {
        if (_set_a_->buffer[i].state == CMC_ES_FILLED)
        {

            if (!CMC_(PFX, _impl_get_entry)(_set_b_, _set_a_->buffer[i].value))
                return false;
        }
    }

    return true;
}

static struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_get_entry)(struct SNAME *_set_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    size_t hash = _set_->f_val->hash(value);
    size_t pos = hash % _set_->capacity;

    struct CMC_DEF_ENTRY(SNAME) *target = &(_set_->buffer[pos]);

    while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED)
    {
        if (target->state != CMC_ES_DELETED && _set_->f_val->cmp(target->value, value) == 0)
            return target;

        pos++;
        target = &(_set_->buffer[pos % _set_->capacity]);
    }

    return NULL;
}

static size_t CMC_(PFX, _impl_calculate_size)(size_t required)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);

    if (cmc_hashtable_primes[count - 1] < required)
        return required;

    size_t i = 0;
    while (cmc_hashtable_primes[i] < required)
        i++;

    return cmc_hashtable_primes[i];
}
