#include <cmc/intervalheap.h>

//CMC_GENERATE_INTERVALHEAP(ih, intervalheap, size_t)

typedef struct intervalheap_s
{
    struct intervalheap_node_s *buffer;
    size_t capacity;
    size_t size;
    size_t count;
    int (*cmp)(size_t, size_t);
    struct intervalheap_iter_s (*it_start)(struct intervalheap_s *);
    struct intervalheap_iter_s (*it_end)(struct intervalheap_s *);
} intervalheap, *intervalheap_ptr;
typedef struct intervalheap_node_s
{
    size_t data[2];
} intervalheap_node, *intervalheap_node_ptr;
typedef struct intervalheap_iter_s
{
    struct intervalheap_s *target;
    size_t cursor;
    _Bool start;
    _Bool end;
} intervalheap_iter, *intervalheap_iter_ptr;
intervalheap *ih_new(size_t capacity, int (*compare)(size_t, size_t));
void ih_clear(intervalheap *_heap_, void (*deallocator)(size_t));
void ih_free(intervalheap *_heap_, void (*deallocator)(size_t));
_Bool ih_insert(intervalheap *_heap_, size_t element);
_Bool ih_remove_max(intervalheap *_heap_, size_t *result);
_Bool ih_remove_min(intervalheap *_heap_, size_t *result);
_Bool ih_update_max(intervalheap *_heap_, size_t element);
_Bool ih_update_min(intervalheap *_heap_, size_t element);
_Bool ih_max(intervalheap *_heap_, size_t *value);
_Bool ih_min(intervalheap *_heap_, size_t *value);
_Bool ih_contains(intervalheap *_heap_, size_t element);
_Bool ih_empty(intervalheap *_heap_);
_Bool ih_full(intervalheap *_heap_);
size_t ih_count(intervalheap *_heap_);
size_t ih_capacity(intervalheap *_heap_);
_Bool ih_resize(intervalheap *_heap_, size_t capacity);
intervalheap *ih_copy_of(intervalheap *_set_, size_t (*copy_func)(size_t));
_Bool ih_equals(intervalheap *_heap1_, intervalheap *_heap2_);
struct cmc_string ih_to_string(intervalheap *_heap_);
intervalheap_iter *ih_iter_new(intervalheap *target);
void ih_iter_free(intervalheap_iter *iter);
void ih_iter_init(intervalheap_iter *iter, intervalheap *target);
_Bool ih_iter_start(intervalheap_iter *iter);
_Bool ih_iter_end(intervalheap_iter *iter);
void ih_iter_to_start(intervalheap_iter *iter);
void ih_iter_to_end(intervalheap_iter *iter);
_Bool ih_iter_next(intervalheap_iter *iter);
_Bool ih_iter_prev(intervalheap_iter *iter);
_Bool ih_iter_advance(intervalheap_iter *iter, size_t steps);
_Bool ih_iter_rewind(intervalheap_iter *iter, size_t steps);
_Bool ih_iter_go_to(intervalheap_iter *iter, size_t index);
size_t ih_iter_value(intervalheap_iter *iter);
size_t ih_iter_index(intervalheap_iter *iter);
static void ih_impl_float_up_max(intervalheap *_heap_);
static void ih_impl_float_up_min(intervalheap *_heap_);
static void ih_impl_float_down_max(intervalheap *_heap_);
static void ih_impl_float_down_min(intervalheap *_heap_);
static intervalheap_iter ih_impl_it_start(intervalheap *_heap_);
static intervalheap_iter ih_impl_it_end(intervalheap *_heap_);
intervalheap *ih_new(size_t capacity, int (*compare)(size_t, size_t))
{
    if (capacity == 0 || capacity == 0xffffffffffffffff)
        return ((void *)0);
    intervalheap *_heap_ = malloc(sizeof(intervalheap));
    if (!_heap_)
        return ((void *)0);
    capacity = capacity % 2 == 0 ? capacity / 2 : (capacity + 1) / 2;
    _heap_->buffer = malloc(sizeof(intervalheap_node) * capacity);
    if (!_heap_->buffer)
    {
        free(_heap_);
        return ((void *)0);
    }
    memset(_heap_->buffer, 0, sizeof(intervalheap_node) * capacity);
    _heap_->capacity = capacity;
    _heap_->size = 0;
    _heap_->count = 0;
    _heap_->cmp = compare;
    _heap_->it_start = ih_impl_it_start;
    _heap_->it_end = ih_impl_it_end;
    return _heap_;
}
void ih_clear(intervalheap *_heap_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _heap_->count; i++)
        {
            deallocator(_heap_->buffer[i / 2].data[i % 2]);
        }
    }
    memset(_heap_->buffer, 0, sizeof(size_t) * _heap_->capacity);
    _heap_->size = 0;
    _heap_->count = 0;
}
void ih_free(intervalheap *_heap_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _heap_->count; i++)
        {
            deallocator(_heap_->buffer[i / 2].data[i % 2]);
        }
    }
    free(_heap_->buffer);
    free(_heap_);
}
_Bool ih_insert(intervalheap *_heap_, size_t element)
{
    if (ih_full(_heap_))
    {
        if (!ih_resize(_heap_, ih_capacity(_heap_) * 4))
            return 0;
    }
    if (ih_count(_heap_) % 2 == 0)
    {
        _heap_->buffer[_heap_->size].data[0] = element;
        _heap_->buffer[_heap_->size].data[1] = (size_t){0};
        _heap_->size++;
    }
    else
    {
        intervalheap_node *curr_node = &(_heap_->buffer[_heap_->size - 1]);
        if (_heap_->cmp(curr_node->data[0], element) > 0)
        {
            curr_node->data[1] = curr_node->data[0];
            curr_node->data[0] = element;
        }
        else
        {
            curr_node->data[1] = element;
        }
    }
    _heap_->count++;
    if (ih_count(_heap_) <= 2)
        return 1;
    intervalheap_node *parent = &(_heap_->buffer[(_heap_->size - 1) / 2]);
    if (_heap_->cmp(parent->data[0], element) > 0)
        ih_impl_float_up_min(_heap_);
    else if (_heap_->cmp(parent->data[1], element) < 0)
        ih_impl_float_up_max(_heap_);
    return 1;
}
_Bool ih_remove_max(intervalheap *_heap_, size_t *result)
{
    if (ih_empty(_heap_))
        return 0;
    if (ih_count(_heap_) == 1)
    {
        *result = _heap_->buffer[0].data[0];
        _heap_->buffer[0].data[0] = (size_t){0};
        _heap_->count--;
        return 1;
    }
    else
        *result = _heap_->buffer[0].data[1];
    intervalheap_node *last_node = &(_heap_->buffer[_heap_->size - 1]);
    if (ih_count(_heap_) % 2 == 1)
    {
        _heap_->buffer[0].data[1] = last_node->data[0];
        last_node->data[0] = (size_t){0};
        _heap_->size--;
    }
    else
    {
        _heap_->buffer[0].data[1] = last_node->data[1];
        last_node->data[1] = (size_t){0};
    }
    _heap_->count--;
    ih_impl_float_down_max(_heap_);
    return 1;
}
_Bool ih_remove_min(intervalheap *_heap_, size_t *result)
{
    if (ih_empty(_heap_))
        return 0;
    *result = _heap_->buffer[0].data[0];
    if (ih_count(_heap_) == 1)
    {
        _heap_->buffer[0].data[0] = (size_t){0};
        _heap_->count--;
        return 1;
    }
    intervalheap_node *last_node = &(_heap_->buffer[_heap_->size - 1]);
    _heap_->buffer[0].data[0] = last_node->data[0];
    if (ih_count(_heap_) % 2 == 1)
    {
        last_node->data[0] = (size_t){0};
        _heap_->size--;
    }
    else
    {
        last_node->data[0] = last_node->data[1];
        last_node->data[1] = (size_t){0};
    }
    _heap_->count--;
    ih_impl_float_down_min(_heap_);
    return 1;
}
_Bool ih_update_max(intervalheap *_heap_, size_t element)
{
    if (ih_empty(_heap_))
        return 0;
    if (ih_count(_heap_) == 1)
    {
        _heap_->buffer[0].data[0] = element;
    }
    else if (_heap_->cmp(element, _heap_->buffer[0].data[0]) < 0)
    {
        _heap_->buffer[0].data[1] = _heap_->buffer[0].data[0];
        _heap_->buffer[0].data[0] = element;
        ih_impl_float_down_max(_heap_);
    }
    else
    {
        _heap_->buffer[0].data[1] = element;
        ih_impl_float_down_max(_heap_);
    }
    return 1;
}
_Bool ih_update_min(intervalheap *_heap_, size_t element)
{
    if (ih_empty(_heap_))
        return 0;
    if (ih_count(_heap_) == 1)
    {
        _heap_->buffer[0].data[0] = element;
    }
    else if (_heap_->cmp(element, _heap_->buffer[0].data[1]) > 0)
    {
        _heap_->buffer[0].data[0] = _heap_->buffer[0].data[1];
        _heap_->buffer[0].data[1] = element;
        ih_impl_float_down_min(_heap_);
    }
    else
    {
        _heap_->buffer[0].data[0] = element;
        ih_impl_float_down_min(_heap_);
    }
    return 1;
}
_Bool ih_max(intervalheap *_heap_, size_t *value)
{
    if (ih_empty(_heap_))
        return 0;
    if (ih_count(_heap_) == 1)
        *value = _heap_->buffer[0].data[0];
    else
        *value = _heap_->buffer[0].data[1];
    return 1;
}
_Bool ih_min(intervalheap *_heap_, size_t *value)
{
    if (ih_empty(_heap_))
        return 0;
    *value = _heap_->buffer[0].data[0];
    return 1;
}
_Bool ih_contains(intervalheap *_heap_, size_t element)
{
    for (size_t i = 0; i < _heap_->count; i++)
    {
        if (_heap_->cmp(_heap_->buffer[i / 2].data[i % 2], element) == 0)
            return 1;
    }
    return 0;
}
_Bool ih_empty(intervalheap *_heap_) { return _heap_->count == 0; }
_Bool ih_full(intervalheap *_heap_) { return _heap_->size >= _heap_->capacity && _heap_->count % 2 == 0; }
size_t ih_count(intervalheap *_heap_) { return _heap_->count; }
size_t ih_capacity(intervalheap *_heap_) { return _heap_->capacity; }
_Bool ih_resize(intervalheap *_heap_, size_t capacity)
{
    if (ih_capacity(_heap_) == capacity)
        return 1;
    if (capacity < ih_count(_heap_))
        return 0;
    capacity = capacity % 2 == 0 ? capacity / 2 : (capacity + 1) / 2;
    intervalheap_node *new_buffer = realloc(_heap_->buffer, sizeof(intervalheap_node) * capacity);
    if (!new_buffer)
        return 0;
    memset(new_buffer + _heap_->capacity, 0, sizeof(intervalheap_node) * _heap_->capacity);
    _heap_->buffer = new_buffer;
    _heap_->capacity = capacity;
    return 1;
}
intervalheap *ih_copy_of(intervalheap *_heap_, size_t (*copy_func)(size_t))
{
    intervalheap *result = malloc(sizeof(intervalheap));
    if (!result)
        return ((void *)0);
    memcpy(result, _heap_, sizeof(intervalheap));
    result->buffer = malloc(sizeof(intervalheap_node) * _heap_->capacity);
    if (!result->buffer)
    {
        free(result);
        return ((void *)0);
    }
    if (copy_func)
    {
        for (size_t i = 0; i < _heap_->count; i++)
            result->buffer[i / 2].data[i % 2] = copy_func(_heap_->buffer[i / 2].data[i % 2]);
    }
    else
        memcpy(result->buffer, _heap_->buffer, sizeof(intervalheap_node) * _heap_->capacity);
    return result;
}
_Bool ih_equals(intervalheap *_heap1_, intervalheap *_heap2_)
{
    if (ih_count(_heap1_) != ih_count(_heap2_))
        return 0;
    for (size_t i = 0; i < _heap1_->count; i++)
    {
        size_t element1 = _heap1_->buffer[i / 2].data[i % 2];
        size_t element2 = _heap2_->buffer[i / 2].data[i % 2];
        if (_heap1_->cmp(element1, element2) == 0)
            return 1;
    }
    return 0;
}
struct cmc_string ih_to_string(intervalheap *_heap_)
{
    struct cmc_string str;
    intervalheap *h_ = _heap_;
    const char *name = "intervalheap";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_intervalheap, name, h_, h_->buffer, h_->capacity, h_->size, h_->count, h_->cmp);
    return str;
}
intervalheap_iter *ih_iter_new(intervalheap *target)
{
    intervalheap_iter *iter = malloc(sizeof(intervalheap_iter));
    if (!iter)
        return ((void *)0);
    ih_iter_init(iter, target);
    return iter;
}
void ih_iter_free(intervalheap_iter *iter) { free(iter); }
void ih_iter_init(intervalheap_iter *iter, intervalheap *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->start = 1;
    iter->end = ih_empty(target);
}
_Bool ih_iter_start(intervalheap_iter *iter) { return ih_empty(iter->target) || iter->start; }
_Bool ih_iter_end(intervalheap_iter *iter) { return ih_empty(iter->target) || iter->end; }
void ih_iter_to_start(intervalheap_iter *iter)
{
    if (!ih_empty(iter->target))
    {
        iter->cursor = 0;
        iter->start = 1;
        iter->end = ih_empty(iter->target);
    }
}
void ih_iter_to_end(intervalheap_iter *iter)
{
    if (!ih_empty(iter->target))
    {
        iter->cursor = iter->target->count - 1;
        iter->start = ih_empty(iter->target);
        iter->end = 1;
    }
}
_Bool ih_iter_next(intervalheap_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == ih_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    iter->start = ih_empty(iter->target);
    iter->cursor++;
    return 1;
}
_Bool ih_iter_prev(intervalheap_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = ih_empty(iter->target);
    iter->cursor--;
    return 1;
}
_Bool ih_iter_advance(intervalheap_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor + steps >= ih_count(iter->target))
        return 0;
    iter->start = ih_empty(iter->target);
    iter->cursor += steps;
    return 1;
}
_Bool ih_iter_rewind(intervalheap_iter *iter, size_t steps)
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
    iter->end = ih_empty(iter->target);
    iter->cursor -= steps;
    return 1;
}
_Bool ih_iter_go_to(intervalheap_iter *iter, size_t index)
{
    if (index >= ih_count(iter->target))
        return 0;
    if (iter->cursor > index)
        return ih_iter_rewind(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return ih_iter_advance(iter, index - iter->cursor);
    return 1;
}
size_t ih_iter_value(intervalheap_iter *iter)
{
    if (ih_empty(iter->target))
        return (size_t){0};
    return iter->target->buffer[iter->cursor / 2].data[iter->cursor % 2];
}
size_t ih_iter_index(intervalheap_iter *iter) { return iter->cursor; }
static void ih_impl_float_up_max(intervalheap *_heap_)
{
    size_t index = _heap_->size - 1;
    intervalheap_node *curr_node = &(_heap_->buffer[index]);
    while (index > 0)
    {
        size_t P_index = (index - 1) / 2;
        intervalheap_node *parent = &(_heap_->buffer[P_index]);
        if (index == _heap_->size - 1 && ih_count(_heap_) % 2 != 0)
        {
            if (_heap_->cmp(curr_node->data[0], parent->data[1]) < 0)
                break;
            size_t tmp = curr_node->data[0];
            curr_node->data[0] = parent->data[1];
            parent->data[1] = tmp;
        }
        else
        {
            if (_heap_->cmp(curr_node->data[1], parent->data[1]) < 0)
                break;
            size_t tmp = curr_node->data[1];
            curr_node->data[1] = parent->data[1];
            parent->data[1] = tmp;
        }
        index = P_index;
        curr_node = parent;
    }
}
static void ih_impl_float_up_min(intervalheap *_heap_)
{
    size_t index = _heap_->size - 1;
    intervalheap_node *curr_node = &(_heap_->buffer[index]);
    while (index > 0)
    {
        size_t P_index = (index - 1) / 2;
        intervalheap_node *parent = &(_heap_->buffer[P_index]);
        if (_heap_->cmp(curr_node->data[0], parent->data[0]) >= 0)
            break;
        size_t tmp = curr_node->data[0];
        curr_node->data[0] = parent->data[0];
        parent->data[0] = tmp;
        index = P_index;
        curr_node = parent;
    }
}
static void ih_impl_float_down_max(intervalheap *_heap_)
{
    size_t index = 0;
    intervalheap_node *curr_node = &(_heap_->buffer[index]);
    while (1)
    {
        if (2 * index + 1 >= _heap_->size)
            break;
        size_t child;
        size_t L_index = index * 2 + 1;
        size_t R_index = index * 2 + 2;
        if (R_index < _heap_->size)
        {
            intervalheap_node *L = &(_heap_->buffer[L_index]);
            intervalheap_node *R = &(_heap_->buffer[R_index]);
            if (R_index == _heap_->size - 1 && ih_count(_heap_) % 2 != 0)
                child = _heap_->cmp(L->data[1], R->data[0]) > 0 ? L_index : R_index;
            else
                child = _heap_->cmp(L->data[1], R->data[1]) > 0 ? L_index : R_index;
        }
        else
            child = L_index;
        intervalheap_node *child_node = &(_heap_->buffer[child]);
        if (child == _heap_->size - 1 && ih_count(_heap_) % 2 != 0)
        {
            if (_heap_->cmp(curr_node->data[1], child_node->data[0]) >= 0)
                break;
            size_t tmp = child_node->data[0];
            child_node->data[0] = curr_node->data[1];
            curr_node->data[1] = tmp;
        }
        else
        {
            if (_heap_->cmp(curr_node->data[1], child_node->data[1]) >= 0)
                break;
            size_t tmp = child_node->data[1];
            child_node->data[1] = curr_node->data[1];
            curr_node->data[1] = tmp;
            if (_heap_->cmp(child_node->data[0], child_node->data[1]) > 0)
            {
                tmp = child_node->data[0];
                child_node->data[0] = child_node->data[1];
                child_node->data[1] = tmp;
            }
        }
        index = child;
        curr_node = child_node;
    }
}
static void ih_impl_float_down_min(intervalheap *_heap_)
{
    size_t index = 0;
    intervalheap_node *curr_node = &(_heap_->buffer[index]);
    while (1)
    {
        if (2 * index + 1 >= _heap_->size)
            break;
        size_t child;
        size_t L_index = index * 2 + 1;
        size_t R_index = index * 2 + 2;
        if (R_index < _heap_->size)
        {
            intervalheap_node *L = &(_heap_->buffer[L_index]);
            intervalheap_node *R = &(_heap_->buffer[R_index]);
            child = _heap_->cmp(L->data[0], R->data[0]) < 0 ? L_index : R_index;
        }
        else
            child = L_index;
        intervalheap_node *child_node = &(_heap_->buffer[child]);
        if (_heap_->cmp(curr_node->data[0], child_node->data[0]) < 0)
            break;
        size_t tmp = child_node->data[0];
        child_node->data[0] = curr_node->data[0];
        curr_node->data[0] = tmp;
        if (child != _heap_->size - 1 || ih_count(_heap_) % 2 == 0)
        {
            if (_heap_->cmp(child_node->data[0], child_node->data[1]) > 0)
            {
                tmp = child_node->data[0];
                child_node->data[0] = child_node->data[1];
                child_node->data[1] = tmp;
            }
        }
        index = child;
        curr_node = child_node;
    }
}
static intervalheap_iter ih_impl_it_start(intervalheap *_heap_)
{
    intervalheap_iter iter;
    ih_iter_init(&iter, _heap_);
    ih_iter_to_start(&iter);
    return iter;
}
static intervalheap_iter ih_impl_it_end(intervalheap *_heap_)
{
    intervalheap_iter iter;
    ih_iter_init(&iter, _heap_);
    ih_iter_to_end(&iter);
    return iter;
}
