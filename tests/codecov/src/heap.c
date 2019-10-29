#include <cmc/heap.h>

//CMC_GENERATE_HEAP(h, heap, size_t)

typedef struct heap_s
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    enum cmc_heap_order HO;
    int (*cmp)(size_t, size_t);
    struct heap_iter_s (*it_start)(struct heap_s *);
    struct heap_iter_s (*it_end)(struct heap_s *);
} heap, *heap_ptr;
typedef struct heap_iter_s
{
    struct heap_s *target;
    size_t cursor;
    _Bool start;
    _Bool end;
} heap_iter, *heap_iter_ptr;
heap *h_new(size_t capacity, enum cmc_heap_order HO, int (*compare)(size_t, size_t));
void h_clear(heap *_heap_, void (*deallocator)(size_t));
void h_free(heap *_heap_, void (*deallocator)(size_t));
_Bool h_insert(heap *_heap_, size_t element);
_Bool h_remove(heap *_heap_, size_t *result);
size_t h_peek(heap *_heap_);
_Bool h_contains(heap *_heap_, size_t element);
_Bool h_empty(heap *_heap_);
_Bool h_full(heap *_heap_);
size_t h_count(heap *_heap_);
size_t h_capacity(heap *_heap_);
_Bool h_resize(heap *_heap_, size_t capacity);
heap *h_copy_of(heap *_heap_, size_t (*copy_func)(size_t));
_Bool h_equals(heap *_heap1_, heap *_heap2_);
struct cmc_string h_to_string(heap *_heap_);
heap_iter *h_iter_new(heap *target);
void h_iter_free(heap_iter *iter);
void h_iter_init(heap_iter *iter, heap *target);
_Bool h_iter_start(heap_iter *iter);
_Bool h_iter_end(heap_iter *iter);
void h_iter_to_start(heap_iter *iter);
void h_iter_to_end(heap_iter *iter);
_Bool h_iter_next(heap_iter *iter);
_Bool h_iter_prev(heap_iter *iter);
_Bool h_iter_advance(heap_iter *iter, size_t steps);
_Bool h_iter_rewind(heap_iter *iter, size_t steps);
_Bool h_iter_go_to(heap_iter *iter, size_t index);
size_t h_iter_value(heap_iter *iter);
size_t h_iter_index(heap_iter *iter);
static _Bool h_impl_float_up(heap *_heap_, size_t index);
static _Bool h_impl_float_down(heap *_heap_, size_t index);
static heap_iter h_impl_it_start(heap *_heap_);
static heap_iter h_impl_it_end(heap *_heap_);
heap *h_new(size_t capacity, enum cmc_heap_order HO, int (*compare)(size_t, size_t))
{
    if (capacity < 1)
        return ((void *)0);
    if (HO != cmc_min_heap && HO != cmc_max_heap)
        return ((void *)0);
    heap *_heap_ = malloc(sizeof(heap));
    if (!_heap_)
        return ((void *)0);
    _heap_->buffer = malloc(sizeof(size_t) * capacity);
    if (!_heap_->buffer)
    {
        free(_heap_);
        return ((void *)0);
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
void h_clear(heap *_heap_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _heap_->count; i++)
        {
            deallocator(_heap_->buffer[i]);
        }
    }
    memset(_heap_->buffer, 0, sizeof(size_t) * _heap_->capacity);
    _heap_->count = 0;
}
void h_free(heap *_heap_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _heap_->count; i++)
        {
            deallocator(_heap_->buffer[i]);
        }
    }
    free(_heap_->buffer);
    free(_heap_);
}
_Bool h_insert(heap *_heap_, size_t element)
{
    if (h_full(_heap_))
    {
        if (!h_resize(_heap_, h_count(_heap_) * 2))
            return 0;
    }
    if (h_empty(_heap_))
    {
        _heap_->buffer[_heap_->count++] = element;
        return 1;
    }
    _heap_->buffer[_heap_->count++] = element;
    if (!h_impl_float_up(_heap_, _heap_->count - 1))
        return 0;
    return 1;
}
_Bool h_remove(heap *_heap_, size_t *result)
{
    if (h_empty(_heap_))
        return 0;
    *result = _heap_->buffer[0];
    _heap_->buffer[0] = _heap_->buffer[_heap_->count - 1];
    _heap_->buffer[_heap_->count - 1] = (size_t){0};
    _heap_->count--;
    if (!h_impl_float_down(_heap_, 0))
        return 0;
    return 1;
}
size_t h_peek(heap *_heap_)
{
    if (h_empty(_heap_))
        return (size_t){0};
    return _heap_->buffer[0];
}
_Bool h_contains(heap *_heap_, size_t element)
{
    for (size_t i = 0; i < _heap_->count; i++)
    {
        if (_heap_->cmp(_heap_->buffer[i], element) == 0)
            return 1;
    }
    return 0;
}
_Bool h_empty(heap *_heap_) { return _heap_->count == 0; }
_Bool h_full(heap *_heap_) { return _heap_->count >= _heap_->capacity; }
size_t h_count(heap *_heap_) { return _heap_->count; }
size_t h_capacity(heap *_heap_) { return _heap_->capacity; }
_Bool h_resize(heap *_heap_, size_t capacity)
{
    if (h_capacity(_heap_) == capacity)
        return 1;
    if (capacity < h_count(_heap_))
        return 0;
    size_t *new_buffer = realloc(_heap_->buffer, sizeof(size_t) * capacity);
    if (!new_buffer)
        return 0;
    _heap_->buffer = new_buffer;
    _heap_->capacity = capacity;
    return 1;
}
heap *h_copy_of(heap *_heap_, size_t (*copy_func)(size_t))
{
    heap *result = h_new(_heap_->capacity, _heap_->HO, _heap_->cmp);
    if (!result)
        return ((void *)0);
    if (copy_func)
    {
        for (size_t i = 0; i < _heap_->count; i++)
            result->buffer[i] = copy_func(_heap_->buffer[i]);
    }
    else
        memcpy(result->buffer, _heap_->buffer, sizeof(size_t) * _heap_->count);
    result->count = _heap_->count;
    return result;
}
_Bool h_equals(heap *_heap1_, heap *_heap2_)
{
    if (h_count(_heap1_) != h_count(_heap2_))
        return 0;
    for (size_t i = 0; i < h_count(_heap1_); i++)
    {
        if (_heap1_->cmp(_heap1_->buffer[i], _heap2_->buffer[i]) != 0)
            return 0;
    }
    return 1;
}
struct cmc_string h_to_string(heap *_heap_)
{
    struct cmc_string str;
    heap *h_ = _heap_;
    const char *name = "heap";
    const char *t = h_->HO == 1 ? "MaxHeap" : "MinHeap";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_heap, name, h_, h_->buffer, h_->capacity, h_->count, t, h_->cmp);
    return str;
}
heap_iter *h_iter_new(heap *target)
{
    heap_iter *iter = malloc(sizeof(heap_iter));
    if (!iter)
        return ((void *)0);
    h_iter_init(iter, target);
    return iter;
}
void h_iter_free(heap_iter *iter) { free(iter); }
void h_iter_init(heap_iter *iter, heap *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->start = 1;
    iter->end = h_empty(target);
}
_Bool h_iter_start(heap_iter *iter) { return h_empty(iter->target) || iter->start; }
_Bool h_iter_end(heap_iter *iter) { return h_empty(iter->target) || iter->end; }
void h_iter_to_start(heap_iter *iter)
{
    if (!h_empty(iter->target))
    {
        iter->cursor = 0;
        iter->start = 1;
        iter->end = h_empty(iter->target);
    }
}
void h_iter_to_end(heap_iter *iter)
{
    if (!h_empty(iter->target))
    {
        iter->cursor = h_count(iter->target) - 1;
        iter->start = h_empty(iter->target);
        iter->end = 1;
    }
}
_Bool h_iter_next(heap_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == h_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    iter->start = h_empty(iter->target);
    iter->cursor++;
    return 1;
}
_Bool h_iter_prev(heap_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = h_empty(iter->target);
    iter->cursor--;
    return 1;
}
_Bool h_iter_advance(heap_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->cursor + 1 == h_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor + steps >= h_count(iter->target))
        return 0;
    iter->start = h_empty(iter->target);
    if (iter->end)
        return 0;
    iter->cursor += steps;
    return 1;
}
_Bool h_iter_rewind(heap_iter *iter, size_t steps)
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
    iter->end = h_empty(iter->target);
    iter->cursor -= steps;
    return 1;
}
_Bool h_iter_go_to(heap_iter *iter, size_t index)
{
    if (index >= h_count(iter->target))
        return 0;
    if (iter->cursor > index)
        return h_iter_rewind(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return h_iter_advance(iter, index - iter->cursor);
    return 1;
}
size_t h_iter_value(heap_iter *iter)
{
    if (h_empty(iter->target))
        return (size_t){0};
    return iter->target->buffer[iter->cursor];
}
size_t h_iter_index(heap_iter *iter) { return iter->cursor; }
static _Bool h_impl_float_up(heap *_heap_, size_t index)
{
    size_t C = index;
    size_t child = _heap_->buffer[C];
    size_t parent = _heap_->buffer[(index - 1) / 2];
    int mod = _heap_->HO;
    while (C > 0 && _heap_->cmp(child, parent) * mod > 0)
    {
        size_t tmp = _heap_->buffer[C];
        _heap_->buffer[C] = _heap_->buffer[(C - 1) / 2];
        _heap_->buffer[(C - 1) / 2] = tmp;
        C = (C - 1) / 2;
        child = _heap_->buffer[C];
        parent = _heap_->buffer[(C - 1) / 2];
    }
    return 1;
}
static _Bool h_impl_float_down(heap *_heap_, size_t index)
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
    return 1;
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
