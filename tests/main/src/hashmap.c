#include "cmc/hashmap.h"

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
    enum EntryState_e state;
} hashmap_entry, *hashmap_entry_ptr;
typedef struct hashmap_iter_s
{
    struct hashmap_s *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    bool start;
    bool end;
} hashmap_iter, *hashmap_iter_ptr;
hashmap *hm_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t));
void hm_clear(hashmap *_map_);
void hm_free(hashmap *_map_);
bool hm_insert(hashmap *_map_, size_t key, size_t value);
bool hm_remove(hashmap *_map_, size_t key, size_t *value);
bool hm_insert_if(hashmap *_map_, size_t key, size_t value, bool condition);
bool hm_remove_if(hashmap *_map_, size_t key, size_t *value, bool condition);
bool hm_max(hashmap *_map_, size_t *key, size_t *value);
bool hm_min(hashmap *_map_, size_t *key, size_t *value);
size_t hm_get(hashmap *_map_, size_t key);
size_t *hm_get_ref(hashmap *_map_, size_t key);
bool hm_contains(hashmap *_map_, size_t key);
bool hm_empty(hashmap *_map_);
size_t hm_count(hashmap *_map_);
size_t hm_capacity(hashmap *_map_);
hashmap_iter *hm_iter_new(hashmap *target);
void hm_iter_free(hashmap_iter *iter);
void hm_iter_init(hashmap_iter *iter, hashmap *target);
bool hm_iter_start(hashmap_iter *iter);
bool hm_iter_end(hashmap_iter *iter);
void hm_iter_to_start(hashmap_iter *iter);
void hm_iter_to_end(hashmap_iter *iter);
bool hm_iter_next(hashmap_iter *iter);
bool hm_iter_prev(hashmap_iter *iter);
size_t hm_iter_key(hashmap_iter *iter);
size_t hm_iter_value(hashmap_iter *iter);
size_t *hm_iter_rvalue(hashmap_iter *iter);
size_t hm_iter_index(hashmap_iter *iter);
static inline size_t hm_impl_default_key(void)
{
    size_t _empty_key_;
    memset(&_empty_key_, 0, sizeof(size_t));
    return _empty_key_;
}
static inline size_t hm_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
static bool hm_impl_grow(hashmap *_map_);
static hashmap_entry *hm_impl_get_entry(hashmap *_map_, size_t key);
static size_t hm_impl_calculate_size(size_t required);
static hashmap_iter hm_impl_it_start(hashmap *_map_);
static hashmap_iter hm_impl_it_end(hashmap *_map_);
hashmap *hm_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t))
{
    if (capacity == 0 || load <= 0 || load >= 1)
        return NULL;
    size_t real_capacity = hm_impl_calculate_size(capacity / load);
    hashmap *_map_ = malloc(sizeof(hashmap));
    if (!_map_)
        return NULL;
    _map_->buffer = malloc(sizeof(hashmap_entry) * real_capacity);
    if (!_map_->buffer)
    {
        free(_map_);
        return NULL;
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
void hm_clear(hashmap *_map_)
{
    memset(_map_->buffer, 0, sizeof(hashmap_entry) * _map_->capacity);
    _map_->count = 0;
}
void hm_free(hashmap *_map_)
{
    free(_map_->buffer);
    free(_map_);
}
bool hm_insert(hashmap *_map_, size_t key, size_t value)
{
    if ((double)_map_->capacity * _map_->load <= (double)_map_->count)
    {
        if (!hm_impl_grow(_map_))
            return false;
    }
    size_t hash = _map_->hash(key);
    size_t original_pos = hash % _map_->capacity;
    size_t pos = original_pos;
    hashmap_entry *target = &(_map_->buffer[pos]);
    if (hm_impl_get_entry(_map_, key) != NULL)
        return false;
    if (target->state == ES_EMPTY || target->state == ES_DELETED)
    {
        target->key = key;
        target->value = value;
        target->dist = original_pos - pos;
        target->state = ES_FILLED;
    }
    else
    {
        while (true)
        {
            pos++;
            target = &(_map_->buffer[pos % _map_->capacity]);
            if (target->state == ES_EMPTY || target->state == ES_DELETED)
            {
                target->key = key;
                target->value = value;
                target->dist = pos - original_pos;
                target->state = ES_FILLED;
                break;
            }
            else if (target->dist < original_pos - pos)
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
    return true;
}
bool hm_remove(hashmap *_map_, size_t key, size_t *value)
{
    hashmap_entry *result = hm_impl_get_entry(_map_, key);
    if (result == NULL)
        return false;
    *value = result->value;
    result->key = hm_impl_default_key();
    result->value = hm_impl_default_value();
    result->dist = 0;
    result->state = ES_DELETED;
    _map_->count--;
    return true;
}
bool hm_insert_if(hashmap *_map_, size_t key, size_t value, bool condition)
{
    if (condition)
        return hm_insert(_map_, key, value);
    return false;
}
bool hm_remove_if(hashmap *_map_, size_t key, size_t *value, bool condition)
{
    if (condition)
        return hm_remove(_map_, key, value);
    return false;
}
bool hm_max(hashmap *_map_, size_t *key, size_t *value)
{
    if (hm_empty(_map_))
        return false;
    hashmap_iter iter;
    for (hm_iter_init(&iter, _map_); !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        size_t result_key = hm_iter_key(&iter);
        size_t result_value = hm_iter_value(&iter);
        size_t index = hm_iter_index(&iter);
        if (index == 0)
        {
            *key = result_key;
            *value = result_value;
        }
        else if (_map_->cmp(result_key, *key) > 0)
        {
            *key = result_key;
            *value = result_value;
        }
    }
    return true;
}
bool hm_min(hashmap *_map_, size_t *key, size_t *value)
{
    if (hm_empty(_map_))
        return false;
    hashmap_iter iter;
    for (hm_iter_init(&iter, _map_); !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        size_t result_key = hm_iter_key(&iter);
        size_t result_value = hm_iter_value(&iter);
        size_t index = hm_iter_index(&iter);
        if (index == 0)
        {
            *key = result_key;
            *value = result_value;
        }
        else if (_map_->cmp(result_key, *key) < 0)
        {
            *key = result_key;
            *value = result_value;
        }
    }
    return true;
}
size_t hm_get(hashmap *_map_, size_t key)
{
    hashmap_entry *entry = hm_impl_get_entry(_map_, key);
    if (!entry)
        return hm_impl_default_value();
    return entry->value;
}
size_t *hm_get_ref(hashmap *_map_, size_t key)
{
    hashmap_entry *entry = hm_impl_get_entry(_map_, key);
    if (!entry)
        return NULL;
    return &(entry->value);
}
bool hm_contains(hashmap *_map_, size_t key) { return hm_impl_get_entry(_map_, key) != NULL; }
bool hm_empty(hashmap *_map_) { return _map_->count == 0; }
size_t hm_count(hashmap *_map_) { return _map_->count; }
size_t hm_capacity(hashmap *_map_) { return _map_->capacity; }
hashmap_iter *hm_iter_new(hashmap *target)
{
    hashmap_iter *iter = malloc(sizeof(hashmap_iter));
    if (!iter)
        return NULL;
    hm_iter_init(iter, target);
    return iter;
}
void hm_iter_free(hashmap_iter *iter) { free(iter); }
void hm_iter_init(hashmap_iter *iter, hashmap *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->index = 0;
    iter->start = true;
    iter->end = hm_empty(target);
    if (!hm_empty(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            if (target->buffer[i].state == ES_FILLED)
            {
                iter->first = i;
                break;
            }
        }
        iter->cursor = iter->first;
        for (size_t i = target->capacity; i > 0; i--)
        {
            if (target->buffer[i - 1].state == ES_FILLED)
            {
                iter->last = i - 1;
                break;
            }
        }
    }
}
bool hm_iter_start(hashmap_iter *iter) { return hm_empty(iter->target) || iter->start; }
bool hm_iter_end(hashmap_iter *iter) { return hm_empty(iter->target) || iter->end; }
void hm_iter_to_start(hashmap_iter *iter)
{
    iter->cursor = iter->first;
    iter->index = 0;
    iter->start = true;
    iter->end = hm_empty(iter->target);
}
void hm_iter_to_end(hashmap_iter *iter)
{
    iter->cursor = iter->last;
    iter->index = iter->target->count - 1;
    iter->start = hm_empty(iter->target);
    iter->end = true;
}
bool hm_iter_next(hashmap_iter *iter)
{
    if (iter->end)
        return false;
    hashmap_entry *scan = &(iter->target->buffer[iter->cursor]);
    if (iter->cursor == iter->last)
    {
        iter->end = true;
        return false;
    }
    iter->index++;
    while (1)
    {
        iter->cursor++;
        scan = &(iter->target->buffer[iter->cursor]);
        if (scan->state == ES_FILLED)
            break;
    }
    iter->start = hm_empty(iter->target);
    return true;
}
bool hm_iter_prev(hashmap_iter *iter)
{
    if (iter->start)
        return false;
    hashmap_entry *scan = &(iter->target->buffer[iter->cursor]);
    if (iter->cursor == iter->first)
    {
        iter->start = true;
        return false;
    }
    iter->index--;
    while (1)
    {
        iter->cursor--;
        scan = &(iter->target->buffer[iter->cursor]);
        if (scan->state == ES_FILLED)
            break;
    }
    iter->end = hm_empty(iter->target);
    return true;
}
size_t hm_iter_key(hashmap_iter *iter)
{
    if (hm_empty(iter->target))
        return hm_impl_default_key();
    return iter->target->buffer[iter->cursor].key;
}
size_t hm_iter_value(hashmap_iter *iter)
{
    if (hm_empty(iter->target))
        return hm_impl_default_value();
    return iter->target->buffer[iter->cursor].value;
}
size_t *hm_iter_rvalue(hashmap_iter *iter)
{
    if (hm_empty(iter->target))
        return NULL;
    return &(iter->target->buffer[iter->cursor].value);
}
size_t hm_iter_index(hashmap_iter *iter) { return iter->index; }
static bool hm_impl_grow(hashmap *_map_)
{
    size_t new_size = hm_impl_calculate_size(_map_->capacity + _map_->capacity / 2);
    hashmap *_new_map_ = hm_new(new_size, _map_->load, _map_->cmp, _map_->hash);
    if (!_new_map_)
        return false;
    hashmap_iter iter;
    for (hm_iter_init(&iter, _map_); !hm_iter_end(&iter); hm_iter_next(&iter))
    {
        size_t key = hm_iter_key(&iter);
        size_t value = hm_iter_value(&iter);
        hm_insert(_new_map_, key, value);
    }
    if (_map_->count != _new_map_->count)
    {
        hm_free(_new_map_);
        return false;
    }
    hashmap_entry *tmp = _map_->buffer;
    _map_->buffer = _new_map_->buffer;
    _new_map_->buffer = tmp;
    _map_->capacity = _new_map_->capacity;
    hm_free(_new_map_);
    return true;
}
static hashmap_entry *hm_impl_get_entry(hashmap *_map_, size_t key)
{
    size_t hash = _map_->hash(key);
    size_t pos = hash % _map_->capacity;
    hashmap_entry *target = &(_map_->buffer[pos]);
    while (target->state == ES_FILLED || target->state == ES_DELETED)
    {
        if (_map_->cmp(target->key, key) == 0)
            return target;
        pos++;
        target = &(_map_->buffer[pos % _map_->capacity]);
    }
    return NULL;
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
