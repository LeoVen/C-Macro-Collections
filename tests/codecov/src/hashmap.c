#include <cmc/hashmap.h>

// CMC_GENERATE_HASHMAP(hm, hashmap, size_t, size_t)

struct hashmap
{
    struct hashmap_entry *buffer;
    size_t capacity;
    size_t count;
    double load;
    int flag;
    struct hashmap_ftab_key *f_key;
    struct hashmap_ftab_val *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
    struct hashmap_iter (*it_start)(struct hashmap *);
    struct hashmap_iter (*it_end)(struct hashmap *);
};
struct hashmap_entry
{
    size_t key;
    size_t value;
    size_t dist;
    enum cmc_entry_state state;
};
struct hashmap_ftab_key
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashmap_ftab_val
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashmap_iter
{
    struct hashmap *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
};
struct hashmap *hm_new(size_t capacity, double load,
                       struct hashmap_ftab_key *f_key,
                       struct hashmap_ftab_val *f_val);
struct hashmap *hm_new_custom(size_t capacity, double load,
                              struct hashmap_ftab_key *f_key,
                              struct hashmap_ftab_val *f_val,
                              struct cmc_alloc_node *alloc,
                              struct cmc_callbacks *callbacks);
void hm_clear(struct hashmap *_map_);
void hm_free(struct hashmap *_map_);
void hm_customize(struct hashmap *_map_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks);
_Bool hm_insert(struct hashmap *_map_, size_t key, size_t value);
_Bool hm_update(struct hashmap *_map_, size_t key, size_t new_value,
                size_t *old_value);
_Bool hm_remove(struct hashmap *_map_, size_t key, size_t *out_value);
_Bool hm_max(struct hashmap *_map_, size_t *key, size_t *value);
_Bool hm_min(struct hashmap *_map_, size_t *key, size_t *value);
size_t hm_get(struct hashmap *_map_, size_t key);
size_t *hm_get_ref(struct hashmap *_map_, size_t key);
_Bool hm_contains(struct hashmap *_map_, size_t key);
_Bool hm_empty(struct hashmap *_map_);
_Bool hm_full(struct hashmap *_map_);
size_t hm_count(struct hashmap *_map_);
size_t hm_capacity(struct hashmap *_map_);
double hm_load(struct hashmap *_map_);
int hm_flag(struct hashmap *_map_);
_Bool hm_resize(struct hashmap *_map_, size_t capacity);
struct hashmap *hm_copy_of(struct hashmap *_map_);
_Bool hm_equals(struct hashmap *_map1_, struct hashmap *_map2_);
struct cmc_string hm_to_string(struct hashmap *_map_);
_Bool hm_print(struct hashmap *_map_, FILE *fptr);
struct hashmap_iter *hm_iter_new(struct hashmap *target);
void hm_iter_free(struct hashmap_iter *iter);
void hm_iter_init(struct hashmap_iter *iter, struct hashmap *target);
_Bool hm_iter_start(struct hashmap_iter *iter);
_Bool hm_iter_end(struct hashmap_iter *iter);
void hm_iter_to_start(struct hashmap_iter *iter);
void hm_iter_to_end(struct hashmap_iter *iter);
_Bool hm_iter_next(struct hashmap_iter *iter);
_Bool hm_iter_prev(struct hashmap_iter *iter);
_Bool hm_iter_advance(struct hashmap_iter *iter, size_t steps);
_Bool hm_iter_rewind(struct hashmap_iter *iter, size_t steps);
_Bool hm_iter_go_to(struct hashmap_iter *iter, size_t index);
size_t hm_iter_key(struct hashmap_iter *iter);
size_t hm_iter_value(struct hashmap_iter *iter);
size_t *hm_iter_rvalue(struct hashmap_iter *iter);
size_t hm_iter_index(struct hashmap_iter *iter);
static struct hashmap_entry *hm_impl_get_entry(struct hashmap *_map_,
                                               size_t key);
static size_t hm_impl_calculate_size(size_t required);
static struct hashmap_iter hm_impl_it_start(struct hashmap *_map_);
static struct hashmap_iter hm_impl_it_end(struct hashmap *_map_);
struct hashmap *hm_new(size_t capacity, double load,
                       struct hashmap_ftab_key *f_key,
                       struct hashmap_ftab_val *f_val)
{
    struct cmc_alloc_node *alloc = &cmc_alloc_node_default;
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffff * load)
        return ((void *)0);
    if (!f_key || !f_val)
        return ((void *)0);
    size_t real_capacity = hm_impl_calculate_size(capacity / load);
    struct hashmap *_map_ = alloc->malloc(sizeof(struct hashmap));
    if (!_map_)
        return ((void *)0);
    _map_->buffer = alloc->calloc(real_capacity, sizeof(struct hashmap_entry));
    if (!_map_->buffer)
    {
        alloc->free(_map_);
        return ((void *)0);
    }
    _map_->count = 0;
    _map_->capacity = real_capacity;
    _map_->load = load;
    _map_->flag = cmc_flags.OK;
    _map_->f_key = f_key;
    _map_->f_val = f_val;
    _map_->alloc = alloc;
    _map_->callbacks = ((void *)0);
    _map_->it_start = hm_impl_it_start;
    _map_->it_end = hm_impl_it_end;
    return _map_;
}
struct hashmap *hm_new_custom(size_t capacity, double load,
                              struct hashmap_ftab_key *f_key,
                              struct hashmap_ftab_val *f_val,
                              struct cmc_alloc_node *alloc,
                              struct cmc_callbacks *callbacks)
{
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffff * load)
        return ((void *)0);
    if (!f_key || !f_val)
        return ((void *)0);
    size_t real_capacity = hm_impl_calculate_size(capacity / load);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct hashmap *_map_ = alloc->malloc(sizeof(struct hashmap));
    if (!_map_)
        return ((void *)0);
    _map_->buffer = alloc->calloc(real_capacity, sizeof(struct hashmap_entry));
    if (!_map_->buffer)
    {
        alloc->free(_map_);
        return ((void *)0);
    }
    _map_->count = 0;
    _map_->capacity = real_capacity;
    _map_->load = load;
    _map_->flag = cmc_flags.OK;
    _map_->f_key = f_key;
    _map_->f_val = f_val;
    _map_->alloc = alloc;
    _map_->callbacks = callbacks;
    _map_->it_start = hm_impl_it_start;
    _map_->it_end = hm_impl_it_end;
    return _map_;
}
void hm_clear(struct hashmap *_map_)
{
    if (_map_->f_key->free || _map_->f_val->free)
    {
        for (size_t i = 0; i < _map_->capacity; i++)
        {
            struct hashmap_entry *entry = &(_map_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                if (_map_->f_key->free)
                    _map_->f_key->free(entry->key);
                if (_map_->f_val->free)
                    _map_->f_val->free(entry->value);
            }
        }
    }
    memset(_map_->buffer, 0, sizeof(struct hashmap_entry) * _map_->capacity);
    _map_->count = 0;
    _map_->flag = cmc_flags.OK;
}
void hm_free(struct hashmap *_map_)
{
    if (_map_->f_key->free || _map_->f_val->free)
    {
        for (size_t i = 0; i < _map_->capacity; i++)
        {
            struct hashmap_entry *entry = &(_map_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                if (_map_->f_key->free)
                    _map_->f_key->free(entry->key);
                if (_map_->f_val->free)
                    _map_->f_val->free(entry->value);
            }
        }
    }
    _map_->alloc->free(_map_->buffer);
    _map_->alloc->free(_map_);
}
void hm_customize(struct hashmap *_map_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _map_->alloc = &cmc_alloc_node_default;
    else
        _map_->alloc = alloc;
    _map_->callbacks = callbacks;
    _map_->flag = cmc_flags.OK;
}
_Bool hm_insert(struct hashmap *_map_, size_t key, size_t value)
{
    if (hm_full(_map_))
    {
        if (!hm_resize(_map_, _map_->capacity + 1))
            return 0;
    }
    if (hm_impl_get_entry(_map_, key) != ((void *)0))
    {
        _map_->flag = cmc_flags.DUPLICATE;
        return 0;
    }
    size_t hash = _map_->f_key->hash(key);
    size_t original_pos = hash % _map_->capacity;
    size_t pos = original_pos;
    struct hashmap_entry *target = &(_map_->buffer[pos]);
    if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)
    {
        target->key = key;
        target->value = value;
        target->dist = 0;
        target->state = CMC_ES_FILLED;
    }
    else
    {
        while (1)
        {
            pos++;
            target = &(_map_->buffer[pos % _map_->capacity]);
            if (target->state == CMC_ES_EMPTY ||
                target->state == CMC_ES_DELETED)
            {
                target->key = key;
                target->value = value;
                target->dist = pos - original_pos;
                target->state = CMC_ES_FILLED;
                break;
            }
            else if (target->dist < pos - original_pos)
            {
                size_t tmp_k = target->key;
                size_t tmp_v = target->value;
                size_t tmp_dist = target->dist;
                target->key = key;
                target->value = value;
                target->dist = pos - original_pos;
                key = tmp_k;
                value = tmp_v;
                original_pos = pos - tmp_dist;
            }
        }
    }
    _map_->count++;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->create)
        _map_->callbacks->create();
    return 1;
}
_Bool hm_update(struct hashmap *_map_, size_t key, size_t new_value,
                size_t *old_value)
{
    if (hm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct hashmap_entry *entry = hm_impl_get_entry(_map_, key);
    if (!entry)
    {
        _map_->flag = cmc_flags.NOT_FOUND;
        return 0;
    }
    if (old_value)
        *old_value = entry->value;
    entry->value = new_value;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->update)
        _map_->callbacks->update();
    return 1;
}
_Bool hm_remove(struct hashmap *_map_, size_t key, size_t *out_value)
{
    if (hm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct hashmap_entry *result = hm_impl_get_entry(_map_, key);
    if (result == ((void *)0))
    {
        _map_->flag = cmc_flags.NOT_FOUND;
        return 0;
    }
    if (out_value)
        *out_value = result->value;
    result->key = (size_t){ 0 };
    result->value = (size_t){ 0 };
    result->dist = 0;
    result->state = CMC_ES_DELETED;
    _map_->count--;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->delete)
        _map_->callbacks->delete ();
    return 1;
}
_Bool hm_max(struct hashmap *_map_, size_t *key, size_t *value)
{
    if (hm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct hashmap_iter iter;
    hm_iter_init(&iter, _map_);
    size_t max_key = hm_iter_key(&iter);
    size_t max_val = hm_iter_value(&iter);
    hm_iter_next(&iter);
    for (; !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        size_t iter_key = hm_iter_key(&iter);
        size_t iter_val = hm_iter_value(&iter);
        if (_map_->f_key->cmp(iter_key, max_key) > 0)
        {
            max_key = iter_key;
            max_val = iter_val;
        }
    }
    if (key)
        *key = max_key;
    if (value)
        *value = max_val;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return 1;
}
_Bool hm_min(struct hashmap *_map_, size_t *key, size_t *value)
{
    if (hm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct hashmap_iter iter;
    hm_iter_init(&iter, _map_);
    size_t min_key = hm_iter_key(&iter);
    size_t min_val = hm_iter_value(&iter);
    hm_iter_next(&iter);
    for (; !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        size_t iter_key = hm_iter_key(&iter);
        size_t iter_val = hm_iter_value(&iter);
        if (_map_->f_key->cmp(iter_key, min_key) < 0)
        {
            min_key = iter_key;
            min_val = iter_val;
        }
    }
    if (key)
        *key = min_key;
    if (value)
        *value = min_val;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return 1;
}
size_t hm_get(struct hashmap *_map_, size_t key)
{
    if (hm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct hashmap_entry *entry = hm_impl_get_entry(_map_, key);
    if (!entry)
    {
        _map_->flag = cmc_flags.NOT_FOUND;
        return (size_t){ 0 };
    }
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return entry->value;
}
size_t *hm_get_ref(struct hashmap *_map_, size_t key)
{
    if (hm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct hashmap_entry *entry = hm_impl_get_entry(_map_, key);
    if (!entry)
    {
        _map_->flag = cmc_flags.NOT_FOUND;
        return ((void *)0);
    }
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return &(entry->value);
}
_Bool hm_contains(struct hashmap *_map_, size_t key)
{
    _map_->flag = cmc_flags.OK;
    _Bool result = hm_impl_get_entry(_map_, key) != ((void *)0);
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return result;
}
_Bool hm_empty(struct hashmap *_map_)
{
    return _map_->count == 0;
}
_Bool hm_full(struct hashmap *_map_)
{
    return (double)_map_->capacity * _map_->load <= (double)_map_->count;
}
size_t hm_count(struct hashmap *_map_)
{
    return _map_->count;
}
size_t hm_capacity(struct hashmap *_map_)
{
    return _map_->capacity;
}
double hm_load(struct hashmap *_map_)
{
    return _map_->load;
}
int hm_flag(struct hashmap *_map_)
{
    return _map_->flag;
}
_Bool hm_resize(struct hashmap *_map_, size_t capacity)
{
    _map_->flag = cmc_flags.OK;
    if (_map_->capacity == capacity)
        goto success;
    if (_map_->capacity > capacity / _map_->load)
        goto success;
    if (capacity >= 0xffffffffffffffff * _map_->load)
    {
        _map_->flag = cmc_flags.ERROR;
        return 0;
    }
    size_t theoretical_size = hm_impl_calculate_size(capacity);
    if (theoretical_size < _map_->count / _map_->load)
    {
        _map_->flag = cmc_flags.INVALID;
        return 0;
    }
    struct hashmap *_new_map_ =
        hm_new_custom(capacity, _map_->load, _map_->f_key, _map_->f_val,
                      _map_->alloc, ((void *)0));
    if (!_new_map_)
    {
        _map_->flag = cmc_flags.ALLOC;
        return 0;
    }
    struct hashmap_iter iter;
    for (hm_iter_init(&iter, _map_); !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        size_t key = hm_iter_key(&iter);
        size_t value = hm_iter_value(&iter);
        hm_insert(_new_map_, key, value);
    }
    if (_map_->count != _new_map_->count)
    {
        hm_free(_new_map_);
        _map_->flag = cmc_flags.ERROR;
        return 0;
    }
    struct hashmap_entry *tmp_b = _map_->buffer;
    _map_->buffer = _new_map_->buffer;
    _new_map_->buffer = tmp_b;
    size_t tmp_c = _map_->capacity;
    _map_->capacity = _new_map_->capacity;
    _new_map_->capacity = tmp_c;
    hm_free(_new_map_);
success:
    if (_map_->callbacks && _map_->callbacks->resize)
        _map_->callbacks->resize();
    return 1;
}
struct hashmap *hm_copy_of(struct hashmap *_map_)
{
    struct hashmap *result =
        hm_new_custom(_map_->capacity * _map_->load, _map_->load, _map_->f_key,
                      _map_->f_val, _map_->alloc, _map_->callbacks);
    if (!result)
    {
        _map_->flag = cmc_flags.ERROR;
        return ((void *)0);
    }
    if (_map_->f_key->cpy || _map_->f_val->cpy)
    {
        for (size_t i = 0; i < _map_->capacity; i++)
        {
            struct hashmap_entry *scan = &(_map_->buffer[i]);
            if (scan->state != CMC_ES_EMPTY)
            {
                struct hashmap_entry *target = &(result->buffer[i]);
                if (scan->state == CMC_ES_DELETED)
                    target->state = CMC_ES_DELETED;
                else
                {
                    target->state = scan->state;
                    target->dist = scan->dist;
                    if (_map_->f_key->cpy)
                        target->key = _map_->f_key->cpy(scan->key);
                    else
                        target->key = scan->key;
                    if (_map_->f_val->cpy)
                        target->value = _map_->f_val->cpy(scan->value);
                    else
                        target->value = scan->value;
                }
            }
        }
    }
    else
        memcpy(result->buffer, _map_->buffer,
               sizeof(struct hashmap_entry) * _map_->capacity);
    result->count = _map_->count;
    _map_->flag = cmc_flags.OK;
    return result;
}
_Bool hm_equals(struct hashmap *_map1_, struct hashmap *_map2_)
{
    _map1_->flag = cmc_flags.OK;
    _map2_->flag = cmc_flags.OK;
    if (_map1_->count != _map2_->count)
        return 0;
    struct hashmap_iter iter;
    hm_iter_init(&iter, _map1_);
    for (hm_iter_to_start(&iter); !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        struct hashmap_entry *entry =
            hm_impl_get_entry(_map2_, hm_iter_key(&iter));
        if (entry == ((void *)0))
            return 0;
        if (_map1_->f_val->cmp(entry->value, hm_iter_value(&iter)) != 0)
            return 0;
    }
    return 1;
}
struct cmc_string hm_to_string(struct hashmap *_map_)
{
    struct cmc_string str;
    struct hashmap *m_ = _map_;
    int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_hashmap, "hashmap",
                     "size_t", "size_t", m_, m_->buffer, m_->capacity,
                     m_->count, m_->load, m_->flag, m_->f_key, m_->f_val,
                     m_->alloc, m_->callbacks);
    return n >= 0 ? str : (struct cmc_string){ 0 };
}
_Bool hm_print(struct hashmap *_map_, FILE *fptr)
{
    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct hashmap_entry *entry = &(_map_->buffer[i]);
        if (entry->state == CMC_ES_FILLED)
        {
            if (!_map_->f_key->str(fptr, entry->key) ||
                !_map_->f_val->str(fptr, entry->value))
                return 0;
        }
    }
    return 1;
}
struct hashmap_iter *hm_iter_new(struct hashmap *target)
{
    struct hashmap_iter *iter =
        target->alloc->malloc(sizeof(struct hashmap_iter));
    if (!iter)
        return ((void *)0);
    hm_iter_init(iter, target);
    return iter;
}
void hm_iter_free(struct hashmap_iter *iter)
{
    iter->target->alloc->free(iter);
}
void hm_iter_init(struct hashmap_iter *iter, struct hashmap *target)
{
    memset(iter, 0, sizeof(struct hashmap_iter));
    iter->target = target;
    iter->start = 1;
    iter->end = hm_empty(target);
    if (!hm_empty(target))
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
_Bool hm_iter_start(struct hashmap_iter *iter)
{
    return hm_empty(iter->target) || iter->start;
}
_Bool hm_iter_end(struct hashmap_iter *iter)
{
    return hm_empty(iter->target) || iter->end;
}
void hm_iter_to_start(struct hashmap_iter *iter)
{
    if (!hm_empty(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = 1;
        iter->end = hm_empty(iter->target);
    }
}
void hm_iter_to_end(struct hashmap_iter *iter)
{
    if (!hm_empty(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = hm_empty(iter->target);
        iter->end = 1;
    }
}
_Bool hm_iter_next(struct hashmap_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = hm_empty(iter->target);
    struct hashmap_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool hm_iter_prev(struct hashmap_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = hm_empty(iter->target);
    struct hashmap_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool hm_iter_advance(struct hashmap_iter *iter, size_t steps)
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
        hm_iter_next(iter);
    return 1;
}
_Bool hm_iter_rewind(struct hashmap_iter *iter, size_t steps)
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
        hm_iter_prev(iter);
    return 1;
}
_Bool hm_iter_go_to(struct hashmap_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return hm_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return hm_iter_advance(iter, index - iter->index);
    return 1;
}
size_t hm_iter_key(struct hashmap_iter *iter)
{
    if (hm_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor].key;
}
size_t hm_iter_value(struct hashmap_iter *iter)
{
    if (hm_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor].value;
}
size_t *hm_iter_rvalue(struct hashmap_iter *iter)
{
    if (hm_empty(iter->target))
        return ((void *)0);
    return &(iter->target->buffer[iter->cursor].value);
}
size_t hm_iter_index(struct hashmap_iter *iter)
{
    return iter->index;
}
static struct hashmap_entry *hm_impl_get_entry(struct hashmap *_map_,
                                               size_t key)
{
    size_t hash = _map_->f_key->hash(key);
    size_t pos = hash % _map_->capacity;
    struct hashmap_entry *target = &(_map_->buffer[pos]);
    while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED)
    {
        if (_map_->f_key->cmp(target->key, key) == 0)
            return target;
        pos++;
        target = &(_map_->buffer[pos % _map_->capacity]);
    }
    return ((void *)0);
}
static size_t hm_impl_calculate_size(size_t required)
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
static struct hashmap_iter hm_impl_it_start(struct hashmap *_map_)
{
    struct hashmap_iter iter;
    hm_iter_init(&iter, _map_);
    return iter;
}
static struct hashmap_iter hm_impl_it_end(struct hashmap *_map_)
{
    struct hashmap_iter iter;
    hm_iter_init(&iter, _map_);
    hm_iter_to_end(&iter);
    return iter;
}
