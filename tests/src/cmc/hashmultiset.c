
#include "cmc/hashmultiset.h"

static size_t hms_impl_multiplicity_of(struct hashmultiset *_set_, size_t value);
static struct hashmultiset_entry *hms_impl_insert_and_return(struct hashmultiset *_set_, size_t value, _Bool *new_node);
static struct hashmultiset_entry *hms_impl_get_entry(struct hashmultiset *_set_, size_t value);
static size_t hms_impl_calculate_size(size_t required);
struct hashmultiset *hms_new(size_t capacity, double load, struct hashmultiset_fval *f_val)
{
    return hms_new_custom(capacity, load, f_val, ((void *)0), ((void *)0));
}
struct hashmultiset *hms_new_custom(size_t capacity, double load, struct hashmultiset_fval *f_val,
                                    struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    ;
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffffULL * load)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    size_t real_capacity = hms_impl_calculate_size(capacity / load);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct hashmultiset *_set_ = alloc->malloc(sizeof(struct hashmultiset));
    if (!_set_)
        return ((void *)0);
    _set_->buffer = alloc->calloc(real_capacity, sizeof(struct hashmultiset_entry));
    if (!_set_->buffer)
    {
        alloc->free(_set_);
        return ((void *)0);
    }
    _set_->count = 0;
    _set_->cardinality = 0;
    _set_->capacity = real_capacity;
    _set_->load = load;
    _set_->flag = CMC_FLAG_OK;
    _set_->f_val = f_val;
    _set_->alloc = alloc;
    (_set_)->callbacks = callbacks;
    return _set_;
}
void hms_clear(struct hashmultiset *_set_)
{
    if (_set_->f_val->free)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct hashmultiset_entry *entry = &(_set_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                _set_->f_val->free(entry->value);
            }
        }
    }
    memset(_set_->buffer, 0, sizeof(struct hashmultiset_entry) * _set_->capacity);
    _set_->count = 0;
    _set_->flag = CMC_FLAG_OK;
}
void hms_free(struct hashmultiset *_set_)
{
    if (_set_->f_val->free)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct hashmultiset_entry *entry = &(_set_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                _set_->f_val->free(entry->value);
            }
        }
    }
    _set_->alloc->free(_set_->buffer);
    _set_->alloc->free(_set_);
}
void hms_customize(struct hashmultiset *_set_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    ;
    if (!alloc)
        _set_->alloc = &cmc_alloc_node_default;
    else
        _set_->alloc = alloc;
    (_set_)->callbacks = callbacks;
    _set_->flag = CMC_FLAG_OK;
}
_Bool hms_insert(struct hashmultiset *_set_, size_t value)
{
    _Bool new_node;
    struct hashmultiset_entry *entry = hms_impl_insert_and_return(_set_, value, &new_node);
    if (!entry)
    {
        _set_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    if (!new_node)
        entry->multiplicity++;
    _set_->cardinality++;
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->create)
        (_set_)->callbacks->create();
    ;
    return 1;
}
_Bool hms_insert_many(struct hashmultiset *_set_, size_t value, size_t count)
{
    if (count == 0)
        goto success;
    _Bool new_node;
    struct hashmultiset_entry *entry = hms_impl_insert_and_return(_set_, value, &new_node);
    if (!entry)
    {
        _set_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    if (new_node)
        entry->multiplicity = count;
    else
        entry->multiplicity += count;
    _set_->cardinality += count;
success:
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->create)
        (_set_)->callbacks->create();
    ;
    return 1;
}
_Bool hms_update(struct hashmultiset *_set_, size_t value, size_t multiplicity)
{
    if (multiplicity == 0)
    {
        struct hashmultiset_entry *result = hms_impl_get_entry(_set_, value);
        if (!result)
            goto success;
        _set_->count--;
        _set_->cardinality -= result->multiplicity;
        result->value = (size_t){ 0 };
        result->multiplicity = 0;
        result->dist = 0;
        result->state = CMC_ES_DELETED;
        goto success;
    }
    _Bool new_node;
    struct hashmultiset_entry *entry = hms_impl_insert_and_return(_set_, value, &new_node);
    if (!entry)
        return 0;
    if (new_node)
        _set_->cardinality++;
    _set_->cardinality = (_set_->cardinality - entry->multiplicity) + multiplicity;
    entry->multiplicity = multiplicity;
success:
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->update)
        (_set_)->callbacks->update();
    ;
    return 1;
}
_Bool hms_remove(struct hashmultiset *_set_, size_t value)
{
    if (hms_empty(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct hashmultiset_entry *result = hms_impl_get_entry(_set_, value);
    if (!result)
    {
        _set_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    if (result->multiplicity > 1)
        result->multiplicity--;
    else
    {
        result->value = (size_t){ 0 };
        result->multiplicity = 0;
        result->dist = 0;
        result->state = CMC_ES_DELETED;
        _set_->count--;
    }
    _set_->cardinality--;
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->delete)
        (_set_)->callbacks->delete ();
    ;
    return 1;
}
size_t hms_remove_all(struct hashmultiset *_set_, size_t value)
{
    if (hms_empty(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct hashmultiset_entry *result = hms_impl_get_entry(_set_, value);
    if (!result)
    {
        _set_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    size_t removed = result->multiplicity;
    result->value = (size_t){ 0 };
    result->multiplicity = 0;
    result->dist = 0;
    result->state = CMC_ES_DELETED;
    _set_->count--;
    _set_->cardinality -= removed;
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->delete)
        (_set_)->callbacks->delete ();
    ;
    return removed;
}
_Bool hms_max(struct hashmultiset *_set_, size_t *value)
{
    if (hms_empty(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    _Bool first = 1;
    size_t max_val = (size_t){ 0 };
    for (size_t i = 0; i < _set_->capacity; i++)
    {
        if (_set_->buffer[i].state == CMC_ES_FILLED)
        {
            if (first)
            {
                max_val = _set_->buffer[i].value;
                first = 0;
            }
            else if (_set_->f_val->cmp(_set_->buffer[i].value, max_val) > 0)
            {
                max_val = _set_->buffer[i].value;
            }
        }
    }
    if (value)
        *value = max_val;
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->read)
        (_set_)->callbacks->read();
    ;
    return 1;
}
_Bool hms_min(struct hashmultiset *_set_, size_t *value)
{
    if (hms_empty(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    _Bool first = 1;
    size_t min_val = (size_t){ 0 };
    for (size_t i = 0; i < _set_->capacity; i++)
    {
        if (_set_->buffer[i].state == CMC_ES_FILLED)
        {
            if (first)
            {
                min_val = _set_->buffer[i].value;
                first = 0;
            }
            else if (_set_->f_val->cmp(_set_->buffer[i].value, min_val) < 0)
            {
                min_val = _set_->buffer[i].value;
            }
        }
    }
    if (value)
        *value = min_val;
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->read)
        (_set_)->callbacks->read();
    ;
    return 1;
}
size_t hms_multiplicity_of(struct hashmultiset *_set_, size_t value)
{
    struct hashmultiset_entry *entry = hms_impl_get_entry(_set_, value);
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->read)
        (_set_)->callbacks->read();
    ;
    if (!entry)
        return 0;
    return entry->multiplicity;
}
_Bool hms_contains(struct hashmultiset *_set_, size_t value)
{
    _set_->flag = CMC_FLAG_OK;
    _Bool result = hms_impl_get_entry(_set_, value) != ((void *)0);
    if ((_set_)->callbacks && (_set_)->callbacks->read)
        (_set_)->callbacks->read();
    ;
    return result;
}
_Bool hms_empty(struct hashmultiset *_set_)
{
    return _set_->count == 0;
}
_Bool hms_full(struct hashmultiset *_set_)
{
    return (double)_set_->capacity * _set_->load <= (double)_set_->count;
}
size_t hms_count(struct hashmultiset *_set_)
{
    return _set_->count;
}
size_t hms_cardinality(struct hashmultiset *_set_)
{
    return _set_->cardinality;
}
size_t hms_capacity(struct hashmultiset *_set_)
{
    return _set_->capacity;
}
double hms_load(struct hashmultiset *_set_)
{
    return _set_->load;
}
int hms_flag(struct hashmultiset *_set_)
{
    return _set_->flag;
}
_Bool hms_resize(struct hashmultiset *_set_, size_t capacity)
{
    _set_->flag = CMC_FLAG_OK;
    if (_set_->capacity == capacity)
        goto success;
    if (_set_->capacity > capacity / _set_->load)
        goto success;
    if (capacity >= 0xffffffffffffffffULL * _set_->load)
    {
        _set_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    size_t theoretical_size = hms_impl_calculate_size(capacity);
    if (theoretical_size < _set_->count / _set_->load)
    {
        _set_->flag = CMC_FLAG_INVALID;
        return 0;
    }
    struct hashmultiset *_new_set_ = hms_new_custom(capacity, _set_->load, _set_->f_val, _set_->alloc, ((void *)0));
    if (!_new_set_)
    {
        _set_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    for (size_t i = 0; i < _set_->capacity; i++)
    {
        if (_set_->buffer[i].state == CMC_ES_FILLED)
        {
            size_t value = _set_->buffer[i].value;
            size_t multiplicity = _set_->buffer[i].multiplicity;
            struct hashmultiset_entry *entry = hms_impl_insert_and_return(_new_set_, value, ((void *)0));
            entry->multiplicity = multiplicity;
        }
    }
    if (_set_->count != _new_set_->count)
    {
        hms_free(_new_set_);
        _set_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    struct hashmultiset_entry *tmp_b = _set_->buffer;
    _set_->buffer = _new_set_->buffer;
    _new_set_->buffer = tmp_b;
    size_t tmp_c = _set_->capacity;
    _set_->capacity = _new_set_->capacity;
    _new_set_->capacity = tmp_c;
    _new_set_->f_val = &(struct hashmultiset_fval){ ((void *)0) };
    hms_free(_new_set_);
success:
    if ((_set_)->callbacks && (_set_)->callbacks->resize)
        (_set_)->callbacks->resize();
    ;
    return 1;
}
struct hashmultiset *hms_copy_of(struct hashmultiset *_set_)
{
    struct hashmultiset *result =
        hms_new_custom(_set_->capacity * _set_->load, _set_->load, _set_->f_val, _set_->alloc, ((void *)0));
    if (!result)
    {
        _set_->flag = CMC_FLAG_ERROR;
        return ((void *)0);
    }
    (result)->callbacks = _set_->callbacks;
    if (_set_->f_val->cpy)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct hashmultiset_entry *scan = &(_set_->buffer[i]);
            if (scan->state != CMC_ES_EMPTY)
            {
                struct hashmultiset_entry *target = &(result->buffer[i]);
                if (scan->state == CMC_ES_DELETED)
                    target->state = CMC_ES_DELETED;
                else
                {
                    target->state = scan->state;
                    target->dist = scan->dist;
                    target->multiplicity = scan->multiplicity;
                    target->value = _set_->f_val->cpy(scan->value);
                }
            }
        }
    }
    else
        memcpy(result->buffer, _set_->buffer, sizeof(struct hashmultiset_entry) * _set_->capacity);
    result->count = _set_->count;
    result->cardinality = _set_->cardinality;
    _set_->flag = CMC_FLAG_OK;
    return result;
}
_Bool hms_equals(struct hashmultiset *_set1_, struct hashmultiset *_set2_)
{
    _set1_->flag = CMC_FLAG_OK;
    _set2_->flag = CMC_FLAG_OK;
    if (_set1_->count != _set2_->count)
        return 0;
    if (_set1_->cardinality != _set2_->cardinality)
        return 0;
    if (_set1_->count == 0)
        return 1;
    struct hashmultiset *_set_a_;
    struct hashmultiset *_set_b_;
    _set_a_ = _set1_->capacity < _set2_->capacity ? _set1_ : _set2_;
    _set_b_ = _set_a_ == _set1_ ? _set2_ : _set1_;
    for (size_t i = 0; i < _set_a_->capacity; i++)
    {
        if (_set_a_->buffer[i].state == CMC_ES_FILLED)
        {
            struct hashmultiset_entry *entry_a = &(_set_a_->buffer[i]);
            struct hashmultiset_entry *entry_b = hms_impl_get_entry(_set_b_, entry_a->value);
            if (!entry_b)
                return 0;
            if (entry_a->multiplicity != entry_b->multiplicity)
                return 0;
        }
    }
    return 1;
}
static size_t hms_impl_multiplicity_of(struct hashmultiset *_set_, size_t value)
{
    struct hashmultiset_entry *entry = hms_impl_get_entry(_set_, value);
    if (!entry)
        return 0;
    return entry->multiplicity;
}
static struct hashmultiset_entry *hms_impl_insert_and_return(struct hashmultiset *_set_, size_t value, _Bool *new_node)
{
    if (new_node)
        *new_node = 0;
    struct hashmultiset_entry *entry = hms_impl_get_entry(_set_, value);
    if (entry != ((void *)0))
        return entry;
    if (new_node)
        *new_node = 1;
    if (hms_full(_set_))
    {
        if (!hms_resize(_set_, _set_->capacity + 1))
            return ((void *)0);
    }
    size_t hash = _set_->f_val->hash(value);
    size_t original_pos = hash % _set_->capacity;
    size_t pos = original_pos;
    size_t curr_mul = 1;
    struct hashmultiset_entry *target = &(_set_->buffer[pos]);
    struct hashmultiset_entry *to_return = ((void *)0);
    if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)
    {
        target->value = value;
        target->multiplicity = curr_mul;
        target->dist = pos - original_pos;
        target->state = CMC_ES_FILLED;
        to_return = target;
    }
    else
    {
        while (1)
        {
            pos++;
            target = &(_set_->buffer[pos % _set_->capacity]);
            if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)
            {
                target->value = value;
                target->multiplicity = curr_mul;
                target->dist = pos - original_pos;
                target->state = CMC_ES_FILLED;
                if (!to_return)
                    to_return = target;
                break;
            }
            else if (target->dist < pos - original_pos)
            {
                size_t tmp = target->value;
                size_t tmp_dist = target->dist;
                size_t tmp_mul = target->multiplicity;
                target->value = value;
                target->dist = pos - original_pos;
                target->multiplicity = curr_mul;
                value = tmp;
                original_pos = pos - tmp_dist;
                curr_mul = tmp_mul;
                if (!to_return)
                    to_return = target;
            }
        }
    }
    _set_->count++;
    return to_return;
}
static struct hashmultiset_entry *hms_impl_get_entry(struct hashmultiset *_set_, size_t value)
{
    size_t hash = _set_->f_val->hash(value);
    size_t pos = hash % _set_->capacity;
    struct hashmultiset_entry *target = &(_set_->buffer[pos]);
    while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED)
    {
        if (_set_->f_val->cmp(target->value, value) == 0)
            return target;
        pos++;
        target = &(_set_->buffer[pos % _set_->capacity]);
    }
    return ((void *)0);
}
static size_t hms_impl_calculate_size(size_t required)
{
    const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);
    if (cmc_hashtable_primes[count - 1] < required)
        return required;
    size_t i = 0;
    while (cmc_hashtable_primes[i] < required)
        i++;
    return cmc_hashtable_primes[i];
}
struct hashmultiset_iter hms_iter_start(struct hashmultiset *target)
{
    struct hashmultiset_iter iter;
    iter.target = target;
    iter.cursor = 0;
    iter.index = 0;
    iter.first = 0;
    iter.last = 0;
    iter.start = 1;
    iter.end = hms_empty(target);
    if (!hms_empty(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            if (target->buffer[i].state == CMC_ES_FILLED)
            {
                iter.first = i;
                break;
            }
        }
        iter.cursor = iter.first;
        for (size_t i = target->capacity; i > 0; i--)
        {
            if (target->buffer[i - 1].state == CMC_ES_FILLED)
            {
                iter.last = i - 1;
                break;
            }
        }
    }
    return iter;
}
struct hashmultiset_iter hms_iter_end(struct hashmultiset *target)
{
    struct hashmultiset_iter iter;
    iter.target = target;
    iter.cursor = 0;
    iter.index = 0;
    iter.first = 0;
    iter.last = 0;
    iter.start = hms_empty(target);
    iter.end = 1;
    if (!hms_empty(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            if (target->buffer[i].state == CMC_ES_FILLED)
            {
                iter.first = i;
                break;
            }
        }
        for (size_t i = target->capacity; i > 0; i--)
        {
            if (target->buffer[i - 1].state == CMC_ES_FILLED)
            {
                iter.last = i - 1;
                break;
            }
        }
        iter.cursor = iter.last;
        iter.index = target->count - 1;
    }
    return iter;
}
_Bool hms_iter_at_start(struct hashmultiset_iter *iter)
{
    return hms_empty(iter->target) || iter->start;
}
_Bool hms_iter_at_end(struct hashmultiset_iter *iter)
{
    return hms_empty(iter->target) || iter->end;
}
_Bool hms_iter_to_start(struct hashmultiset_iter *iter)
{
    if (!hms_empty(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = 1;
        iter->end = hms_empty(iter->target);
        return 1;
    }
    return 0;
}
_Bool hms_iter_to_end(struct hashmultiset_iter *iter)
{
    if (!hms_empty(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = hms_empty(iter->target);
        iter->end = 1;
        return 1;
    }
    return 0;
}
_Bool hms_iter_next(struct hashmultiset_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = hms_empty(iter->target);
    struct hashmultiset_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool hms_iter_prev(struct hashmultiset_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = hms_empty(iter->target);
    struct hashmultiset_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool hms_iter_advance(struct hashmultiset_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= iter->target->count)
        return 0;
    for (size_t i = 0; i < steps; i++)
        hms_iter_next(iter);
    return 1;
}
_Bool hms_iter_rewind(struct hashmultiset_iter *iter, size_t steps)
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
        hms_iter_prev(iter);
    return 1;
}
_Bool hms_iter_go_to(struct hashmultiset_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return hms_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return hms_iter_advance(iter, index - iter->index);
    return 1;
}
size_t hms_iter_value(struct hashmultiset_iter *iter)
{
    if (hms_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor].value;
}
size_t hms_iter_multiplicity(struct hashmultiset_iter *iter)
{
    if (hms_empty(iter->target))
        return 0;
    return iter->target->buffer[iter->cursor].multiplicity;
}
size_t hms_iter_index(struct hashmultiset_iter *iter)
{
    return iter->index;
}
struct hashmultiset *hms_union(struct hashmultiset *_set1_, struct hashmultiset *_set2_)
{
    struct hashmultiset *_set_r_ =
        hms_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct hashmultiset_iter iter1 = hms_iter_start(_set1_);
    struct hashmultiset_iter iter2 = hms_iter_start(_set2_);
    for (; !hms_iter_at_end(&iter1); hms_iter_next(&iter1))
    {
        size_t value = hms_iter_value(&iter1);
        size_t m1 = hms_iter_multiplicity(&iter1);
        size_t m2 = hms_impl_multiplicity_of(_set2_, value);
        size_t max_ = m1 > m2 ? m1 : m2;
        hms_update(_set_r_, value, max_);
    }
    for (; !hms_iter_at_end(&iter2); hms_iter_next(&iter2))
    {
        size_t value = hms_iter_value(&iter2);
        size_t m1 = hms_impl_multiplicity_of(_set1_, value);
        size_t m2 = hms_iter_multiplicity(&iter2);
        size_t max_ = m1 > m2 ? m1 : m2;
        hms_update(_set_r_, value, max_);
    }
    (_set_r_)->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct hashmultiset *hms_intersection(struct hashmultiset *_set1_, struct hashmultiset *_set2_)
{
    struct hashmultiset *_set_r_ =
        hms_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct hashmultiset *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;
    struct hashmultiset *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;
    struct hashmultiset_iter iter = hms_iter_start(_set_A_);
    for (; !hms_iter_at_end(&iter); hms_iter_next(&iter))
    {
        size_t value = hms_iter_value(&iter);
        size_t m1 = hms_iter_multiplicity(&iter);
        size_t m2 = hms_impl_multiplicity_of(_set_B_, value);
        size_t max_ = m1 < m2 ? m1 : m2;
        hms_update(_set_r_, value, max_);
    }
    (_set_r_)->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct hashmultiset *hms_difference(struct hashmultiset *_set1_, struct hashmultiset *_set2_)
{
    struct hashmultiset *_set_r_ =
        hms_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct hashmultiset_iter iter = hms_iter_start(_set1_);
    for (; !hms_iter_at_end(&iter); hms_iter_next(&iter))
    {
        size_t value = hms_iter_value(&iter);
        size_t m1 = hms_iter_multiplicity(&iter);
        size_t m2 = hms_impl_multiplicity_of(_set2_, value);
        if (m1 > m2)
            hms_update(_set_r_, value, m1 - m2);
    }
    (_set_r_)->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct hashmultiset *hms_summation(struct hashmultiset *_set1_, struct hashmultiset *_set2_)
{
    struct hashmultiset *_set_r_ =
        hms_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct hashmultiset_iter iter1 = hms_iter_start(_set1_);
    struct hashmultiset_iter iter2 = hms_iter_start(_set2_);
    for (; !hms_iter_at_end(&iter1); hms_iter_next(&iter1))
    {
        hms_insert_many(_set_r_, hms_iter_value(&iter1), hms_iter_multiplicity(&iter1));
    }
    for (; !hms_iter_at_end(&iter2); hms_iter_next(&iter2))
    {
        hms_insert_many(_set_r_, hms_iter_value(&iter2), hms_iter_multiplicity(&iter2));
    }
    (_set_r_)->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct hashmultiset *hms_symmetric_difference(struct hashmultiset *_set1_, struct hashmultiset *_set2_)
{
    struct hashmultiset *_set_r_ =
        hms_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct hashmultiset_iter iter1 = hms_iter_start(_set1_);
    struct hashmultiset_iter iter2 = hms_iter_start(_set2_);
    for (; !hms_iter_at_end(&iter1); hms_iter_next(&iter1))
    {
        size_t value = hms_iter_value(&iter1);
        size_t m1 = hms_iter_multiplicity(&iter1);
        size_t m2 = hms_impl_multiplicity_of(_set2_, value);
        if (m1 != m2)
        {
            if (m1 > m2)
                hms_update(_set_r_, value, m1 - m2);
            else
                hms_update(_set_r_, value, m2 - m1);
        }
    }
    for (; !hms_iter_at_end(&iter2); hms_iter_next(&iter2))
    {
        size_t value = hms_iter_value(&iter2);
        size_t m1 = hms_impl_multiplicity_of(_set1_, value);
        size_t m2 = hms_iter_multiplicity(&iter2);
        if (m1 != m2)
        {
            if (m1 > m2)
                hms_update(_set_r_, value, m1 - m2);
            else
                hms_update(_set_r_, value, m2 - m1);
        }
    }
    (_set_r_)->callbacks = _set1_->callbacks;
    return _set_r_;
}
_Bool hms_is_subset(struct hashmultiset *_set1_, struct hashmultiset *_set2_)
{
    if (_set1_->count > _set2_->count)
        return 0;
    if (hms_empty(_set1_))
        return 1;
    struct hashmultiset_iter iter = hms_iter_start(_set1_);
    for (; !hms_iter_at_end(&iter); hms_iter_next(&iter))
    {
        size_t value = hms_iter_value(&iter);
        size_t m1 = hms_iter_multiplicity(&iter);
        size_t m2 = hms_impl_multiplicity_of(_set2_, value);
        if (m1 > m2)
            return 0;
    }
    return 1;
}
_Bool hms_is_superset(struct hashmultiset *_set1_, struct hashmultiset *_set2_)
{
    return hms_is_subset(_set2_, _set1_);
}
_Bool hms_is_proper_subset(struct hashmultiset *_set1_, struct hashmultiset *_set2_)
{
    if (_set1_->count >= _set2_->count)
        return 0;
    if (hms_empty(_set1_))
    {
        if (!hms_empty(_set2_))
            return 1;
        else
            return 0;
    }
    struct hashmultiset_iter iter = hms_iter_start(_set1_);
    for (; !hms_iter_at_end(&iter); hms_iter_next(&iter))
    {
        size_t value = hms_iter_value(&iter);
        size_t m1 = hms_iter_multiplicity(&iter);
        size_t m2 = hms_impl_multiplicity_of(_set2_, value);
        if (m1 >= m2)
            return 0;
    }
    return 1;
}
_Bool hms_is_proper_superset(struct hashmultiset *_set1_, struct hashmultiset *_set2_)
{
    return hms_is_proper_subset(_set2_, _set1_);
}
_Bool hms_is_disjointset(struct hashmultiset *_set1_, struct hashmultiset *_set2_)
{
    if (hms_empty(_set1_))
        return 1;
    struct hashmultiset_iter iter = hms_iter_start(_set1_);
    for (; !hms_iter_at_end(&iter); hms_iter_next(&iter))
    {
        size_t value = hms_iter_value(&iter);
        if (hms_impl_get_entry(_set2_, value) != ((void *)0))
            return 0;
    }
    return 1;
}
_Bool hms_to_string(struct hashmultiset *_set_, FILE *fptr)
{
    struct hashmultiset *s_ = _set_;
    return 0 <= fprintf(fptr,
                        "struct %s<%s> "
                        "at %p { "
                        "buffer:%p, "
                        "capacity:%"
                        "I64u"
                        ", "
                        "count:%"
                        "I64u"
                        ", "
                        "cardinality:%"
                        "I64u"
                        ", "
                        "load:%lf, "
                        "flag:%d, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        "hashmultiset", "size_t", s_, s_->buffer, s_->capacity, s_->count, s_->cardinality, s_->load,
                        s_->flag, s_->f_val, s_->alloc, (s_)->callbacks);
}
_Bool hms_print(struct hashmultiset *_set_, FILE *fptr, const char *start, const char *separator, const char *end,
                const char *val_mul_sep)
{
    fprintf(fptr, "%s", start);
    size_t last = 0;
    for (size_t i = _set_->capacity; i > 0; i--)
    {
        if ((_set_->buffer[i - 1]).state == CMC_ES_FILLED)
        {
            last = i - 1;
            break;
        }
    }
    for (size_t i = 0; i < _set_->capacity; i++)
    {
        struct hashmultiset_entry *entry = &(_set_->buffer[i]);
        if (entry->state == CMC_ES_FILLED)
        {
            if (!_set_->f_val->str(fptr, entry->value))
                return 0;
            fprintf(fptr, "%s", val_mul_sep);
            if (fprintf(fptr,
                        "%"
                        "I64u"
                        "",
                        entry->multiplicity) < 0)
                return 0;
            if (i + 1 < last)
                fprintf(fptr, "%s", separator);
        }
    }
    fprintf(fptr, "%s", end);
    return 1;
}
