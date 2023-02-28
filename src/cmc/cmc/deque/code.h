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

/* Implementation Detail Functions */
/* None */

struct SNAME *CMC_(PFX, _new)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _new_custom)(capacity, f_val, NULL, NULL);
}

struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val, CMC_ALLOC_TYPE *alloc,
                                     CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (capacity < 1)
        return NULL;

    if (!f_val)
        return NULL;

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    struct SNAME *_deque_ = alloc->malloc(sizeof(struct SNAME));

    if (!_deque_)
        return NULL;

    _deque_->buffer = alloc->calloc(capacity, sizeof(V));

    if (!_deque_->buffer)
    {
        alloc->free(_deque_);
        return NULL;
    }

    _deque_->capacity = capacity;
    _deque_->count = 0;
    _deque_->front = 0;
    _deque_->back = 0;
    _deque_->flag = CMC_FLAG_OK;
    _deque_->f_val = f_val;
    _deque_->alloc = alloc;
    CMC_CALLBACKS_ASSIGN(_deque_, callbacks);

    return _deque_;
}

void CMC_(PFX, _clear)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_deque_->f_val->free)
    {
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
        {
            _deque_->f_val->free(_deque_->buffer[i]);

            i = (i + 1) % _deque_->capacity;
        }
    }

    memset(_deque_->buffer, 0, sizeof(V) * _deque_->capacity);

    _deque_->count = 0;
    _deque_->front = 0;
    _deque_->back = 0;

    _deque_->flag = CMC_FLAG_OK;
}

void CMC_(PFX, _free)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_deque_->f_val->free)
    {
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
        {
            _deque_->f_val->free(_deque_->buffer[i]);

            i = (i + 1) % _deque_->capacity;
        }
    }

    _deque_->alloc->free(_deque_->buffer);
    _deque_->alloc->free(_deque_);
}

void CMC_(PFX, _customize)(struct SNAME *_deque_, CMC_ALLOC_TYPE *alloc, CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (!alloc)
        _deque_->alloc = &cmc_alloc_node_default;
    else
        _deque_->alloc = alloc;

    CMC_CALLBACKS_ASSIGN(_deque_, callbacks);

    _deque_->flag = CMC_FLAG_OK;
}

bool CMC_(PFX, _push_front)(struct SNAME *_deque_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _full)(_deque_))
    {
        if (!CMC_(PFX, _resize)(_deque_, _deque_->capacity * 2))
            return false;
    }

    _deque_->front = (_deque_->front == 0) ? _deque_->capacity - 1 : _deque_->front - 1;

    _deque_->buffer[_deque_->front] = value;

    _deque_->count++;
    _deque_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_deque_);

    return true;
}

bool CMC_(PFX, _push_back)(struct SNAME *_deque_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _full)(_deque_))
    {
        if (!CMC_(PFX, _resize)(_deque_, _deque_->capacity * 2))
            return false;
    }

    _deque_->buffer[_deque_->back] = value;

    _deque_->back = (_deque_->back == _deque_->capacity - 1) ? 0 : _deque_->back + 1;

    _deque_->count++;
    _deque_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_deque_);

    return true;
}

bool CMC_(PFX, _pop_front)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_deque_))
    {
        _deque_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    _deque_->buffer[_deque_->front] = (V){ 0 };

    _deque_->front = (_deque_->front == _deque_->capacity - 1) ? 0 : _deque_->front + 1;

    _deque_->count--;
    _deque_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_deque_);

    return true;
}

bool CMC_(PFX, _pop_back)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_deque_))
    {
        _deque_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    _deque_->back = (_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1;

    _deque_->buffer[_deque_->back] = (V){ 0 };

    _deque_->count--;
    _deque_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_deque_);

    return true;
}

V CMC_(PFX, _front)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_deque_))
    {
        _deque_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    _deque_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_deque_);

    return _deque_->buffer[_deque_->front];
}

V CMC_(PFX, _back)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_deque_))
    {
        _deque_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    _deque_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_deque_);

    return _deque_->buffer[(_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1];
}

bool CMC_(PFX, _contains)(struct SNAME *_deque_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _deque_->flag = CMC_FLAG_OK;

    bool result = false;

    for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
    {
        if (_deque_->f_val->cmp(_deque_->buffer[i], value) == 0)
        {
            result = true;
            break;
        }

        i = (i + 1) % _deque_->capacity;
    }

    CMC_CALLBACKS_CALL(_deque_);

    return result;
}

bool CMC_(PFX, _empty)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _deque_->count == 0;
}

bool CMC_(PFX, _full)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _deque_->count >= _deque_->capacity;
}

size_t CMC_(PFX, _count)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _deque_->count;
}

size_t CMC_(PFX, _capacity)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _deque_->capacity;
}

int CMC_(PFX, _flag)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _deque_->flag;
}

bool CMC_(PFX, _resize)(struct SNAME *_deque_, size_t capacity)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _deque_->flag = CMC_FLAG_OK;

    if (_deque_->capacity == capacity)
        goto success;

    if (capacity < _deque_->count)
    {
        _deque_->flag = CMC_FLAG_INVALID;
        return false;
    }

    V *new_buffer = _deque_->alloc->malloc(sizeof(V) * capacity);

    if (!new_buffer)
    {
        _deque_->flag = CMC_FLAG_ALLOC;
        return false;
    }

    for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
    {
        new_buffer[j] = _deque_->buffer[i];

        i = (i + 1) % _deque_->capacity;
    }

    _deque_->alloc->free(_deque_->buffer);

    _deque_->buffer = new_buffer;
    _deque_->capacity = capacity;
    _deque_->front = 0;
    _deque_->back = _deque_->count;

success:

    CMC_CALLBACKS_CALL(_deque_);

    return true;
}

struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_deque_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct SNAME *result = CMC_(PFX, _new_custom)(_deque_->capacity, _deque_->f_val, _deque_->alloc, NULL);

    if (!result)
    {
        _deque_->flag = CMC_FLAG_ERROR;
        return NULL;
    }

    CMC_CALLBACKS_ASSIGN(result, _deque_->callbacks);

    if (_deque_->f_val->cpy)
    {
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
        {
            result->buffer[j] = _deque_->f_val->cpy(_deque_->buffer[i]);

            i = (i + 1) % _deque_->capacity;
        }
    }
    else
    {
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
        {
            result->buffer[j] = _deque_->buffer[i];

            i = (i + 1) % _deque_->capacity;
        }
    }

    result->count = _deque_->count;
    result->front = 0;
    result->back = _deque_->count;

    _deque_->flag = CMC_FLAG_OK;

    return result;
}

bool CMC_(PFX, _equals)(struct SNAME *_deque1_, struct SNAME *_deque2_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _deque1_->flag = CMC_FLAG_OK;
    _deque2_->flag = CMC_FLAG_OK;

    if (_deque1_->count != _deque2_->count)
        return false;

    size_t i, j, k;
    for (i = _deque1_->front, j = _deque2_->front, k = 0; k < _deque1_->count; k++)
    {
        if (_deque1_->f_val->cmp(_deque1_->buffer[i], _deque2_->buffer[j]) != 0)
            return false;

        i = (i + 1) % _deque1_->capacity;
        j = (j + 1) % _deque2_->capacity;
    }

    return true;
}
