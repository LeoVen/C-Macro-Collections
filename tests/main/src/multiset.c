#include "cmc/multiset.h"

struct multiset
{
    struct multiset_entry *buffer;
    size_t capacity;
    size_t count;
    size_t cardinality;
    double load;
    int flag;
    struct multiset_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
    struct multiset_iter (*it_start)(struct multiset *);
    struct multiset_iter (*it_end)(struct multiset *);
};
struct multiset_entry
{
    size_t value;
    size_t multiplicity;
    size_t dist;
    enum cmc_entry_state state;
};
struct multiset_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct multiset_iter
{
    struct multiset *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
};
struct multiset *ms_new(size_t capacity, double load,
                        struct multiset_fval *f_val);
struct multiset *ms_new_custom(size_t capacity, double load,
                               struct multiset_fval *f_val,
                               struct cmc_alloc_node *alloc,
                               struct cmc_callbacks *callbacks);
void ms_clear(struct multiset *_set_);
void ms_free(struct multiset *_set_);
void ms_customize(struct multiset *_set_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks);
_Bool ms_insert(struct multiset *_set_, size_t element);
_Bool ms_insert_many(struct multiset *_set_, size_t element, size_t count);
_Bool ms_update(struct multiset *_set_, size_t element, size_t multiplicity);
_Bool ms_remove(struct multiset *_set_, size_t element);
size_t ms_remove_all(struct multiset *_set_, size_t element);
_Bool ms_max(struct multiset *_set_, size_t *value);
_Bool ms_min(struct multiset *_set_, size_t *value);
size_t ms_multiplicity_of(struct multiset *_set_, size_t element);
_Bool ms_contains(struct multiset *_set_, size_t element);
_Bool ms_empty(struct multiset *_set_);
_Bool ms_full(struct multiset *_set_);
size_t ms_count(struct multiset *_set_);
size_t ms_cardinality(struct multiset *_set_);
size_t ms_capacity(struct multiset *_set_);
double ms_load(struct multiset *_set_);
int ms_flag(struct multiset *_set_);
_Bool ms_resize(struct multiset *_set_, size_t capacity);
struct multiset *ms_copy_of(struct multiset *_set_);
_Bool ms_equals(struct multiset *_set1_, struct multiset *_set2_);
struct cmc_string ms_to_string(struct multiset *_set_);
_Bool ms_print(struct multiset *_set_, FILE *fptr);
struct multiset *ms_union(struct multiset *_set1_, struct multiset *_set2_);
struct multiset *ms_intersection(struct multiset *_set1_,
                                 struct multiset *_set2_);
struct multiset *ms_difference(struct multiset *_set1_,
                               struct multiset *_set2_);
struct multiset *ms_summation(struct multiset *_set1_, struct multiset *_set2_);
struct multiset *ms_symmetric_difference(struct multiset *_set1_,
                                         struct multiset *_set2_);
_Bool ms_is_subset(struct multiset *_set1_, struct multiset *_set2_);
_Bool ms_is_superset(struct multiset *_set1_, struct multiset *_set2_);
_Bool ms_is_proper_subset(struct multiset *_set1_, struct multiset *_set2_);
_Bool ms_is_proper_superset(struct multiset *_set1_, struct multiset *_set2_);
_Bool ms_is_disjointset(struct multiset *_set1_, struct multiset *_set2_);
struct multiset_iter *ms_iter_new(struct multiset *target);
void ms_iter_free(struct multiset_iter *iter);
void ms_iter_init(struct multiset_iter *iter, struct multiset *target);
_Bool ms_iter_start(struct multiset_iter *iter);
_Bool ms_iter_end(struct multiset_iter *iter);
void ms_iter_to_start(struct multiset_iter *iter);
void ms_iter_to_end(struct multiset_iter *iter);
_Bool ms_iter_next(struct multiset_iter *iter);
_Bool ms_iter_prev(struct multiset_iter *iter);
_Bool ms_iter_advance(struct multiset_iter *iter, size_t steps);
_Bool ms_iter_rewind(struct multiset_iter *iter, size_t steps);
_Bool ms_iter_go_to(struct multiset_iter *iter, size_t index);
size_t ms_iter_value(struct multiset_iter *iter);
size_t ms_iter_multiplicity(struct multiset_iter *iter);
size_t ms_iter_index(struct multiset_iter *iter);
static size_t ms_impl_multiplicity_of(struct multiset *_set_, size_t element);
static struct multiset_entry *ms_impl_insert_and_return(struct multiset *_set_,
                                                        size_t element,
                                                        _Bool *new_node);
static struct multiset_entry *ms_impl_get_entry(struct multiset *_set_,
                                                size_t element);
static size_t ms_impl_calculate_size(size_t required);
static struct multiset_iter ms_impl_it_start(struct multiset *_set_);
static struct multiset_iter ms_impl_it_end(struct multiset *_set_);
struct multiset *ms_new(size_t capacity, double load,
                        struct multiset_fval *f_val)
{
    struct cmc_alloc_node *alloc = &cmc_alloc_node_default;
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffffULL * load)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    size_t real_capacity = ms_impl_calculate_size(capacity / load);
    struct multiset *_set_ = alloc->malloc(sizeof(struct multiset));
    if (!_set_)
        return ((void *)0);
    _set_->buffer = alloc->calloc(real_capacity, sizeof(struct multiset_entry));
    if (!_set_->buffer)
    {
        alloc->free(_set_);
        return ((void *)0);
    }
    _set_->count = 0;
    _set_->cardinality = 0;
    _set_->capacity = real_capacity;
    _set_->load = load;
    _set_->flag = cmc_flags.OK;
    _set_->f_val = f_val;
    _set_->alloc = alloc;
    _set_->callbacks = ((void *)0);
    _set_->it_start = ms_impl_it_start;
    _set_->it_end = ms_impl_it_end;
    return _set_;
}
struct multiset *ms_new_custom(size_t capacity, double load,
                               struct multiset_fval *f_val,
                               struct cmc_alloc_node *alloc,
                               struct cmc_callbacks *callbacks)
{
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffffULL * load)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    size_t real_capacity = ms_impl_calculate_size(capacity / load);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct multiset *_set_ = alloc->malloc(sizeof(struct multiset));
    if (!_set_)
        return ((void *)0);
    _set_->buffer = alloc->calloc(real_capacity, sizeof(struct multiset_entry));
    if (!_set_->buffer)
    {
        alloc->free(_set_);
        return ((void *)0);
    }
    _set_->count = 0;
    _set_->cardinality = 0;
    _set_->capacity = real_capacity;
    _set_->load = load;
    _set_->flag = cmc_flags.OK;
    _set_->f_val = f_val;
    _set_->alloc = alloc;
    _set_->callbacks = callbacks;
    _set_->it_start = ms_impl_it_start;
    _set_->it_end = ms_impl_it_end;
    return _set_;
}
void ms_clear(struct multiset *_set_)
{
    if (_set_->f_val->free)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct multiset_entry *entry = &(_set_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                _set_->f_val->free(entry->value);
            }
        }
    }
    memset(_set_->buffer, 0, sizeof(struct multiset_entry) * _set_->capacity);
    _set_->count = 0;
    _set_->flag = cmc_flags.OK;
}
void ms_free(struct multiset *_set_)
{
    if (_set_->f_val->free)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct multiset_entry *entry = &(_set_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                _set_->f_val->free(entry->value);
            }
        }
    }
    _set_->alloc->free(_set_->buffer);
    _set_->alloc->free(_set_);
}
void ms_customize(struct multiset *_set_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _set_->alloc = &cmc_alloc_node_default;
    else
        _set_->alloc = alloc;
    _set_->callbacks = callbacks;
    _set_->flag = cmc_flags.OK;
}
_Bool ms_insert(struct multiset *_set_, size_t element)
{
    _Bool new_node;
    struct multiset_entry *entry =
        ms_impl_insert_and_return(_set_, element, &new_node);
    if (!entry)
    {
        _set_->flag = cmc_flags.ERROR;
        return 0;
    }
    if (!new_node)
        entry->multiplicity++;
    _set_->cardinality++;
    _set_->flag = cmc_flags.OK;
    if (_set_->callbacks && _set_->callbacks->create)
        _set_->callbacks->create();
    return 1;
}
_Bool ms_insert_many(struct multiset *_set_, size_t element, size_t count)
{
    if (count == 0)
        goto success;
    _Bool new_node;
    struct multiset_entry *entry =
        ms_impl_insert_and_return(_set_, element, &new_node);
    if (!entry)
    {
        _set_->flag = cmc_flags.ERROR;
        return 0;
    }
    if (new_node)
        entry->multiplicity = count;
    else
        entry->multiplicity += count;
    _set_->cardinality += count;
success:
    _set_->flag = cmc_flags.OK;
    if (_set_->callbacks && _set_->callbacks->create)
        _set_->callbacks->create();
    return 1;
}
_Bool ms_update(struct multiset *_set_, size_t element, size_t multiplicity)
{
    if (multiplicity == 0)
    {
        struct multiset_entry *result = ms_impl_get_entry(_set_, element);
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
    struct multiset_entry *entry =
        ms_impl_insert_and_return(_set_, element, &new_node);
    if (!entry)
        return 0;
    if (new_node)
        _set_->cardinality++;
    _set_->cardinality =
        (_set_->cardinality - entry->multiplicity) + multiplicity;
    entry->multiplicity = multiplicity;
success:
    _set_->flag = cmc_flags.OK;
    if (_set_->callbacks && _set_->callbacks->update)
        _set_->callbacks->update();
    return 1;
}
_Bool ms_remove(struct multiset *_set_, size_t element)
{
    if (ms_empty(_set_))
    {
        _set_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct multiset_entry *result = ms_impl_get_entry(_set_, element);
    if (!result)
    {
        _set_->flag = cmc_flags.NOT_FOUND;
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
    _set_->flag = cmc_flags.OK;
    if (_set_->callbacks && _set_->callbacks->delete)
        _set_->callbacks->delete ();
    return 1;
}
size_t ms_remove_all(struct multiset *_set_, size_t element)
{
    if (ms_empty(_set_))
    {
        _set_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct multiset_entry *result = ms_impl_get_entry(_set_, element);
    if (!result)
    {
        _set_->flag = cmc_flags.NOT_FOUND;
        return 0;
    }
    size_t removed = result->multiplicity;
    result->value = (size_t){ 0 };
    result->multiplicity = 0;
    result->dist = 0;
    result->state = CMC_ES_DELETED;
    _set_->count--;
    _set_->cardinality -= removed;
    _set_->flag = cmc_flags.OK;
    if (_set_->callbacks && _set_->callbacks->delete)
        _set_->callbacks->delete ();
    return removed;
}
_Bool ms_max(struct multiset *_set_, size_t *value)
{
    if (ms_empty(_set_))
    {
        _set_->flag = cmc_flags.EMPTY;
        return 0;
    }
    size_t max_val;
    struct multiset_iter iter;
    for (ms_iter_init(&iter, _set_); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t result = ms_iter_value(&iter);
        size_t index = ms_iter_index(&iter);
        if (index == 0)
            max_val = result;
        else if (_set_->f_val->cmp(result, max_val) > 0)
            max_val = result;
    }
    if (value)
        *value = max_val;
    _set_->flag = cmc_flags.OK;
    if (_set_->callbacks && _set_->callbacks->read)
        _set_->callbacks->read();
    return 1;
}
_Bool ms_min(struct multiset *_set_, size_t *value)
{
    if (ms_empty(_set_))
    {
        _set_->flag = cmc_flags.EMPTY;
        return 0;
    }
    size_t min_val;
    struct multiset_iter iter;
    for (ms_iter_init(&iter, _set_); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t result = ms_iter_value(&iter);
        size_t index = ms_iter_index(&iter);
        if (index == 0)
            min_val = result;
        else if (_set_->f_val->cmp(result, min_val) < 0)
            min_val = result;
    }
    if (value)
        *value = min_val;
    _set_->flag = cmc_flags.OK;
    if (_set_->callbacks && _set_->callbacks->read)
        _set_->callbacks->read();
    return 1;
}
size_t ms_multiplicity_of(struct multiset *_set_, size_t element)
{
    struct multiset_entry *entry = ms_impl_get_entry(_set_, element);
    _set_->flag = cmc_flags.OK;
    if (_set_->callbacks && _set_->callbacks->read)
        _set_->callbacks->read();
    if (!entry)
        return 0;
    return entry->multiplicity;
}
_Bool ms_contains(struct multiset *_set_, size_t element)
{
    _set_->flag = cmc_flags.OK;
    _Bool result = ms_impl_get_entry(_set_, element) != ((void *)0);
    if (_set_->callbacks && _set_->callbacks->read)
        _set_->callbacks->read();
    return result;
}
_Bool ms_empty(struct multiset *_set_)
{
    return _set_->count == 0;
}
_Bool ms_full(struct multiset *_set_)
{
    return (double)_set_->capacity * _set_->load <= (double)_set_->count;
}
size_t ms_count(struct multiset *_set_)
{
    return _set_->count;
}
size_t ms_cardinality(struct multiset *_set_)
{
    return _set_->cardinality;
}
size_t ms_capacity(struct multiset *_set_)
{
    return _set_->capacity;
}
double ms_load(struct multiset *_set_)
{
    return _set_->load;
}
int ms_flag(struct multiset *_set_)
{
    return _set_->flag;
}
_Bool ms_resize(struct multiset *_set_, size_t capacity)
{
    _set_->flag = cmc_flags.OK;
    if (_set_->capacity == capacity)
        goto success;
    if (_set_->capacity > capacity / _set_->load)
        goto success;
    if (capacity >= 0xffffffffffffffffULL * _set_->load)
    {
        _set_->flag = cmc_flags.ERROR;
        return 0;
    }
    size_t theoretical_size = ms_impl_calculate_size(capacity);
    if (theoretical_size < _set_->count / _set_->load)
    {
        _set_->flag = cmc_flags.INVALID;
        return 0;
    }
    struct multiset *_new_set_ = ms_new_custom(
        capacity, _set_->load, _set_->f_val, _set_->alloc, ((void *)0));
    if (!_new_set_)
    {
        _set_->flag = cmc_flags.ERROR;
        return 0;
    }
    struct multiset_iter iter;
    for (ms_iter_init(&iter, _set_); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        ms_insert_many(_new_set_, ms_iter_value(&iter),
                       ms_iter_multiplicity(&iter));
    }
    if (_set_->count != _new_set_->count)
    {
        ms_free(_new_set_);
        _set_->flag = cmc_flags.ERROR;
        return 0;
    }
    struct multiset_entry *tmp_b = _set_->buffer;
    _set_->buffer = _new_set_->buffer;
    _new_set_->buffer = tmp_b;
    size_t tmp_c = _set_->capacity;
    _set_->capacity = _new_set_->capacity;
    _new_set_->capacity = tmp_c;
    ms_free(_new_set_);
success:
    if (_set_->callbacks && _set_->callbacks->resize)
        _set_->callbacks->resize();
    return 1;
}
struct multiset *ms_copy_of(struct multiset *_set_)
{
    struct multiset *result =
        ms_new_custom(_set_->capacity * _set_->load, _set_->load, _set_->f_val,
                      _set_->alloc, _set_->callbacks);
    if (!result)
    {
        _set_->flag = cmc_flags.ERROR;
        return ((void *)0);
    }
    if (_set_->f_val->cpy)
    {
        for (size_t i = 0; i < _set_->capacity; i++)
        {
            struct multiset_entry *scan = &(_set_->buffer[i]);
            if (scan->state != CMC_ES_EMPTY)
            {
                struct multiset_entry *target = &(result->buffer[i]);
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
        memcpy(result->buffer, _set_->buffer,
               sizeof(struct multiset_entry) * _set_->capacity);
    result->count = _set_->count;
    result->cardinality = _set_->cardinality;
    _set_->flag = cmc_flags.OK;
    return result;
}
_Bool ms_equals(struct multiset *_set1_, struct multiset *_set2_)
{
    _set1_->flag = cmc_flags.OK;
    _set2_->flag = cmc_flags.OK;
    if (_set1_->count != _set2_->count)
        return 0;
    if (_set1_->cardinality != _set2_->cardinality)
        return 0;
    if (_set1_->count == 0)
        return 1;
    struct multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        struct multiset_entry *entry =
            ms_impl_get_entry(_set2_, ms_iter_value(&iter));
        if (!entry)
            return 0;
        if (entry->multiplicity != ms_iter_multiplicity(&iter))
            return 0;
    }
    return 1;
}
struct cmc_string ms_to_string(struct multiset *_set_)
{
    struct cmc_string str;
    struct multiset *s_ = _set_;
    int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_multiset, "multiset",
                     "size_t", s_, s_->buffer, s_->capacity, s_->count,
                     s_->cardinality, s_->load, s_->flag, s_->f_val, s_->alloc,
                     s_->callbacks);
    return n >= 0 ? str : (struct cmc_string){ 0 };
}
_Bool ms_print(struct multiset *_set_, FILE *fptr)
{
    for (size_t i = 0; i < _set_->capacity; i++)
    {
        struct multiset_entry *entry = &(_set_->buffer[i]);
        if (entry->state == CMC_ES_FILLED)
        {
            if (!_set_->f_val->str(fptr, entry->value))
                return 0;
        }
    }
    return 1;
}
struct multiset *ms_union(struct multiset *_set1_, struct multiset *_set2_)
{
    struct multiset *_set_r_ =
        ms_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val,
                      _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct multiset_iter iter1, iter2;
    ms_iter_init(&iter1, _set1_);
    ms_iter_init(&iter2, _set2_);
    for (ms_iter_to_start(&iter1); !ms_iter_end(&iter1); ms_iter_next(&iter1))
    {
        size_t value = ms_iter_value(&iter1);
        size_t m1 = ms_iter_multiplicity(&iter1);
        size_t m2 = ms_impl_multiplicity_of(_set2_, value);
        size_t max_ = m1 > m2 ? m1 : m2;
        ms_update(_set_r_, value, max_);
    }
    for (ms_iter_to_start(&iter2); !ms_iter_end(&iter2); ms_iter_next(&iter2))
    {
        size_t value = ms_iter_value(&iter2);
        size_t m1 = ms_impl_multiplicity_of(_set1_, value);
        size_t m2 = ms_iter_multiplicity(&iter2);
        size_t max_ = m1 > m2 ? m1 : m2;
        ms_update(_set_r_, value, max_);
    }
    _set_r_->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct multiset *ms_intersection(struct multiset *_set1_,
                                 struct multiset *_set2_)
{
    struct multiset *_set_r_ =
        ms_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val,
                      _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct multiset *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;
    struct multiset *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;
    struct multiset_iter iter;
    ms_iter_init(&iter, _set_A_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        size_t m1 = ms_iter_multiplicity(&iter);
        size_t m2 = ms_impl_multiplicity_of(_set_B_, value);
        size_t max_ = m1 < m2 ? m1 : m2;
        ms_update(_set_r_, value, max_);
    }
    _set_r_->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct multiset *ms_difference(struct multiset *_set1_, struct multiset *_set2_)
{
    struct multiset *_set_r_ =
        ms_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val,
                      _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        size_t m1 = ms_iter_multiplicity(&iter);
        size_t m2 = ms_impl_multiplicity_of(_set2_, value);
        if (m1 > m2)
            ms_update(_set_r_, value, m1 - m2);
    }
    _set_r_->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct multiset *ms_summation(struct multiset *_set1_, struct multiset *_set2_)
{
    struct multiset *_set_r_ =
        ms_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val,
                      _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct multiset_iter iter1, iter2;
    ms_iter_init(&iter1, _set1_);
    ms_iter_init(&iter2, _set2_);
    for (ms_iter_to_start(&iter1); !ms_iter_end(&iter1); ms_iter_next(&iter1))
    {
        ms_insert_many(_set_r_, ms_iter_value(&iter1),
                       ms_iter_multiplicity(&iter1));
    }
    for (ms_iter_to_start(&iter2); !ms_iter_end(&iter2); ms_iter_next(&iter2))
    {
        ms_insert_many(_set_r_, ms_iter_value(&iter2),
                       ms_iter_multiplicity(&iter2));
    }
    _set_r_->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct multiset *ms_symmetric_difference(struct multiset *_set1_,
                                         struct multiset *_set2_)
{
    struct multiset *_set_r_ =
        ms_new_custom(_set1_->capacity, _set1_->load, _set1_->f_val,
                      _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct multiset_iter iter1, iter2;
    ms_iter_init(&iter1, _set1_);
    ms_iter_init(&iter2, _set2_);
    for (ms_iter_to_start(&iter1); !ms_iter_end(&iter1); ms_iter_next(&iter1))
    {
        size_t value = ms_iter_value(&iter1);
        size_t m1 = ms_iter_multiplicity(&iter1);
        size_t m2 = ms_impl_multiplicity_of(_set2_, value);
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
        size_t m1 = ms_impl_multiplicity_of(_set1_, value);
        size_t m2 = ms_iter_multiplicity(&iter2);
        if (m1 != m2)
        {
            if (m1 > m2)
                ms_update(_set_r_, value, m1 - m2);
            else
                ms_update(_set_r_, value, m2 - m1);
        }
    }
    _set_r_->callbacks = _set1_->callbacks;
    return _set_r_;
}
_Bool ms_is_subset(struct multiset *_set1_, struct multiset *_set2_)
{
    if (_set1_->count > _set2_->count)
        return 0;
    if (ms_empty(_set1_))
        return 1;
    struct multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        size_t m1 = ms_iter_multiplicity(&iter);
        size_t m2 = ms_impl_multiplicity_of(_set2_, value);
        if (m1 > m2)
            return 0;
    }
    return 1;
}
_Bool ms_is_superset(struct multiset *_set1_, struct multiset *_set2_)
{
    return ms_is_subset(_set2_, _set1_);
}
_Bool ms_is_proper_subset(struct multiset *_set1_, struct multiset *_set2_)
{
    if (_set1_->count >= _set2_->count)
        return 0;
    if (ms_empty(_set1_))
    {
        if (!ms_empty(_set2_))
            return 1;
        else
            return 0;
    }
    struct multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        size_t m1 = ms_iter_multiplicity(&iter);
        size_t m2 = ms_impl_multiplicity_of(_set2_, value);
        if (m1 >= m2)
            return 0;
    }
    return 1;
}
_Bool ms_is_proper_superset(struct multiset *_set1_, struct multiset *_set2_)
{
    return ms_is_proper_subset(_set2_, _set1_);
}
_Bool ms_is_disjointset(struct multiset *_set1_, struct multiset *_set2_)
{
    if (ms_empty(_set1_))
        return 1;
    struct multiset_iter iter;
    ms_iter_init(&iter, _set1_);
    for (ms_iter_to_start(&iter); !ms_iter_end(&iter); ms_iter_next(&iter))
    {
        size_t value = ms_iter_value(&iter);
        if (ms_impl_get_entry(_set2_, value) != ((void *)0))
            return 0;
    }
    return 1;
}
struct multiset_iter *ms_iter_new(struct multiset *target)
{
    struct multiset_iter *iter =
        target->alloc->malloc(sizeof(struct multiset_iter));
    if (!iter)
        return ((void *)0);
    ms_iter_init(iter, target);
    return iter;
}
void ms_iter_free(struct multiset_iter *iter)
{
    iter->target->alloc->free(iter);
}
void ms_iter_init(struct multiset_iter *iter, struct multiset *target)
{
    memset(iter, 0, sizeof(struct multiset_iter));
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
_Bool ms_iter_start(struct multiset_iter *iter)
{
    return ms_empty(iter->target) || iter->start;
}
_Bool ms_iter_end(struct multiset_iter *iter)
{
    return ms_empty(iter->target) || iter->end;
}
void ms_iter_to_start(struct multiset_iter *iter)
{
    if (!ms_empty(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = 1;
        iter->end = ms_empty(iter->target);
    }
}
void ms_iter_to_end(struct multiset_iter *iter)
{
    if (!ms_empty(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = ms_empty(iter->target);
        iter->end = 1;
    }
}
_Bool ms_iter_next(struct multiset_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = ms_empty(iter->target);
    struct multiset_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool ms_iter_prev(struct multiset_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = ms_empty(iter->target);
    struct multiset_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool ms_iter_advance(struct multiset_iter *iter, size_t steps)
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
        ms_iter_next(iter);
    return 1;
}
_Bool ms_iter_rewind(struct multiset_iter *iter, size_t steps)
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
_Bool ms_iter_go_to(struct multiset_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return ms_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return ms_iter_advance(iter, index - iter->index);
    return 1;
}
size_t ms_iter_value(struct multiset_iter *iter)
{
    if (ms_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor].value;
}
size_t ms_iter_multiplicity(struct multiset_iter *iter)
{
    if (ms_empty(iter->target))
        return 0;
    return iter->target->buffer[iter->cursor].multiplicity;
}
size_t ms_iter_index(struct multiset_iter *iter)
{
    return iter->index;
}
static size_t ms_impl_multiplicity_of(struct multiset *_set_, size_t element)
{
    struct multiset_entry *entry = ms_impl_get_entry(_set_, element);
    if (!entry)
        return 0;
    return entry->multiplicity;
}
static struct multiset_entry *ms_impl_insert_and_return(struct multiset *_set_,
                                                        size_t element,
                                                        _Bool *new_node)
{
    *new_node = 0;
    struct multiset_entry *entry = ms_impl_get_entry(_set_, element);
    if (entry != ((void *)0))
        return entry;
    *new_node = 1;
    if (ms_full(_set_))
    {
        if (!ms_resize(_set_, _set_->capacity + 1))
            return ((void *)0);
    }
    size_t hash = _set_->f_val->hash(element);
    size_t original_pos = hash % _set_->capacity;
    size_t pos = original_pos;
    size_t curr_mul = 1;
    struct multiset_entry *target = &(_set_->buffer[pos]);
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
            if (target->state == CMC_ES_EMPTY ||
                target->state == CMC_ES_DELETED)
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
static struct multiset_entry *ms_impl_get_entry(struct multiset *_set_,
                                                size_t element)
{
    size_t hash = _set_->f_val->hash(element);
    size_t pos = hash % _set_->capacity;
    struct multiset_entry *target = &(_set_->buffer[pos]);
    while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED)
    {
        if (_set_->f_val->cmp(target->value, element) == 0)
            return target;
        pos++;
        target = &(_set_->buffer[pos % _set_->capacity]);
    }
    return ((void *)0);
}
static size_t ms_impl_calculate_size(size_t required)
{
    const size_t count =
        sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);
    if (cmc_hashtable_primes[count - 1] < required)
        return required;
    size_t i = 0;
    while (cmc_hashtable_primes[i] < required)
        i++;
    return cmc_hashtable_primes[i];
}
static struct multiset_iter ms_impl_it_start(struct multiset *_set_)
{
    struct multiset_iter iter;
    ms_iter_init(&iter, _set_);
    return iter;
}
static struct multiset_iter ms_impl_it_end(struct multiset *_set_)
{
    struct multiset_iter iter;
    ms_iter_init(&iter, _set_);
    ms_iter_to_end(&iter);
    return iter;
}
