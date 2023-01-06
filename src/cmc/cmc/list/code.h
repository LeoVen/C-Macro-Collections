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
    return CMC_(PFX, _new_custom)(capacity, f_val, NULL, NULL);
}

struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val,
                                     struct CMC_ALLOC_NODE_NAME *alloc, struct CMC_CALLBACKS_NAME *callbacks)
{
    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (capacity < 1)
        return NULL;

    if (!f_val)
        return NULL;

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    struct SNAME *_list_ = alloc->malloc(sizeof(struct SNAME));

    if (!_list_)
        return NULL;

    _list_->buffer = alloc->calloc(capacity, sizeof(V));

    if (!_list_->buffer)
    {
        alloc->free(_list_);
        return NULL;
    }

    _list_->capacity = capacity;
    _list_->count = 0;
    _list_->flag = CMC_FLAG_OK;
    _list_->f_val = f_val;
    _list_->alloc = alloc;
    CMC_CALLBACKS_ASSIGN(_list_, callbacks);

    return _list_;
}

void CMC_(PFX, _clear)(struct SNAME *_list_)
{
    if (_list_->f_val->free)
    {
        for (size_t i = 0; i < _list_->count; i++)
            _list_->f_val->free(_list_->buffer[i]);
    }

    memset(_list_->buffer, 0, sizeof(V) * _list_->capacity);

    _list_->count = 0;
    _list_->flag = CMC_FLAG_OK;
}

void CMC_(PFX, _free)(struct SNAME *_list_)
{
    if (_list_->f_val->free)
    {
        for (size_t i = 0; i < _list_->count; i++)
            _list_->f_val->free(_list_->buffer[i]);
    }

    _list_->alloc->free(_list_->buffer);
    _list_->alloc->free(_list_);
}

void CMC_(PFX, _customize)(struct SNAME *_list_, struct CMC_ALLOC_NODE_NAME *alloc,
                           struct CMC_CALLBACKS_NAME *callbacks)
{
    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (!alloc)
        _list_->alloc = &cmc_alloc_node_default;
    else
        _list_->alloc = alloc;

    CMC_CALLBACKS_ASSIGN(_list_, callbacks);

    _list_->flag = CMC_FLAG_OK;
}

bool CMC_(PFX, _push_front)(struct SNAME *_list_, V value)
{
    if (CMC_(PFX, _full)(_list_))
    {
        if (!CMC_(PFX, _resize)(_list_, _list_->count * 2))
            return false;
    }

    if (!CMC_(PFX, _empty)(_list_))
    {
        memmove(_list_->buffer + 1, _list_->buffer, _list_->count * sizeof(V));
    }

    _list_->buffer[0] = value;

    _list_->count++;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, create);

    return true;
}

bool CMC_(PFX, _push_at)(struct SNAME *_list_, V value, size_t index)
{
    if (index > _list_->count)
    {
        _list_->flag = CMC_FLAG_RANGE;
        return false;
    }

    if (CMC_(PFX, _full)(_list_))
    {
        if (!CMC_(PFX, _resize)(_list_, _list_->count * 2))
            return false;
    }

    memmove(_list_->buffer + index + 1, _list_->buffer + index, (_list_->count - index) * sizeof(V));

    _list_->buffer[index] = value;
    _list_->count++;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, create);

    return true;
}

bool CMC_(PFX, _push_back)(struct SNAME *_list_, V value)
{
    if (CMC_(PFX, _full)(_list_))
    {
        if (!CMC_(PFX, _resize)(_list_, _list_->count * 2))
            return false;
    }

    _list_->buffer[_list_->count++] = value;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, create);

    return true;
}

bool CMC_(PFX, _pop_front)(struct SNAME *_list_)
{
    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    memmove(_list_->buffer, _list_->buffer + 1, (_list_->count - 1) * sizeof(V));

    _list_->buffer[--_list_->count] = (V){ 0 };
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, delete);

    return true;
}

bool CMC_(PFX, _pop_at)(struct SNAME *_list_, size_t index)
{
    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    if (index >= _list_->count)
    {
        _list_->flag = CMC_FLAG_RANGE;
        return false;
    }

    memmove(_list_->buffer + index, _list_->buffer + index + 1, (_list_->count - index - 1) * sizeof(V));

    _list_->buffer[--_list_->count] = (V){ 0 };
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, delete);

    return true;
}

bool CMC_(PFX, _pop_back)(struct SNAME *_list_)
{
    if (CMC_(PFX, _empty)(_list_))
        return false;

    _list_->buffer[--_list_->count] = (V){ 0 };
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, delete);

    return true;
}

V CMC_(PFX, _front)(struct SNAME *_list_)
{
    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, read);

    return _list_->buffer[0];
}

V CMC_(PFX, _get)(struct SNAME *_list_, size_t index)
{
    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    if (index >= _list_->count)
    {
        _list_->flag = CMC_FLAG_RANGE;
        return (V){ 0 };
    }

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, read);

    return _list_->buffer[index];
}

V *CMC_(PFX, _get_ref)(struct SNAME *_list_, size_t index)
{
    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return NULL;
    }

    if (index >= _list_->count)
    {
        _list_->flag = CMC_FLAG_RANGE;
        return NULL;
    }

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, read);

    return &(_list_->buffer[index]);
}

V CMC_(PFX, _back)(struct SNAME *_list_)
{
    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, read);

    return _list_->buffer[_list_->count - 1];
}

size_t CMC_(PFX, _index_of)(struct SNAME *_list_, V value, bool from_start)
{
    _list_->flag = CMC_FLAG_OK;

    size_t result = _list_->count;

    if (from_start)
    {
        for (size_t i = 0; i < _list_->count; i++)
        {
            if (_list_->f_val->cmp(_list_->buffer[i], value) == 0)
            {
                result = i;
                break;
            }
        }
    }
    else
    {
        for (size_t i = _list_->count; i > 0; i--)
        {
            if (_list_->f_val->cmp(_list_->buffer[i - 1], value) == 0)
            {
                result = i - 1;
                break;
            }
        }
    }

    CMC_CALLBACKS_CALL(_list_, read);

    return result;
}

bool CMC_(PFX, _contains)(struct SNAME *_list_, V value)
{
    _list_->flag = CMC_FLAG_OK;

    bool result = false;

    for (size_t i = 0; i < _list_->count; i++)
    {
        if (_list_->f_val->cmp(_list_->buffer[i], value) == 0)
        {
            result = true;
            break;
        }
    }

    CMC_CALLBACKS_CALL(_list_, read);

    return result;
}

bool CMC_(PFX, _empty)(struct SNAME *_list_)
{
    return _list_->count == 0;
}

bool CMC_(PFX, _full)(struct SNAME *_list_)
{
    return _list_->count >= _list_->capacity;
}

size_t CMC_(PFX, _count)(struct SNAME *_list_)
{
    return _list_->count;
}

bool CMC_(PFX, _fits)(struct SNAME *_list_, size_t size)
{
    return _list_->count + size <= _list_->capacity;
}

size_t CMC_(PFX, _capacity)(struct SNAME *_list_)
{
    return _list_->capacity;
}

int CMC_(PFX, _flag)(struct SNAME *_list_)
{
    return _list_->flag;
}

bool CMC_(PFX, _resize)(struct SNAME *_list_, size_t capacity)
{
    _list_->flag = CMC_FLAG_OK;

    if (_list_->capacity == capacity)
        return true;

    if (capacity < _list_->count)
    {
        _list_->flag = CMC_FLAG_INVALID;
        return false;
    }

    V *new_buffer = _list_->alloc->realloc(_list_->buffer, sizeof(V) * capacity);

    if (!new_buffer)
    {
        _list_->flag = CMC_FLAG_ALLOC;
        return false;
    }

    /* TODO zero out new slots */

    _list_->buffer = new_buffer;
    _list_->capacity = capacity;

    CMC_CALLBACKS_CALL(_list_, resize);

    return true;
}

struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_list_)
{
    struct SNAME *result = CMC_(PFX, _new_custom)(_list_->capacity, _list_->f_val, _list_->alloc, NULL);

    if (!result)
    {
        _list_->flag = CMC_FLAG_ALLOC;
        return NULL;
    }

    CMC_CALLBACKS_ASSIGN(result, _list_->callbacks);

    if (_list_->f_val->cpy)
    {
        for (size_t i = 0; i < _list_->count; i++)
            result->buffer[i] = _list_->f_val->cpy(_list_->buffer[i]);
    }
    else
        memcpy(result->buffer, _list_->buffer, sizeof(V) * _list_->count);

    result->count = _list_->count;

    _list_->flag = CMC_FLAG_OK;

    return result;
}

bool CMC_(PFX, _equals)(struct SNAME *_list1_, struct SNAME *_list2_)
{
    _list1_->flag = CMC_FLAG_OK;
    _list2_->flag = CMC_FLAG_OK;

    if (_list1_->count != _list2_->count)
        return false;

    for (size_t i = 0; i < _list1_->count; i++)
    {
        if (0 != _list1_->f_val->cmp(_list1_->buffer[i], _list2_->buffer[i]))
            return false;
    }

    return true;
}
