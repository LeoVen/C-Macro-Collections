#include <cmc/multiset.h>

//CMC_GENERATE_MULTISET(ms, multiset, size_t)

typedef struct multiset_s
{
    struct multiset_entry_s *buffer;
    size_t capacity;
    size_t count;
    size_t cardinality;
    double load;
    int (*cmp)(size_t, size_t);
    size_t (*hash)(size_t);
    struct multiset_iter_s (*it_start)(struct multiset_s *);
    struct multiset_iter_s (*it_end)(struct multiset_s *);
} multiset, *multiset_ptr;
typedef struct multiset_entry_s
{
    size_t value;
    size_t multiplicity;
    size_t dist;
    enum cmc_entry_state state;
} multiset_entry, *multiset_entry_ptr;
typedef struct multiset_iter_s
{
    struct multiset_s *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
} multiset_iter, *multiset_iter_ptr;
multiset *ms_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t));
void ms_clear(multiset *_set_, void (*deallocator)(size_t));
void ms_free(multiset *_set_, void (*deallocator)(size_t));
_Bool ms_insert(multiset *_set_, size_t element);
_Bool ms_insert_many(multiset *_set_, size_t element, size_t count);
_Bool ms_update(multiset *_set_, size_t element, size_t multiplicity);
_Bool ms_remove(multiset *_set_, size_t element);
size_t ms_remove_all(multiset *_set_, size_t element);
_Bool ms_max(multiset *_set_, size_t *value);
_Bool ms_min(multiset *_set_, size_t *value);
size_t ms_multiplicity_of(multiset *_set_, size_t element);
_Bool ms_contains(multiset *_set_, size_t element);
_Bool ms_empty(multiset *_set_);
_Bool ms_full(multiset *_set_);
size_t ms_count(multiset *_set_);
size_t ms_cardinality(multiset *_set_);
size_t ms_capacity(multiset *_set_);
double ms_load(multiset *_set_);
_Bool ms_resize(multiset *_set_, size_t capacity);
multiset *ms_copy_of(multiset *_set_, size_t (*copy_func)(size_t));
_Bool ms_equals(multiset *_set1_, multiset *_set2_, _Bool ignore_multiplicity);
struct cmc_string ms_to_string(multiset *_set_);
multiset *ms_union(multiset *_set1_, multiset *_set2_);
multiset *ms_intersection(multiset *_set1_, multiset *_set2_);
multiset *ms_difference(multiset *_set1_, multiset *_set2_);
multiset *ms_summation(multiset *_set1_, multiset *_set2_);
multiset *ms_symmetric_difference(multiset *_set1_, multiset *_set2_);
_Bool ms_is_subset(multiset *_set1_, multiset *_set2_);
_Bool ms_is_superset(multiset *_set1_, multiset *_set2_);
_Bool ms_is_proper_subset(multiset *_set1_, multiset *_set2_);
_Bool ms_is_proper_superset(multiset *_set1_, multiset *_set2_);
_Bool ms_is_disjointset(multiset *_set1_, multiset *_set2_);
multiset_iter *ms_iter_new(multiset *target);
void ms_iter_free(multiset_iter *iter);
void ms_iter_init(multiset_iter *iter, multiset *target);
_Bool ms_iter_start(multiset_iter *iter);
_Bool ms_iter_end(multiset_iter *iter);
void ms_iter_to_start(multiset_iter *iter);
void ms_iter_to_end(multiset_iter *iter);
_Bool ms_iter_next(multiset_iter *iter);
_Bool ms_iter_prev(multiset_iter *iter);
_Bool ms_iter_advance(multiset_iter *iter, size_t steps);
_Bool ms_iter_rewind(multiset_iter *iter, size_t steps);
_Bool ms_iter_go_to(multiset_iter *iter, size_t index);
size_t ms_iter_value(multiset_iter *iter);
size_t ms_iter_multiplicity(multiset_iter *iter);
size_t ms_iter_index(multiset_iter *iter);
static multiset_entry *ms_impl_insert_and_return(multiset *_set_, size_t element, _Bool *new_node);
static multiset_entry *ms_impl_get_entry(multiset *_set_, size_t element);
static size_t ms_impl_calculate_size(size_t required);
static multiset_iter ms_impl_it_start(multiset *_set_);
static multiset_iter ms_impl_it_end(multiset *_set_);
multiset *ms_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t))
{
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffff * load)
        return ((void *)0);
    size_t real_capacity = ms_impl_calculate_size(capacity / load);
    multiset *_set_ = malloc(sizeof(multiset));
    if (!_set_)
        return ((void *)0);
    _set_->buffer = malloc(sizeof(multiset_entry) * real_capacity);
    if (!_set_->buffer)
    {
        free(_set_);
        return ((void *)0);
    }
    memset(_set_->buffer, 0, sizeof(multiset_entry) * real_capacity);
    _set_->count = 0;
    _set_->cardinality = 0;
    _set_->capacity = real_capacity;
    _set_->load = load;
    _set_->cmp = compare;
    _set_->hash = hash;
    _set_->it_start = ms_impl_it_start;
    _set_->it_end = ms_impl_it_end;
    return _set_;
}
void ms_clear(multiset *_set_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            multiset_entry *entry = &(_set_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                deallocator(entry->value);
            }
        }
    }
    memset(_set_->buffer, 0, sizeof(multiset_entry) * _set_->capacity);
    _set_->count = 0;
}
void ms_free(multiset *_set_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            multiset_entry *entry = &(_set_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                deallocator(entry->value);
            }
        }
    }
    free(_set_->buffer);
    free(_set_);
}
_Bool ms_insert(multiset *_set_, size_t element)
{
    _Bool new_node;
    multiset_entry *entry = ms_impl_insert_and_return(_set_, element, &new_node);
    if (!entry)
        return 0;
    if (!new_node)
        entry->multiplicity++;
    _set_->cardinality++;
    return 1;
}
_Bool ms_insert_many(multiset *_set_, size_t element, size_t count)
{
    if (count == 0)
        return 1;
    _Bool new_node;
    multiset_entry *entry = ms_impl_insert_and_return(_set_, element, &new_node);
    if (!entry)
        return 0;
    if (new_node)
        entry->multiplicity = count;
    else
        entry->multiplicity += count;
    _set_->cardinality += count;
    return 1;
}
_Bool ms_update(multiset *_set_, size_t element, size_t multiplicity)
{
    if (multiplicity == 0)
        return ms_remove_all(_set_, element);
    _Bool new_node;
    multiset_entry *entry = ms_impl_insert_and_return(_set_, element, &new_node);
    if (!entry)
        return 0;
    if (new_node)
        _set_->cardinality++;
    _set_->cardinality = (_set_->cardinality - entry->multiplicity) + multiplicity;
    entry->multiplicity = multiplicity;
    return 1;
}
_Bool ms_remove(multiset *_set_, size_t element)
{
    multiset_entry *result = ms_impl_get_entry(_set_, element);
    if (result == ((void *)0))
        return 0;
    if (result->multiplicity > 1)
        result->multiplicity--;
    else
    {
        result->value = (size_t){0};
        result->multiplicity = 0;
        result->dist = 0;
        result->state = CMC_ES_DELETED;
        _set_->count--;
    }
    _set_->cardinality--;
    return 1;
}
size_t ms_remove_all(multiset *_set_, size_t element)
{
    multiset_entry *result = ms_impl_get_entry(_set_, element);
    if (result == ((void *)0))
        return 0;
    size_t removed = result->multiplicity;
    result->value = (size_t){0};
    result->multiplicity = 0;
    result->dist = 0;
    result->state = CMC_ES_DELETED;
    _set_->count--;
    _set_->cardinality -= removed;
    return removed;
}
_Bool ms_max(multiset *_set_, size_t *value)
{
    if (ms_empty(_set_))
        return 0;
    multiset_iter iter;
    for (ms_iter_init(&iter, _set_); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t result = ms_iter_value(&iter);
        size_t index = ms_iter_index(&iter);
        if (index == 0)
            *value = result;
        else if (_set_->cmp(result, *value) > 0)
            *value = result;
    }
    return 1;
}
_Bool ms_min(multiset *_set_, size_t *value)
{
    if (ms_empty(_set_))
        return 0;
    multiset_iter iter;
    for (ms_iter_init(&iter, _set_); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t result = ms_iter_value(&iter);
        size_t index = ms_iter_index(&iter);
        if (index == 0)
            *value = result;
        else if (_set_->cmp(result, *value) < 0)
            *value = result;
    }
    return 1;
}
size_t ms_multiplicity_of(multiset *_set_, size_t element)
{
    multiset_entry *entry = ms_impl_get_entry(_set_, element);
    if (!entry)
        return 0;
    return entry->multiplicity;
}
_Bool ms_contains(multiset *_set_, size_t element) { return ms_impl_get_entry(_set_, element) != ((void *)0); }
_Bool ms_empty(multiset *_set_) { return _set_->count == 0; }
_Bool ms_full(multiset *_set_) { return (double)ms_capacity(_set_) * ms_load(_set_) <= (double)ms_count(_set_); }
size_t ms_count(multiset *_set_) { return _set_->count; }
size_t ms_cardinality(multiset *_set_) { return _set_->cardinality; }
size_t ms_capacity(multiset *_set_) { return _set_->capacity; }
double ms_load(multiset *_set_) { return _set_->load; }
_Bool ms_resize(multiset *_set_, size_t capacity)
{
    if (ms_capacity(_set_) == capacity)
        return 1;
    if (ms_capacity(_set_) > capacity / ms_load(_set_))
        return 1;
    if (capacity >= 0xffffffffffffffff * ms_load(_set_))
        return 0;
    size_t theoretical_size = ms_impl_calculate_size(capacity);
    if (theoretical_size < ms_count(_set_) / ms_load(_set_))
        return 0;
    multiset *_new_set_ = ms_new(capacity, ms_load(_set_), _set_->cmp, _set_->hash);
    if (!_new_set_)
        return 0;
    multiset_iter iter;
    for (ms_iter_init(&iter, _set_); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        ms_insert_many(_new_set_, ms_iter_value(&iter), ms_iter_multiplicity(&iter));
    }
    if (ms_count(_set_) != ms_count(_new_set_))
    {
        ms_free(_new_set_, ((void *)0));
        return 0;
    }
    multiset_entry *tmp_b = _set_->buffer;
    _set_->buffer = _new_set_->buffer;
    _new_set_->buffer = tmp_b;
    size_t tmp_c = _set_->capacity;
    _set_->capacity = _new_set_->capacity;
    _new_set_->capacity = tmp_c;
    ms_free(_new_set_, ((void *)0));
    return 1;
}
multiset *ms_copy_of(multiset *_set_, size_t (*copy_func)(size_t))
{
    multiset *result = ms_new(_set_->capacity, _set_->load, _set_->cmp, _set_->hash);
    if (!result)
        return ((void *)0);
    if (copy_func)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            multiset_entry *scan = &(_set_->buffer[i]);
            if (scan->state != CMC_ES_EMPTY)
            {
                multiset_entry *target = &(result->buffer[i]);
                if (scan->state == CMC_ES_DELETED)
                    target->state = CMC_ES_DELETED;
                else
                {
                    target->state = scan->state;
                    target->dist = scan->dist;
                    target->multiplicity = scan->multiplicity;
                    target->value = copy_func(scan->value);
                }
            }
        }
    }
    else
        memcpy(result->buffer, _set_->buffer, sizeof(multiset_entry) * _set_->capacity);
    result->count = _set_->count;
    result->cardinality = _set_->cardinality;
    return result;
}
_Bool ms_equals(multiset *_set1_, multiset *_set2_, _Bool ignore_multiplicity)
{
    if (ms_count(_set1_) != ms_count(_set2_))
        return 0;
    if (!ignore_multiplicity && ms_cardinality(_set1_) != ms_cardinality(_set2_))
        return 0;
    if (ms_count(_set1_) == 0)
        return 1;
    multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        multiset_entry *entry = ms_impl_get_entry(_set2_, ms_iter_value(&iter));
        if (entry == ((void *)0))
            return 0;
        if (!ignore_multiplicity && entry->multiplicity != ms_iter_multiplicity(&iter))
            return 0;
    }
    return 1;
}
struct cmc_string ms_to_string(multiset *_set_)
{
    struct cmc_string str;
    multiset *s_ = _set_;
    const char *name = "multiset";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_multiset, name, s_, s_->buffer, s_->capacity, s_->count, s_->cardinality, s_->load, s_->cmp, s_->hash);
    return str;
}
multiset *ms_union(multiset *_set1_, multiset *_set2_)
{
    multiset *_set_r_ = ms_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return ((void *)0);
    multiset_iter iter1, iter2;
    ms_iter_init(&iter1, _set1_);
    ms_iter_init(&iter2, _set2_);
    for (ms_iter_to_start(&iter1); !ms_iter_end(&iter1); ms_iter_next(&iter1))
    {
        size_t value = ms_iter_value(&iter1);
        size_t m1 = ms_iter_multiplicity(&iter1);
        size_t m2 = ms_multiplicity_of(_set2_, value);
        size_t max_ = m1 > m2 ? m1 : m2;
        ms_update(_set_r_, value, max_);
    }
    for (ms_iter_to_start(&iter2); !ms_iter_end(&iter2); ms_iter_next(&iter2))
    {
        size_t value = ms_iter_value(&iter2);
        size_t m1 = ms_multiplicity_of(_set1_, value);
        size_t m2 = ms_iter_multiplicity(&iter2);
        size_t max_ = m1 > m2 ? m1 : m2;
        ms_update(_set_r_, value, max_);
    }
    return _set_r_;
}
multiset *ms_intersection(multiset *_set1_, multiset *_set2_)
{
    multiset *_set_r_ = ms_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return ((void *)0);
    multiset *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;
    multiset *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;
    multiset_iter iter;
    ms_iter_init(&iter, _set_A_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        size_t m1 = ms_iter_multiplicity(&iter);
        size_t m2 = ms_multiplicity_of(_set_B_, value);
        size_t max_ = m1 < m2 ? m1 : m2;
        ms_update(_set_r_, value, max_);
    }
    return _set_r_;
}
multiset *ms_difference(multiset *_set1_, multiset *_set2_)
{
    multiset *_set_r_ = ms_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return ((void *)0);
    multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        size_t m1 = ms_iter_multiplicity(&iter);
        size_t m2 = ms_multiplicity_of(_set2_, value);
        if (m1 > m2)
            ms_update(_set_r_, value, m1 - m2);
    }
    return _set_r_;
}
multiset *ms_summation(multiset *_set1_, multiset *_set2_)
{
    multiset *_set_r_ = ms_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return ((void *)0);
    multiset_iter iter1, iter2;
    ms_iter_init(&iter1, _set1_);
    ms_iter_init(&iter2, _set2_);
    for (ms_iter_to_start(&iter1); !ms_iter_end(&iter1); ms_iter_next(&iter1))
    {
        ms_insert_many(_set_r_, ms_iter_value(&iter1), ms_iter_multiplicity(&iter1));
    }
    for (ms_iter_to_start(&iter2); !ms_iter_end(&iter2); ms_iter_next(&iter2))
    {
        ms_insert_many(_set_r_, ms_iter_value(&iter2), ms_iter_multiplicity(&iter2));
    }
    return _set_r_;
}
multiset *ms_symmetric_difference(multiset *_set1_, multiset *_set2_)
{
    multiset_iter iter1, iter2;
    multiset *_set_r_ = ms_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return ((void *)0);
    ms_iter_init(&iter1, _set1_);
    ms_iter_init(&iter2, _set2_);
    for (ms_iter_to_start(&iter1); !ms_iter_end(&iter1); ms_iter_next(&iter1))
    {
        size_t value = ms_iter_value(&iter1);
        size_t m1 = ms_iter_multiplicity(&iter1);
        size_t m2 = ms_multiplicity_of(_set2_, value);
        if (m1 != m2)
        {
            if (m1 > m2)
                ms_update(_set_r_, value, m1 - m2);
            else
                ms_update(_set_r_, value, m2 - m1);
        }
    }
    for (ms_iter_to_start(&iter2); !ms_iter_end(&iter2); ms_iter_next(&iter2))
    {
        size_t value = ms_iter_value(&iter2);
        size_t m1 = ms_multiplicity_of(_set1_, value);
        size_t m2 = ms_iter_multiplicity(&iter2);
        if (m1 != m2)
        {
            if (m1 > m2)
                ms_update(_set_r_, value, m1 - m2);
            else
                ms_update(_set_r_, value, m2 - m1);
        }
    }
    return _set_r_;
}
_Bool ms_is_subset(multiset *_set1_, multiset *_set2_)
{
    if (ms_count(_set1_) > ms_count(_set2_))
        return 0;
    if (ms_empty(_set1_))
        return 1;
    multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        size_t m1 = ms_iter_multiplicity(&iter);
        size_t m2 = ms_multiplicity_of(_set2_, value);
        if (m1 > m2)
            return 0;
    }
    return 1;
}
_Bool ms_is_superset(multiset *_set1_, multiset *_set2_) { return ms_is_subset(_set2_, _set1_); }
_Bool ms_is_proper_subset(multiset *_set1_, multiset *_set2_)
{
    if (ms_count(_set1_) >= ms_count(_set2_))
        return 0;
    if (ms_empty(_set1_))
    {
        if (!ms_empty(_set2_))
            return 1;
        else
            return 0;
    }
    multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        size_t m1 = ms_iter_multiplicity(&iter);
        size_t m2 = ms_multiplicity_of(_set2_, value);
        if (m1 >= m2)
            return 0;
    }
    return 1;
}
_Bool ms_is_proper_superset(multiset *_set1_, multiset *_set2_) { return ms_is_proper_subset(_set2_, _set1_); }
_Bool ms_is_disjointset(multiset *_set1_, multiset *_set2_)
{
    if (ms_empty(_set1_))
        return 1;
    multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        if (ms_contains(_set2_, value))
            return 0;
    }
    return 1;
}
multiset_iter *ms_iter_new(multiset *target)
{
    multiset_iter *iter = malloc(sizeof(multiset_iter));
    if (!iter)
        return ((void *)0);
    ms_iter_init(iter, target);
    return iter;
}
void ms_iter_free(multiset_iter *iter) { free(iter); }
void ms_iter_init(multiset_iter *iter, multiset *target)
{
    memset(iter, 0, sizeof(multiset_iter));
    iter->target = target;
    iter->start = 1;
    iter->end = ms_empty(target);
    if (!ms_empty(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            if (target->buffer[i].state == CMC_ES_FILLED)
            {
                iter->first = i;
                break;
            }
        }
        iter->cursor = iter->first;
        for (size_t i = target->capacity; i > 0; i--)
        {
            if (target->buffer[i - 1].state == CMC_ES_FILLED)
            {
                iter->last = i - 1;
                break;
            }
        }
    }
}
_Bool ms_iter_start(multiset_iter *iter) { return ms_empty(iter->target) || iter->start; }
_Bool ms_iter_end(multiset_iter *iter) { return ms_empty(iter->target) || iter->end; }
void ms_iter_to_start(multiset_iter *iter)
{
    if (!ms_empty(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = 1;
        iter->end = ms_empty(iter->target);
    }
}
void ms_iter_to_end(multiset_iter *iter)
{
    if (!ms_empty(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = ms_empty(iter->target);
        iter->end = 1;
    }
}
_Bool ms_iter_next(multiset_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == ms_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    iter->start = ms_empty(iter->target);
    multiset_entry *scan = &(iter->target->buffer[iter->cursor]);
    iter->index++;
    while (1)
    {
        iter->cursor++;
        scan = &(iter->target->buffer[iter->cursor]);
        if (scan->state == CMC_ES_FILLED)
            break;
    }
    return 1;
}
_Bool ms_iter_prev(multiset_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = ms_empty(iter->target);
    multiset_entry *scan = &(iter->target->buffer[iter->cursor]);
    iter->index--;
    while (1)
    {
        iter->cursor--;
        scan = &(iter->target->buffer[iter->cursor]);
        if (scan->state == CMC_ES_FILLED)
            break;
    }
    return 1;
}
_Bool ms_iter_advance(multiset_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == ms_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= ms_count(iter->target))
        return 0;
    for (size_t i = 0; i < steps; i++)
        ms_iter_next(iter);
    return 1;
}
_Bool ms_iter_rewind(multiset_iter *iter, size_t steps)
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
    for (size_t i = 0; i < steps; i++)
        ms_iter_prev(iter);
    return 1;
}
_Bool ms_iter_go_to(multiset_iter *iter, size_t index)
{
    if (index >= ms_count(iter->target))
        return 0;
    if (iter->index > index)
        return ms_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return ms_iter_advance(iter, index - iter->index);
    return 1;
}
size_t ms_iter_value(multiset_iter *iter)
{
    if (ms_empty(iter->target))
        return (size_t){0};
    return iter->target->buffer[iter->cursor].value;
}
size_t ms_iter_multiplicity(multiset_iter *iter)
{
    if (ms_empty(iter->target))
        return 0;
    return iter->target->buffer[iter->cursor].multiplicity;
}
size_t ms_iter_index(multiset_iter *iter) { return iter->index; }
static multiset_entry *ms_impl_insert_and_return(multiset *_set_, size_t element, _Bool *new_node)
{
    *new_node = 0;
    multiset_entry *entry = ms_impl_get_entry(_set_, element);
    if (entry != ((void *)0))
        return entry;
    *new_node = 1;
    if (ms_full(_set_))
    {
        if (!ms_resize(_set_, ms_capacity(_set_) + 1))
            return ((void *)0);
    }
    size_t hash = _set_->hash(element);
    size_t original_pos = hash % _set_->capacity;
    size_t pos = original_pos;
    size_t curr_mul = 1;
    multiset_entry *target = &(_set_->buffer[pos]);
    if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)
    {
        target->value = element;
        target->multiplicity = curr_mul;
        target->dist = pos - original_pos;
        target->state = CMC_ES_FILLED;
    }
    else
    {
        while (1)
        {
            pos++;
            target = &(_set_->buffer[pos % _set_->capacity]);
            if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)
            {
                target->value = element;
                target->multiplicity = curr_mul;
                target->dist = pos - original_pos;
                target->state = CMC_ES_FILLED;
                break;
            }
            else if (target->dist < pos - original_pos)
            {
                size_t tmp = target->value;
                size_t tmp_dist = target->dist;
                size_t tmp_mul = target->multiplicity;
                target->value = element;
                target->dist = pos - original_pos;
                target->multiplicity = curr_mul;
                element = tmp;
                original_pos = pos - tmp_dist;
                curr_mul = tmp_mul;
            }
        }
    }
    _set_->count++;
    return target;
}
static multiset_entry *ms_impl_get_entry(multiset *_set_, size_t element)
{
    size_t hash = _set_->hash(element);
    size_t pos = hash % _set_->capacity;
    multiset_entry *target = &(_set_->buffer[pos]);
    while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED)
    {
        if (_set_->cmp(target->value, element) == 0)
            return target;
        pos++;
        target = &(_set_->buffer[pos % _set_->capacity]);
    }
    return ((void *)0);
}
static size_t ms_impl_calculate_size(size_t required)
{
    const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);
    if (cmc_hashtable_primes[count - 1] < required)
        return required;
    size_t i = 0;
    while (cmc_hashtable_primes[i] < required)
        i++;
    return cmc_hashtable_primes[i];
}
static multiset_iter ms_impl_it_start(multiset *_set_)
{
    multiset_iter iter;
    ms_iter_init(&iter, _set_);
    return iter;
}
static multiset_iter ms_impl_it_end(multiset *_set_)
{
    multiset_iter iter;
    ms_iter_init(&iter, _set_);
    ms_iter_to_end(&iter);
    return iter;
}
