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
    iter.cursor = target->head;
    iter.index = 0;
    iter.start = true;
    iter.end = CMC_(PFX, _empty)(target);

    return iter;
}

struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME *target)
{
    struct CMC_DEF_ITER(SNAME) iter;

    iter.target = target;
    iter.cursor = target->tail;
    iter.index = 0;
    iter.start = CMC_(PFX, _empty)(target);
    iter.end = true;

    if (!CMC_(PFX, _empty)(target))
        iter.index = target->count - 1;

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
        iter->cursor = iter->target->head;
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
        iter->cursor = iter->target->tail;
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

    if (iter->cursor->next == NULL)
    {
        iter->end = true;
        return false;
    }

    iter->start = CMC_(PFX, _empty)(iter->target);

    iter->cursor = iter->cursor->next;
    iter->index++;

    return true;
}

bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (iter->start)
        return false;

    if (iter->cursor->prev == NULL)
    {
        iter->start = true;
        return false;
    }

    iter->end = CMC_(PFX, _empty)(iter->target);

    iter->cursor = iter->cursor->prev;
    iter->index--;

    return true;
}

/* Returns true only if the iterator moved */
bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps)
{
    if (iter->end)
        return false;

    if (iter->cursor->next == NULL)
    {
        iter->end = true;
        return false;
    }

    if (steps == 0 || iter->index + steps >= iter->target->count)
        return false;

    iter->start = CMC_(PFX, _empty)(iter->target);

    iter->index += steps;

    for (size_t i = 0; i < steps; i++)
        iter->cursor = iter->cursor->next;

    return true;
}

/* Returns true only if the iterator moved */
bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps)
{
    if (iter->start)
        return false;

    if (iter->cursor->prev == NULL)
    {
        iter->start = true;
        return false;
    }

    if (steps == 0 || iter->index < steps)
        return false;

    iter->end = CMC_(PFX, _empty)(iter->target);

    iter->index -= steps;

    for (size_t i = 0; i < steps; i++)
        iter->cursor = iter->cursor->prev;

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

    return iter->cursor->value;
}

V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter)
{
    if (CMC_(PFX, _empty)(iter->target))
        return NULL;

    return &(iter->cursor->value);
}

size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)
{
    return iter->index;
}

struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _iter_node)(struct CMC_DEF_ITER(SNAME) * iter)
{
    return iter->cursor;
}

#endif /* CMC_EXT_ITER */

/**
 * NODE
 *
 * Operations relative to a linkedlist node
 */
#ifdef CMC_EXT_NODE

struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _new_node)(struct SNAME *_list_, V value)
{
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

void CMC_(PFX, _free_node)(struct SNAME *_list_, struct CMC_DEF_NODE(SNAME) * _node_)
{
    _list_->alloc->free(_node_);
}

struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _head)(struct SNAME *_list_)
{
    return _list_->head;
}

struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _get_node)(struct SNAME *_list_, size_t index)
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

struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _tail)(struct SNAME *_list_)
{
    return _list_->tail;
}

bool CMC_(PFX, _add_next)(struct SNAME *_owner_, struct CMC_DEF_NODE(SNAME) * _node_, V value)
{
    struct CMC_DEF_NODE(SNAME) *new_node = CMC_(PFX, _new_node)(_owner_, value);

    if (!new_node)
        return false;

    new_node->next = _node_->next;

    if (_node_->next != NULL)
        _node_->next->prev = new_node;
    else
        _owner_->tail = new_node;

    new_node->prev = _node_;
    _node_->next = new_node;

    _owner_->count++;
    _owner_->flag = CMC_FLAG_OK;

    return true;
}

bool CMC_(PFX, _add_prev)(struct SNAME *_owner_, struct CMC_DEF_NODE(SNAME) * _node_, V value)
{
    struct CMC_DEF_NODE(SNAME) *new_node = CMC_(PFX, _new_node)(_owner_, value);

    if (!new_node)
        return false;

    new_node->prev = _node_->prev;

    if (_node_->prev != NULL)
        _node_->prev->next = new_node;
    else
        _owner_->head = new_node;

    new_node->next = _node_;
    _node_->prev = new_node;

    _owner_->count++;
    _owner_->flag = CMC_FLAG_OK;

    return true;
}

bool CMC_(PFX, _del_next)(struct SNAME *_owner_, struct CMC_DEF_NODE(SNAME) * _node_)
{
    if (_node_->next == NULL)
    {
        _owner_->flag = CMC_FLAG_INVALID;
        return false;
    }

    struct CMC_DEF_NODE(SNAME) *tmp = _node_->next;

    _node_->next = _node_->next->next;

    if (tmp->next != NULL)
        tmp->next->prev = _node_;
    else
        _owner_->tail = _node_;

    _owner_->alloc->free(tmp);

    _owner_->count--;
    _owner_->flag = CMC_FLAG_OK;

    return true;
}

bool CMC_(PFX, _del_curr)(struct SNAME *_owner_, struct CMC_DEF_NODE(SNAME) * _node_)
{
    if (_node_->prev != NULL)
        _node_->prev->next = _node_->next;
    else
        _owner_->head = _node_->next;

    if (_node_->next != NULL)
        _node_->next->prev = _node_->prev;
    else
        _owner_->tail = _node_->prev;

    _owner_->alloc->free(_node_);

    _owner_->count--;
    _owner_->flag = CMC_FLAG_OK;

    return true;
}

bool CMC_(PFX, _del_prev)(struct SNAME *_owner_, struct CMC_DEF_NODE(SNAME) * _node_)
{
    if (_node_->prev == NULL)
    {
        _owner_->flag = CMC_FLAG_INVALID;
        return false;
    }

    struct CMC_DEF_NODE(SNAME) *tmp = _node_->prev;

    _node_->prev = _node_->prev->prev;

    if (tmp->prev != NULL)
        tmp->prev->next = _node_;
    else
        _owner_->head = _node_;

    _owner_->alloc->free(tmp);

    _owner_->count--;
    _owner_->flag = CMC_FLAG_OK;

    return true;
}

struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _next_node)(struct CMC_DEF_NODE(SNAME) * _node_)
{
    return _node_->next;
}

struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _prev_node)(struct CMC_DEF_NODE(SNAME) * _node_)
{
    return _node_->prev;
}

#endif /* CMC_EXT_NODE */

/**
 * STR
 *
 * Print helper functions.
 */
#ifdef CMC_EXT_STR

bool CMC_(PFX, _to_string)(struct SNAME *_list_, FILE *fptr)
{
    struct SNAME *l_ = _list_;

    return 0 <= fprintf(fptr,
                        "struct %s<%s> "
                        "at %p { "
                        "count:%" PRIuMAX ", "
                        "head:%p, "
                        "tail:%p, "
                        "flag:%d, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        CMC_TO_STRING(SNAME), CMC_TO_STRING(V), l_, l_->count, l_->head, l_->tail, l_->flag, l_->f_val,
                        l_->alloc, CMC_CALLBACKS_GET(l_));
}

bool CMC_(PFX, _print)(struct SNAME *_list_, FILE *fptr, const char *start, const char *separator, const char *end)
{
    fprintf(fptr, "%s", start);

    struct CMC_DEF_NODE(SNAME) *scan = _list_->head;

    while (scan != NULL)
    {
        if (!_list_->f_val->str(fptr, scan->value))
            return false;

        scan = scan->next;

        if (scan)
            fprintf(fptr, "%s", separator);
    }

    fprintf(fptr, "%s", end);

    return true;
}

#endif /* CMC_EXT_STR */
