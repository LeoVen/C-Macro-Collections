#include "cmc/list.h"

struct list
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    int flag;
    struct list_fval *f_val;
    struct list_iter (*it_start)(struct list *);
    struct list_iter (*it_end)(struct list *);
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct list_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct list_iter
{
    struct list *target;
    size_t cursor;
    _Bool start;
    _Bool end;
};
struct list *l_new(size_t capacity, struct list_fval *f_val);
struct list *l_new_custom(size_t capacity, struct list_fval *f_val,
                          struct cmc_alloc_node *alloc,
                          struct cmc_callbacks *callbacks);
void l_clear(struct list *_list_);
void l_free(struct list *_list_);
void l_customize(struct list *_list_, struct cmc_alloc_node *alloc,
                 struct cmc_callbacks *callbacks);
_Bool l_push_front(struct list *_list_, size_t value);
_Bool l_push_at(struct list *_list_, size_t value, size_t index);
_Bool l_push_back(struct list *_list_, size_t value);
_Bool l_pop_front(struct list *_list_);
_Bool l_pop_at(struct list *_list_, size_t index);
_Bool l_pop_back(struct list *_list_);
_Bool l_seq_push_front(struct list *_list_, size_t *values, size_t size);
_Bool l_seq_push_at(struct list *_list_, size_t *values, size_t size,
                    size_t index);
_Bool l_seq_push_back(struct list *_list_, size_t *values, size_t size);
_Bool l_seq_pop_at(struct list *_list_, size_t from, size_t to);
struct list *l_seq_sublist(struct list *_list_, size_t from, size_t to);
size_t l_front(struct list *_list_);
size_t l_get(struct list *_list_, size_t index);
size_t *l_get_ref(struct list *_list_, size_t index);
size_t l_back(struct list *_list_);
size_t l_index_of(struct list *_list_, size_t value, _Bool from_start);
_Bool l_contains(struct list *_list_, size_t value);
_Bool l_empty(struct list *_list_);
_Bool l_full(struct list *_list_);
size_t l_count(struct list *_list_);
_Bool l_fits(struct list *_list_, size_t size);
size_t l_capacity(struct list *_list_);
int l_flag(struct list *_list_);
_Bool l_resize(struct list *_list_, size_t capacity);
struct list *l_copy_of(struct list *_list_);
_Bool l_equals(struct list *_list1_, struct list *_list2_);
struct cmc_string l_to_string(struct list *_list_);
_Bool l_print(struct list *_list_, FILE *fptr);
struct list_iter *l_iter_new(struct list *target);
void l_iter_free(struct list_iter *iter);
void l_iter_init(struct list_iter *iter, struct list *target);
_Bool l_iter_start(struct list_iter *iter);
_Bool l_iter_end(struct list_iter *iter);
void l_iter_to_start(struct list_iter *iter);
void l_iter_to_end(struct list_iter *iter);
_Bool l_iter_next(struct list_iter *iter);
_Bool l_iter_prev(struct list_iter *iter);
_Bool l_iter_advance(struct list_iter *iter, size_t steps);
_Bool l_iter_rewind(struct list_iter *iter, size_t steps);
_Bool l_iter_go_to(struct list_iter *iter, size_t index);
size_t l_iter_value(struct list_iter *iter);
size_t *l_iter_rvalue(struct list_iter *iter);
size_t l_iter_index(struct list_iter *iter);
static struct list_iter l_impl_it_start(struct list *_list_);
static struct list_iter l_impl_it_end(struct list *_list_);
struct list *l_new(size_t capacity, struct list_fval *f_val)
{
    struct cmc_alloc_node *alloc = &cmc_alloc_node_default;
    if (capacity < 1)
        return ((void *)0);
    struct list *_list_ = alloc->malloc(sizeof(struct list));
    if (!_list_)
        return ((void *)0);
    _list_->buffer = alloc->calloc(capacity, sizeof(size_t));
    if (!_list_->buffer)
    {
        alloc->free(_list_);
        return ((void *)0);
    }
    _list_->capacity = capacity;
    _list_->count = 0;
    _list_->flag = cmc_flags.OK;
    _list_->f_val = f_val;
    _list_->alloc = alloc;
    _list_->callbacks = ((void *)0);
    _list_->it_start = l_impl_it_start;
    _list_->it_end = l_impl_it_end;
    return _list_;
}
struct list *l_new_custom(size_t capacity, struct list_fval *f_val,
                          struct cmc_alloc_node *alloc,
                          struct cmc_callbacks *callbacks)
{
    if (capacity < 1)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct list *_list_ = alloc->malloc(sizeof(struct list));
    if (!_list_)
        return ((void *)0);
    _list_->buffer = alloc->calloc(capacity, sizeof(size_t));
    if (!_list_->buffer)
    {
        alloc->free(_list_);
        return ((void *)0);
    }
    _list_->capacity = capacity;
    _list_->count = 0;
    _list_->flag = cmc_flags.OK;
    _list_->f_val = f_val;
    _list_->alloc = alloc;
    _list_->callbacks = callbacks;
    _list_->it_start = l_impl_it_start;
    _list_->it_end = l_impl_it_end;
    return _list_;
}
void l_clear(struct list *_list_)
{
    if (_list_->f_val->free)
    {
        for (size_t i = 0; i < _list_->count; i++)
            _list_->f_val->free(_list_->buffer[i]);
    }
    memset(_list_->buffer, 0, sizeof(size_t) * _list_->capacity);
    _list_->count = 0;
    _list_->flag = cmc_flags.OK;
}
void l_free(struct list *_list_)
{
    if (_list_->f_val->free)
    {
        for (size_t i = 0; i < _list_->count; i++)
            _list_->f_val->free(_list_->buffer[i]);
    }
    _list_->alloc->free(_list_->buffer);
    _list_->alloc->free(_list_);
}
void l_customize(struct list *_list_, struct cmc_alloc_node *alloc,
                 struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _list_->alloc = &cmc_alloc_node_default;
    else
        _list_->alloc = alloc;
    _list_->callbacks = callbacks;
    _list_->flag = cmc_flags.OK;
}
_Bool l_push_front(struct list *_list_, size_t value)
{
    if (l_full(_list_))
    {
        if (!l_resize(_list_, _list_->count * 2))
            return 0;
    }
    if (!l_empty(_list_))
    {
        memmove(_list_->buffer + 1, _list_->buffer,
                _list_->count * sizeof(size_t));
    }
    _list_->buffer[0] = value;
    _list_->count++;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->create)
        _list_->callbacks->create();
    return 1;
}
_Bool l_push_at(struct list *_list_, size_t value, size_t index)
{
    if (index > _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return 0;
    }
    if (l_full(_list_))
    {
        if (!l_resize(_list_, _list_->count * 2))
            return 0;
    }
    memmove(_list_->buffer + index + 1, _list_->buffer + index,
            (_list_->count - index) * sizeof(size_t));
    _list_->buffer[index] = value;
    _list_->count++;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->create)
        _list_->callbacks->create();
    return 1;
}
_Bool l_push_back(struct list *_list_, size_t value)
{
    if (l_full(_list_))
    {
        if (!l_resize(_list_, _list_->count * 2))
            return 0;
    }
    _list_->buffer[_list_->count++] = value;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->create)
        _list_->callbacks->create();
    return 1;
}
_Bool l_pop_front(struct list *_list_)
{
    if (l_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return 0;
    }
    memmove(_list_->buffer, _list_->buffer + 1,
            (_list_->count - 1) * sizeof(size_t));
    _list_->buffer[--_list_->count] = (size_t){ 0 };
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->delete)
        _list_->callbacks->delete ();
    return 1;
}
_Bool l_pop_at(struct list *_list_, size_t index)
{
    if (l_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return 0;
    }
    if (index >= _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return 0;
    }
    memmove(_list_->buffer + index, _list_->buffer + index + 1,
            (_list_->count - index - 1) * sizeof(size_t));
    _list_->buffer[--_list_->count] = (size_t){ 0 };
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->delete)
        _list_->callbacks->delete ();
    return 1;
}
_Bool l_pop_back(struct list *_list_)
{
    if (l_empty(_list_))
        return 0;
    _list_->buffer[--_list_->count] = (size_t){ 0 };
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->delete)
        _list_->callbacks->delete ();
    return 1;
}
_Bool l_seq_push_front(struct list *_list_, size_t *values, size_t size)
{
    if (size == 0)
    {
        _list_->flag = cmc_flags.INVALID;
        return 0;
    }
    if (!l_fits(_list_, size))
    {
        if (!l_resize(_list_, _list_->count + size))
            return 0;
    }
    memmove(_list_->buffer + size, _list_->buffer,
            _list_->count * sizeof(size_t));
    memcpy(_list_->buffer, values, size * sizeof(size_t));
    _list_->count += size;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->create)
        _list_->callbacks->create();
    return 1;
}
_Bool l_seq_push_at(struct list *_list_, size_t *values, size_t size,
                    size_t index)
{
    if (size == 0)
    {
        _list_->flag = cmc_flags.INVALID;
        return 0;
    }
    if (index > _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return 0;
    }
    if (index == 0)
        return l_seq_push_front(_list_, values, size);
    else if (index == _list_->count)
        return l_seq_push_back(_list_, values, size);
    if (!l_fits(_list_, size))
    {
        if (!l_resize(_list_, _list_->count + size))
            return 0;
    }
    memmove(_list_->buffer + index + size, _list_->buffer + index,
            (_list_->count - index) * sizeof(size_t));
    memcpy(_list_->buffer + index, values, size * sizeof(size_t));
    _list_->count += size;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->create)
        _list_->callbacks->create();
    return 1;
}
_Bool l_seq_push_back(struct list *_list_, size_t *values, size_t size)
{
    if (size == 0)
    {
        _list_->flag = cmc_flags.INVALID;
        return 0;
    }
    if (!l_fits(_list_, size))
    {
        if (!l_resize(_list_, _list_->count + size))
            return 0;
    }
    memcpy(_list_->buffer + _list_->count, values, size * sizeof(size_t));
    _list_->count += size;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->create)
        _list_->callbacks->create();
    return 1;
}
_Bool l_seq_pop_at(struct list *_list_, size_t from, size_t to)
{
    if (from > to)
    {
        _list_->flag = cmc_flags.INVALID;
        return 0;
    }
    if (to >= _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return 0;
    }
    size_t length = (to - from + 1);
    memmove(_list_->buffer + from, _list_->buffer + to + 1,
            (_list_->count - to - 1) * sizeof(size_t));
    memset(_list_->buffer + _list_->count - length, 0, length * sizeof(size_t));
    _list_->count -= to - from + 1;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->delete)
        _list_->callbacks->delete ();
    return 1;
}
struct list *l_seq_sublist(struct list *_list_, size_t from, size_t to)
{
    if (from > to)
    {
        _list_->flag = cmc_flags.INVALID;
        return ((void *)0);
    }
    if (to >= _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return ((void *)0);
    }
    size_t length = to - from + 1;
    struct list *result =
        l_new_custom(length, _list_->f_val, _list_->alloc, _list_->callbacks);
    if (!result)
    {
        _list_->flag = cmc_flags.ALLOC;
        return ((void *)0);
    }
    memcpy(result->buffer, _list_->buffer, length * sizeof(size_t));
    memmove(_list_->buffer + from, _list_->buffer + to + 1,
            (_list_->count - to - 1) * sizeof(size_t));
    memset(_list_->buffer + _list_->count - length, 0, length * sizeof(size_t));
    _list_->count -= length;
    result->count = length;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->delete)
        _list_->callbacks->delete ();
    return result;
}
size_t l_front(struct list *_list_)
{
    if (l_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return (size_t){ 0 };
    }
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->read)
        _list_->callbacks->read();
    return _list_->buffer[0];
}
size_t l_get(struct list *_list_, size_t index)
{
    if (l_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return (size_t){ 0 };
    }
    if (index >= _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return (size_t){ 0 };
    }
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->read)
        _list_->callbacks->read();
    return _list_->buffer[index];
}
size_t *l_get_ref(struct list *_list_, size_t index)
{
    if (l_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return ((void *)0);
    }
    if (index >= _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return ((void *)0);
    }
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->read)
        _list_->callbacks->read();
    return &(_list_->buffer[index]);
}
size_t l_back(struct list *_list_)
{
    if (l_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return (size_t){ 0 };
    }
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->read)
        _list_->callbacks->read();
    return _list_->buffer[_list_->count - 1];
}
size_t l_index_of(struct list *_list_, size_t value, _Bool from_start)
{
    _list_->flag = cmc_flags.OK;
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
    if (_list_->callbacks && _list_->callbacks->read)
        _list_->callbacks->read();
    return result;
}
_Bool l_contains(struct list *_list_, size_t value)
{
    _list_->flag = cmc_flags.OK;
    _Bool result = 0;
    for (size_t i = 0; i < _list_->count; i++)
    {
        if (_list_->f_val->cmp(_list_->buffer[i], value) == 0)
        {
            result = 1;
            break;
        }
    }
    if (_list_->callbacks && _list_->callbacks->read)
        _list_->callbacks->read();
    return result;
}
_Bool l_empty(struct list *_list_)
{
    return _list_->count == 0;
}
_Bool l_full(struct list *_list_)
{
    return _list_->count >= _list_->capacity;
}
size_t l_count(struct list *_list_)
{
    return _list_->count;
}
_Bool l_fits(struct list *_list_, size_t size)
{
    return _list_->count + size <= _list_->capacity;
}
size_t l_capacity(struct list *_list_)
{
    return _list_->capacity;
}
int l_flag(struct list *_list_)
{
    return _list_->flag;
}
_Bool l_resize(struct list *_list_, size_t capacity)
{
    _list_->flag = cmc_flags.OK;
    if (_list_->capacity == capacity)
        return 1;
    if (capacity < _list_->count)
    {
        _list_->flag = cmc_flags.INVALID;
        return 0;
    }
    size_t *new_buffer =
        _list_->alloc->realloc(_list_->buffer, sizeof(size_t) * capacity);
    if (!new_buffer)
    {
        _list_->flag = cmc_flags.ALLOC;
        return 0;
    }
    _list_->buffer = new_buffer;
    _list_->capacity = capacity;
    if (_list_->callbacks && _list_->callbacks->resize)
        _list_->callbacks->resize();
    return 1;
}
struct list *l_copy_of(struct list *_list_)
{
    struct list *result = l_new_custom(_list_->capacity, _list_->f_val,
                                       _list_->alloc, _list_->callbacks);
    if (!result)
    {
        _list_->flag = cmc_flags.ALLOC;
        return ((void *)0);
    }
    if (_list_->f_val->cpy)
    {
        for (size_t i = 0; i < _list_->count; i++)
            result->buffer[i] = _list_->f_val->cpy(_list_->buffer[i]);
    }
    else
        memcpy(result->buffer, _list_->buffer, sizeof(size_t) * _list_->count);
    result->count = _list_->count;
    _list_->flag = cmc_flags.OK;
    return result;
}
_Bool l_equals(struct list *_list1_, struct list *_list2_)
{
    _list1_->flag = cmc_flags.OK;
    _list2_->flag = cmc_flags.OK;
    if (_list1_->count != _list2_->count)
        return 0;
    for (size_t i = 0; i < _list1_->count; i++)
    {
        if (_list1_->f_val->cmp(_list1_->buffer[i], _list2_->buffer[i]) != 0)
            return 0;
    }
    return 1;
}
struct cmc_string l_to_string(struct list *_list_)
{
    struct cmc_string str;
    struct list *l_ = _list_;
    int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_list, "list",
                     "size_t", l_, l_->buffer, l_->capacity, l_->count,
                     l_->flag, l_->f_val, l_->alloc, l_->callbacks);
    return n >= 0 ? str : (struct cmc_string){ 0 };
}
_Bool l_print(struct list *_list_, FILE *fptr)
{
    for (size_t i = 0; i < _list_->count; i++)
    {
        if (!_list_->f_val->str(fptr, _list_->buffer[i]))
            return 0;
    }
    return 1;
}
struct list_iter *l_iter_new(struct list *target)
{
    struct list_iter *iter = target->alloc->malloc(sizeof(struct list_iter));
    if (!iter)
        return ((void *)0);
    l_iter_init(iter, target);
    return iter;
}
void l_iter_free(struct list_iter *iter)
{
    iter->target->alloc->free(iter);
}
void l_iter_init(struct list_iter *iter, struct list *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->start = 1;
    iter->end = l_empty(target);
}
_Bool l_iter_start(struct list_iter *iter)
{
    return l_empty(iter->target) || iter->start;
}
_Bool l_iter_end(struct list_iter *iter)
{
    return l_empty(iter->target) || iter->end;
}
void l_iter_to_start(struct list_iter *iter)
{
    if (!l_empty(iter->target))
    {
        iter->cursor = 0;
        iter->start = 1;
        iter->end = l_empty(iter->target);
    }
}
void l_iter_to_end(struct list_iter *iter)
{
    if (!l_empty(iter->target))
    {
        iter->start = l_empty(iter->target);
        iter->cursor = l_empty(iter->target) ? 0 : iter->target->count - 1;
        iter->end = 1;
    }
}
_Bool l_iter_next(struct list_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = l_empty(iter->target);
    iter->cursor++;
    return 1;
}
_Bool l_iter_prev(struct list_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = l_empty(iter->target);
    iter->cursor--;
    return 1;
}
_Bool l_iter_advance(struct list_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor + steps >= iter->target->count)
        return 0;
    iter->start = l_empty(iter->target);
    iter->cursor += steps;
    return 1;
}
_Bool l_iter_rewind(struct list_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor < steps)
        return 0;
    iter->end = l_empty(iter->target);
    iter->cursor -= steps;
    return 1;
}
_Bool l_iter_go_to(struct list_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->cursor > index)
        return l_iter_rewind(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return l_iter_advance(iter, index - iter->cursor);
    return 1;
}
size_t l_iter_value(struct list_iter *iter)
{
    if (l_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor];
}
size_t *l_iter_rvalue(struct list_iter *iter)
{
    if (l_empty(iter->target))
        return ((void *)0);
    return &(iter->target->buffer[iter->cursor]);
}
size_t l_iter_index(struct list_iter *iter)
{
    return iter->cursor;
}
static struct list_iter l_impl_it_start(struct list *_list_)
{
    struct list_iter iter;
    l_iter_init(&iter, _list_);
    l_iter_to_start(&iter);
    return iter;
}
static struct list_iter l_impl_it_end(struct list *_list_)
{
    struct list_iter iter;
    l_iter_init(&iter, _list_);
    l_iter_to_end(&iter);
    return iter;
}
