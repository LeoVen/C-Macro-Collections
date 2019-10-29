#include <cmc/hashset.h>

//CMC_GENERATE_HASHSET(hs, hashset, size_t)

typedef struct hashset_s
{
    struct hashset_entry_s *buffer;
    size_t capacity;
    size_t count;
    double load;
    int (*cmp)(size_t, size_t);
    size_t (*hash)(size_t);
    struct hashset_iter_s (*it_start)(struct hashset_s *);
    struct hashset_iter_s (*it_end)(struct hashset_s *);
} hashset, *hashset_ptr;
typedef struct hashset_entry_s
{
    size_t value;
    size_t dist;
    enum cmc_entry_state state;
} hashset_entry, *hashset_entry_ptr;
typedef struct hashset_iter_s
{
    struct hashset_s *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
} hashset_iter, *hashset_iter_ptr;
hashset *hs_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t));
void hs_clear(hashset *_set_, void (*deallocator)(size_t));
void hs_free(hashset *_set_, void (*deallocator)(size_t));
_Bool hs_insert(hashset *_set_, size_t element);
_Bool hs_remove(hashset *_set_, size_t element);
_Bool hs_max(hashset *_set_, size_t *value);
_Bool hs_min(hashset *_set_, size_t *value);
_Bool hs_contains(hashset *_set_, size_t element);
_Bool hs_empty(hashset *_set_);
_Bool hs_full(hashset *_set_);
size_t hs_count(hashset *_set_);
size_t hs_capacity(hashset *_set_);
double hs_load(hashset *_set_);
_Bool hs_resize(hashset *_set_, size_t capacity);
hashset *hs_copy_of(hashset *_set_, size_t (*copy_func)(size_t));
_Bool hs_equals(hashset *_set1_, hashset *_set2_);
struct cmc_string hs_to_string(hashset *_set_);
hashset *hs_union(hashset *_set1_, hashset *_set2_);
hashset *hs_intersection(hashset *_set1_, hashset *_set2_);
hashset *hs_difference(hashset *_set1_, hashset *_set2_);
hashset *hs_symmetric_difference(hashset *_set1_, hashset *_set2_);
_Bool hs_is_subset(hashset *_set1_, hashset *_set2_);
_Bool hs_is_superset(hashset *_set1_, hashset *_set2_);
_Bool hs_is_proper_subset(hashset *_set1_, hashset *_set2_);
_Bool hs_is_proper_superset(hashset *_set1_, hashset *_set2_);
_Bool hs_is_disjointset(hashset *_set1_, hashset *_set2_);
hashset_iter *hs_iter_new(hashset *target);
void hs_iter_free(hashset_iter *iter);
void hs_iter_init(hashset_iter *iter, hashset *target);
_Bool hs_iter_start(hashset_iter *iter);
_Bool hs_iter_end(hashset_iter *iter);
void hs_iter_to_start(hashset_iter *iter);
void hs_iter_to_end(hashset_iter *iter);
_Bool hs_iter_next(hashset_iter *iter);
_Bool hs_iter_prev(hashset_iter *iter);
_Bool hs_iter_advance(hashset_iter *iter, size_t steps);
_Bool hs_iter_rewind(hashset_iter *iter, size_t steps);
_Bool hs_iter_go_to(hashset_iter *iter, size_t index);
size_t hs_iter_value(hashset_iter *iter);
size_t hs_iter_index(hashset_iter *iter);
static hashset_entry *hs_impl_get_entry(hashset *_set_, size_t element);
static size_t hs_impl_calculate_size(size_t required);
static hashset_iter hs_impl_it_start(hashset *_set_);
static hashset_iter hs_impl_it_end(hashset *_set_);
hashset *hs_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t))
{
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffff * load)
        return ((void *)0);
    size_t real_capacity = hs_impl_calculate_size(capacity / load);
    hashset *_set_ = malloc(sizeof(hashset));
    if (!_set_)
        return ((void *)0);
    _set_->buffer = malloc(sizeof(hashset_entry) * real_capacity);
    if (!_set_->buffer)
    {
        free(_set_);
        return ((void *)0);
    }
    memset(_set_->buffer, 0, sizeof(hashset_entry) * real_capacity);
    _set_->count = 0;
    _set_->capacity = real_capacity;
    _set_->load = load;
    _set_->cmp = compare;
    _set_->hash = hash;
    _set_->it_start = hs_impl_it_start;
    _set_->it_end = hs_impl_it_end;
    return _set_;
}
void hs_clear(hashset *_set_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            hashset_entry *entry = &(_set_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                deallocator(entry->value);
            }
        }
    }
    memset(_set_->buffer, 0, sizeof(hashset_entry) * _set_->capacity);
    _set_->count = 0;
}
void hs_free(hashset *_set_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            hashset_entry *entry = &(_set_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                deallocator(entry->value);
            }
        }
    }
    free(_set_->buffer);
    free(_set_);
}
_Bool hs_insert(hashset *_set_, size_t element)
{
    if (hs_full(_set_))
    {
        if (!hs_resize(_set_, hs_capacity(_set_) + 1))
            return 0;
    }
    size_t hash = _set_->hash(element);
    size_t original_pos = hash % _set_->capacity;
    size_t pos = original_pos;
    hashset_entry *target = &(_set_->buffer[pos]);
    if (hs_impl_get_entry(_set_, element) != ((void *)0))
        return 0;
    if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)
    {
        target->value = element;
        target->dist = 0;
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
                target->dist = pos - original_pos;
                target->state = CMC_ES_FILLED;
                break;
            }
            else if (target->dist < pos - original_pos)
            {
                size_t tmp = target->value;
                size_t tmp_dist = target->dist;
                target->value = element;
                target->dist = pos - original_pos;
                element = tmp;
                original_pos = pos - tmp_dist;
            }
        }
    }
    _set_->count++;
    return 1;
}
_Bool hs_remove(hashset *_set_, size_t element)
{
    hashset_entry *result = hs_impl_get_entry(_set_, element);
    if (result == ((void *)0))
        return 0;
    result->value = (size_t){0};
    result->dist = 0;
    result->state = CMC_ES_DELETED;
    _set_->count--;
    return 1;
}
_Bool hs_max(hashset *_set_, size_t *value)
{
    if (hs_empty(_set_))
        return 0;
    hashset_iter iter;
    for (hs_iter_init(&iter, _set_); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t result = hs_iter_value(&iter);
        size_t index = hs_iter_index(&iter);
        if (index == 0)
            *value = result;
        else if (_set_->cmp(result, *value) > 0)
            *value = result;
    }
    return 1;
}
_Bool hs_min(hashset *_set_, size_t *value)
{
    if (hs_empty(_set_))
        return 0;
    hashset_iter iter;
    for (hs_iter_init(&iter, _set_); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t result = hs_iter_value(&iter);
        size_t index = hs_iter_index(&iter);
        if (index == 0)
            *value = result;
        else if (_set_->cmp(result, *value) < 0)
            *value = result;
    }
    return 1;
}
_Bool hs_contains(hashset *_set_, size_t element) { return hs_impl_get_entry(_set_, element) != ((void *)0); }
_Bool hs_empty(hashset *_set_) { return _set_->count == 0; }
_Bool hs_full(hashset *_set_) { return (double)hs_capacity(_set_) * hs_load(_set_) <= (double)hs_count(_set_); }
size_t hs_count(hashset *_set_) { return _set_->count; }
size_t hs_capacity(hashset *_set_) { return _set_->capacity; }
double hs_load(hashset *_set_) { return _set_->load; }
_Bool hs_resize(hashset *_set_, size_t capacity)
{
    if (hs_capacity(_set_) == capacity)
        return 1;
    if (hs_capacity(_set_) > capacity / hs_load(_set_))
        return 1;
    if (capacity >= 0xffffffffffffffff * hs_load(_set_))
        return 0;
    size_t theoretical_size = hs_impl_calculate_size(capacity);
    if (theoretical_size < hs_count(_set_) / hs_load(_set_))
        return 0;
    hashset *_new_set_ = hs_new(capacity, hs_load(_set_), _set_->cmp, _set_->hash);
    if (!_new_set_)
        return 0;
    hashset_iter iter;
    for (hs_iter_init(&iter, _set_); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        hs_insert(_new_set_, value);
    }
    if (hs_count(_set_) != hs_count(_new_set_))
    {
        hs_free(_new_set_, ((void *)0));
        return 0;
    }
    hashset_entry *tmp_b = _set_->buffer;
    _set_->buffer = _new_set_->buffer;
    _new_set_->buffer = tmp_b;
    size_t tmp_c = _set_->capacity;
    _set_->capacity = _new_set_->capacity;
    _new_set_->capacity = tmp_c;
    hs_free(_new_set_, ((void *)0));
    return 1;
}
hashset *hs_copy_of(hashset *_set_, size_t (*copy_func)(size_t))
{
    hashset *result = hs_new(_set_->capacity, _set_->load, _set_->cmp, _set_->hash);
    if (!result)
        return ((void *)0);
    if (copy_func)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            hashset_entry *scan = &(_set_->buffer[i]);
            if (scan->state != CMC_ES_EMPTY)
            {
                hashset_entry *target = &(result->buffer[i]);
                if (scan->state == CMC_ES_DELETED)
                    target->state = CMC_ES_DELETED;
                else
                {
                    target->state = scan->state;
                    target->dist = scan->dist;
                    target->value = copy_func(scan->value);
                }
            }
        }
    }
    else
        memcpy(result->buffer, _set_->buffer, sizeof(hashset_entry) * _set_->capacity);
    result->count = _set_->count;
    return result;
}
_Bool hs_equals(hashset *_set1_, hashset *_set2_)
{
    if (hs_count(_set1_) != hs_count(_set2_))
        return 0;
    if (hs_count(_set1_) == 0)
        return 1;
    hashset_iter iter;
    hs_iter_init(&iter, _set1_);
    for (hs_iter_to_start(&iter); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        if (hs_impl_get_entry(_set2_, hs_iter_value(&iter)) == ((void *)0))
            return 0;
    }
    return 1;
}
struct cmc_string hs_to_string(hashset *_set_)
{
    struct cmc_string str;
    hashset *s_ = _set_;
    const char *name = "hashset";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_hashset, name, s_, s_->buffer, s_->capacity, s_->count, s_->load, s_->cmp, s_->hash);
    return str;
}
hashset *hs_union(hashset *_set1_, hashset *_set2_)
{
    hashset *_set_r_ = hs_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return ((void *)0);
    hashset_iter iter1, iter2;
    hs_iter_init(&iter1, _set1_);
    hs_iter_init(&iter2, _set2_);
    for (hs_iter_to_start(&iter1); !hs_iter_end(&iter1); hs_iter_next(&iter1))
    {
        hs_insert(_set_r_, hs_iter_value(&iter1));
    }
    for (hs_iter_to_start(&iter2); !hs_iter_end(&iter2); hs_iter_next(&iter2))
    {
        hs_insert(_set_r_, hs_iter_value(&iter2));
    }
    return _set_r_;
}
hashset *hs_intersection(hashset *_set1_, hashset *_set2_)
{
    hashset *_set_r_ = hs_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return ((void *)0);
    hashset *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;
    hashset *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;
    hashset_iter iter;
    hs_iter_init(&iter, _set_A_);
    for (hs_iter_to_start(&iter); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (hs_impl_get_entry(_set_B_, value) != ((void *)0))
            hs_insert(_set_r_, value);
    }
    return _set_r_;
}
hashset *hs_difference(hashset *_set1_, hashset *_set2_)
{
    hashset *_set_r_ = hs_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return ((void *)0);
    hashset_iter iter;
    hs_iter_init(&iter, _set1_);
    for (hs_iter_to_start(&iter); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (hs_impl_get_entry(_set2_, value) == ((void *)0))
            hs_insert(_set_r_, value);
    }
    return _set_r_;
}
hashset *hs_symmetric_difference(hashset *_set1_, hashset *_set2_)
{
    hashset_iter iter1, iter2;
    hashset *_set_r_ = hs_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return ((void *)0);
    hs_iter_init(&iter1, _set1_);
    hs_iter_init(&iter2, _set2_);
    for (hs_iter_to_start(&iter1); !hs_iter_end(&iter1); hs_iter_next(&iter1))
    {
        size_t value = hs_iter_value(&iter1);
        if (hs_impl_get_entry(_set2_, value) == ((void *)0))
            hs_insert(_set_r_, value);
    }
    for (hs_iter_to_start(&iter2); !hs_iter_end(&iter2); hs_iter_next(&iter2))
    {
        size_t value = hs_iter_value(&iter2);
        if (hs_impl_get_entry(_set1_, value) == ((void *)0))
            hs_insert(_set_r_, value);
    }
    return _set_r_;
}
_Bool hs_is_subset(hashset *_set1_, hashset *_set2_)
{
    if (hs_count(_set1_) > hs_count(_set2_))
        return 0;
    if (hs_empty(_set1_))
        return 1;
    hashset_iter iter;
    hs_iter_init(&iter, _set1_);
    for (hs_iter_to_start(&iter); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (!hs_contains(_set2_, value))
            return 0;
    }
    return 1;
}
_Bool hs_is_superset(hashset *_set1_, hashset *_set2_) { return hs_is_subset(_set2_, _set1_); }
_Bool hs_is_proper_subset(hashset *_set1_, hashset *_set2_)
{
    if (hs_count(_set1_) >= hs_count(_set2_))
        return 0;
    if (hs_empty(_set1_))
    {
        if (!hs_empty(_set2_))
            return 1;
        else
            return 0;
    }
    hashset_iter iter;
    hs_iter_init(&iter, _set1_);
    for (hs_iter_to_start(&iter); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (!hs_contains(_set2_, value))
            return 0;
    }
    return 1;
}
_Bool hs_is_proper_superset(hashset *_set1_, hashset *_set2_) { return hs_is_proper_subset(_set2_, _set1_); }
_Bool hs_is_disjointset(hashset *_set1_, hashset *_set2_)
{
    if (hs_empty(_set1_))
        return 1;
    hashset_iter iter;
    hs_iter_init(&iter, _set1_);
    for (hs_iter_to_start(&iter); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (hs_contains(_set2_, value))
            return 0;
    }
    return 1;
}
hashset_iter *hs_iter_new(hashset *target)
{
    hashset_iter *iter = malloc(sizeof(hashset_iter));
    if (!iter)
        return ((void *)0);
    hs_iter_init(iter, target);
    return iter;
}
void hs_iter_free(hashset_iter *iter) { free(iter); }
void hs_iter_init(hashset_iter *iter, hashset *target)
{
    memset(iter, 0, sizeof(hashset_iter));
    iter->target = target;
    iter->start = 1;
    iter->end = hs_empty(target);
    if (!hs_empty(target))
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
_Bool hs_iter_start(hashset_iter *iter) { return hs_empty(iter->target) || iter->start; }
_Bool hs_iter_end(hashset_iter *iter) { return hs_empty(iter->target) || iter->end; }
void hs_iter_to_start(hashset_iter *iter)
{
    if (!hs_empty(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = 1;
        iter->end = hs_empty(iter->target);
    }
}
void hs_iter_to_end(hashset_iter *iter)
{
    if (!hs_empty(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = hs_count(iter->target) - 1;
        iter->start = hs_empty(iter->target);
        iter->end = 1;
    }
}
_Bool hs_iter_next(hashset_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == hs_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    iter->start = hs_empty(iter->target);
    hashset_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool hs_iter_prev(hashset_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = hs_empty(iter->target);
    hashset_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool hs_iter_advance(hashset_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == hs_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= hs_count(iter->target))
        return 0;
    for (size_t i = 0; i < steps; i++)
        hs_iter_next(iter);
    return 1;
}
_Bool hs_iter_rewind(hashset_iter *iter, size_t steps)
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
        hs_iter_prev(iter);
    return 1;
}
_Bool hs_iter_go_to(hashset_iter *iter, size_t index)
{
    if (index >= hs_count(iter->target))
        return 0;
    if (iter->index > index)
        return hs_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return hs_iter_advance(iter, index - iter->index);
    return 1;
}
size_t hs_iter_value(hashset_iter *iter)
{
    if (hs_empty(iter->target))
        return (size_t){0};
    return iter->target->buffer[iter->cursor].value;
}
size_t hs_iter_index(hashset_iter *iter) { return iter->index; }
static hashset_entry *hs_impl_get_entry(hashset *_set_, size_t element)
{
    size_t hash = _set_->hash(element);
    size_t pos = hash % _set_->capacity;
    hashset_entry *target = &(_set_->buffer[pos]);
    while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED)
    {
        if (_set_->cmp(target->value, element) == 0)
            return target;
        pos++;
        target = &(_set_->buffer[pos % _set_->capacity]);
    }
    return ((void *)0);
}
static size_t hs_impl_calculate_size(size_t required)
{
    const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);
    if (cmc_hashtable_primes[count - 1] < required)
        return required;
    size_t i = 0;
    while (cmc_hashtable_primes[i] < required)
        i++;
    return cmc_hashtable_primes[i];
}
static hashset_iter hs_impl_it_start(hashset *_set_)
{
    hashset_iter iter;
    hs_iter_init(&iter, _set_);
    return iter;
}
static hashset_iter hs_impl_it_end(hashset *_set_)
{
    hashset_iter iter;
    hs_iter_init(&iter, _set_);
    hs_iter_to_end(&iter);
    return iter;
}
