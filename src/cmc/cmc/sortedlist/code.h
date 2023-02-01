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
static size_t CMC_(PFX, _impl_binary_search_first)(struct SNAME *_list_, V value);
static size_t CMC_(PFX, _impl_binary_search_last)(struct SNAME *_list_, V value);
void CMC_(PFX, _impl_sort_quicksort)(V *array, int (*cmp)(V, V), size_t low, size_t high);
void CMC_(PFX, _impl_sort_insertion)(V *array, int (*cmp)(V, V), size_t low, size_t high);

struct SNAME *CMC_(PFX, _new)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _new_custom)(capacity, f_val, NULL, NULL);
}

struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, struct CMC_DEF_FVAL(SNAME) * f_val,
                                     struct CMC_ALLOC_NODE_NAME *alloc, struct CMC_CALLBACKS_NAME *callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (capacity < 1)
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
    _list_->is_sorted = false;
    _list_->flag = CMC_FLAG_OK;
    _list_->f_val = f_val;
    _list_->alloc = alloc;
    CMC_CALLBACKS_ASSIGN(_list_, callbacks);

    return _list_;
}

void CMC_(PFX, _clear)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

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
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

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
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (!alloc)
        _list_->alloc = &cmc_alloc_node_default;
    else
        _list_->alloc = alloc;

    CMC_CALLBACKS_ASSIGN(_list_, callbacks);

    _list_->flag = CMC_FLAG_OK;
}

bool CMC_(PFX, _insert)(struct SNAME *_list_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _full)(_list_))
    {
        if (!CMC_(PFX, _resize)(_list_, _list_->capacity * 2))
            return false;
    }

    _list_->buffer[_list_->count++] = value;

    _list_->is_sorted = false;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, create);

    return true;
}

bool CMC_(PFX, _remove)(struct SNAME *_list_, size_t index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

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

    memmove(_list_->buffer + index, _list_->buffer + index + 1, (_list_->count - index) * sizeof(V));

    _list_->buffer[--_list_->count] = (V){ 0 };

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, delete);

    return true;
}

V CMC_(PFX, _max)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    CMC_(PFX, _sort)(_list_);

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, read);

    return _list_->buffer[_list_->count - 1];
}

V CMC_(PFX, _min)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    CMC_(PFX, _sort)(_list_);

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, read);

    return _list_->buffer[0];
}

V CMC_(PFX, _get)(struct SNAME *_list_, size_t index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

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

    CMC_(PFX, _sort)(_list_);

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, read);

    return _list_->buffer[index];
}

size_t CMC_(PFX, _index_of)(struct SNAME *_list_, V value, bool from_start)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _list_->flag = CMC_FLAG_OK;

    CMC_(PFX, _sort)(_list_);

    CMC_CALLBACKS_CALL(_list_, read);

    if (from_start)
    {
        return CMC_(PFX, _impl_binary_search_first)(_list_, value);
    }

    return CMC_(PFX, _impl_binary_search_last)(_list_, value);
}

bool CMC_(PFX, _contains)(struct SNAME *_list_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _list_->flag = CMC_FLAG_OK;

    if (CMC_(PFX, _empty)(_list_))
        return false;

    CMC_(PFX, _sort)(_list_);

    CMC_CALLBACKS_CALL(_list_, read);

    return CMC_(PFX, _impl_binary_search_first)(_list_, value) < _list_->count;
}

bool CMC_(PFX, _empty)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _list_->count == 0;
}

bool CMC_(PFX, _full)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _list_->count >= _list_->capacity;
}

size_t CMC_(PFX, _count)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _list_->count;
}

size_t CMC_(PFX, _capacity)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _list_->capacity;
}

int CMC_(PFX, _flag)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _list_->flag;
}

bool CMC_(PFX, _resize)(struct SNAME *_list_, size_t capacity)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_list_->capacity == capacity)
        goto success;

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

    /* TODO zero out remaining slots */

    _list_->buffer = new_buffer;
    _list_->capacity = capacity;

success:

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_, resize);

    return true;
}

void CMC_(PFX, _sort)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _list_->flag = CMC_FLAG_OK;

    if (!_list_->is_sorted && _list_->count > 1)
    {
        CMC_(PFX, _impl_sort_quicksort)
        (_list_->buffer, _list_->f_val->cmp, 0, _list_->count - 1);

        _list_->is_sorted = true;
    }
}

struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct SNAME *result = CMC_(PFX, _new_custom)(_list_->capacity, _list_->f_val, _list_->alloc, NULL);

    if (!result)
    {
        _list_->flag = CMC_FLAG_ERROR;
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
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (_list1_->count != _list2_->count)
        return false;

    CMC_(PFX, _sort)(_list1_);
    CMC_(PFX, _sort)(_list2_);

    for (size_t i = 0; i < _list1_->count; i++)
    {
        if (_list1_->f_val->cmp(_list1_->buffer[i], _list2_->buffer[i]) != 0)
            return false;
    }

    return true;
}

static size_t CMC_(PFX, _impl_binary_search_first)(struct SNAME *_list_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_list_))
        return 1;

    size_t L = 0;
    size_t R = _list_->count;

    while (L < R)
    {
        size_t M = L + (R - L) / 2;

        if (_list_->f_val->cmp(_list_->buffer[M], value) < 0)
            L = M + 1;
        else
            R = M;
    }

    if (_list_->f_val->cmp(_list_->buffer[L], value) == 0)
        return L;

    /* Not found */
    return _list_->count;
}

static size_t CMC_(PFX, _impl_binary_search_last)(struct SNAME *_list_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_list_))
        return 1;

    size_t L = 0;
    size_t R = _list_->count;

    while (L < R)
    {
        size_t M = L + (R - L) / 2;

        if (_list_->f_val->cmp(_list_->buffer[M], value) > 0)
            R = M;
        else
            L = M + 1;
    }

    if (L > 0 && _list_->f_val->cmp(_list_->buffer[L - 1], value) == 0)
        return L - 1;

    /* Not found */
    return _list_->count;
}

/* Characteristics of this quicksort implementation: */
/* - Hybrid: uses insertion sort for small arrays */
/* - Partition: Lomuto's Method */
/* - Tail recursion: minimize recursion depth */
void CMC_(PFX, _impl_sort_quicksort)(V *array, int (*cmp)(V, V), size_t low, size_t high)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    while (low < high)
    {
        /* Quicksort performs poorly for smaller arrays so let */
        /* insertion sort do the job */
        if (high - low < 10)
        {
            CMC_(PFX, _impl_sort_insertion)(array, cmp, low, high);
            break;
        }
        else
        {
            /* Partition */
            V pivot = array[high];

            size_t pindex = low;

            for (size_t i = low; i < high; i++)
            {
                if (cmp(array[i], pivot) <= 0)
                {
                    V _tmp_ = array[i];
                    array[i] = array[pindex];
                    array[pindex] = _tmp_;

                    pindex++;
                }
            }

            V _tmp_ = array[pindex];
            array[pindex] = array[high];
            array[high] = _tmp_;

            /* Tail recursion */
            if (pindex - low < high - pindex)
            {
                CMC_(PFX, _impl_sort_quicksort)
                (array, cmp, low, pindex - 1);

                low = pindex + 1;
            }
            else
            {
                CMC_(PFX, _impl_sort_quicksort)
                (array, cmp, pindex + 1, high);

                high = pindex - 1;
            }
        }
    }
}

void CMC_(PFX, _impl_sort_insertion)(V *array, int (*cmp)(V, V), size_t low, size_t high)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    for (size_t i = low + 1; i <= high; i++)
    {
        V _tmp_ = array[i];
        size_t j = i;

        while (j > low && cmp(array[j - 1], _tmp_) > 0)
        {
            array[j] = array[j - 1];
            j--;
        }

        array[j] = _tmp_;
    }
}
