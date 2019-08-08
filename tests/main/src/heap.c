#include "cmc/heap.h"

//HEAP_GENERATE(h, heap, , size_t)

typedef struct heap_s
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    enum cmc_heap_order_e HO;
    int (*cmp)(size_t, size_t);
    struct heap_iter_s (*it_start)(struct heap_s *);
    struct heap_iter_s (*it_end)(struct heap_s *);
} heap, *heap_ptr;
typedef struct heap_iter_s
{
    struct heap_s *target;
    size_t cursor;
    bool start;
    bool end;
} heap_iter, *heap_iter_ptr;
heap *h_new(size_t capacity, cmc_heap_order HO, int (*compare)(size_t, size_t));
void h_clear(heap *_heap_);
void h_free(heap *_heap_);
bool h_insert(heap *_heap_, size_t element);
bool h_remove(heap *_heap_, size_t *result);
bool h_insert_if(heap *_heap_, size_t element, bool condition);
bool h_remove_if(heap *_heap_, size_t *result, bool condition);
size_t h_peek(heap *_heap_);
size_t *h_peek_ref(heap *_heap_);
bool h_contains(heap *_heap_, size_t element);
bool h_empty(heap *_heap_);
bool h_full(heap *_heap_);
size_t h_count(heap *_heap_);
size_t h_capacity(heap *_heap_);
heap_iter *h_iter_new(heap *target);
void h_iter_free(heap_iter *iter);
void h_iter_init(heap_iter *iter, heap *target);
bool h_iter_start(heap_iter *iter);
bool h_iter_end(heap_iter *iter);
void h_iter_to_start(heap_iter *iter);
void h_iter_to_end(heap_iter *iter);
bool h_iter_next(heap_iter *iter);
bool h_iter_prev(heap_iter *iter);
size_t h_iter_value(heap_iter *iter);
size_t h_iter_index(heap_iter *iter);
static inline size_t h_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
static bool h_impl_grow(heap *_heap_);
static bool h_impl_float_up(heap *_heap_, size_t index);
static bool h_impl_float_down(heap *_heap_, size_t index);
static heap_iter h_impl_it_start(heap *_heap_);
static heap_iter h_impl_it_end(heap *_heap_);
heap *h_new(size_t capacity, cmc_heap_order HO, int (*compare)(size_t, size_t))
{
    if (capacity < 1)
        return NULL;
    if (HO != cmc_min_heap && HO != cmc_max_heap)
        return NULL;
    heap *_heap_ = malloc(sizeof(heap));
    if (!_heap_)
        return NULL;
    _heap_->buffer = malloc(sizeof(size_t) * capacity);
    if (!_heap_->buffer)
    {
        free(_heap_);
        return NULL;
    }
    memset(_heap_->buffer, 0, sizeof(size_t) * capacity);
    _heap_->capacity = capacity;
    _heap_->count = 0;
    _heap_->HO = HO;
    _heap_->cmp = compare;
    _heap_->it_start = h_impl_it_start;
    _heap_->it_end = h_impl_it_end;
    return _heap_;
}
void h_clear(heap *_heap_)
{
    memset(_heap_->buffer, 0, sizeof(size_t) * _heap_->capacity);
    _heap_->count = 0;
}
void h_free(heap *_heap_)
{
    free(_heap_->buffer);
    free(_heap_);
}
bool h_insert(heap *_heap_, size_t element)
{
    if (h_full(_heap_))
    {
        if (!h_impl_grow(_heap_))
            return false;
    }
    if (_heap_->count == 0)
    {
        _heap_->buffer[_heap_->count++] = element;
        return true;
    }
    _heap_->buffer[_heap_->count++] = element;
    if (!h_impl_float_up(_heap_, _heap_->count - 1))
        return false;
    return true;
}
bool h_remove(heap *_heap_, size_t *result)
{
    if (h_empty(_heap_))
        return false;
    *result = _heap_->buffer[0];
    _heap_->buffer[0] = _heap_->buffer[_heap_->count - 1];
    _heap_->buffer[_heap_->count - 1] = h_impl_default_value();
    _heap_->count--;
    if (!h_impl_float_down(_heap_, 0))
        return false;
    return true;
}
bool h_insert_if(heap *_heap_, size_t element, bool condition)
{
    if (condition)
        return h_insert(_heap_, element);
    return false;
}
bool h_remove_if(heap *_heap_, size_t *result, bool condition)
{
    if (condition)
        return h_remove(_heap_, result);
    return false;
}
size_t h_peek(heap *_heap_)
{
    if (h_empty(_heap_))
        return h_impl_default_value();
    return _heap_->buffer[0];
}
size_t *h_peek_ref(heap *_heap_)
{
    if (h_empty(_heap_))
        return NULL;
    return &(_heap_->buffer[0]);
}
bool h_contains(heap *_heap_, size_t element)
{
    for (size_t i = 0; i < _heap_->count; i++)
    {
        if (_heap_->cmp(_heap_->buffer[i], element) == 0)
            return true;
    }
    return false;
}
bool h_empty(heap *_heap_) { return _heap_->count == 0; }
bool h_full(heap *_heap_) { return _heap_->count >= _heap_->capacity; }
size_t h_count(heap *_heap_) { return _heap_->count; }
size_t h_capacity(heap *_heap_) { return _heap_->capacity; }
heap_iter *h_iter_new(heap *target)
{
    heap_iter *iter = malloc(sizeof(heap_iter));
    if (!iter)
        return NULL;
    h_iter_init(iter, target);
    return iter;
}
void h_iter_free(heap_iter *iter) { free(iter); }
void h_iter_init(heap_iter *iter, heap *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->start = true;
    iter->end = h_empty(target);
}
bool h_iter_start(heap_iter *iter) { return h_empty(iter->target) || iter->start; }
bool h_iter_end(heap_iter *iter) { return h_empty(iter->target) || iter->end; }
void h_iter_to_start(heap_iter *iter)
{
    iter->cursor = 0;
    iter->start = true;
    iter->end = h_empty(iter->target);
}
void h_iter_to_end(heap_iter *iter)
{
    iter->cursor = iter->target->count - 1;
    iter->start = h_empty(iter->target);
    iter->end = true;
}
bool h_iter_next(heap_iter *iter)
{
    if (iter->end)
        return false;
    iter->start = false;
    if (iter->cursor == iter->target->count - 1)
        iter->end = true;
    else
        iter->cursor++;
    return true;
}
bool h_iter_prev(heap_iter *iter)
{
    if (iter->start)
        return false;
    iter->end = false;
    if (iter->cursor == 0)
        iter->start = true;
    else
        iter->cursor--;
    return true;
}
size_t h_iter_value(heap_iter *iter)
{
    if (h_empty(iter->target))
        return h_impl_default_value();
    return iter->target->buffer[iter->cursor];
}
size_t h_iter_index(heap_iter *iter) { return iter->cursor; }
static bool h_impl_grow(heap *_heap_)
{
    size_t new_capacity = _heap_->capacity * 2;
    size_t *new_buffer = realloc(_heap_->buffer, sizeof(size_t) * new_capacity);
    if (!new_buffer)
        return false;
    _heap_->buffer = new_buffer;
    _heap_->capacity = new_capacity;
    return true;
}
static bool h_impl_float_up(heap *_heap_, size_t index)
{
    size_t C = index;
    size_t child = _heap_->buffer[C];
    size_t parent = _heap_->buffer[(index - 1) / 2];
    int mod = _heap_->HO;
    while (C > 0 && _heap_->cmp(child, parent) * mod > 0)
    {
        size_t tmp = _heap_->buffer[C];
        _heap_->buffer[C] = _heap_->buffer[(index - 1) / 2];
        _heap_->buffer[(index - 1) / 2] = tmp;
        C = (index - 1) / 2;
        child = _heap_->buffer[C];
        parent = _heap_->buffer[(index - 1) / 2];
    }
    return true;
}
static bool h_impl_float_down(heap *_heap_, size_t index)
{
    int mod = _heap_->HO;
    while (index < _heap_->count)
    {
        size_t L = 2 * index + 1;
        size_t R = 2 * index + 2;
        size_t C = index;
        if (L < _heap_->count && _heap_->cmp(_heap_->buffer[L], _heap_->buffer[C]) * mod > 0)
        {
            C = L;
        }
        if (R < _heap_->count && _heap_->cmp(_heap_->buffer[R], _heap_->buffer[C]) * mod > 0)
        {
            C = R;
        }
        if (C != index)
        {
            size_t tmp = _heap_->buffer[index];
            _heap_->buffer[index] = _heap_->buffer[C];
            _heap_->buffer[C] = tmp;
            index = C;
        }
        else
            break;
    }
    return true;
}
static heap_iter h_impl_it_start(heap *_heap_)
{
    heap_iter iter;
    h_iter_init(&iter, _heap_);
    h_iter_to_start(&iter);
    return iter;
}
static heap_iter h_impl_it_end(heap *_heap_)
{
    heap_iter iter;
    h_iter_init(&iter, _heap_);
    h_iter_to_end(&iter);
    return iter;
}
