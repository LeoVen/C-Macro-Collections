#include "macro_collections.h"

typedef struct list_s
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    struct list_iter_s (*it_start)(struct list_s *);
    struct list_iter_s (*it_end)(struct list_s *);
} list, *list_ptr;
typedef struct list_iter_s
{
    struct list_s *target;
    size_t cursor;
    bool start;
    bool end;
} list_iter, *list_iter_ptr;
list *l_new(size_t capacity);
list *l_new_from(size_t *elements, size_t size);
void l_clear(list *_list_);
void l_free(list *_list_);
bool l_push_front(list *_list_, size_t element);
bool l_push(list *_list_, size_t element, size_t index);
bool l_push_back(list *_list_, size_t element);
bool l_pop_front(list *_list_);
bool l_pop(list *_list_, size_t index);
bool l_pop_back(list *_list_);
bool l_push_if(list *_list_, size_t element, size_t index, bool condition);
bool l_pop_if(list *_list_, size_t index, bool condition);
bool l_prepend(list *_list_, size_t *elements, size_t size);
bool l_insert(list *_list_, size_t *elements, size_t size, size_t index);
bool l_append(list *_list_, size_t *elements, size_t size);
bool l_remove(list *_list_, size_t from, size_t to);
list *l_extract(list *_list_, size_t from, size_t to);
size_t l_front(list *_list_);
size_t l_get(list *_list_, size_t index);
size_t *l_get_ref(list *_list_, size_t index);
size_t l_back(list *_list_);
size_t l_indexof(list *_list_, size_t element, int (*comparator)(size_t, size_t), bool from_start);
bool l_contains(list *_list_, size_t element, int (*comparator)(size_t, size_t));
bool l_empty(list *_list_);
bool l_full(list *_list_);
size_t l_count(list *_list_);
bool l_fits(list *_list_, size_t size);
size_t l_capacity(list *_list_);
list_iter *l_iter_new(list *target);
void l_iter_free(list_iter *iter);
void l_iter_init(list_iter *iter, list *target);
bool l_iter_start(list_iter *iter);
bool l_iter_end(list_iter *iter);
void l_iter_to_start(list_iter *iter);
void l_iter_to_end(list_iter *iter);
bool l_iter_next(list_iter *iter);
bool l_iter_prev(list_iter *iter);
size_t l_iter_value(list_iter *iter);
size_t *l_iter_rvalue(list_iter *iter);
size_t l_iter_index(list_iter *iter);
static inline size_t l_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}

static bool l_impl_grow(list *_list_, size_t required);
static list_iter l_impl_it_start(list *_list_);
static list_iter l_impl_it_end(list *_list_);
list *l_new(size_t capacity)
{
    if (capacity < 1)
        return NULL;
    list *_list_ = malloc(sizeof(list));
    if (!_list_)
        return NULL;
    _list_->buffer = malloc(capacity * sizeof(size_t));
    if (!_list_->buffer)
    {
        free(_list_);
        return NULL;
    }
    memset(_list_->buffer, 0, capacity * sizeof(size_t));
    _list_->capacity = capacity;
    _list_->count = 0;
    _list_->it_start = l_impl_it_start;
    _list_->it_end = l_impl_it_end;
    return _list_;
}
list *l_new_from(size_t *elements, size_t size)
{
    if (size == 0)
        return NULL;
    list *_list_ = l_new(size + size / 2);
    if (!_list_)
        return NULL;
    memcpy(_list_->buffer, elements, size * sizeof(size_t));
    _list_->count = size;
    return _list_;
}
void l_clear(list *_list_)
{
    memset(_list_->buffer, 0, sizeof(size_t) * _list_->capacity);
    _list_->count = 0;
}
void l_free(list *_list_)
{
    free(_list_->buffer);
    free(_list_);
}
bool l_push_front(list *_list_, size_t element)
{
    if (l_full(_list_))
    {
        if (!l_impl_grow(_list_, _list_->count + 1))
            return false;
    }
    if (!l_empty(_list_))
    {
        memmove(_list_->buffer + 1, _list_->buffer, _list_->count * sizeof(size_t));
    }
    _list_->buffer[0] = element;
    _list_->count++;
    return true;
}
bool l_push(list *_list_, size_t element, size_t index)
{
    if (index > _list_->count)
        return false;
    if (index == 0)
    {
        return l_push_front(_list_, element);
    }
    else if (index == _list_->count)
    {
        return l_push_back(_list_, element);
    }
    if (l_full(_list_))
    {
        if (!l_impl_grow(_list_, _list_->count + 1))
            return false;
    }
    memmove(_list_->buffer + index + 1, _list_->buffer + index, (_list_->count - index) * sizeof(size_t));
    _list_->buffer[index] = element;
    _list_->count++;
    return true;
}
bool l_push_back(list *_list_, size_t element)
{
    if (l_full(_list_))
    {
        if (!l_impl_grow(_list_, _list_->count + 1))
            return false;
    }
    _list_->buffer[_list_->count++] = element;
    return true;
}
bool l_pop_front(list *_list_)
{
    if (l_empty(_list_))
        return false;
    memmove(_list_->buffer, _list_->buffer + 1, _list_->count * sizeof(size_t));
    _list_->buffer[--_list_->count] = l_impl_default_value();
    return true;
}
bool l_pop(list *_list_, size_t index)
{
    if (l_empty(_list_))
        return false;
    if (index >= _list_->count)
        return false;
    if (index == 0)
    {
        return l_pop_front(_list_);
    }
    else if (index == _list_->count - 1)
    {
        return l_pop_back(_list_);
    }
    memmove(_list_->buffer + index, _list_->buffer + index + 1, (_list_->count - index) * sizeof(size_t));
    _list_->buffer[--_list_->count] = l_impl_default_value();
    return true;
}
bool l_pop_back(list *_list_)
{
    if (l_empty(_list_))
        return false;
    _list_->buffer[--_list_->count] = l_impl_default_value();
    return true;
}
bool l_push_if(list *_list_, size_t element, size_t index, bool condition)
{
    if (condition)
        return l_push(_list_, element, index);
    return false;
}
bool l_pop_if(list *_list_, size_t index, bool condition)
{
    if (condition)
        return l_pop(_list_, index);
    return false;
}
bool l_prepend(list *_list_, size_t *elements, size_t size)
{
    if (size == 0)
        return false;
    if (!l_fits(_list_, size))
    {
        if (!l_impl_grow(_list_, _list_->count + size))
            return false;
    }
    memmove(_list_->buffer + size, _list_->buffer, _list_->count * sizeof(size_t));
    memcpy(_list_->buffer, elements, size * sizeof(size_t));
    _list_->count += size;
    return true;
}
bool l_insert(list *_list_, size_t *elements, size_t size, size_t index)
{
    if (size == 0 || index > _list_->count)
        return false;
    if (index == 0)
        return l_prepend(_list_, elements, size);
    else if (index == _list_->count)
        return l_append(_list_, elements, size);
    else
    {
        if (!l_fits(_list_, size))
        {
            if (!l_impl_grow(_list_, _list_->count + size))
                return false;
        }
        memmove(_list_->buffer + index + size, _list_->buffer + index, (_list_->count - index) * sizeof(size_t));
        memcpy(_list_->buffer + index, elements, size * sizeof(size_t));
        _list_->count += size;
    }
    return true;
}
bool l_append(list *_list_, size_t *elements, size_t size)
{
    if (size == 0)
        return false;
    if (!l_fits(_list_, size))
    {
        if (!l_impl_grow(_list_, _list_->count + size))
            return false;
    }
    memcpy(_list_->buffer + _list_->count, elements, size * sizeof(size_t));
    _list_->count += size;
    return true;
}
bool l_remove(list *_list_, size_t from, size_t to)
{
    if (from > to || to >= _list_->count)
        return false;
    size_t length = (to - from + 1);
    memmove(_list_->buffer + from, _list_->buffer + to + 1, (_list_->count - to - 1) * sizeof(size_t));
    memset(_list_->buffer + _list_->count - length, 0, length * sizeof(size_t));
    _list_->count -= to - from + 1;
    return true;
}
list *l_extract(list *_list_, size_t from, size_t to)
{
    if (from > to || to >= _list_->count)
        return false;
    size_t length = to - from + 1;
    list *result = l_new_from(_list_->buffer + from, length);
    if (!result)
        return NULL;
    memmove(_list_->buffer + from, _list_->buffer + to + 1, (_list_->count - to - 1) * sizeof(size_t));
    memset(_list_->buffer + _list_->count - length, 0, length * sizeof(size_t));
    _list_->count -= length;
    return result;
}
size_t l_front(list *_list_)
{
    if (l_empty(_list_))
        return l_impl_default_value();
    return _list_->buffer[0];
}
size_t l_get(list *_list_, size_t index)
{
    if (index >= _list_->count || l_empty(_list_))
        return l_impl_default_value();
    return _list_->buffer[index];
}
size_t *l_get_ref(list *_list_, size_t index)
{
    if (index >= _list_->count)
        return NULL;
    if (l_empty(_list_))
        return NULL;
    return &(_list_->buffer[index]);
}
size_t l_back(list *_list_)
{
    if (l_empty(_list_))
        return l_impl_default_value();
    return _list_->buffer[_list_->count - 1];
}
size_t l_indexof(list *_list_, size_t element, int (*comparator)(size_t, size_t), bool from_start)
{
    if (from_start)
    {
        for (size_t i = 0; i < _list_->count; i++)
        {
            if (comparator(_list_->buffer[i], element) == 0)
                return i;
        }
    }
    else
    {
        for (size_t i = _list_->count; i > 0; i--)
        {
            if (comparator(_list_->buffer[i - 1], element) == 0)
                return i - 1;
        }
    }
    return _list_->count;
}
bool l_contains(list *_list_, size_t element, int (*comparator)(size_t, size_t))
{
    for (size_t i = 0; i < _list_->count; i++)
    {
        if (comparator(_list_->buffer[i], element) == 0)
            return true;
    }
    return false;
}
bool l_empty(list *_list_) { return _list_->count == 0; }
bool l_full(list *_list_) { return _list_->count >= _list_->capacity; }
size_t l_count(list *_list_) { return _list_->count; }
bool l_fits(list *_list_, size_t size) { return _list_->count + size <= _list_->capacity; }
size_t l_capacity(list *_list_) { return _list_->capacity; }
list_iter *l_iter_new(list *target)
{
    list_iter *iter = malloc(sizeof(list_iter));
    if (!iter)
        return NULL;
    l_iter_init(iter, target);
    return iter;
}
void l_iter_free(list_iter *iter) { free(iter); }
void l_iter_init(list_iter *iter, list *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->start = true;
    iter->end = l_empty(target);
}
bool l_iter_start(list_iter *iter) { return l_empty(iter->target) || iter->start; }
bool l_iter_end(list_iter *iter) { return l_empty(iter->target) || iter->end; }
void l_iter_to_start(list_iter *iter)
{
    iter->cursor = 0;
    iter->start = true;
    iter->end = l_empty(iter->target);
}
void l_iter_to_end(list_iter *iter)
{
    iter->start = l_empty(iter->target);
    iter->cursor = l_empty(iter->target) ? 0 : iter->target->count - 1;
    iter->end = true;
}
bool l_iter_next(list_iter *iter)
{
    if (iter->end)
        return false;
    iter->start = l_empty(iter->target);
    if (iter->cursor == iter->target->count - 1)
        iter->end = true;
    else
        iter->cursor++;
    return true;
}
bool l_iter_prev(list_iter *iter)
{
    if (iter->start)
        return false;
    iter->end = l_empty(iter->target);
    if (iter->cursor == 0)
        iter->start = true;
    else
        iter->cursor--;
    return true;
}
size_t l_iter_value(list_iter *iter)
{
    if (l_empty(iter->target))
        return l_impl_default_value();
    return iter->target->buffer[iter->cursor];
}
size_t *l_iter_rvalue(list_iter *iter)
{
    if (l_empty(iter->target))
        return NULL;
    return &(iter->target->buffer[iter->cursor]);
}
size_t l_iter_index(list_iter *iter) { return iter->cursor; }
static bool l_impl_grow(list *_list_, size_t required)
{
    size_t new_capacity = _list_->capacity * 2;
    if (new_capacity < required)
        new_capacity = required;
    size_t *new_buffer = realloc(_list_->buffer, sizeof(size_t) * new_capacity);
    if (!new_buffer)
        return false;
    _list_->buffer = new_buffer;
    _list_->capacity = new_capacity;
    return true;
}
static list_iter l_impl_it_start(list *_list_)
{
    list_iter iter;
    l_iter_init(&iter, _list_);
    l_iter_to_start(&iter);
    return iter;
}
static list_iter l_impl_it_end(list *_list_)
{
    list_iter iter;
    l_iter_init(&iter, _list_);
    l_iter_to_end(&iter);
    return iter;
}
