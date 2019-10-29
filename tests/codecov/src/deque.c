#include <cmc/deque.h>

//CMC_GENERATE_DEQUE(d, deque, size_t)

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
void d_clear(deque *_deque_, void (*deallocator)(size_t));
void d_free(deque *_deque_, void (*deallocator)(size_t));
_Bool d_push_front(deque *_deque_, size_t element);
_Bool d_push_back(deque *_deque_, size_t element);
_Bool d_pop_front(deque *_deque_);
_Bool d_pop_back(deque *_deque_);
size_t d_front(deque *_deque_);
size_t d_back(deque *_deque_);
_Bool d_contains(deque *_deque_, size_t element, int (*comparator)(size_t, size_t));
_Bool d_empty(deque *_deque_);
_Bool d_full(deque *_deque_);
size_t d_count(deque *_deque_);
size_t d_capacity(deque *_deque_);
_Bool d_resize(deque *_deque_, size_t capacity);
deque *d_copy_of(deque *_deque_, size_t (*copy_func)(size_t));
_Bool d_equals(deque *_deque1_, deque *_deque2_, int (*comparator)(size_t, size_t));
struct cmc_string d_to_string(deque *_deque_);
deque_iter *d_iter_new(deque *target);
void d_iter_free(deque_iter *iter);
void d_iter_init(deque_iter *iter, deque *target);
_Bool d_iter_start(deque_iter *iter);
_Bool d_iter_end(deque_iter *iter);
void d_iter_to_start(deque_iter *iter);
void d_iter_to_end(deque_iter *iter);
_Bool d_iter_next(deque_iter *iter);
_Bool d_iter_prev(deque_iter *iter);
_Bool d_iter_advance(deque_iter *iter, size_t steps);
_Bool d_iter_rewind(deque_iter *iter, size_t steps);
_Bool d_iter_go_to(deque_iter *iter, size_t index);
size_t d_iter_value(deque_iter *iter);
size_t *d_iter_rvalue(deque_iter *iter);
size_t d_iter_index(deque_iter *iter);
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
void d_clear(deque *_deque_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
        {
            deallocator(_deque_->buffer[i]);
            i = (i + 1) % _deque_->capacity;
        }
    }
    memset(_deque_->buffer, 0, sizeof(size_t) * _deque_->capacity);
    _deque_->count = 0;
    _deque_->front = 0;
    _deque_->back = 0;
}
void d_free(deque *_deque_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
        {
            deallocator(_deque_->buffer[i]);
            i = (i + 1) % _deque_->capacity;
        }
    }
    free(_deque_->buffer);
    free(_deque_);
}
_Bool d_push_front(deque *_deque_, size_t element)
{
    if (d_full(_deque_))
    {
        if (!d_resize(_deque_, d_capacity(_deque_) * 2))
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
        if (!d_resize(_deque_, d_capacity(_deque_) * 2))
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
    _deque_->buffer[_deque_->front] = (size_t){0};
    _deque_->front = (_deque_->front == _deque_->capacity - 1) ? 0 : _deque_->front + 1;
    _deque_->count--;
    return 1;
}
_Bool d_pop_back(deque *_deque_)
{
    if (d_empty(_deque_))
        return 0;
    _deque_->back = (_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1;
    _deque_->buffer[_deque_->back] = (size_t){0};
    _deque_->count--;
    return 1;
}
size_t d_front(deque *_deque_)
{
    if (d_empty(_deque_))
        return (size_t){0};
    return _deque_->buffer[_deque_->front];
}
size_t d_back(deque *_deque_)
{
    if (d_empty(_deque_))
        return (size_t){0};
    return _deque_->buffer[(_deque_->back == 0) ? _deque_->capacity - 1 : _deque_->back - 1];
}
_Bool d_contains(deque *_deque_, size_t element, int (*comparator)(size_t, size_t))
{
    for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
    {
        if (comparator(_deque_->buffer[i], element) == 0)
            return 1;
        i = (i + 1) % _deque_->capacity;
    }
    return 0;
}
_Bool d_empty(deque *_deque_) { return _deque_->count == 0; }
_Bool d_full(deque *_deque_) { return _deque_->count >= _deque_->capacity; }
size_t d_count(deque *_deque_) { return _deque_->count; }
size_t d_capacity(deque *_deque_) { return _deque_->capacity; }
_Bool d_resize(deque *_deque_, size_t capacity)
{
    if (d_capacity(_deque_) == capacity)
        return 1;
    if (capacity < d_count(_deque_))
        return 0;
    size_t *new_buffer = malloc(sizeof(size_t) * capacity);
    if (!new_buffer)
        return 0;
    for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
    {
        new_buffer[j] = _deque_->buffer[i];
        i = (i + 1) % d_capacity(_deque_);
    }
    free(_deque_->buffer);
    _deque_->buffer = new_buffer;
    _deque_->capacity = capacity;
    _deque_->front = 0;
    _deque_->back = _deque_->count;
    return 1;
}
deque *d_copy_of(deque *_deque_, size_t (*copy_func)(size_t))
{
    deque *result = d_new(_deque_->capacity);
    if (!result)
        return ((void *)0);
    if (copy_func)
    {
        for (size_t i = _deque_->front, j = 0; j < _deque_->count; j++)
        {
            result->buffer[j] = copy_func(_deque_->buffer[i]);
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
    return result;
}
_Bool d_equals(deque *_deque1_, deque *_deque2_, int (*comparator)(size_t, size_t))
{
    if (d_count(_deque1_) != d_count(_deque2_))
        return 0;
    size_t i, j, k;
    for (i = _deque1_->front, j = _deque2_->front, k = 0; k < d_count(_deque1_); k++)
    {
        if (comparator(_deque1_->buffer[i], _deque2_->buffer[j]) != 0)
            return 0;
        i = (i + 1) % _deque1_->capacity;
        j = (j + 1) % _deque2_->capacity;
    }
    return 1;
}
struct cmc_string d_to_string(deque *_deque_)
{
    struct cmc_string str;
    deque *d_ = _deque_;
    const char *name = "deque";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_deque, name, d_, d_->buffer, d_->capacity, d_->count, d_->front, d_->back);
    return str;
}
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
    {
        iter->cursor = 0;
        iter->index = 0;
    }
    else
    {
        if (iter->target->back == 0)
            iter->cursor = iter->target->capacity - 1;
        else
            iter->cursor = iter->target->back - 1;
        iter->index = d_count(iter->target) - 1;
    }
    iter->start = d_empty(iter->target);
    iter->end = 1;
}
_Bool d_iter_next(deque_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == d_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    iter->start = d_empty(iter->target);
    iter->cursor = (iter->cursor + 1) % (iter->target->capacity);
    iter->index++;
    return 1;
}
_Bool d_iter_prev(deque_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = d_empty(iter->target);
    iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1 : iter->cursor - 1;
    iter->index--;
    return 1;
}
_Bool d_iter_advance(deque_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == d_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= d_count(iter->target))
        return 0;
    iter->start = d_empty(iter->target);
    iter->index += steps;
    iter->cursor = (iter->cursor + steps) % iter->target->capacity;
    return 1;
}
_Bool d_iter_rewind(deque_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    if (steps == 0 || iter->index < steps)
        return 0;
    iter->end = d_empty(iter->target);
    iter->index -= steps;
    if (iter->cursor < steps)
        iter->cursor += d_capacity(iter->target);
    iter->cursor -= steps;
    return 1;
}
_Bool d_iter_go_to(deque_iter *iter, size_t index)
{
    if (index >= d_count(iter->target))
        return 0;
    if (iter->index > index)
        return d_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return d_iter_advance(iter, index - iter->index);
    return 1;
}
size_t d_iter_value(deque_iter *iter)
{
    if (d_empty(iter->target))
        return (size_t){0};
    return iter->target->buffer[iter->cursor];
}
size_t *d_iter_rvalue(deque_iter *iter)
{
    if (d_empty(iter->target))
        return ((void *)0);
    return &(iter->target->buffer[iter->cursor]);
}
size_t d_iter_index(deque_iter *iter) { return iter->index; }
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
