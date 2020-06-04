#ifndef CMC_TEST_SRC_HASHSET
#define CMC_TEST_SRC_HASHSET

#include "cmc/hashset.h"

struct hashset
{
    struct hashset_entry *buffer;
    size_t capacity;
    size_t count;
    double load;
    int flag;
    struct hashset_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct hashset_entry
{
    size_t value;
    size_t dist;
    enum cmc_entry_state state;
};
struct hashset_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashset_iter
{
    struct hashset *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
};
struct hashset *hs_new(size_t capacity, double load, struct hashset_fval *f_val);
struct hashset *hs_new_custom(size_t capacity, double load, struct hashset_fval *f_val, struct cmc_alloc_node *alloc,
                              struct cmc_callbacks *callbacks);
void hs_clear(struct hashset *_set_);
void hs_free(struct hashset *_set_);
void hs_customize(struct hashset *_set_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool hs_insert(struct hashset *_set_, size_t value);
_Bool hs_remove(struct hashset *_set_, size_t value);
_Bool hs_max(struct hashset *_set_, size_t *value);
_Bool hs_min(struct hashset *_set_, size_t *value);
_Bool hs_contains(struct hashset *_set_, size_t value);
_Bool hs_empty(struct hashset *_set_);
_Bool hs_full(struct hashset *_set_);
size_t hs_count(struct hashset *_set_);
size_t hs_capacity(struct hashset *_set_);
double hs_load(struct hashset *_set_);
int hs_flag(struct hashset *_set_);
_Bool hs_resize(struct hashset *_set_, size_t capacity);
struct hashset *hs_copy_of(struct hashset *_set_);
_Bool hs_equals(struct hashset *_set1_, struct hashset *_set2_);
struct cmc_string hs_to_string(struct hashset *_set_);
_Bool hs_print(struct hashset *_set_, FILE *fptr);
struct hashset *hs_union(struct hashset *_set1_, struct hashset *_set2_);
struct hashset *hs_intersection(struct hashset *_set1_, struct hashset *_set2_);
struct hashset *hs_difference(struct hashset *_set1_, struct hashset *_set2_);
struct hashset *hs_symmetric_difference(struct hashset *_set1_, struct hashset *_set2_);
_Bool hs_is_subset(struct hashset *_set1_, struct hashset *_set2_);
_Bool hs_is_superset(struct hashset *_set1_, struct hashset *_set2_);
_Bool hs_is_proper_subset(struct hashset *_set1_, struct hashset *_set2_);
_Bool hs_is_proper_superset(struct hashset *_set1_, struct hashset *_set2_);
_Bool hs_is_disjointset(struct hashset *_set1_, struct hashset *_set2_);
struct hashset_iter hs_iter_start(struct hashset *target);
struct hashset_iter hs_iter_end(struct hashset *target);
_Bool hs_iter_at_start(struct hashset_iter *iter);
_Bool hs_iter_at_end(struct hashset_iter *iter);
_Bool hs_iter_to_start(struct hashset_iter *iter);
_Bool hs_iter_to_end(struct hashset_iter *iter);
_Bool hs_iter_next(struct hashset_iter *iter);
_Bool hs_iter_prev(struct hashset_iter *iter);
_Bool hs_iter_advance(struct hashset_iter *iter, size_t steps);
_Bool hs_iter_rewind(struct hashset_iter *iter, size_t steps);
_Bool hs_iter_go_to(struct hashset_iter *iter, size_t index);
size_t hs_iter_value(struct hashset_iter *iter);
size_t hs_iter_index(struct hashset_iter *iter);
static struct hashset_entry *hs_impl_get_entry(struct hashset *_set_, size_t value);
static size_t hs_impl_calculate_size(size_t required);
static struct hashset_iter hs_impl_it_start(struct hashset *_set_);
static struct hashset_iter hs_impl_it_end(struct hashset *_set_);
struct hashset *hs_new(size_t capacity, double load, struct hashset_fval *f_val)
{
    struct cmc_alloc_node *alloc = &cmc_alloc_node_default;
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffffULL * load)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    size_t real_capacity = hs_impl_calculate_size(capacity / load);
    struct hashset *_set_ = alloc->malloc(sizeof(struct hashset));
    if (!_set_)
        return ((void *)0);
    _set_->buffer = alloc->calloc(real_capacity, sizeof(struct hashset_entry));
    if (!_set_->buffer)
    {
        alloc->free(_set_);
        return ((void *)0);
    }
    _set_->count = 0;
    _set_->capacity = real_capacity;
    _set_->load = load;
    _set_->flag = CMC_FLAG_OK;
    _set_->f_val = f_val;
    _set_->alloc = alloc;
    _set_->callbacks = ((void *)0);
    return _set_;
}
struct hashset *hs_new_custom(size_t capacity, double load, struct hashset_fval *f_val, struct cmc_alloc_node *alloc,
                              struct cmc_callbacks *callbacks)
{
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffffULL * load)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    size_t real_capacity = hs_impl_calculate_size(capacity / load);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct hashset *_set_ = alloc->malloc(sizeof(struct hashset));
    if (!_set_)
        return ((void *)0);
    _set_->buffer = alloc->calloc(real_capacity, sizeof(struct hashset_entry));
    if (!_set_->buffer)
    {
        alloc->free(_set_);
        return ((void *)0);
    }
    _set_->count = 0;
    _set_->capacity = real_capacity;
    _set_->load = load;
    _set_->flag = CMC_FLAG_OK;
    _set_->f_val = f_val;
    _set_->alloc = alloc;
    _set_->callbacks = callbacks;
    return _set_;
}
void hs_clear(struct hashset *_set_)
{
    if (_set_->f_val->free)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct hashset_entry *entry = &(_set_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                _set_->f_val->free(entry->value);
            }
        }
    }
    memset(_set_->buffer, 0, sizeof(struct hashset_entry) * _set_->capacity);
    _set_->count = 0;
    _set_->flag = CMC_FLAG_OK;
}
void hs_free(struct hashset *_set_)
{
    if (_set_->f_val->free)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct hashset_entry *entry = &(_set_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                _set_->f_val->free(entry->value);
            }
        }
    }
    _set_->alloc->free(_set_->buffer);
    _set_->alloc->free(_set_);
}
void hs_customize(struct hashset *_set_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _set_->alloc = &cmc_alloc_node_default;
    else
        _set_->alloc = alloc;
    _set_->callbacks = callbacks;
    _set_->flag = CMC_FLAG_OK;
}
_Bool hs_insert(struct hashset *_set_, size_t value)
{
    if (hs_full(_set_))
    {
        if (!hs_resize(_set_, _set_->capacity + 1))
            return 0;
    }
    if (hs_impl_get_entry(_set_, value) != ((void *)0))
    {
        _set_->flag = CMC_FLAG_DUPLICATE;
        return 0;
    }
    size_t hash = _set_->f_val->hash(value);
    size_t original_pos = hash % _set_->capacity;
    size_t pos = original_pos;
    struct hashset_entry *target = &(_set_->buffer[pos]);
    if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)
    {
        target->value = value;
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
                target->value = value;
                target->dist = pos - original_pos;
                target->state = CMC_ES_FILLED;
                break;
            }
            else if (target->dist < pos - original_pos)
            {
                size_t tmp = target->value;
                size_t tmp_dist = target->dist;
                target->value = value;
                target->dist = pos - original_pos;
                value = tmp;
                original_pos = pos - tmp_dist;
            }
        }
    }
    _set_->count++;
    _set_->flag = CMC_FLAG_OK;
    if (_set_->callbacks && _set_->callbacks->create)
        _set_->callbacks->create();
    return 1;
}
_Bool hs_remove(struct hashset *_set_, size_t value)
{
    if (hs_empty(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct hashset_entry *result = hs_impl_get_entry(_set_, value);
    if (result == ((void *)0))
    {
        _set_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    result->value = (size_t){ 0 };
    result->dist = 0;
    result->state = CMC_ES_DELETED;
    _set_->count--;
    _set_->flag = CMC_FLAG_OK;
    if (_set_->callbacks && _set_->callbacks->delete)
        _set_->callbacks->delete ();
    return 1;
}
_Bool hs_max(struct hashset *_set_, size_t *value)
{
    if (hs_empty(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    size_t max_value;
    struct hashset_iter iter = hs_iter_start(_set_);
    for (; !hs_iter_at_end(&iter); hs_iter_next(&iter))
    {
        size_t result = hs_iter_value(&iter);
        size_t index = hs_iter_index(&iter);
        if (index == 0)
            max_value = result;
        else if (_set_->f_val->cmp(result, max_value) > 0)
            max_value = result;
    }
    if (value)
        *value = max_value;
    _set_->flag = CMC_FLAG_OK;
    if (_set_->callbacks && _set_->callbacks->read)
        _set_->callbacks->read();
    return 1;
}
_Bool hs_min(struct hashset *_set_, size_t *value)
{
    if (hs_empty(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    size_t min_value;
    struct hashset_iter iter = hs_iter_start(_set_);
    for (; !hs_iter_at_end(&iter); hs_iter_next(&iter))
    {
        size_t result = hs_iter_value(&iter);
        size_t index = hs_iter_index(&iter);
        if (index == 0)
            min_value = result;
        else if (_set_->f_val->cmp(result, min_value) < 0)
            min_value = result;
    }
    if (value)
        *value = min_value;
    _set_->flag = CMC_FLAG_OK;
    if (_set_->callbacks && _set_->callbacks->read)
        _set_->callbacks->read();
    return 1;
}
_Bool hs_contains(struct hashset *_set_, size_t value)
{
    _set_->flag = CMC_FLAG_OK;
    _Bool result = hs_impl_get_entry(_set_, value) != ((void *)0);
    if (_set_->callbacks && _set_->callbacks->read)
        _set_->callbacks->read();
    return result;
}
_Bool hs_empty(struct hashset *_set_)
{
    return _set_->count == 0;
}
_Bool hs_full(struct hashset *_set_)
{
    return (double)_set_->capacity * _set_->load <= (double)_set_->count;
}
size_t hs_count(struct hashset *_set_)
{
    return _set_->count;
}
size_t hs_capacity(struct hashset *_set_)
{
    return _set_->capacity;
}
double hs_load(struct hashset *_set_)
{
    return _set_->load;
}
int hs_flag(struct hashset *_set_)
{
    return _set_->flag;
}
_Bool hs_resize(struct hashset *_set_, size_t capacity)
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
    size_t theoretical_size = hs_impl_calculate_size(capacity);
    if (theoretical_size < _set_->count / _set_->load)
    {
        _set_->flag = CMC_FLAG_INVALID;
        return 0;
    }
    struct hashset *_new_set_ = hs_new_custom(capacity, _set_->load, _set_->f_val, _set_->alloc, ((void *)0));
    if (!_new_set_)
    {
        _set_->flag = CMC_FLAG_ALLOC;
        return 0;
    }
    struct hashset_iter iter = hs_iter_start(_set_);
    for (; !hs_iter_at_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        hs_insert(_new_set_, value);
    }
    if (_set_->count != _new_set_->count)
    {
        hs_free(_new_set_);
        _set_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    struct hashset_entry *tmp_b = _set_->buffer;
    _set_->buffer = _new_set_->buffer;
    _new_set_->buffer = tmp_b;
    size_t tmp_c = _set_->capacity;
    _set_->capacity = _new_set_->capacity;
    _new_set_->capacity = tmp_c;
    _new_set_->f_val = &(struct hashset_fval){ ((void *)0) };
    hs_free(_new_set_);
success:
    if (_set_->callbacks && _set_->callbacks->resize)
        _set_->callbacks->resize();
    return 1;
}
struct hashset *hs_copy_of(struct hashset *_set_)
{
    struct hashset *result =
        hs_new_custom(_set_->capacity * _set_->load, _set_->load, _set_->f_val, _set_->alloc, _set_->callbacks);
    if (!result)
    {
        _set_->flag = CMC_FLAG_ERROR;
        return ((void *)0);
    }
    if (_set_->f_val->cpy)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct hashset_entry *scan = &(_set_->buffer[i]);
            if (scan->state != CMC_ES_EMPTY)
            {
                struct hashset_entry *target = &(result->buffer[i]);
                if (scan->state == CMC_ES_DELETED)
                    target->state = CMC_ES_DELETED;
                else
                {
                    target->state = scan->state;
                    target->dist = scan->dist;
                    target->value = _set_->f_val->cpy(scan->value);
                }
            }
        }
    }
    else
        memcpy(result->buffer, _set_->buffer, sizeof(struct hashset_entry) * _set_->capacity);
    result->count = _set_->count;
    _set_->flag = CMC_FLAG_OK;
    return result;
}
_Bool hs_equals(struct hashset *_set1_, struct hashset *_set2_)
{
    _set1_->flag = CMC_FLAG_OK;
    _set2_->flag = CMC_FLAG_OK;
    if (_set1_->count != _set2_->count)
        return 0;
    if (_set1_->count == 0)
        return 1;
    struct hashset_iter iter = hs_iter_start(_set1_);
    for (; !hs_iter_at_end(&iter); hs_iter_next(&iter))
    {
        if (hs_impl_get_entry(_set2_, hs_iter_value(&iter)) == ((void *)0))
            return 0;
    }
    return 1;
}
struct hashset *hs_union(struct hashset *_set1_, struct hashset *_set2_)
{
    struct hashset *_set_r_ = hs_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
    {
        _set1_->flag = CMC_FLAG_ALLOC;
        _set2_->flag = CMC_FLAG_ALLOC;
        return ((void *)0);
    }
    struct hashset_iter iter1 = hs_iter_start(_set1_);
    struct hashset_iter iter2 = hs_iter_start(_set2_);
    for (; !hs_iter_at_end(&iter1); hs_iter_next(&iter1))
    {
        hs_insert(_set_r_, hs_iter_value(&iter1));
    }
    for (; !hs_iter_at_end(&iter2); hs_iter_next(&iter2))
    {
        hs_insert(_set_r_, hs_iter_value(&iter2));
    }
    _set_r_->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct hashset *hs_intersection(struct hashset *_set1_, struct hashset *_set2_)
{
    struct hashset *_set_r_ = hs_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
    {
        _set1_->flag = CMC_FLAG_ALLOC;
        _set2_->flag = CMC_FLAG_ALLOC;
        return ((void *)0);
    }
    struct hashset *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;
    struct hashset *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;
    struct hashset_iter iter = hs_iter_start(_set_A_);
    for (; !hs_iter_at_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (hs_impl_get_entry(_set_B_, value) != ((void *)0))
            hs_insert(_set_r_, value);
    }
    _set_r_->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct hashset *hs_difference(struct hashset *_set1_, struct hashset *_set2_)
{
    struct hashset *_set_r_ = hs_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
    {
        _set1_->flag = CMC_FLAG_ALLOC;
        _set2_->flag = CMC_FLAG_ALLOC;
        return ((void *)0);
    }
    struct hashset_iter iter = hs_iter_start(_set1_);
    for (; !hs_iter_at_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (hs_impl_get_entry(_set2_, value) == ((void *)0))
            hs_insert(_set_r_, value);
    }
    _set_r_->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct hashset *hs_symmetric_difference(struct hashset *_set1_, struct hashset *_set2_)
{
    struct hashset *_set_r_ = hs_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
    {
        _set1_->flag = CMC_FLAG_ALLOC;
        _set2_->flag = CMC_FLAG_ALLOC;
        return ((void *)0);
    }
    struct hashset_iter iter1 = hs_iter_start(_set1_);
    struct hashset_iter iter2 = hs_iter_start(_set2_);
    for (; !hs_iter_at_end(&iter1); hs_iter_next(&iter1))
    {
        size_t value = hs_iter_value(&iter1);
        if (hs_impl_get_entry(_set2_, value) == ((void *)0))
            hs_insert(_set_r_, value);
    }
    for (; !hs_iter_at_end(&iter2); hs_iter_next(&iter2))
    {
        size_t value = hs_iter_value(&iter2);
        if (hs_impl_get_entry(_set1_, value) == ((void *)0))
            hs_insert(_set_r_, value);
    }
    _set_r_->callbacks = _set1_->callbacks;
    return _set_r_;
}
_Bool hs_is_subset(struct hashset *_set1_, struct hashset *_set2_)
{
    _set1_->flag = CMC_FLAG_OK;
    _set2_->flag = CMC_FLAG_OK;
    if (_set1_->count > _set2_->count)
        return 0;
    if (hs_empty(_set1_))
        return 1;
    struct hashset_iter iter = hs_iter_start(_set1_);
    for (; !hs_iter_at_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (hs_impl_get_entry(_set2_, value) == ((void *)0))
            return 0;
    }
    return 1;
}
_Bool hs_is_superset(struct hashset *_set1_, struct hashset *_set2_)
{
    return hs_is_subset(_set2_, _set1_);
}
_Bool hs_is_proper_subset(struct hashset *_set1_, struct hashset *_set2_)
{
    _set1_->flag = CMC_FLAG_OK;
    _set2_->flag = CMC_FLAG_OK;
    if (_set1_->count >= _set2_->count)
        return 0;
    if (hs_empty(_set1_))
    {
        if (!hs_empty(_set2_))
            return 1;
        else
            return 0;
    }
    struct hashset_iter iter = hs_iter_start(_set1_);
    for (; !hs_iter_at_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (hs_impl_get_entry(_set2_, value) == ((void *)0))
            return 0;
    }
    return 1;
}
_Bool hs_is_proper_superset(struct hashset *_set1_, struct hashset *_set2_)
{
    return hs_is_proper_subset(_set2_, _set1_);
}
_Bool hs_is_disjointset(struct hashset *_set1_, struct hashset *_set2_)
{
    _set1_->flag = CMC_FLAG_OK;
    _set2_->flag = CMC_FLAG_OK;
    if (hs_empty(_set1_))
        return 1;
    struct hashset_iter iter = hs_iter_start(_set1_);
    for (; !hs_iter_at_end(&iter); hs_iter_next(&iter))
    {
        size_t value = hs_iter_value(&iter);
        if (hs_impl_get_entry(_set2_, value) != ((void *)0))
            return 0;
    }
    return 1;
}
struct hashset_iter hs_iter_start(struct hashset *target)
{
    struct hashset_iter iter;
    iter.target = target;
    iter.cursor = 0;
    iter.index = 0;
    iter.first = 0;
    iter.last = 0;
    iter.start = 1;
    iter.end = hs_empty(target);
    if (!hs_empty(target))
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
struct hashset_iter hs_iter_end(struct hashset *target)
{
    struct hashset_iter iter;
    iter.target = target;
    iter.cursor = 0;
    iter.index = 0;
    iter.first = 0;
    iter.last = 0;
    iter.start = hs_empty(target);
    iter.end = 1;
    if (!hs_empty(target))
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
_Bool hs_iter_at_start(struct hashset_iter *iter)
{
    return hs_empty(iter->target) || iter->start;
}
_Bool hs_iter_at_end(struct hashset_iter *iter)
{
    return hs_empty(iter->target) || iter->end;
}
_Bool hs_iter_to_start(struct hashset_iter *iter)
{
    if (!hs_empty(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = 1;
        iter->end = hs_empty(iter->target);
        return 1;
    }
    return 0;
}
_Bool hs_iter_to_end(struct hashset_iter *iter)
{
    if (!hs_empty(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = hs_empty(iter->target);
        iter->end = 1;
        return 1;
    }
    return 0;
}
_Bool hs_iter_next(struct hashset_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = hs_empty(iter->target);
    struct hashset_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool hs_iter_prev(struct hashset_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = hs_empty(iter->target);
    struct hashset_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool hs_iter_advance(struct hashset_iter *iter, size_t steps)
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
        hs_iter_next(iter);
    return 1;
}
_Bool hs_iter_rewind(struct hashset_iter *iter, size_t steps)
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
_Bool hs_iter_go_to(struct hashset_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return hs_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return hs_iter_advance(iter, index - iter->index);
    return 1;
}
size_t hs_iter_value(struct hashset_iter *iter)
{
    if (hs_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor].value;
}
size_t hs_iter_index(struct hashset_iter *iter)
{
    return iter->index;
}
static struct hashset_entry *hs_impl_get_entry(struct hashset *_set_, size_t value)
{
    size_t hash = _set_->f_val->hash(value);
    size_t pos = hash % _set_->capacity;
    struct hashset_entry *target = &(_set_->buffer[pos]);
    while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED)
    {
        if (_set_->f_val->cmp(target->value, value) == 0)
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

#endif /* CMC_TEST_SRC_HASHSET */
