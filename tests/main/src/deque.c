#include "macro_collections.h"

typedef struct deque_s
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    size_t front;
    size_t back;
    struct deque_iter_s (*it_start)(struct deque_s *);
    struct deque_iter_s (*it_end)(struct deque_s *);
} deque, *deque_ptr;
typedef struct deque_iter_s
{
    struct deque_s *target;
    size_t cursor;
    size_t index;
    _Bool start;
    _Bool end;
} deque_iter, *deque_iter_ptr;
deque *d_new(size_t capacity);
void d_clear(deque *_deque_);
void d_free(deque *_deque_);
_Bool d_push_front(deque *_deque_, size_t element);
_Bool d_push_back(deque *_deque_, size_t element);
_Bool d_pop_front(deque *_deque_);
_Bool d_pop_back(deque *_deque_);
_Bool d_push_front_if(deque *_deque_, size_t element, _Bool condition);
_Bool d_push_back_if(deque *_deque_, size_t element, _Bool condition);
_Bool d_pop_front_if(deque *_deque_, _Bool condition);
_Bool d_pop_back_if(deque *_deque_, _Bool condition);
size_t d_front(deque *_deque_);
size_t d_back(deque *_deque_);
_Bool d_contains(deque *_deque_, size_t element, int (*comparator)(size_t, size_t));
_Bool d_empty(deque *_deque_);
_Bool d_full(deque *_deque_);
size_t d_count(deque *_deque_);
size_t d_capacity(deque *_deque_);
deque_iter *d_iter_new(deque *target);
void d_iter_free(deque_iter *iter);
void d_iter_init(deque_iter *iter, deque *target);
_Bool d_iter_start(deque_iter *iter);
_Bool d_iter_end(deque_iter *iter);
void d_iter_to_start(deque_iter *iter);
void d_iter_to_end(deque_iter *iter);
_Bool d_iter_next(deque_iter *iter);
_Bool d_iter_prev(deque_iter *iter);
size_t d_iter_value(deque_iter *iter);
size_t *d_iter_rvalue(deque_iter *iter);
size_t d_iter_index(deque_iter *iter);
static inline size_t d_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
static _Bool d_impl_grow(deque *_deque_);
static deque_iter d_impl_it_start(deque *_deque_);
static deque_iter d_impl_it_end(deque *_deque_);
deque *d_new(size_t capacity)
{
    if (capacity < 1)
        return ((void *)0);
    deque *_deque_ = malloc(sizeof(deque));
    if (!_deque_)
        return ((void *)0);
    _deque_->buffer = malloc(sizeof(size_t) * capacity);
    if (!_deque_->buffer)
    {
        free(_deque_);
        return ((void *)0);
    }
    memset(_deque_->buffer, 0, sizeof(size_t) * capacity);
    _deque_->capacity = capacity;
    _deque_->count = 0;
    _deque_->front = 0;
    _deque_->back = 0;
    _deque_->it_start = d_impl_it_start;
    _deque_->it_end = d_impl_it_end;
    return _deque_;
}
void d_clear(deque *_deque_)
{
    memset(_deque_->buffer, 0, sizeof(size_t) * _deque_->capacity);
    _deque_->count = 0;
    _deque_->front = 0;
    _deque_->back = 0;
}
void d_free(deque *_deque_)
{
    free(_deque_->buffer);
    free(_deque_);
}
_Bool d_push_front(deque *_deque_, size_t element)
{
    if (d_full(_deque_))
    {
        if (!d_impl_grow(_deque_))
            return 0;
    }
    _deque_->front = (_deque_->front == 0) ? _deque_->capacity - 1 : _deque_->front - 1;
    _deque_->buffer[_deque_->front] = element;
    _deque_->count++;
    return 1;
}
_Bool d_push_back(deque *_deque_, size_t element)
{
    if (d_full(_deque_))
    {
        if (!d_impl_grow(_deque_))
            return 0;
    }
    _deque_->buffer[_deque_->back] = element;
    _deque_->back = (_deque_->back == _deque_->capacity - 1) ? 0 : _deque_->back + 1;
    _deque_->count++;
    return 1;
}
_Bool d_pop_front(deque *_deque_)
{
    if (d_empty(_deque_))
        return 0;
    _deque_->buffer[_deque_->front] = d_impl_default_value();
    _deque_->front = (_deque_->front == _deque_->capacity - 1) ? 0 : _deque_->front + 1;
    _deque_->count--;
    return 1;
}
_Bool d_pop_back(deque *_deque_)
{
    if (d_empty(_deque_))
        return 0;
    _deque_->back = (_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1;
    _deque_->buffer[_deque_->back] = d_impl_default_value();
    _deque_->count--;
    return 1;
}
_Bool d_push_front_if(deque *_deque_, size_t element, _Bool condition)
{
    if (condition)
        return d_push_front(_deque_, element);
    return 0;
}
_Bool d_push_back_if(deque *_deque_, size_t element, _Bool condition)
{
    if (condition)
        return d_push_back(_deque_, element);
    return 0;
}
_Bool d_pop_front_if(deque *_deque_, _Bool condition)
{
    if (condition)
        return d_pop_front(_deque_);
    return 0;
}
_Bool d_pop_back_if(deque *_deque_, _Bool condition)
{
    if (condition)
        return d_pop_back(_deque_);
    return 0;
}
size_t d_front(deque *_deque_)
{
    if (d_empty(_deque_))
        d_impl_default_value();
    return _deque_->buffer[_deque_->front];
}
size_t d_back(deque *_deque_)
{
    if (d_empty(_deque_))
        d_impl_default_value();
    return _deque_->buffer[(_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1];
}
_Bool d_contains(deque *_deque_, size_t element, int (*comparator)(size_t, size_t))
{
    for (size_t i = _deque_->front, j = 0; j < _deque_->count; i = (i + 1) % _deque_->count, j++)
    {
        if (comparator(_deque_->buffer[i], element) == 0)
            return 1;
    }
    return 0;
}
_Bool d_empty(deque *_deque_) { return _deque_->count == 0; }
_Bool d_full(deque *_deque_) { return _deque_->count >= _deque_->capacity; }
size_t d_count(deque *_deque_) { return _deque_->count; }
size_t d_capacity(deque *_deque_) { return _deque_->capacity; }
deque_iter *d_iter_new(deque *target)
{
    deque_iter *iter = malloc(sizeof(deque_iter));
    if (!iter)
        return ((void *)0);
    d_iter_init(iter, target);
    return iter;
}
void d_iter_free(deque_iter *iter) { free(iter); }
void d_iter_init(deque_iter *iter, deque *target)
{
    iter->target = target;
    iter->cursor = target->front;
    iter->index = 0;
    iter->start = 1;
    iter->end = d_empty(target);
}
_Bool d_iter_start(deque_iter *iter) { return d_empty(iter->target) || iter->start; }
_Bool d_iter_end(deque_iter *iter) { return d_empty(iter->target) || iter->end; }
void d_iter_to_start(deque_iter *iter)
{
    iter->cursor = iter->target->front;
    iter->index = 0;
    iter->start = 1;
    iter->end = d_empty(iter->target);
}
void d_iter_to_end(deque_iter *iter)
{
    if (d_empty(iter->target))
        iter->cursor = 0;
    else
        iter->cursor = (iter->target->back == 0) ? iter->target->capacity - 1 : iter->target->back - 1;
    iter->index = iter->target->count - 1;
    iter->start = d_empty(iter->target);
    iter->end = 1;
}
_Bool d_iter_next(deque_iter *iter)
{
    if (iter->end)
        return 0;
    iter->start = d_empty(iter->target);
    if (iter->index == iter->target->count - 1)
        iter->end = 1;
    else
    {
        iter->cursor = (iter->cursor + 1) % (iter->target->capacity);
        iter->index++;
    }
    return 1;
}
_Bool d_iter_prev(deque_iter *iter)
{
    if (iter->start)
        return 0;
    iter->end = d_empty(iter->target);
    if (iter->index == 0)
        iter->start = 1;
    else
    {
        iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1 : iter->cursor - 1;
        iter->index--;
    }
    return 1;
}
size_t d_iter_value(deque_iter *iter)
{
    if (d_empty(iter->target))
        d_impl_default_value();
    return iter->target->buffer[iter->cursor];
}
size_t *d_iter_rvalue(deque_iter *iter)
{
    if (d_empty(iter->target))
        return ((void *)0);
    return &(iter->target->buffer[iter->cursor]);
}
size_t d_iter_index(deque_iter *iter) { return iter->index; }
static _Bool d_impl_grow(deque *_deque_)
{
    size_t new_capacity = _deque_->capacity * 2;
    size_t *new_buffer = malloc(sizeof(size_t) * new_capacity);
    if (!new_buffer)
        return 0;
    for (size_t i = _deque_->front, j = 0; j < _deque_->count; i = (i + 1) % _deque_->capacity, j++)
    {
        new_buffer[j] = _deque_->buffer[i];
    }
    free(_deque_->buffer);
    _deque_->buffer = new_buffer;
    _deque_->capacity = new_capacity;
    _deque_->front = 0;
    _deque_->back = _deque_->count;
    return 1;
}
static deque_iter d_impl_it_start(deque *_deque_)
{
    deque_iter iter;
    d_iter_init(&iter, _deque_);
    return iter;
}
static deque_iter d_impl_it_end(deque *_deque_)
{
    deque_iter iter;
    d_iter_init(&iter, _deque_);
    d_iter_to_end(&iter);
    return iter;
}
