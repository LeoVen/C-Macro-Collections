#include <cmc/list.h>

//CMC_GENERATE_LIST(l, list, size_t)

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
    _Bool start;
    _Bool end;
} list_iter, *list_iter_ptr;
list *l_new(size_t capacity);
list *l_new_from(size_t *elements, size_t size);
void l_clear(list *_list_, void (*deallocator)(size_t));
void l_free(list *_list_, void (*deallocator)(size_t));
_Bool l_push_front(list *_list_, size_t element);
_Bool l_push_at(list *_list_, size_t element, size_t index);
_Bool l_push_back(list *_list_, size_t element);
_Bool l_pop_front(list *_list_);
_Bool l_pop_at(list *_list_, size_t index);
_Bool l_pop_back(list *_list_);
_Bool l_seq_push_front(list *_list_, size_t *elements, size_t size);
_Bool l_seq_push_at(list *_list_, size_t *elements, size_t size, size_t index);
_Bool l_seq_push_back(list *_list_, size_t *elements, size_t size);
_Bool l_seq_pop_at(list *_list_, size_t from, size_t to);
list *l_seq_sublist(list *_list_, size_t from, size_t to);
size_t l_front(list *_list_);
size_t l_get(list *_list_, size_t index);
size_t *l_get_ref(list *_list_, size_t index);
size_t l_back(list *_list_);
size_t l_indexof(list *_list_, size_t element, int (*comparator)(size_t, size_t), _Bool from_start);
_Bool l_contains(list *_list_, size_t element, int (*comparator)(size_t, size_t));
_Bool l_empty(list *_list_);
_Bool l_full(list *_list_);
size_t l_count(list *_list_);
_Bool l_fits(list *_list_, size_t size);
size_t l_capacity(list *_list_);
_Bool l_resize(list *_list_, size_t capacity);
list *l_copy_of(list *_list_, size_t (*copy_func)(size_t));
_Bool l_equals(list *_list1_, list *_list2_, int (*comparator)(size_t, size_t));
struct cmc_string l_to_string(list *_list_);
list_iter *l_iter_new(list *target);
void l_iter_free(list_iter *iter);
void l_iter_init(list_iter *iter, list *target);
_Bool l_iter_start(list_iter *iter);
_Bool l_iter_end(list_iter *iter);
void l_iter_to_start(list_iter *iter);
void l_iter_to_end(list_iter *iter);
_Bool l_iter_next(list_iter *iter);
_Bool l_iter_prev(list_iter *iter);
_Bool l_iter_advance(list_iter *iter, size_t steps);
_Bool l_iter_rewind(list_iter *iter, size_t steps);
_Bool l_iter_go_to(list_iter *iter, size_t index);
size_t l_iter_value(list_iter *iter);
size_t *l_iter_rvalue(list_iter *iter);
size_t l_iter_index(list_iter *iter);
static list_iter l_impl_it_start(list *_list_);
static list_iter l_impl_it_end(list *_list_);
list *l_new(size_t capacity)
{
    if (capacity < 1)
        return ((void *)0);
    list *_list_ = malloc(sizeof(list));
    if (!_list_)
        return ((void *)0);
    _list_->buffer = malloc(capacity * sizeof(size_t));
    if (!_list_->buffer)
    {
        free(_list_);
        return ((void *)0);
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
        return ((void *)0);
    list *_list_ = l_new(size + size / 2);
    if (!_list_)
        return ((void *)0);
    memcpy(_list_->buffer, elements, size * sizeof(size_t));
    _list_->count = size;
    return _list_;
}
void l_clear(list *_list_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _list_->count; i++)
            deallocator(_list_->buffer[i]);
    }
    memset(_list_->buffer, 0, sizeof(size_t) * _list_->capacity);
    _list_->count = 0;
}
void l_free(list *_list_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _list_->count; i++)
            deallocator(_list_->buffer[i]);
    }
    free(_list_->buffer);
    free(_list_);
}
_Bool l_push_front(list *_list_, size_t element)
{
    if (l_full(_list_))
    {
        if (!l_resize(_list_, l_count(_list_) * 2))
            return 0;
    }
    if (!l_empty(_list_))
    {
        memmove(_list_->buffer + 1, _list_->buffer, _list_->count * sizeof(size_t));
    }
    _list_->buffer[0] = element;
    _list_->count++;
    return 1;
}
_Bool l_push_at(list *_list_, size_t element, size_t index)
{
    if (index > _list_->count)
        return 0;
    if (l_full(_list_))
    {
        if (!l_resize(_list_, l_count(_list_) * 2))
            return 0;
    }
    memmove(_list_->buffer + index + 1, _list_->buffer + index, (_list_->count - index) * sizeof(size_t));
    _list_->buffer[index] = element;
    _list_->count++;
    return 1;
}
_Bool l_push_back(list *_list_, size_t element)
{
    if (l_full(_list_))
    {
        if (!l_resize(_list_, l_count(_list_) * 2))
            return 0;
    }
    _list_->buffer[_list_->count++] = element;
    return 1;
}
_Bool l_pop_front(list *_list_)
{
    if (l_empty(_list_))
        return 0;
    memmove(_list_->buffer, _list_->buffer + 1, _list_->count * sizeof(size_t));
    _list_->buffer[--_list_->count] = (size_t){0};
    return 1;
}
_Bool l_pop_at(list *_list_, size_t index)
{
    if (index >= _list_->count)
        return 0;
    memmove(_list_->buffer + index, _list_->buffer + index + 1, (_list_->count - index) * sizeof(size_t));
    _list_->buffer[--_list_->count] = (size_t){0};
    return 1;
}
_Bool l_pop_back(list *_list_)
{
    if (l_empty(_list_))
        return 0;
    _list_->buffer[--_list_->count] = (size_t){0};
    return 1;
}
_Bool l_seq_push_front(list *_list_, size_t *elements, size_t size)
{
    if (size == 0)
        return 0;
    if (!l_fits(_list_, size))
    {
        if (!l_resize(_list_, l_count(_list_) + size))
            return 0;
    }
    memmove(_list_->buffer + size, _list_->buffer, _list_->count * sizeof(size_t));
    memcpy(_list_->buffer, elements, size * sizeof(size_t));
    _list_->count += size;
    return 1;
}
_Bool l_seq_push_at(list *_list_, size_t *elements, size_t size, size_t index)
{
    if (size == 0 || index > _list_->count)
        return 0;
    if (index == 0)
        return l_seq_push_front(_list_, elements, size);
    else if (index == _list_->count)
        return l_seq_push_back(_list_, elements, size);
    else
    {
        if (!l_fits(_list_, size))
        {
            if (!l_resize(_list_, l_count(_list_) + size))
                return 0;
        }
        memmove(_list_->buffer + index + size, _list_->buffer + index, (_list_->count - index) * sizeof(size_t));
        memcpy(_list_->buffer + index, elements, size * sizeof(size_t));
        _list_->count += size;
    }
    return 1;
}
_Bool l_seq_push_back(list *_list_, size_t *elements, size_t size)
{
    if (size == 0)
        return 0;
    if (!l_fits(_list_, size))
    {
        if (!l_resize(_list_, _list_->count + size))
            return 0;
    }
    memcpy(_list_->buffer + _list_->count, elements, size * sizeof(size_t));
    _list_->count += size;
    return 1;
}
_Bool l_seq_pop_at(list *_list_, size_t from, size_t to)
{
    if (from > to || to >= _list_->count)
        return 0;
    size_t length = (to - from + 1);
    memmove(_list_->buffer + from, _list_->buffer + to + 1, (_list_->count - to - 1) * sizeof(size_t));
    memset(_list_->buffer + _list_->count - length, 0, length * sizeof(size_t));
    _list_->count -= to - from + 1;
    return 1;
}
list *l_seq_sublist(list *_list_, size_t from, size_t to)
{
    if (from > to || to >= _list_->count)
        return 0;
    size_t length = to - from + 1;
    list *result = l_new(length);
    if (!result)
        return ((void *)0);
    memcpy(result->buffer, _list_->buffer, _list_->count * sizeof(size_t));
    memmove(_list_->buffer + from, _list_->buffer + to + 1, (_list_->count - to - 1) * sizeof(size_t));
    memset(_list_->buffer + _list_->count - length, 0, length * sizeof(size_t));
    _list_->count -= length;
    result->count = length;
    return result;
}
size_t l_front(list *_list_)
{
    if (l_empty(_list_))
        return (size_t){0};
    return _list_->buffer[0];
}
size_t l_get(list *_list_, size_t index)
{
    if (index >= _list_->count || l_empty(_list_))
        return (size_t){0};
    return _list_->buffer[index];
}
size_t *l_get_ref(list *_list_, size_t index)
{
    if (index >= _list_->count)
        return ((void *)0);
    if (l_empty(_list_))
        return ((void *)0);
    return &(_list_->buffer[index]);
}
size_t l_back(list *_list_)
{
    if (l_empty(_list_))
        return (size_t){0};
    return _list_->buffer[_list_->count - 1];
}
size_t l_indexof(list *_list_, size_t element, int (*comparator)(size_t, size_t), _Bool from_start)
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
_Bool l_contains(list *_list_, size_t element, int (*comparator)(size_t, size_t))
{
    for (size_t i = 0; i < _list_->count; i++)
    {
        if (comparator(_list_->buffer[i], element) == 0)
            return 1;
    }
    return 0;
}
_Bool l_empty(list *_list_) { return _list_->count == 0; }
_Bool l_full(list *_list_) { return _list_->count >= _list_->capacity; }
size_t l_count(list *_list_) { return _list_->count; }
_Bool l_fits(list *_list_, size_t size) { return _list_->count + size <= _list_->capacity; }
size_t l_capacity(list *_list_) { return _list_->capacity; }
_Bool l_resize(list *_list_, size_t capacity)
{
    if (l_capacity(_list_) == capacity)
        return 1;
    if (capacity < l_count(_list_))
        return 0;
    size_t *new_buffer = realloc(_list_->buffer, sizeof(size_t) * capacity);
    if (!new_buffer)
        return 0;
    _list_->buffer = new_buffer;
    _list_->capacity = capacity;
    return 1;
}
list *l_copy_of(list *_list_, size_t (*copy_func)(size_t))
{
    list *result = l_new(_list_->capacity);
    if (!result)
        return ((void *)0);
    if (copy_func)
    {
        for (size_t i = 0; i < _list_->count; i++)
            result->buffer[i] = copy_func(_list_->buffer[i]);
    }
    else
        memcpy(result->buffer, _list_->buffer, sizeof(size_t) * _list_->count);
    result->count = _list_->count;
    return result;
}
_Bool l_equals(list *_list1_, list *_list2_, int (*comparator)(size_t, size_t))
{
    if (l_count(_list1_) != l_count(_list2_))
        return 0;
    for (size_t i = 0; i < l_count(_list1_); i++)
    {
        if (comparator(_list1_->buffer[i], _list2_->buffer[i]) != 0)
            return 0;
    }
    return 0;
}
struct cmc_string l_to_string(list *_list_)
{
    struct cmc_string str;
    list *l_ = _list_;
    const char *name = "list";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_list, name, l_, l_->buffer, l_->capacity, l_->count);
    return str;
}
list_iter *l_iter_new(list *target)
{
    list_iter *iter = malloc(sizeof(list_iter));
    if (!iter)
        return ((void *)0);
    l_iter_init(iter, target);
    return iter;
}
void l_iter_free(list_iter *iter) { free(iter); }
void l_iter_init(list_iter *iter, list *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->start = 1;
    iter->end = l_empty(target);
}
_Bool l_iter_start(list_iter *iter) { return l_empty(iter->target) || iter->start; }
_Bool l_iter_end(list_iter *iter) { return l_empty(iter->target) || iter->end; }
void l_iter_to_start(list_iter *iter)
{
    if (!l_empty(iter->target))
    {
        iter->cursor = 0;
        iter->start = 1;
        iter->end = l_empty(iter->target);
    }
}
void l_iter_to_end(list_iter *iter)
{
    if (!l_empty(iter->target))
    {
        iter->start = l_empty(iter->target);
        iter->cursor = l_empty(iter->target) ? 0 : iter->target->count - 1;
        iter->end = 1;
    }
}
_Bool l_iter_next(list_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == l_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    iter->start = l_empty(iter->target);
    iter->cursor++;
    return 1;
}
_Bool l_iter_prev(list_iter *iter)
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
_Bool l_iter_advance(list_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor + steps >= l_count(iter->target))
        return 0;
    iter->start = l_empty(iter->target);
    iter->cursor += steps;
    return 1;
}
_Bool l_iter_rewind(list_iter *iter, size_t steps)
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
_Bool l_iter_go_to(list_iter *iter, size_t index)
{
    if (index >= l_count(iter->target))
        return 0;
    if (iter->cursor > index)
        return l_iter_rewind(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return l_iter_advance(iter, index - iter->cursor);
    return 1;
}
size_t l_iter_value(list_iter *iter)
{
    if (l_empty(iter->target))
        return (size_t){0};
    return iter->target->buffer[iter->cursor];
}
size_t *l_iter_rvalue(list_iter *iter)
{
    if (l_empty(iter->target))
        return ((void *)0);
    return &(iter->target->buffer[iter->cursor]);
}
size_t l_iter_index(list_iter *iter) { return iter->cursor; }
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
