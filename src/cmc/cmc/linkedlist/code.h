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
struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_get_node)(struct SNAME *_list_, size_t index);
struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_new_node)(struct SNAME *_list_, V value);

struct SNAME *CMC_(PFX, _new)(struct CMC_DEF_FVAL(SNAME) * f_val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _new_custom)(f_val, NULL, NULL);
}

struct SNAME *CMC_(PFX, _new_custom)(struct CMC_DEF_FVAL(SNAME) * f_val, CMC_ALLOC_TYPE alloc,
                                     CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (!f_val)
        return NULL;

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    struct SNAME *_list_ = alloc->malloc(sizeof(struct SNAME));

    if (!_list_)
        return NULL;

    _list_->count = 0;
    _list_->head = NULL;
    _list_->tail = NULL;
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

    struct CMC_DEF_NODE(SNAME) *scan = _list_->head;

    while (_list_->head != NULL)
    {
        _list_->head = _list_->head->next;

        if (_list_->f_val->free)
        {
            _list_->f_val->free(scan->value);
        }

        _list_->alloc->free(scan);

        scan = _list_->head;
    }

    _list_->count = 0;
    _list_->head = NULL;
    _list_->tail = NULL;
    _list_->flag = CMC_FLAG_OK;
}

void CMC_(PFX, _free)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_(PFX, _clear)(_list_);

    _list_->alloc->free(_list_);
}

void CMC_(PFX, _customize)(struct SNAME *_list_, CMC_ALLOC_TYPE alloc, CMC_CALLBACK_TYPE callbacks)
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

bool CMC_(PFX, _push_front)(struct SNAME *_list_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_NODE(SNAME) *_node_ = CMC_(PFX, _impl_new_node)(_list_, value);

    if (!_node_)
        return false;

    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->head = _node_;
        _list_->tail = _node_;
    }
    else
    {
        _node_->next = _list_->head;
        _list_->head->prev = _node_;
        _list_->head = _node_;
    }

    _list_->count++;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return true;
}

bool CMC_(PFX, _push_at)(struct SNAME *_list_, V value, size_t index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (index > _list_->count)
    {
        _list_->flag = CMC_FLAG_RANGE;
        return false;
    }

    if (index == 0)
    {
        return CMC_(PFX, _push_front)(_list_, value);
    }
    else if (index == _list_->count)
    {
        return CMC_(PFX, _push_back)(_list_, value);
    }

    struct CMC_DEF_NODE(SNAME) *_node_ = CMC_(PFX, _impl_new_node)(_list_, value);

    if (!_node_)
        return false;

    struct CMC_DEF_NODE(SNAME) *scan = CMC_(PFX, _impl_get_node)(_list_, index - 1);

    _node_->next = scan->next;
    _node_->prev = scan;
    _node_->next->prev = _node_;
    _node_->prev->next = _node_;

    _list_->count++;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return true;
}

bool CMC_(PFX, _push_back)(struct SNAME *_list_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_NODE(SNAME) *_node_ = CMC_(PFX, _impl_new_node)(_list_, value);

    if (!_node_)
        return false;

    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->head = _node_;
        _list_->tail = _node_;
    }
    else
    {
        _node_->prev = _list_->tail;
        _list_->tail->next = _node_;
        _list_->tail = _node_;
    }

    _list_->count++;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return true;
}

bool CMC_(PFX, _pop_front)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    struct CMC_DEF_NODE(SNAME) *_node_ = _list_->head;
    _list_->head = _list_->head->next;

    _list_->alloc->free(_node_);

    if (_list_->head == NULL)
        _list_->tail = NULL;
    else
        _list_->head->prev = NULL;

    _list_->count--;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return true;
}

bool CMC_(PFX, _pop_at)(struct SNAME *_list_, size_t index)
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

    if (index == 0)
    {
        return CMC_(PFX, _pop_front)(_list_);
    }
    else if (index == _list_->count - 1)
    {
        return CMC_(PFX, _pop_back)(_list_);
    }

    struct CMC_DEF_NODE(SNAME) *_node_ = CMC_(PFX, _impl_get_node)(_list_, index);

    if (!_node_)
        return false;

    _node_->next->prev = _node_->prev;
    _node_->prev->next = _node_->next;

    _list_->alloc->free(_node_);

    _list_->count--;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return true;
}

bool CMC_(PFX, _pop_back)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    struct CMC_DEF_NODE(SNAME) *_node_ = _list_->tail;
    _list_->tail = _list_->tail->prev;

    _list_->alloc->free(_node_);

    if (_list_->tail == NULL)
        _list_->head = NULL;
    else
        _list_->tail->next = NULL;

    _list_->count--;
    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return true;
}

V CMC_(PFX, _front)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return _list_->head->value;
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

    struct CMC_DEF_NODE(SNAME) *scan = CMC_(PFX, _impl_get_node)(_list_, index);

    if (scan == NULL)
        return (V){ 0 };

    CMC_CALLBACKS_CALL(_list_);

    return scan->value;
}

V *CMC_(PFX, _get_ref)(struct SNAME *_list_, size_t index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

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

    struct CMC_DEF_NODE(SNAME) *scan = CMC_(PFX, _impl_get_node)(_list_, index);

    if (scan == NULL)
        return NULL;

    CMC_CALLBACKS_CALL(_list_);

    return &(scan->value);
}

V CMC_(PFX, _back)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_list_))
    {
        _list_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    _list_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_list_);

    return _list_->tail->value;
}

bool CMC_(PFX, _contains)(struct SNAME *_list_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _list_->flag = CMC_FLAG_OK;

    bool result = false;

    struct CMC_DEF_NODE(SNAME) *scan = _list_->head;

    while (scan != NULL)
    {
        if (_list_->f_val->cmp(scan->value, value) == 0)
        {
            result = true;
            break;
        }

        scan = scan->next;
    }

    CMC_CALLBACKS_CALL(_list_);

    return result;
}

bool CMC_(PFX, _empty)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _list_->count == 0;
}

size_t CMC_(PFX, _count)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _list_->count;
}

int CMC_(PFX, _flag)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _list_->flag;
}

struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_list_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct SNAME *result = CMC_(PFX, _new_custom)(_list_->f_val, _list_->alloc, NULL);

    if (!result)
        return NULL;

    CMC_CALLBACKS_ASSIGN(result, _list_->callbacks);

    struct CMC_DEF_NODE(SNAME) *scan = _list_->head;

    while (scan != NULL)
    {
        /* This allocation should never fail since it might not be */
        /* possible to recover from it. That is why it isn't checked */
        struct CMC_DEF_NODE(SNAME) * new_node;

        if (_list_->f_val->cpy)
            new_node = CMC_(PFX, _impl_new_node)(_list_, _list_->f_val->cpy(scan->value));
        else
            new_node = CMC_(PFX, _impl_new_node)(_list_, scan->value);

        if (!result->head)
        {
            result->head = new_node;
            result->tail = new_node;
        }
        else
        {
            new_node->prev = result->tail;
            result->tail->next = new_node;
            result->tail = new_node;
        }

        scan = scan->next;
    }

    result->count = _list_->count;

    _list_->flag = CMC_FLAG_OK;

    return result;
}

bool CMC_(PFX, _equals)(struct SNAME *_list1_, struct SNAME *_list2_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _list1_->flag = CMC_FLAG_OK;
    _list2_->flag = CMC_FLAG_OK;

    if (_list1_->count != _list2_->count)
        return false;

    struct CMC_DEF_NODE(SNAME) *scan1 = _list1_->head;
    struct CMC_DEF_NODE(SNAME) *scan2 = _list2_->head;

    while (scan1 != NULL && scan2 != NULL)
    {
        if (_list1_->f_val->cmp(scan1->value, scan2->value) != 0)
            return false;

        scan1 = scan1->next;
        scan2 = scan2->next;
    }

    return true;
}

struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_new_node)(struct SNAME *_list_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_NODE(SNAME) *_node_ = _list_->alloc->malloc(sizeof(struct CMC_DEF_NODE(SNAME)));

    if (!_node_)
    {
        _list_->flag = CMC_FLAG_ALLOC;
        return NULL;
    }

    _node_->value = value;
    _node_->next = NULL;
    _node_->prev = NULL;

    return _node_;
}

struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_get_node)(struct SNAME *_list_, size_t index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

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

    struct CMC_DEF_NODE(SNAME) *_node_ = NULL;

    if (index <= _list_->count / 2)
    {
        _node_ = _list_->head;
        for (size_t i = 0; i < index; i++)
        {
            _node_ = _node_->next;
        }
    }
    else
    {
        _node_ = _list_->tail;
        for (size_t i = _list_->count - 1; i > index; i--)
        {
            _node_ = _node_->prev;
        }
    }

    _list_->flag = CMC_FLAG_OK;

    return _node_;
}
