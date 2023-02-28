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

    struct SNAME *_stack_ = alloc->malloc(sizeof(struct SNAME));

    if (!_stack_)
        return NULL;

    _stack_->buffer = alloc->calloc(capacity, sizeof(V));

    if (!_stack_->buffer)
    {
        alloc->free(_stack_);
        return NULL;
    }

    _stack_->capacity = capacity;
    _stack_->count = 0;
    _stack_->flag = CMC_FLAG_OK;
    _stack_->f_val = f_val;
    _stack_->alloc = alloc;
    CMC_CALLBACKS_ASSIGN(_stack_, callbacks);

    return _stack_;
}

void CMC_(PFX, _clear)(struct SNAME *_stack_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_stack_->f_val->free)
    {
        for (size_t i = 0; i < _stack_->count; i++)
            _stack_->f_val->free(_stack_->buffer[i]);
    }

    memset(_stack_->buffer, 0, sizeof(V) * _stack_->capacity);

    _stack_->count = 0;
    _stack_->flag = CMC_FLAG_OK;
}

void CMC_(PFX, _free)(struct SNAME *_stack_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_stack_->f_val->free)
    {
        for (size_t i = 0; i < _stack_->count; i++)
            _stack_->f_val->free(_stack_->buffer[i]);
    }

    _stack_->alloc->free(_stack_->buffer);
    _stack_->alloc->free(_stack_);
}

void CMC_(PFX, _customize)(struct SNAME *_stack_, CMC_ALLOC_TYPE *alloc, CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (!alloc)
        _stack_->alloc = &cmc_alloc_node_default;
    else
        _stack_->alloc = alloc;

    CMC_CALLBACKS_ASSIGN(_stack_, callbacks);

    _stack_->flag = CMC_FLAG_OK;
}

bool CMC_(PFX, _push)(struct SNAME *_stack_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _full)(_stack_))
    {
        if (!CMC_(PFX, _resize)(_stack_, _stack_->capacity * 2))
            return false;
    }

    _stack_->buffer[_stack_->count++] = value;

    _stack_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_stack_);

    return true;
}

bool CMC_(PFX, _pop)(struct SNAME *_stack_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_stack_))
    {
        _stack_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    _stack_->buffer[--_stack_->count] = (V){ 0 };

    _stack_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_stack_);

    return true;
}

V CMC_(PFX, _top)(struct SNAME *_stack_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_stack_))
    {
        _stack_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    _stack_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_stack_);

    return _stack_->buffer[_stack_->count - 1];
}

bool CMC_(PFX, _contains)(struct SNAME *_stack_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _stack_->flag = CMC_FLAG_OK;

    bool result = false;

    for (size_t i = 0; i < _stack_->count; i++)
    {
        if (_stack_->f_val->cmp(_stack_->buffer[i], value) == 0)
        {
            result = true;
            break;
        }
    }

    CMC_CALLBACKS_CALL(_stack_);

    return result;
}

bool CMC_(PFX, _empty)(struct SNAME *_stack_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _stack_->count == 0;
}

bool CMC_(PFX, _full)(struct SNAME *_stack_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _stack_->count >= _stack_->capacity;
}

size_t CMC_(PFX, _count)(struct SNAME *_stack_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _stack_->count;
}

size_t CMC_(PFX, _capacity)(struct SNAME *_stack_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _stack_->capacity;
}

int CMC_(PFX, _flag)(struct SNAME *_stack_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _stack_->flag;
}

bool CMC_(PFX, _resize)(struct SNAME *_stack_, size_t capacity)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_stack_->capacity == capacity)
        goto success;

    if (capacity < _stack_->count)
    {
        _stack_->flag = CMC_FLAG_INVALID;
        return false;
    }

    V *new_buffer = _stack_->alloc->realloc(_stack_->buffer, sizeof(V) * capacity);

    if (!new_buffer)
    {
        _stack_->flag = CMC_FLAG_ALLOC;
        return false;
    }

    /* TODO zero out new slots */

    _stack_->buffer = new_buffer;
    _stack_->capacity = capacity;

success:

    _stack_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_stack_);

    return true;
}

struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_stack_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct SNAME *result = CMC_(PFX, _new_custom)(_stack_->capacity, _stack_->f_val, _stack_->alloc, NULL);

    if (!result)
    {
        _stack_->flag = CMC_FLAG_ERROR;
        return NULL;
    }

    CMC_CALLBACKS_ASSIGN(result, _stack_->callbacks);

    if (_stack_->f_val->cpy)
    {
        for (size_t i = 0; i < _stack_->count; i++)
            result->buffer[i] = _stack_->f_val->cpy(_stack_->buffer[i]);
    }
    else
        memcpy(result->buffer, _stack_->buffer, sizeof(V) * _stack_->count);

    result->count = _stack_->count;

    _stack_->flag = CMC_FLAG_OK;

    return result;
}

bool CMC_(PFX, _equals)(struct SNAME *_stack1_, struct SNAME *_stack2_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _stack1_->flag = CMC_FLAG_OK;
    _stack2_->flag = CMC_FLAG_OK;

    if (_stack1_->count != _stack2_->count)
        return false;

    for (size_t i = 0; i < _stack1_->count; i++)
    {
        if (_stack1_->f_val->cmp(_stack1_->buffer[i], _stack2_->buffer[i]) != 0)
            return false;
    }

    return true;
}