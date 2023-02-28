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

    struct SNAME *_queue_ = alloc->malloc(sizeof(struct SNAME));

    if (!_queue_)
        return NULL;

    _queue_->buffer = alloc->calloc(capacity, sizeof(V));

    if (!_queue_->buffer)
    {
        alloc->free(_queue_);
        return NULL;
    }

    _queue_->capacity = capacity;
    _queue_->count = 0;
    _queue_->front = 0;
    _queue_->back = 0;
    _queue_->flag = CMC_FLAG_OK;
    _queue_->f_val = f_val;
    _queue_->alloc = alloc;
    CMC_CALLBACKS_ASSIGN(_queue_, callbacks);

    return _queue_;
}

void CMC_(PFX, _clear)(struct SNAME *_queue_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_queue_->f_val->free)
    {
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
        {
            _queue_->f_val->free(_queue_->buffer[i]);

            i = (i + 1) % _queue_->capacity;
        }
    }

    memset(_queue_->buffer, 0, sizeof(V) * _queue_->capacity);

    _queue_->count = 0;
    _queue_->front = 0;
    _queue_->back = 0;
    _queue_->flag = CMC_FLAG_OK;
}

void CMC_(PFX, _free)(struct SNAME *_queue_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_queue_->f_val->free)
    {
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
        {
            _queue_->f_val->free(_queue_->buffer[i]);

            i = (i + 1) % _queue_->capacity;
        }
    }

    _queue_->alloc->free(_queue_->buffer);
    _queue_->alloc->free(_queue_);
}

void CMC_(PFX, _customize)(struct SNAME *_queue_, CMC_ALLOC_TYPE *alloc, CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (!alloc)
        _queue_->alloc = &cmc_alloc_node_default;
    else
        _queue_->alloc = alloc;

    CMC_CALLBACKS_ASSIGN(_queue_, callbacks);

    _queue_->flag = CMC_FLAG_OK;
}

bool CMC_(PFX, _enqueue)(struct SNAME *_queue_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _full)(_queue_))
    {
        if (!CMC_(PFX, _resize)(_queue_, _queue_->capacity * 2))
            return false;
    }

    _queue_->buffer[_queue_->back] = value;

    _queue_->back = (_queue_->back == _queue_->capacity - 1) ? 0 : _queue_->back + 1;
    _queue_->count++;
    _queue_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_queue_);

    return true;
}

bool CMC_(PFX, _dequeue)(struct SNAME *_queue_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_queue_))
    {
        _queue_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    _queue_->buffer[_queue_->front] = (V){ 0 };

    _queue_->front = (_queue_->front == _queue_->capacity - 1) ? 0 : _queue_->front + 1;
    _queue_->count--;
    _queue_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_queue_);

    return true;
}

V CMC_(PFX, _peek)(struct SNAME *_queue_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_queue_))
    {
        _queue_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    _queue_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_queue_);

    return _queue_->buffer[_queue_->front];
}

bool CMC_(PFX, _contains)(struct SNAME *_queue_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _queue_->flag = CMC_FLAG_OK;

    bool result = false;

    for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
    {
        if (_queue_->f_val->cmp(_queue_->buffer[i], value) == 0)
        {
            result = true;
            break;
        }

        i = (i + 1) % _queue_->capacity;
    }

    CMC_CALLBACKS_CALL(_queue_);

    return result;
}

bool CMC_(PFX, _empty)(struct SNAME *_queue_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _queue_->count == 0;
}

bool CMC_(PFX, _full)(struct SNAME *_queue_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _queue_->count >= _queue_->capacity;
}

size_t CMC_(PFX, _count)(struct SNAME *_queue_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _queue_->count;
}

size_t CMC_(PFX, _capacity)(struct SNAME *_queue_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _queue_->capacity;
}

int CMC_(PFX, _flag)(struct SNAME *_queue_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _queue_->flag;
}

bool CMC_(PFX, _resize)(struct SNAME *_queue_, size_t capacity)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_queue_->capacity == capacity)
        goto success;

    if (capacity < _queue_->count)
    {
        _queue_->flag = CMC_FLAG_INVALID;
        return false;
    }

    V *new_buffer = _queue_->alloc->malloc(sizeof(V) * capacity);

    if (!new_buffer)
    {
        _queue_->flag = CMC_FLAG_ALLOC;
        return false;
    }

    for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
    {
        new_buffer[j] = _queue_->buffer[i];

        i = (i + 1) % _queue_->capacity;
    }

    _queue_->alloc->free(_queue_->buffer);

    _queue_->buffer = new_buffer;
    _queue_->capacity = capacity;
    _queue_->front = 0;
    _queue_->back = _queue_->count;

success:

    _queue_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_queue_);

    return true;
}

struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_queue_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct SNAME *result = CMC_(PFX, _new_custom)(_queue_->capacity, _queue_->f_val, _queue_->alloc, NULL);

    if (!result)
    {
        _queue_->flag = CMC_FLAG_ERROR;
        return NULL;
    }

    CMC_CALLBACKS_ASSIGN(result, _queue_->callbacks);

    if (_queue_->f_val->cpy)
    {
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
        {
            result->buffer[j] = _queue_->f_val->cpy(_queue_->buffer[i]);

            i = (i + 1) % _queue_->capacity;
        }
    }
    else
    {
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
        {
            result->buffer[j] = _queue_->buffer[i];

            i = (i + 1) % _queue_->capacity;
        }
    }

    result->count = _queue_->count;
    result->front = 0;
    result->back = _queue_->count;

    _queue_->flag = CMC_FLAG_OK;

    return result;
}

bool CMC_(PFX, _equals)(struct SNAME *_queue1_, struct SNAME *_queue2_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _queue1_->flag = CMC_FLAG_OK;
    _queue2_->flag = CMC_FLAG_OK;

    if (_queue1_->count != _queue2_->count)
        return false;

    size_t i, j, k;
    for (i = _queue1_->front, j = _queue2_->front, k = 0; k < _queue1_->count; k++)
    {
        if (_queue1_->f_val->cmp(_queue1_->buffer[i], _queue2_->buffer[j]) != 0)
            return false;

        i = (i + 1) % _queue1_->capacity;
        j = (j + 1) % _queue2_->capacity;
    }

    return true;
}
