#include "cmc/hashset.h"

//HASHSET_GENERATE(hs, hashset, , size_t)

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
    enum cmc_entry_state_e state;
} hashset_entry, *hashset_entry_ptr;
typedef struct hashset_iter_s
{
    struct hashset_s *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    bool start;
    bool end;
} hashset_iter, *hashset_iter_ptr;
hashset *hs_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t));
void hs_clear(hashset *_set_);
void hs_free(hashset *_set_);
bool hs_insert(hashset *_set_, size_t element);
bool hs_remove(hashset *_set_, size_t element);
bool hs_insert_if(hashset *_set_, size_t element, bool condition);
bool hs_remove_if(hashset *_set_, size_t element, bool condition);
bool hs_max(hashset *_set_, size_t *value);
bool hs_min(hashset *_set_, size_t *value);
bool hs_contains(hashset *_set_, size_t element);
bool hs_empty(hashset *_set_);
size_t hs_count(hashset *_set_);
size_t hs_capacity(hashset *_set_);
hashset *hs_union(hashset *_set1_, hashset *_set2_);
hashset *hs_intersection(hashset *_set1_, hashset *_set2_);
hashset *hs_difference(hashset *_set1_, hashset *_set2_);
hashset *hs_symmetric_difference(hashset *_set1_, hashset *_set2_);
bool hs_is_subset(hashset *_set1_, hashset *_set2_);
bool hs_is_superset(hashset *_set1_, hashset *_set2_);
bool hs_is_proper_subset(hashset *_set1_, hashset *_set2_);
bool hs_is_proper_superset(hashset *_set1_, hashset *_set2_);
bool hs_is_disjointset(hashset *_set1_, hashset *_set2_);
hashset_iter *hs_iter_new(hashset *target);
void hs_iter_free(hashset_iter *iter);
void hs_iter_init(hashset_iter *iter, hashset *target);
bool hs_iter_start(hashset_iter *iter);
bool hs_iter_end(hashset_iter *iter);
void hs_iter_to_start(hashset_iter *iter);
void hs_iter_to_end(hashset_iter *iter);
bool hs_iter_next(hashset_iter *iter);
bool hs_iter_prev(hashset_iter *iter);
size_t hs_iter_value(hashset_iter *iter);
size_t hs_iter_index(hashset_iter *iter);
static inline size_t hs_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
static bool hs_impl_grow(hashset *_set_);
static hashset_entry *hs_impl_get_entry(hashset *_set_, size_t element);
static size_t hs_impl_calculate_size(size_t required);
static hashset_iter hs_impl_it_start(hashset *_set_);
static hashset_iter hs_impl_it_end(hashset *_set_);
hashset *hs_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t))
{
    if (capacity == 0 || load <= 0 || load >= 1)
        return NULL;
    size_t real_capacity = hs_impl_calculate_size(capacity / load);
    hashset *_set_ = malloc(sizeof(hashset));
    if (!_set_)
        return NULL;
    _set_->buffer = malloc(sizeof(hashset_entry) * real_capacity);
    if (!_set_->buffer)
    {
        free(_set_);
        return NULL;
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
void hs_clear(hashset *_set_)
{
    memset(_set_->buffer, 0, sizeof(hashset_entry) * _set_->capacity);
    _set_->count = 0;
}
void hs_free(hashset *_set_)
{
    free(_set_->buffer);
    free(_set_);
}
bool hs_insert(hashset *_set_, size_t element)
{
    if ((double)_set_->capacity * _set_->load <= (double)_set_->count)
    {
        if (!hs_impl_grow(_set_))
            return false;
    }
    size_t hash = _set_->hash(element);
    size_t original_pos = hash % _set_->capacity;
    size_t pos = original_pos;
    hashset_entry *target = &(_set_->buffer[pos]);
    if (hs_impl_get_entry(_set_, element) != NULL)
        return false;
    if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)
    {
        target->value = element;
        target->dist = pos - original_pos;
        target->state = CMC_ES_FILLED;
    }
    else
    {
        while (true)
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
    return true;
}
bool hs_remove(hashset *_set_, size_t element)
{
    hashset_entry *result = hs_impl_get_entry(_set_, element);
    if (result == NULL)
        return false;
    result->value = hs_impl_default_value();
    result->dist = 0;
    result->state = CMC_ES_DELETED;
    _set_->count--;
    return true;
}
bool hs_insert_if(hashset *_set_, size_t element, bool condition)
{
    if (condition)
        return hs_insert(_set_, element);
    return false;
}
bool hs_remove_if(hashset *_set_, size_t element, bool condition)
{
    if (condition)
        return hs_remove(_set_, element);
    return false;
}
bool hs_max(hashset *_set_, size_t *value)
{
    if (hs_empty(_set_))
        return false;
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
    return true;
}
bool hs_min(hashset *_set_, size_t *value)
{
    if (hs_empty(_set_))
        return false;
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
    return true;
}
bool hs_contains(hashset *_set_, size_t element) { return hs_impl_get_entry(_set_, element) != NULL; }
bool hs_empty(hashset *_set_) { return _set_->count == 0; }
size_t hs_count(hashset *_set_) { return _set_->count; }
size_t hs_capacity(hashset *_set_) { return _set_->capacity; }
hashset *hs_union(hashset *_set1_, hashset *_set2_)
{
    hashset *_set_r_ = hs_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return NULL;
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
        return NULL;
    hashset *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;
    hashset *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;
    hashset_iter iter;
    hs_iter_init(&iter, _set_A_);
    for (hs_iter_to_start(&iter); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (hs_impl_get_entry(_set_B_, value) != NULL)
            hs_insert(_set_r_, value);
    }
    return _set_r_;
}
hashset *hs_difference(hashset *_set1_, hashset *_set2_)
{
    hashset *_set_r_ = hs_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return NULL;
    hashset_iter iter;
    hs_iter_init(&iter, _set1_);
    for (hs_iter_to_start(&iter); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (hs_impl_get_entry(_set2_, value) == NULL)
            hs_insert(_set_r_, value);
    }
    return _set_r_;
}
hashset *hs_symmetric_difference(hashset *_set1_, hashset *_set2_)
{
    hashset_iter iter1, iter2;
    hashset *_set_r_ = hs_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return NULL;
    hs_iter_init(&iter1, _set1_);
    hs_iter_init(&iter2, _set2_);
    for (hs_iter_to_start(&iter1); !hs_iter_end(&iter1); hs_iter_next(&iter1))
    {
        size_t value = hs_iter_value(&iter1);
        if (hs_impl_get_entry(_set2_, value) == NULL)
            hs_insert(_set_r_, value);
    }
    for (hs_iter_to_start(&iter2); !hs_iter_end(&iter2); hs_iter_next(&iter2))
    {
        size_t value = hs_iter_value(&iter2);
        if (hs_impl_get_entry(_set1_, value) == NULL)
            hs_insert(_set_r_, value);
    }
    return _set_r_;
}
bool hs_is_subset(hashset *_set1_, hashset *_set2_)
{
    if (hs_count(_set1_) > hs_count(_set2_))
        return false;
    if (hs_empty(_set1_))
        return true;
    hashset_iter iter;
    hs_iter_init(&iter, _set1_);
    for (hs_iter_to_start(&iter); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (!hs_contains(_set2_, value))
            return false;
    }
    return true;
}
bool hs_is_superset(hashset *_set1_, hashset *_set2_) { return hs_is_subset(_set2_, _set1_); }
bool hs_is_proper_subset(hashset *_set1_, hashset *_set2_)
{
    if (hs_count(_set1_) >= hs_count(_set2_))
        return false;
    if (hs_empty(_set1_))
    {
        if (!hs_empty(_set2_))
            return true;
        else
            return false;
    }
    hashset_iter iter;
    hs_iter_init(&iter, _set1_);
    for (hs_iter_to_start(&iter); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (!hs_contains(_set2_, value))
            return false;
    }
    return true;
}
bool hs_is_proper_superset(hashset *_set1_, hashset *_set2_) { return hs_is_proper_subset(_set2_, _set1_); }
bool hs_is_disjointset(hashset *_set1_, hashset *_set2_)
{
    if (hs_empty(_set1_))
        return true;
    hashset_iter iter;
    hs_iter_init(&iter, _set1_);
    for (hs_iter_to_start(&iter); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (hs_contains(_set2_, value))
            return false;
    }
    return true;
}
hashset_iter *hs_iter_new(hashset *target)
{
    hashset_iter *iter = malloc(sizeof(hashset_iter));
    if (!iter)
        return NULL;
    hs_iter_init(iter, target);
    return iter;
}
void hs_iter_free(hashset_iter *iter) { free(iter); }
void hs_iter_init(hashset_iter *iter, hashset *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->index = 0;
    iter->start = true;
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
bool hs_iter_start(hashset_iter *iter) { return hs_empty(iter->target) || iter->start; }
bool hs_iter_end(hashset_iter *iter) { return hs_empty(iter->target) || iter->end; }
void hs_iter_to_start(hashset_iter *iter)
{
    iter->cursor = iter->first;
    iter->index = 0;
    iter->start = true;
    iter->end = hs_empty(iter->target);
}
void hs_iter_to_end(hashset_iter *iter)
{
    iter->cursor = iter->last;
    iter->index = iter->target->count - 1;
    iter->start = hs_empty(iter->target);
    iter->end = true;
}
bool hs_iter_next(hashset_iter *iter)
{
    if (iter->end)
        return false;
    hashset_entry *scan = &(iter->target->buffer[iter->cursor]);
    if (iter->cursor == iter->last)
        iter->end = true;
    else
    {
        iter->index++;
        while (1)
        {
            iter->cursor++;
            scan = &(iter->target->buffer[iter->cursor]);
            if (scan->state == CMC_ES_FILLED)
                break;
        }
    }
    iter->start = hs_empty(iter->target);
    return true;
}
bool hs_iter_prev(hashset_iter *iter)
{
    if (iter->start)
        return false;
    hashset_entry *scan = &(iter->target->buffer[iter->cursor]);
    if (iter->cursor == iter->first)
        iter->start = true;
    else
    {
        iter->index--;
        while (1)
        {
            iter->cursor--;
            scan = &(iter->target->buffer[iter->cursor]);
            if (scan->state == CMC_ES_FILLED)
                break;
        }
    }
    iter->end = hs_empty(iter->target);
    return true;
}
size_t hs_iter_value(hashset_iter *iter)
{
    if (hs_empty(iter->target))
        return hs_impl_default_value();
    return iter->target->buffer[iter->cursor].value;
}
size_t hs_iter_index(hashset_iter *iter) { return iter->index; }
static bool hs_impl_grow(hashset *_set_)
{
    size_t new_size = hs_impl_calculate_size(_set_->capacity + _set_->capacity / 2);
    hashset *_new_set_ = hs_new(new_size, _set_->load, _set_->cmp, _set_->hash);
    if (!_new_set_)
        return false;
    hashset_iter iter;
    for (hs_iter_init(&iter, _set_); !hs_iter_end(&iter); hs_iter_next(&iter))
    {
        hs_insert(_new_set_, hs_iter_value(&iter));
    }
    if (_set_->count != _new_set_->count)
    {
        hs_free(_new_set_);
        return false;
    }
    hashset_entry *tmp = _set_->buffer;
    _set_->buffer = _new_set_->buffer;
    _new_set_->buffer = tmp;
    _set_->capacity = _new_set_->capacity;
    hs_free(_new_set_);
    return true;
}
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
    return NULL;
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
