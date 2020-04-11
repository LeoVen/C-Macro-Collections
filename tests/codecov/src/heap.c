#include <cmc/heap.h>

// CMC_GENERATE_HEAP(h, heap, size_t)

struct heap
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    enum cmc_heap_order HO;
    int flag;
    struct heap_ftab_val *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
    struct heap_iter (*it_start)(struct heap *);
    struct heap_iter (*it_end)(struct heap *);
};
struct heap_ftab_val
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct heap_iter
{
    struct heap *target;
    size_t cursor;
    _Bool start;
    _Bool end;
};
struct heap *h_new(size_t capacity, enum cmc_heap_order HO,
                   struct heap_ftab_val *f_val);
struct heap *h_new_custom(size_t capacity, enum cmc_heap_order HO,
                          struct heap_ftab_val *f_val,
                          struct cmc_alloc_node *alloc,
                          struct cmc_callbacks *callbacks);
void h_clear(struct heap *_heap_);
void h_free(struct heap *_heap_);
void h_customize(struct heap *_heap_, struct cmc_alloc_node *alloc,
                 struct cmc_callbacks *callbacks);
_Bool h_insert(struct heap *_heap_, size_t element);
_Bool h_remove(struct heap *_heap_);
size_t h_peek(struct heap *_heap_);
_Bool h_contains(struct heap *_heap_, size_t element);
_Bool h_empty(struct heap *_heap_);
_Bool h_full(struct heap *_heap_);
size_t h_count(struct heap *_heap_);
size_t h_capacity(struct heap *_heap_);
int h_flag(struct heap *_heap_);
_Bool h_resize(struct heap *_heap_, size_t capacity);
struct heap *h_copy_of(struct heap *_heap_);
_Bool h_equals(struct heap *_heap1_, struct heap *_heap2_);
struct cmc_string h_to_string(struct heap *_heap_);
_Bool h_print(struct heap *_heap_, FILE *fptr);
struct heap_iter *h_iter_new(struct heap *target);
void h_iter_free(struct heap_iter *iter);
void h_iter_init(struct heap_iter *iter, struct heap *target);
_Bool h_iter_start(struct heap_iter *iter);
_Bool h_iter_end(struct heap_iter *iter);
void h_iter_to_start(struct heap_iter *iter);
void h_iter_to_end(struct heap_iter *iter);
_Bool h_iter_next(struct heap_iter *iter);
_Bool h_iter_prev(struct heap_iter *iter);
_Bool h_iter_advance(struct heap_iter *iter, size_t steps);
_Bool h_iter_rewind(struct heap_iter *iter, size_t steps);
_Bool h_iter_go_to(struct heap_iter *iter, size_t index);
size_t h_iter_value(struct heap_iter *iter);
size_t h_iter_index(struct heap_iter *iter);
static void h_impl_float_up(struct heap *_heap_, size_t index);
static void h_impl_float_down(struct heap *_heap_, size_t index);
static struct heap_iter h_impl_it_start(struct heap *_heap_);
static struct heap_iter h_impl_it_end(struct heap *_heap_);
struct heap *h_new(size_t capacity, enum cmc_heap_order HO,
                   struct heap_ftab_val *f_val)
{
    struct cmc_alloc_node *alloc = &cmc_alloc_node_default;
    if (capacity < 1)
        return ((void *)0);
    if (HO != cmc_min_heap && HO != cmc_max_heap)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    struct heap *_heap_ = alloc->malloc(sizeof(struct heap));
    if (!_heap_)
        return ((void *)0);
    _heap_->buffer = alloc->calloc(capacity, sizeof(size_t));
    if (!_heap_->buffer)
    {
        alloc->free(_heap_);
        return ((void *)0);
    }
    _heap_->capacity = capacity;
    _heap_->count = 0;
    _heap_->HO = HO;
    _heap_->flag = cmc_flags.OK;
    _heap_->f_val = f_val;
    _heap_->alloc = alloc;
    _heap_->callbacks = ((void *)0);
    _heap_->it_start = h_impl_it_start;
    _heap_->it_end = h_impl_it_end;
    return _heap_;
}
struct heap *h_new_custom(size_t capacity, enum cmc_heap_order HO,
                          struct heap_ftab_val *f_val,
                          struct cmc_alloc_node *alloc,
                          struct cmc_callbacks *callbacks)
{
    if (capacity < 1)
        return ((void *)0);
    if (HO != cmc_min_heap && HO != cmc_max_heap)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct heap *_heap_ = alloc->malloc(sizeof(struct heap));
    if (!_heap_)
        return ((void *)0);
    _heap_->buffer = alloc->calloc(capacity, sizeof(size_t));
    if (!_heap_->buffer)
    {
        alloc->free(_heap_);
        return ((void *)0);
    }
    _heap_->capacity = capacity;
    _heap_->count = 0;
    _heap_->HO = HO;
    _heap_->flag = cmc_flags.OK;
    _heap_->f_val = f_val;
    _heap_->alloc = alloc;
    _heap_->callbacks = callbacks;
    _heap_->it_start = h_impl_it_start;
    _heap_->it_end = h_impl_it_end;
    return _heap_;
}
void h_clear(struct heap *_heap_)
{
    if (_heap_->f_val->free)
    {
        for (size_t i = 0; i < _heap_->count; i++)
        {
            _heap_->f_val->free(_heap_->buffer[i]);
        }
    }
    memset(_heap_->buffer, 0, sizeof(size_t) * _heap_->capacity);
    _heap_->count = 0;
    _heap_->flag = cmc_flags.OK;
}
void h_free(struct heap *_heap_)
{
    if (_heap_->f_val->free)
    {
        for (size_t i = 0; i < _heap_->count; i++)
        {
            _heap_->f_val->free(_heap_->buffer[i]);
        }
    }
    _heap_->alloc->free(_heap_->buffer);
    _heap_->alloc->free(_heap_);
}
void h_customize(struct heap *_heap_, struct cmc_alloc_node *alloc,
                 struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _heap_->alloc = &cmc_alloc_node_default;
    else
        _heap_->alloc = alloc;
    _heap_->callbacks = callbacks;
    _heap_->flag = cmc_flags.OK;
}
_Bool h_insert(struct heap *_heap_, size_t element)
{
    if (h_full(_heap_))
    {
        if (!h_resize(_heap_, _heap_->count * 2))
            return 0;
    }
    _heap_->buffer[_heap_->count++] = element;
    if (!h_empty(_heap_))
    {
        h_impl_float_up(_heap_, _heap_->count - 1);
    }
    _heap_->flag = cmc_flags.OK;
    if (_heap_->callbacks && _heap_->callbacks->create)
        _heap_->callbacks->create();
    return 1;
}
_Bool h_remove(struct heap *_heap_)
{
    if (h_empty(_heap_))
    {
        _heap_->flag = cmc_flags.EMPTY;
        return 0;
    }
    _heap_->buffer[0] = _heap_->buffer[_heap_->count - 1];
    _heap_->buffer[_heap_->count - 1] = (size_t){ 0 };
    _heap_->count--;
    h_impl_float_down(_heap_, 0);
    _heap_->flag = cmc_flags.OK;
    if (_heap_->callbacks && _heap_->callbacks->delete)
        _heap_->callbacks->delete ();
    return 1;
}
size_t h_peek(struct heap *_heap_)
{
    if (h_empty(_heap_))
    {
        _heap_->flag = cmc_flags.EMPTY;
        return (size_t){ 0 };
    }
    _heap_->flag = cmc_flags.OK;
    if (_heap_->callbacks && _heap_->callbacks->read)
        _heap_->callbacks->read();
    return _heap_->buffer[0];
}
_Bool h_contains(struct heap *_heap_, size_t element)
{
    _heap_->flag = cmc_flags.OK;
    _Bool result = 0;
    for (size_t i = 0; i < _heap_->count; i++)
    {
        if (_heap_->f_val->cmp(_heap_->buffer[i], element) == 0)
        {
            result = 1;
            break;
        }
    }
    if (_heap_->callbacks && _heap_->callbacks->read)
        _heap_->callbacks->read();
    return result;
}
_Bool h_empty(struct heap *_heap_)
{
    return _heap_->count == 0;
}
_Bool h_full(struct heap *_heap_)
{
    return _heap_->count >= _heap_->capacity;
}
size_t h_count(struct heap *_heap_)
{
    return _heap_->count;
}
size_t h_capacity(struct heap *_heap_)
{
    return _heap_->capacity;
}
int h_flag(struct heap *_heap_)
{
    return _heap_->flag;
}
_Bool h_resize(struct heap *_heap_, size_t capacity)
{
    _heap_->flag = cmc_flags.OK;
    if (_heap_->capacity == capacity)
        goto success;
    if (capacity < _heap_->count)
    {
        _heap_->flag = cmc_flags.INVALID;
        return 0;
    }
    size_t *new_buffer =
        _heap_->alloc->realloc(_heap_->buffer, sizeof(size_t) * capacity);
    if (!new_buffer)
    {
        _heap_->flag = cmc_flags.ALLOC;
        return 0;
    }
    _heap_->buffer = new_buffer;
    _heap_->capacity = capacity;
success:
    if (_heap_->callbacks && _heap_->callbacks->resize)
        _heap_->callbacks->resize();
    return 1;
}
struct heap *h_copy_of(struct heap *_heap_)
{
    struct heap *result =
        h_new_custom(_heap_->capacity, _heap_->HO, _heap_->f_val, _heap_->alloc,
                     _heap_->callbacks);
    if (!result)
    {
        _heap_->flag = cmc_flags.ERROR;
        return ((void *)0);
    }
    if (_heap_->f_val->cpy)
    {
        for (size_t i = 0; i < _heap_->count; i++)
            result->buffer[i] = _heap_->f_val->cpy(_heap_->buffer[i]);
    }
    else
        memcpy(result->buffer, _heap_->buffer, sizeof(size_t) * _heap_->count);
    result->count = _heap_->count;
    _heap_->flag = cmc_flags.OK;
    return result;
}
_Bool h_equals(struct heap *_heap1_, struct heap *_heap2_)
{
    _heap1_->flag = cmc_flags.OK;
    _heap2_->flag = cmc_flags.OK;
    if (_heap1_->count != _heap2_->count)
        return 0;
    for (size_t i = 0; i < _heap1_->count; i++)
    {
        if (_heap1_->f_val->cmp(_heap1_->buffer[i], _heap2_->buffer[i]) != 0)
            return 0;
    }
    return 1;
}
struct cmc_string h_to_string(struct heap *_heap_)
{
    struct cmc_string str;
    struct heap *h_ = _heap_;
    const char *t = h_->HO == 1 ? "MaxHeap" : "MinHeap";
    int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_heap, "heap",
                     "size_t", h_, h_->buffer, h_->capacity, h_->count, t,
                     h_->flag, h_->f_val, h_->alloc, h_->callbacks);
    return n >= 0 ? str : (struct cmc_string){ 0 };
}
_Bool h_print(struct heap *_heap_, FILE *fptr)
{
    for (size_t i = 0; i < _heap_->count; i++)
    {
        if (!_heap_->f_val->str(fptr, _heap_->buffer[i]))
            return 0;
    }
    return 1;
}
struct heap_iter *h_iter_new(struct heap *target)
{
    struct heap_iter *iter = target->alloc->malloc(sizeof(struct heap_iter));
    if (!iter)
        return ((void *)0);
    h_iter_init(iter, target);
    return iter;
}
void h_iter_free(struct heap_iter *iter)
{
    iter->target->alloc->free(iter);
}
void h_iter_init(struct heap_iter *iter, struct heap *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->start = 1;
    iter->end = h_empty(target);
}
_Bool h_iter_start(struct heap_iter *iter)
{
    return h_empty(iter->target) || iter->start;
}
_Bool h_iter_end(struct heap_iter *iter)
{
    return h_empty(iter->target) || iter->end;
}
void h_iter_to_start(struct heap_iter *iter)
{
    if (!h_empty(iter->target))
    {
        iter->cursor = 0;
        iter->start = 1;
        iter->end = h_empty(iter->target);
    }
}
void h_iter_to_end(struct heap_iter *iter)
{
    if (!h_empty(iter->target))
    {
        iter->cursor = iter->target->count - 1;
        iter->start = h_empty(iter->target);
        iter->end = 1;
    }
}
_Bool h_iter_next(struct heap_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = h_empty(iter->target);
    iter->cursor++;
    return 1;
}
_Bool h_iter_prev(struct heap_iter *iter)
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
_Bool h_iter_advance(struct heap_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor + steps >= iter->target->count)
        return 0;
    iter->start = h_empty(iter->target);
    if (iter->end)
        return 0;
    iter->cursor += steps;
    return 1;
}
_Bool h_iter_rewind(struct heap_iter *iter, size_t steps)
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
_Bool h_iter_go_to(struct heap_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->cursor > index)
        return h_iter_rewind(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return h_iter_advance(iter, index - iter->cursor);
    return 1;
}
size_t h_iter_value(struct heap_iter *iter)
{
    if (h_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor];
}
size_t h_iter_index(struct heap_iter *iter)
{
    return iter->cursor;
}
static void h_impl_float_up(struct heap *_heap_, size_t index)
{
    size_t C = index;
    size_t child = _heap_->buffer[C];
    size_t parent = _heap_->buffer[(index - 1) / 2];
    int mod = _heap_->HO;
    while (C > 0 && _heap_->f_val->cmp(child, parent) * mod > 0)
    {
        size_t tmp = _heap_->buffer[C];
        _heap_->buffer[C] = _heap_->buffer[(C - 1) / 2];
        _heap_->buffer[(C - 1) / 2] = tmp;
        C = (C - 1) / 2;
        child = _heap_->buffer[C];
        parent = _heap_->buffer[(C - 1) / 2];
    }
}
static void h_impl_float_down(struct heap *_heap_, size_t index)
{
    int mod = _heap_->HO;
    while (index < _heap_->count)
    {
        size_t L = 2 * index + 1;
        size_t R = 2 * index + 2;
        size_t C = index;
        if (L < _heap_->count &&
            _heap_->f_val->cmp(_heap_->buffer[L], _heap_->buffer[C]) * mod > 0)
        {
            C = L;
        }
        if (R < _heap_->count &&
            _heap_->f_val->cmp(_heap_->buffer[R], _heap_->buffer[C]) * mod > 0)
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
}
static struct heap_iter h_impl_it_start(struct heap *_heap_)
{
    struct heap_iter iter;
    h_iter_init(&iter, _heap_);
    h_iter_to_start(&iter);
    return iter;
}
static struct heap_iter h_impl_it_end(struct heap *_heap_)
{
    struct heap_iter iter;
    h_iter_init(&iter, _heap_);
    h_iter_to_end(&iter);
    return iter;
}
