#include <cmc/hashmap.h>

//CMC_GENERATE_HASHMAP(hm, hashmap, size_t, size_t)

typedef struct hashmap_s
{
    struct hashmap_entry_s *buffer;
    size_t capacity;
    size_t count;
    double load;
    int (*cmp)(size_t, size_t);
    size_t (*hash)(size_t);
    struct hashmap_iter_s (*it_start)(struct hashmap_s *);
    struct hashmap_iter_s (*it_end)(struct hashmap_s *);
} hashmap, *hashmap_ptr;
typedef struct hashmap_entry_s
{
    size_t key;
    size_t value;
    size_t dist;
    enum cmc_entry_state state;
} hashmap_entry, *hashmap_entry_ptr;
typedef struct hashmap_iter_s
{
    struct hashmap_s *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
} hashmap_iter, *hashmap_iter_ptr;
hashmap *hm_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t));
void hm_clear(hashmap *_map_, void (*deallocator)(size_t, size_t));
void hm_free(hashmap *_map_, void (*deallocator)(size_t, size_t));
_Bool hm_insert(hashmap *_map_, size_t key, size_t value);
_Bool hm_update(hashmap *_map_, size_t key, size_t new_value, size_t *old_value);
_Bool hm_remove(hashmap *_map_, size_t key, size_t *out_value);
_Bool hm_max(hashmap *_map_, size_t *key, size_t *value);
_Bool hm_min(hashmap *_map_, size_t *key, size_t *value);
size_t hm_get(hashmap *_map_, size_t key);
size_t *hm_get_ref(hashmap *_map_, size_t key);
_Bool hm_contains(hashmap *_map_, size_t key);
_Bool hm_empty(hashmap *_map_);
_Bool hm_full(hashmap *_map_);
size_t hm_count(hashmap *_map_);
size_t hm_capacity(hashmap *_map_);
double hm_load(hashmap *_map_);
_Bool hm_resize(hashmap *_map_, size_t capacity);
hashmap *hm_copy_of(hashmap *_map_, size_t (*key_copy_func)(size_t), size_t (*value_copy_func)(size_t));
_Bool hm_equals(hashmap *_map1_, hashmap *_map2_, int (*value_comparator)(size_t, size_t));
struct cmc_string hm_to_string(hashmap *_map_);
hashmap_iter *hm_iter_new(hashmap *target);
void hm_iter_free(hashmap_iter *iter);
void hm_iter_init(hashmap_iter *iter, hashmap *target);
_Bool hm_iter_start(hashmap_iter *iter);
_Bool hm_iter_end(hashmap_iter *iter);
void hm_iter_to_start(hashmap_iter *iter);
void hm_iter_to_end(hashmap_iter *iter);
_Bool hm_iter_next(hashmap_iter *iter);
_Bool hm_iter_prev(hashmap_iter *iter);
_Bool hm_iter_advance(hashmap_iter *iter, size_t steps);
_Bool hm_iter_rewind(hashmap_iter *iter, size_t steps);
_Bool hm_iter_go_to(hashmap_iter *iter, size_t index);
size_t hm_iter_key(hashmap_iter *iter);
size_t hm_iter_value(hashmap_iter *iter);
size_t *hm_iter_rvalue(hashmap_iter *iter);
size_t hm_iter_index(hashmap_iter *iter);
static hashmap_entry *hm_impl_get_entry(hashmap *_map_, size_t key);
static size_t hm_impl_calculate_size(size_t required);
static hashmap_iter hm_impl_it_start(hashmap *_map_);
static hashmap_iter hm_impl_it_end(hashmap *_map_);
hashmap *hm_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t))
{
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffff * load)
        return ((void *)0);
    size_t real_capacity = hm_impl_calculate_size(capacity / load);
    hashmap *_map_ = malloc(sizeof(hashmap));
    if (!_map_)
        return ((void *)0);
    _map_->buffer = malloc(sizeof(hashmap_entry) * real_capacity);
    if (!_map_->buffer)
    {
        free(_map_);
        return ((void *)0);
    }
    memset(_map_->buffer, 0, sizeof(hashmap_entry) * real_capacity);
    _map_->count = 0;
    _map_->capacity = real_capacity;
    _map_->load = load;
    _map_->cmp = compare;
    _map_->hash = hash;
    _map_->it_start = hm_impl_it_start;
    _map_->it_end = hm_impl_it_end;
    return _map_;
}
void hm_clear(hashmap *_map_, void (*deallocator)(size_t, size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _map_->capacity; i++)
        {
            hashmap_entry *entry = &(_map_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                deallocator(entry->key, entry->value);
            }
        }
    }
    memset(_map_->buffer, 0, sizeof(hashmap_entry) * _map_->capacity);
    _map_->count = 0;
}
void hm_free(hashmap *_map_, void (*deallocator)(size_t, size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _map_->capacity; i++)
        {
            hashmap_entry *entry = &(_map_->buffer[i]);
            if (entry->state == CMC_ES_FILLED)
            {
                deallocator(entry->key, entry->value);
            }
        }
    }
    free(_map_->buffer);
    free(_map_);
}
_Bool hm_insert(hashmap *_map_, size_t key, size_t value)
{
    if (hm_full(_map_))
    {
        if (!hm_resize(_map_, hm_capacity(_map_) + 1))
            return 0;
    }
    size_t hash = _map_->hash(key);
    size_t original_pos = hash % _map_->capacity;
    size_t pos = original_pos;
    hashmap_entry *target = &(_map_->buffer[pos]);
    if (hm_impl_get_entry(_map_, key) != ((void *)0))
        return 0;
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
            if (target->state == CMC_ES_EMPTY || target->state == CMC_ES_DELETED)
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
    return 1;
}
_Bool hm_update(hashmap *_map_, size_t key, size_t new_value, size_t *old_value)
{
    hashmap_entry *entry = hm_impl_get_entry(_map_, key);
    if (!entry)
        return 0;
    if (old_value)
        *old_value = entry->value;
    entry->value = new_value;
    return 1;
}
_Bool hm_remove(hashmap *_map_, size_t key, size_t *out_value)
{
    hashmap_entry *result = hm_impl_get_entry(_map_, key);
    if (result == ((void *)0))
        return 0;
    if (out_value)
        *out_value = result->value;
    result->key = (size_t){0};
    result->value = (size_t){0};
    result->dist = 0;
    result->state = CMC_ES_DELETED;
    _map_->count--;
    return 1;
}
_Bool hm_max(hashmap *_map_, size_t *key, size_t *value)
{
    if (hm_empty(_map_))
        return 0;
    hashmap_iter iter;
    hm_iter_init(&iter, _map_);
    size_t max_key = hm_iter_key(&iter);
    size_t max_val = hm_iter_value(&iter);
    hm_iter_next(&iter);
    for (; !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        size_t iter_key = hm_iter_key(&iter);
        size_t iter_val = hm_iter_value(&iter);
        if (_map_->cmp(iter_key, max_key) > 0)
        {
            max_key = iter_key;
            max_val = iter_val;
        }
    }
    if (key)
        *key = max_key;
    if (value)
        *value = max_val;
    return 1;
}
_Bool hm_min(hashmap *_map_, size_t *key, size_t *value)
{
    if (hm_empty(_map_))
        return 0;
    hashmap_iter iter;
    hm_iter_init(&iter, _map_);
    size_t min_key = hm_iter_key(&iter);
    size_t min_val = hm_iter_value(&iter);
    hm_iter_next(&iter);
    for (; !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        size_t iter_key = hm_iter_key(&iter);
        size_t iter_val = hm_iter_value(&iter);
        if (_map_->cmp(iter_key, min_key) < 0)
        {
            min_key = iter_key;
            min_val = iter_val;
        }
    }
    if (key)
        *key = min_key;
    if (value)
        *value = min_val;
    return 1;
}
size_t hm_get(hashmap *_map_, size_t key)
{
    hashmap_entry *entry = hm_impl_get_entry(_map_, key);
    if (!entry)
        return (size_t){0};
    return entry->value;
}
size_t *hm_get_ref(hashmap *_map_, size_t key)
{
    hashmap_entry *entry = hm_impl_get_entry(_map_, key);
    if (!entry)
        return ((void *)0);
    return &(entry->value);
}
_Bool hm_contains(hashmap *_map_, size_t key) { return hm_impl_get_entry(_map_, key) != ((void *)0); }
_Bool hm_empty(hashmap *_map_) { return _map_->count == 0; }
_Bool hm_full(hashmap *_map_) { return (double)hm_capacity(_map_) * hm_load(_map_) <= (double)hm_count(_map_); }
size_t hm_count(hashmap *_map_) { return _map_->count; }
size_t hm_capacity(hashmap *_map_) { return _map_->capacity; }
double hm_load(hashmap *_map_) { return _map_->load; }
_Bool hm_resize(hashmap *_map_, size_t capacity)
{
    if (hm_capacity(_map_) == capacity)
        return 1;
    if (hm_capacity(_map_) > capacity / hm_load(_map_))
        return 1;
    if (capacity >= 0xffffffffffffffff * hm_load(_map_))
        return 0;
    size_t theoretical_size = hm_impl_calculate_size(capacity);
    if (theoretical_size < hm_count(_map_) / hm_load(_map_))
        return 0;
    hashmap *_new_map_ = hm_new(capacity, hm_load(_map_), _map_->cmp, _map_->hash);
    if (!_new_map_)
        return 0;
    hashmap_iter iter;
    for (hm_iter_init(&iter, _map_); !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        size_t key = hm_iter_key(&iter);
        size_t value = hm_iter_value(&iter);
        hm_insert(_new_map_, key, value);
    }
    if (hm_count(_map_) != hm_count(_new_map_))
    {
        hm_free(_new_map_, ((void *)0));
        return 0;
    }
    hashmap_entry *tmp_b = _map_->buffer;
    _map_->buffer = _new_map_->buffer;
    _new_map_->buffer = tmp_b;
    size_t tmp_c = _map_->capacity;
    _map_->capacity = _new_map_->capacity;
    _new_map_->capacity = tmp_c;
    hm_free(_new_map_, ((void *)0));
    return 1;
}
hashmap *hm_copy_of(hashmap *_map_, size_t (*key_copy_func)(size_t), size_t (*value_copy_func)(size_t))
{
    hashmap *result = hm_new(_map_->capacity, _map_->load, _map_->cmp, _map_->hash);
    if (!result)
        return ((void *)0);
    if (key_copy_func || value_copy_func)
    {
        for (size_t i = 0; i < _map_->capacity; i++)
        {
            hashmap_entry *scan = &(_map_->buffer[i]);
            if (scan->state != CMC_ES_EMPTY)
            {
                hashmap_entry *target = &(result->buffer[i]);
                if (scan->state == CMC_ES_DELETED)
                    target->state = CMC_ES_DELETED;
                else
                {
                    target->state = scan->state;
                    target->dist = scan->dist;
                    if (key_copy_func)
                        target->key = key_copy_func(scan->key);
                    else
                        target->key = scan->key;
                    if (value_copy_func)
                        target->value = value_copy_func(scan->value);
                    else
                        target->value = scan->value;
                }
            }
        }
    }
    else
        memcpy(result->buffer, _map_->buffer, sizeof(hashmap_entry) * _map_->capacity);
    result->count = _map_->count;
    return result;
}
_Bool hm_equals(hashmap *_map1_, hashmap *_map2_, int (*value_comparator)(size_t, size_t))
{
    if (hm_count(_map1_) != hm_count(_map2_))
        return 0;
    hashmap_iter iter;
    hm_iter_init(&iter, _map1_);
    for (hm_iter_to_start(&iter); !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        hashmap_entry *entry = hm_impl_get_entry(_map2_, hm_iter_key(&iter));
        if (entry == ((void *)0))
            return 0;
        if (value_comparator)
        {
            if (value_comparator(entry->value, hm_iter_value(&iter)) != 0)
                return 0;
        }
    }
    return 1;
}
struct cmc_string hm_to_string(hashmap *_map_)
{
    struct cmc_string str;
    hashmap *m_ = _map_;
    const char *name = "hashmap";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_hashmap, name, m_, m_->buffer, m_->capacity, m_->count, m_->load, m_->cmp, m_->hash);
    return str;
}
hashmap_iter *hm_iter_new(hashmap *target)
{
    hashmap_iter *iter = malloc(sizeof(hashmap_iter));
    if (!iter)
        return ((void *)0);
    hm_iter_init(iter, target);
    return iter;
}
void hm_iter_free(hashmap_iter *iter) { free(iter); }
void hm_iter_init(hashmap_iter *iter, hashmap *target)
{
    memset(iter, 0, sizeof(hashmap_iter));
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
_Bool hm_iter_start(hashmap_iter *iter) { return hm_empty(iter->target) || iter->start; }
_Bool hm_iter_end(hashmap_iter *iter) { return hm_empty(iter->target) || iter->end; }
void hm_iter_to_start(hashmap_iter *iter)
{
    if (!hm_empty(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = 1;
        iter->end = hm_empty(iter->target);
    }
}
void hm_iter_to_end(hashmap_iter *iter)
{
    if (!hm_empty(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = hm_count(iter->target) - 1;
        iter->start = hm_empty(iter->target);
        iter->end = 1;
    }
}
_Bool hm_iter_next(hashmap_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == hm_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    iter->start = hm_empty(iter->target);
    hashmap_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool hm_iter_prev(hashmap_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = hm_empty(iter->target);
    hashmap_entry *scan = &(iter->target->buffer[iter->cursor]);
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
_Bool hm_iter_advance(hashmap_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == hm_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= hm_count(iter->target))
        return 0;
    for (size_t i = 0; i < steps; i++)
        hm_iter_next(iter);
    return 1;
}
_Bool hm_iter_rewind(hashmap_iter *iter, size_t steps)
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
_Bool hm_iter_go_to(hashmap_iter *iter, size_t index)
{
    if (index >= hm_count(iter->target))
        return 0;
    if (iter->index > index)
        return hm_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return hm_iter_advance(iter, index - iter->index);
    return 1;
}
size_t hm_iter_key(hashmap_iter *iter)
{
    if (hm_empty(iter->target))
        return (size_t){0};
    return iter->target->buffer[iter->cursor].key;
}
size_t hm_iter_value(hashmap_iter *iter)
{
    if (hm_empty(iter->target))
        return (size_t){0};
    return iter->target->buffer[iter->cursor].value;
}
size_t *hm_iter_rvalue(hashmap_iter *iter)
{
    if (hm_empty(iter->target))
        return ((void *)0);
    return &(iter->target->buffer[iter->cursor].value);
}
size_t hm_iter_index(hashmap_iter *iter) { return iter->index; }
static hashmap_entry *hm_impl_get_entry(hashmap *_map_, size_t key)
{
    size_t hash = _map_->hash(key);
    size_t pos = hash % _map_->capacity;
    hashmap_entry *target = &(_map_->buffer[pos]);
    while (target->state == CMC_ES_FILLED || target->state == CMC_ES_DELETED)
    {
        if (_map_->cmp(target->key, key) == 0)
            return target;
        pos++;
        target = &(_map_->buffer[pos % _map_->capacity]);
    }
    return ((void *)0);
}
static size_t hm_impl_calculate_size(size_t required)
{
    const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);
    if (cmc_hashtable_primes[count - 1] < required)
        return required;
    size_t i = 0;
    while (cmc_hashtable_primes[i] < required)
        i++;
    return cmc_hashtable_primes[i];
}
static hashmap_iter hm_impl_it_start(hashmap *_map_)
{
    hashmap_iter iter;
    hm_iter_init(&iter, _map_);
    return iter;
}
static hashmap_iter hm_impl_it_end(hashmap *_map_)
{
    hashmap_iter iter;
    hm_iter_init(&iter, _map_);
    hm_iter_to_end(&iter);
    return iter;
}
