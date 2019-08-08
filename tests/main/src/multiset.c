#include "ext/multiset.h"

//MULTISET_GENERATE(ms, multiset, , size_t)

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
    enum cmc_entry_state_e state;
} multiset_entry, *multiset_entry_ptr;
typedef struct multiset_iter_s
{
    struct multiset_s *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    bool start;
    bool end;
} multiset_iter, *multiset_iter_ptr;
multiset *ms_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t));
void ms_clear(multiset *_set_);
void ms_free(multiset *_set_);
bool ms_insert(multiset *_set_, size_t element);
bool ms_insert_many(multiset *_set_, size_t element, size_t count);
bool ms_update(multiset *_set_, size_t element, size_t multiplicity);
bool ms_remove(multiset *_set_, size_t element);
size_t ms_remove_all(multiset *_set_, size_t element);
bool ms_insert_if(multiset *_set_, size_t element, bool condition);
bool ms_remove_if(multiset *_set_, size_t element, bool condition);
bool ms_max(multiset *_set_, size_t *value);
bool ms_min(multiset *_set_, size_t *value);
size_t ms_multiplicity_of(multiset *_set_, size_t element);
bool ms_contains(multiset *_set_, size_t element);
bool ms_empty(multiset *_set_);
size_t ms_count(multiset *_set_);
size_t ms_cardinality(multiset *_set_);
size_t ms_capacity(multiset *_set_);
cmc_string ms_to_string(multiset *_set_);
multiset *ms_union(multiset *_set1_, multiset *_set2_);
multiset *ms_intersection(multiset *_set1_, multiset *_set2_);
multiset *ms_difference(multiset *_set1_, multiset *_set2_);
multiset *ms_summation(multiset *_set1_, multiset *_set2_);
multiset *ms_symmetric_difference(multiset *_set1_, multiset *_set2_);
bool ms_is_subset(multiset *_set1_, multiset *_set2_);
bool ms_is_superset(multiset *_set1_, multiset *_set2_);
bool ms_is_proper_subset(multiset *_set1_, multiset *_set2_);
bool ms_is_proper_superset(multiset *_set1_, multiset *_set2_);
bool ms_is_disjointset(multiset *_set1_, multiset *_set2_);
multiset_iter *ms_iter_new(multiset *target);
void ms_iter_free(multiset_iter *iter);
void ms_iter_init(multiset_iter *iter, multiset *target);
bool ms_iter_start(multiset_iter *iter);
bool ms_iter_end(multiset_iter *iter);
void ms_iter_to_start(multiset_iter *iter);
void ms_iter_to_end(multiset_iter *iter);
bool ms_iter_next(multiset_iter *iter);
bool ms_iter_prev(multiset_iter *iter);
size_t ms_iter_value(multiset_iter *iter);
size_t ms_iter_multiplicity(multiset_iter *iter);
size_t ms_iter_index(multiset_iter *iter);
static inline size_t ms_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
static multiset_entry *ms_impl_insert_and_return(multiset *_set_, size_t element, bool *new_node);
static bool ms_impl_grow(multiset *_set_);
static multiset_entry *ms_impl_get_entry(multiset *_set_, size_t element);
static size_t ms_impl_calculate_size(size_t required);
static multiset_iter ms_impl_it_start(multiset *_set_);
static multiset_iter ms_impl_it_end(multiset *_set_);
multiset *ms_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t))
{
    if (capacity == 0 || load <= 0 || load >= 1)
        return NULL;
    if (capacity >= UINTMAX_MAX * load)
        return NULL;
    size_t real_capacity = ms_impl_calculate_size(capacity / load);
    multiset *_set_ = malloc(sizeof(multiset));
    if (!_set_)
        return NULL;
    _set_->buffer = malloc(sizeof(multiset_entry) * real_capacity);
    if (!_set_->buffer)
    {
        free(_set_);
        return NULL;
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
void ms_clear(multiset *_set_)
{
    memset(_set_->buffer, 0, sizeof(multiset_entry) * _set_->capacity);
    _set_->count = 0;
}
void ms_free(multiset *_set_)
{
    free(_set_->buffer);
    free(_set_);
}
bool ms_insert(multiset *_set_, size_t element)
{
    bool new_node;
    multiset_entry *entry = ms_impl_insert_and_return(_set_, element, &new_node);
    if (!entry)
        return false;
    if (!new_node)
        entry->multiplicity++;
    _set_->cardinality++;
    return true;
}
bool ms_insert_many(multiset *_set_, size_t element, size_t count)
{
    if (count == 0)
        return true;
    bool new_node;
    multiset_entry *entry = ms_impl_insert_and_return(_set_, element, &new_node);
    if (!entry)
        return false;
    if (new_node)
        entry->multiplicity = count;
    else
        entry->multiplicity += count;
    _set_->cardinality += count;
    return true;
}
bool ms_update(multiset *_set_, size_t element, size_t multiplicity)
{
    if (multiplicity == 0)
        return ms_remove_all(_set_, element);
    bool new_node;
    multiset_entry *entry = ms_impl_insert_and_return(_set_, element, &new_node);
    if (!entry)
        return false;
    if (new_node)
        _set_->cardinality++;
    _set_->cardinality = (_set_->cardinality - entry->multiplicity) + multiplicity;
    entry->multiplicity = multiplicity;
    return true;
}
bool ms_remove(multiset *_set_, size_t element)
{
    multiset_entry *result = ms_impl_get_entry(_set_, element);
    if (result == NULL)
        return false;
    if (result->multiplicity > 1)
        result->multiplicity--;
    else
    {
        result->value = ms_impl_default_value();
        result->multiplicity = 0;
        result->dist = 0;
        result->state = CMC_ES_DELETED;
        _set_->count--;
    }
    _set_->cardinality--;
    return true;
}
size_t ms_remove_all(multiset *_set_, size_t element)
{
    multiset_entry *result = ms_impl_get_entry(_set_, element);
    if (result == NULL)
        return 0;
    size_t removed = result->multiplicity;
    result->value = ms_impl_default_value();
    result->multiplicity = 0;
    result->dist = 0;
    result->state = CMC_ES_DELETED;
    _set_->count--;
    _set_->cardinality -= removed;
    return removed;
}
bool ms_insert_if(multiset *_set_, size_t element, bool condition)
{
    if (condition)
        return ms_insert(_set_, element);
    return false;
}
bool ms_remove_if(multiset *_set_, size_t element, bool condition)
{
    if (condition)
        return ms_remove(_set_, element);
    return false;
}
bool ms_max(multiset *_set_, size_t *value)
{
    if (ms_empty(_set_))
        return false;
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
    return true;
}
bool ms_min(multiset *_set_, size_t *value)
{
    if (ms_empty(_set_))
        return false;
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
    return true;
}
size_t ms_multiplicity_of(multiset *_set_, size_t element)
{
    multiset_entry *entry = ms_impl_get_entry(_set_, element);
    if (!entry)
        return 0;
    return entry->multiplicity;
}
bool ms_contains(multiset *_set_, size_t element) { return ms_impl_get_entry(_set_, element) != NULL; }
bool ms_empty(multiset *_set_) { return _set_->count == 0; }
size_t ms_count(multiset *_set_) { return _set_->count; }
size_t ms_cardinality(multiset *_set_) { return _set_->cardinality; }
size_t ms_capacity(multiset *_set_) { return _set_->capacity; }
cmc_string ms_to_string(multiset *_set_)
{
    cmc_string str;
    multiset *s_ = _set_;
    const char *name = "multiset";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_multiset, name, s_, s_->buffer, s_->capacity, s_->count, s_->cardinality, s_->load, s_->cmp, s_->hash);
    return str;
}
multiset *ms_union(multiset *_set1_, multiset *_set2_)
{
    multiset *_set_r_ = ms_new(_set1_->capacity, _set1_->load, _set1_->cmp, _set1_->hash);
    if (!_set_r_)
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
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
        return NULL;
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
bool ms_is_subset(multiset *_set1_, multiset *_set2_)
{
    if (ms_count(_set1_) > ms_count(_set2_))
        return false;
    if (ms_empty(_set1_))
        return true;
    multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        size_t m1 = ms_iter_multiplicity(&iter);
        size_t m2 = ms_multiplicity_of(_set2_, value);
        if (m1 > m2)
            return false;
    }
    return true;
}
bool ms_is_superset(multiset *_set1_, multiset *_set2_) { return ms_is_subset(_set2_, _set1_); }
bool ms_is_proper_subset(multiset *_set1_, multiset *_set2_)
{
    if (ms_count(_set1_) >= ms_count(_set2_))
        return false;
    if (ms_empty(_set1_))
    {
        if (!ms_empty(_set2_))
            return true;
        else
            return false;
    }
    multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        size_t m1 = ms_iter_multiplicity(&iter);
        size_t m2 = ms_multiplicity_of(_set2_, value);
        if (m1 >= m2)
            return false;
    }
    return true;
}
bool ms_is_proper_superset(multiset *_set1_, multiset *_set2_) { return ms_is_proper_subset(_set2_, _set1_); }
bool ms_is_disjointset(multiset *_set1_, multiset *_set2_)
{
    if (ms_empty(_set1_))
        return true;
    multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        if (ms_contains(_set2_, value))
            return false;
    }
    return true;
}
multiset_iter *ms_iter_new(multiset *target)
{
    multiset_iter *iter = malloc(sizeof(multiset_iter));
    if (!iter)
        return NULL;
    ms_iter_init(iter, target);
    return iter;
}
void ms_iter_free(multiset_iter *iter) { free(iter); }
void ms_iter_init(multiset_iter *iter, multiset *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->index = 0;
    iter->start = true;
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
bool ms_iter_start(multiset_iter *iter) { return ms_empty(iter->target) || iter->start; }
bool ms_iter_end(multiset_iter *iter) { return ms_empty(iter->target) || iter->end; }
void ms_iter_to_start(multiset_iter *iter)
{
    iter->cursor = iter->first;
    iter->index = 0;
    iter->start = true;
    iter->end = ms_empty(iter->target);
}
void ms_iter_to_end(multiset_iter *iter)
{
    iter->cursor = iter->last;
    iter->index = iter->target->count - 1;
    iter->start = ms_empty(iter->target);
    iter->end = true;
}
bool ms_iter_next(multiset_iter *iter)
{
    if (iter->end)
        return false;
    multiset_entry *scan = &(iter->target->buffer[iter->cursor]);
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
    iter->start = ms_empty(iter->target);
    return true;
}
bool ms_iter_prev(multiset_iter *iter)
{
    if (iter->start)
        return false;
    multiset_entry *scan = &(iter->target->buffer[iter->cursor]);
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
    iter->end = ms_empty(iter->target);
    return true;
}
size_t ms_iter_value(multiset_iter *iter)
{
    if (ms_empty(iter->target))
        return ms_impl_default_value();
    return iter->target->buffer[iter->cursor].value;
}
size_t ms_iter_multiplicity(multiset_iter *iter)
{
    if (ms_empty(iter->target))
        return 0;
    return iter->target->buffer[iter->cursor].multiplicity;
}
size_t ms_iter_index(multiset_iter *iter) { return iter->index; }
static multiset_entry *ms_impl_insert_and_return(multiset *_set_, size_t element, bool *new_node)
{
    *new_node = false;
    multiset_entry *entry = ms_impl_get_entry(_set_, element);
    if (entry != NULL)
        return entry;
    *new_node = true;
    if ((double)_set_->capacity * _set_->load <= (double)_set_->count)
    {
        if (!ms_impl_grow(_set_))
            return NULL;
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
        while (true)
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
static bool ms_impl_grow(multiset *_set_)
{
    size_t new_size = ms_impl_calculate_size(_set_->capacity + _set_->capacity / 2);
    multiset *_new_set_ = ms_new(new_size, _set_->load, _set_->cmp, _set_->hash);
    if (!_new_set_)
        return false;
    multiset_iter iter;
    for (ms_iter_init(&iter, _set_); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        ms_insert_many(_new_set_, ms_iter_value(&iter), ms_iter_multiplicity(&iter));
    }
    if (_set_->count != _new_set_->count)
    {
        ms_free(_new_set_);
        return false;
    }
    multiset_entry *tmp = _set_->buffer;
    _set_->buffer = _new_set_->buffer;
    _new_set_->buffer = tmp;
    _set_->capacity = _new_set_->capacity;
    ms_free(_new_set_);
    return true;
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
    return NULL;
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
