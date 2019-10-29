#include <cmc/sortedlist.h>

//CMC_GENERATE_SORTEDLIST(sl, sortedlist, size_t)

typedef struct sortedlist_s
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    _Bool is_sorted;
    int (*cmp)(size_t, size_t);
    struct sortedlist_iter_s (*it_start)(struct sortedlist_s *);
    struct sortedlist_iter_s (*it_end)(struct sortedlist_s *);
} sortedlist, *sortedlist_ptr;
typedef struct sortedlist_iter_s
{
    struct sortedlist_s *target;
    size_t cursor;
    _Bool start;
    _Bool end;
} sortedlist_iter, *sortedlist_iter_ptr;
sortedlist *sl_new(size_t capacity, int (*compare)(size_t, size_t));
void sl_clear(sortedlist *_list_, void (*deallocator)(size_t));
void sl_free(sortedlist *_list_, void (*deallocator)(size_t));
_Bool sl_insert(sortedlist *_list_, size_t element);
_Bool sl_remove(sortedlist *_list_, size_t index);
_Bool sl_max(sortedlist *_list_, size_t *result);
_Bool sl_min(sortedlist *_list_, size_t *result);
size_t sl_get(sortedlist *_list_, size_t index);
size_t sl_indexof(sortedlist *_list_, size_t element, _Bool from_start);
_Bool sl_contains(sortedlist *_list_, size_t element);
_Bool sl_empty(sortedlist *_list_);
_Bool sl_full(sortedlist *_list_);
size_t sl_count(sortedlist *_list_);
size_t sl_capacity(sortedlist *_list_);
_Bool sl_resize(sortedlist *_list_, size_t capacity);
void sl_sort(sortedlist *_list_);
sortedlist *sl_copy_of(sortedlist *_list_, size_t (*copy_func)(size_t));
_Bool sl_equals(sortedlist *_list1_, sortedlist *_list2_);
struct cmc_string sl_to_string(sortedlist *_list_);
sortedlist_iter *sl_iter_new(sortedlist *target);
void sl_iter_free(sortedlist_iter *iter);
void sl_iter_init(sortedlist_iter *iter, sortedlist *target);
_Bool sl_iter_start(sortedlist_iter *iter);
_Bool sl_iter_end(sortedlist_iter *iter);
void sl_iter_to_start(sortedlist_iter *iter);
void sl_iter_to_end(sortedlist_iter *iter);
_Bool sl_iter_next(sortedlist_iter *iter);
_Bool sl_iter_prev(sortedlist_iter *iter);
_Bool sl_iter_advance(sortedlist_iter *iter, size_t steps);
_Bool sl_iter_rewind(sortedlist_iter *iter, size_t steps);
_Bool sl_iter_go_to(sortedlist_iter *iter, size_t index);
size_t sl_iter_value(sortedlist_iter *iter);
size_t sl_iter_index(sortedlist_iter *iter);
static size_t sl_impl_binary_search_first(sortedlist *_list_, size_t value);
static size_t sl_impl_binary_search_last(sortedlist *_list_, size_t value);
void sl_impl_sort_quicksort(size_t *array, int (*cmp)(size_t, size_t), size_t low, size_t high);
void sl_impl_sort_insertion(size_t *array, int (*cmp)(size_t, size_t), size_t low, size_t high);
static sortedlist_iter sl_impl_it_start(sortedlist *_list_);
static sortedlist_iter sl_impl_it_end(sortedlist *_list_);
sortedlist *sl_new(size_t capacity, int (*compare)(size_t, size_t))
{
    if (capacity < 1)
        return ((void *)0);
    sortedlist *_list_ = malloc(sizeof(sortedlist));
    if (!_list_)
        return ((void *)0);
    _list_->buffer = calloc(capacity, sizeof(size_t));
    if (!_list_->buffer)
    {
        free(_list_);
        return ((void *)0);
    }
    _list_->capacity = capacity;
    _list_->count = 0;
    _list_->cmp = compare;
    _list_->is_sorted = 0;
    _list_->it_start = sl_impl_it_start;
    _list_->it_end = sl_impl_it_end;
    return _list_;
}
void sl_clear(sortedlist *_list_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _list_->count; i++)
            deallocator(_list_->buffer[i]);
    }
    memset(_list_->buffer, 0, sizeof(size_t) * _list_->capacity);
    _list_->count = 0;
}
void sl_free(sortedlist *_list_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _list_->count; i++)
            deallocator(_list_->buffer[i]);
    }
    free(_list_->buffer);
    free(_list_);
}
_Bool sl_insert(sortedlist *_list_, size_t element)
{
    if (sl_full(_list_))
    {
        if (!sl_resize(_list_, sl_capacity(_list_) * 2))
            return 0;
    }
    _list_->buffer[_list_->count++] = element;
    _list_->is_sorted = 0;
    return 1;
}
_Bool sl_remove(sortedlist *_list_, size_t index)
{
    if (index >= _list_->count)
        return 0;
    memmove(_list_->buffer + index, _list_->buffer + index + 1, (_list_->count - index) * sizeof(size_t));
    _list_->buffer[--_list_->count] = (size_t){0};
    return 1;
}
_Bool sl_min(sortedlist *_list_, size_t *result)
{
    if (sl_empty(_list_))
        return 0;
    sl_sort(_list_);
    *result = _list_->buffer[0];
    return 1;
}
_Bool sl_max(sortedlist *_list_, size_t *result)
{
    if (sl_empty(_list_))
        return 0;
    sl_sort(_list_);
    *result = _list_->buffer[_list_->count - 1];
    return 1;
}
size_t sl_get(sortedlist *_list_, size_t index)
{
    if (index >= _list_->count)
        return (size_t){0};
    sl_sort(_list_);
    return _list_->buffer[index];
}
size_t sl_indexof(sortedlist *_list_, size_t element, _Bool from_start)
{
    sl_sort(_list_);
    if (from_start)
    {
        return sl_impl_binary_search_first(_list_, element);
    }
    return sl_impl_binary_search_last(_list_, element);
}
_Bool sl_contains(sortedlist *_list_, size_t element)
{
    if (sl_empty(_list_))
        return 0;
    sl_sort(_list_);
    return sl_impl_binary_search_first(_list_, element) < sl_count(_list_);
}
_Bool sl_empty(sortedlist *_list_) { return _list_->count == 0; }
_Bool sl_full(sortedlist *_list_) { return _list_->count >= _list_->capacity; }
size_t sl_count(sortedlist *_list_) { return _list_->count; }
size_t sl_capacity(sortedlist *_list_) { return _list_->capacity; }
_Bool sl_resize(sortedlist *_list_, size_t capacity)
{
    if (sl_capacity(_list_) == capacity)
        return 1;
    if (capacity < sl_count(_list_))
        return 0;
    size_t *new_buffer = realloc(_list_->buffer, sizeof(size_t) * capacity);
    if (!new_buffer)
        return 0;
    _list_->buffer = new_buffer;
    _list_->capacity = capacity;
    return 1;
}
void sl_sort(sortedlist *_list_)
{
    if (!_list_->is_sorted && _list_->count > 1)
    {
        sl_impl_sort_quicksort(_list_->buffer, _list_->cmp, 0, _list_->count - 1);
        _list_->is_sorted = 1;
    }
}
sortedlist *sl_copy_of(sortedlist *_list_, size_t (*copy_func)(size_t))
{
    sortedlist *result = sl_new(_list_->capacity, _list_->cmp);
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
_Bool sl_equals(sortedlist *_list1_, sortedlist *_list2_)
{
    if (sl_count(_list1_) != sl_count(_list2_))
        return 0;
    for (size_t i = 0; i < sl_count(_list1_); i++)
    {
        if (_list1_->cmp(_list1_->buffer[i], _list2_->buffer[i]) != 0)
            return 0;
    }
    return 0;
}
struct cmc_string sl_to_string(sortedlist *_list_)
{
    struct cmc_string str;
    sortedlist *l_ = _list_;
    const char *name = "sortedlist";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_sortedlist, name, l_, l_->buffer, l_->capacity, l_->count, l_->is_sorted ? "true" : "false", l_->cmp);
    return str;
}
sortedlist_iter *sl_iter_new(sortedlist *target)
{
    sortedlist_iter *iter = malloc(sizeof(sortedlist_iter));
    if (!iter)
        return ((void *)0);
    sl_iter_init(iter, target);
    return iter;
}
void sl_iter_free(sortedlist_iter *iter) { free(iter); }
void sl_iter_init(sortedlist_iter *iter, sortedlist *target)
{
    sl_sort(target);
    iter->target = target;
    iter->cursor = 0;
    iter->start = 1;
    iter->end = sl_empty(target);
}
_Bool sl_iter_start(sortedlist_iter *iter) { return sl_empty(iter->target) || iter->start; }
_Bool sl_iter_end(sortedlist_iter *iter) { return sl_empty(iter->target) || iter->end; }
void sl_iter_to_start(sortedlist_iter *iter)
{
    if (!sl_empty(iter->target))
    {
        iter->cursor = 0;
        iter->start = 1;
        iter->end = sl_empty(iter->target);
    }
}
void sl_iter_to_end(sortedlist_iter *iter)
{
    if (!sl_empty(iter->target))
    {
        iter->start = sl_empty(iter->target);
        iter->cursor = sl_empty(iter->target) ? 0 : iter->target->count - 1;
        iter->end = 1;
    }
}
_Bool sl_iter_next(sortedlist_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == sl_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    iter->start = sl_empty(iter->target);
    iter->cursor++;
    return 1;
}
_Bool sl_iter_prev(sortedlist_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = sl_empty(iter->target);
    iter->cursor--;
    return 1;
}
_Bool sl_iter_advance(sortedlist_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->cursor + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor + steps >= sl_count(iter->target))
        return 0;
    iter->start = sl_empty(iter->target);
    iter->cursor += steps;
    return 1;
}
_Bool sl_iter_rewind(sortedlist_iter *iter, size_t steps)
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
    iter->end = sl_empty(iter->target);
    iter->cursor -= steps;
    return 1;
}
_Bool sl_iter_go_to(sortedlist_iter *iter, size_t index)
{
    if (index >= sl_count(iter->target))
        return 0;
    if (iter->cursor > index)
        return sl_iter_rewind(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return sl_iter_advance(iter, index - iter->cursor);
    return 1;
}
size_t sl_iter_value(sortedlist_iter *iter)
{
    if (sl_empty(iter->target))
        return (size_t){0};
    return iter->target->buffer[iter->cursor];
}
size_t sl_iter_index(sortedlist_iter *iter) { return iter->cursor; }
static size_t sl_impl_binary_search_first(sortedlist *_list_, size_t value)
{
    if (sl_empty(_list_))
        return 1;
    size_t L = 0;
    size_t R = sl_count(_list_);
    while (L < R)
    {
        size_t M = L + (R - L) / 2;
        if (_list_->cmp(_list_->buffer[M], value) < 0)
            L = M + 1;
        else
            R = M;
    }
    if (_list_->cmp(_list_->buffer[L], value) == 0)
        return L;
    return sl_count(_list_);
}
static size_t sl_impl_binary_search_last(sortedlist *_list_, size_t value)
{
    if (sl_empty(_list_))
        return 1;
    size_t L = 0;
    size_t R = sl_count(_list_);
    while (L < R)
    {
        size_t M = L + (R - L) / 2;
        if (_list_->cmp(_list_->buffer[M], value) > 0)
            R = M;
        else
            L = M + 1;
    }
    if (_list_->cmp(_list_->buffer[L - 1], value) == 0)
        return L - 1;
    return sl_count(_list_);
}
void sl_impl_sort_quicksort(size_t *array, int (*cmp)(size_t, size_t), size_t low, size_t high)
{
    while (low < high)
    {
        if (high - low < 10)
        {
            sl_impl_sort_insertion(array, cmp, low, high);
            break;
        }
        else
        {
            size_t pivot = array[high];
            size_t pindex = low;
            for (size_t i = low; i < high; i++)
            {
                if (cmp(array[i], pivot) <= 0)
                {
                    size_t _tmp_ = array[i];
                    array[i] = array[pindex];
                    array[pindex] = _tmp_;
                    pindex++;
                }
            }
            size_t _tmp_ = array[pindex];
            array[pindex] = array[high];
            array[high] = _tmp_;
            if (pindex - low < high - pindex)
            {
                sl_impl_sort_quicksort(array, cmp, low, pindex - 1);
                low = pindex + 1;
            }
            else
            {
                sl_impl_sort_quicksort(array, cmp, pindex + 1, high);
                high = pindex - 1;
            }
        }
    }
}
void sl_impl_sort_insertion(size_t *array, int (*cmp)(size_t, size_t), size_t low, size_t high)
{
    for (size_t i = low + 1; i <= high; i++)
    {
        size_t _tmp_ = array[i];
        size_t j = i;
        while (j > low && cmp(array[j - 1], _tmp_) > 0)
        {
            array[j] = array[j - 1];
            j--;
        }
        array[j] = _tmp_;
    }
}
static sortedlist_iter sl_impl_it_start(sortedlist *_list_)
{
    sortedlist_iter iter;
    sl_iter_init(&iter, _list_);
    sl_iter_to_start(&iter);
    return iter;
}
static sortedlist_iter sl_impl_it_end(sortedlist *_list_)
{
    sortedlist_iter iter;
    sl_iter_init(&iter, _list_);
    sl_iter_to_end(&iter);
    return iter;
}
