#include "ext/multimap.h"

typedef struct multimap_s
{
    struct multimap_entry_s *(*buffer)[2];
    size_t capacity;
    size_t count;
    double load;
    int (*cmp)(size_t, size_t);
    size_t (*hash)(size_t);
    struct multimap_iter_s (*it_start)(struct multimap_s *);
    struct multimap_iter_s (*it_end)(struct multimap_s *);
} multimap, *multimap_ptr;
typedef struct multimap_entry_s
{
    size_t key;
    size_t value;
    struct multimap_entry_s *next;
    struct multimap_entry_s *prev;
} multimap_entry, *multimap_entry_ptr;
typedef struct multimap_iter_s
{
    struct multimap_s *target;
    struct multimap_entry_s *curr_entry;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    bool start;
    bool end;
} multimap_iter, *multimap_iter_ptr;
multimap *mm_new(size_t size, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t));
void mm_clear(multimap *_map_);
void mm_free(multimap *_map_);
bool mm_insert(multimap *_map_, size_t key, size_t value);
bool mm_remove(multimap *_map_, size_t key, size_t *value);
size_t mm_remove_all(multimap *_map_, size_t key);
bool mm_insert_if(multimap *_map_, size_t key, size_t value, bool condition);
bool mm_remove_if(multimap *_map_, size_t key, size_t *value, bool condition);
bool mm_max(multimap *_map_, size_t *key, size_t *value);
bool mm_min(multimap *_map_, size_t *key, size_t *value);
size_t mm_get(multimap *_map_, size_t key);
size_t *mm_get_ref(multimap *_map_, size_t key);
bool mm_contains(multimap *_map_, size_t key);
bool mm_empty(multimap *_map_);
size_t mm_count(multimap *_map_);
size_t mm_capacity(multimap *_map_);
multimap_iter *mm_iter_new(multimap *target);
void mm_iter_free(multimap_iter *iter);
void mm_iter_init(multimap_iter *iter, multimap *target);
bool mm_iter_start(multimap_iter *iter);
bool mm_iter_end(multimap_iter *iter);
void mm_iter_to_start(multimap_iter *iter);
void mm_iter_to_end(multimap_iter *iter);
bool mm_iter_next(multimap_iter *iter);
bool mm_iter_prev(multimap_iter *iter);
size_t mm_iter_key(multimap_iter *iter);
size_t mm_iter_value(multimap_iter *iter);
size_t *mm_iter_rvalue(multimap_iter *iter);
size_t mm_iter_index(multimap_iter *iter);
static inline size_t mm_impl_default_key(void)
{
    size_t _empty_key_;
    memset(&_empty_key_, 0, sizeof(size_t));
    return _empty_key_;
}
static inline size_t mm_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
bool mm_impl_grow(multimap *_map_);
multimap_entry *mm_impl_new_entry(size_t key, size_t value);
multimap_entry *mm_impl_get_entry(multimap *_map_, size_t key);
size_t mm_impl_calculate_size(size_t required);
static multimap_iter mm_impl_it_start(multimap *_map_);
static multimap_iter mm_impl_it_end(multimap *_map_);
multimap *mm_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t))
{
    if (capacity == 0 || load <= 0)
        return NULL;
    size_t real_capacity = mm_impl_calculate_size(capacity / load);
    multimap *_map_ = malloc(sizeof(multimap));
    if (!_map_)
        return NULL;
    _map_->buffer = malloc(sizeof(multimap_entry_ptr[2]) * real_capacity);
    if (!_map_->buffer)
    {
        free(_map_);
        return NULL;
    }
    memset(_map_->buffer, 0, sizeof(multimap_entry_ptr[2]) * real_capacity);
    _map_->count = 0;
    _map_->capacity = real_capacity;
    _map_->load = load;
    _map_->cmp = compare;
    _map_->hash = hash;
    return _map_;
}
void mm_clear(multimap *_map_)
{
    size_t index = 0;
    multimap_entry *scan;
    while (index < _map_->capacity)
    {
        scan = _map_->buffer[index][0];
        if (scan != NULL)
        {
            if (scan->next == NULL && scan->prev == NULL)
                free(scan);
            else
            {
                while (scan != NULL)
                {
                    multimap_entry *tmp = scan;
                    scan = scan->next;
                    free(tmp);
                }
            }
        }
        index++;
    }
    memset(_map_->buffer, 0, sizeof(multimap_entry_ptr[2]) * _map_->capacity);
    _map_->count = 0;
}
void mm_free(multimap *_map_)
{
    mm_clear(_map_);
    free(_map_->buffer);
    free(_map_);
}
bool mm_insert(multimap *_map_, size_t key, size_t value)
{
    if ((double)_map_->capacity * _map_->load <= (double)_map_->count)
    {
        if (!mm_impl_grow(_map_))
            return false;
    }
    size_t hash = _map_->hash(key);
    size_t pos = hash % _map_->capacity;
    multimap_entry *entry = mm_impl_new_entry(key, value);
    if (_map_->buffer[pos][0] == NULL)
    {
        _map_->buffer[pos][0] = entry;
        _map_->buffer[pos][1] = entry;
    }
    else
    {
        entry->prev = _map_->buffer[pos][1];
        _map_->buffer[pos][1]->next = entry;
        _map_->buffer[pos][1] = entry;
    }
    _map_->count++;
    return true;
}
bool mm_remove(multimap *_map_, size_t key, size_t *value)
{
    size_t hash = _map_->hash(key);
    multimap_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);
    multimap_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);
    if (*head == NULL)
        return false;
    multimap_entry *entry = *head;
    if (entry->next == NULL && entry->prev == NULL)
    {
        if (_map_->cmp(entry->key, key) == 0)
        {
            *head = NULL;
            *tail = NULL;
            *value = entry->value;
        }
        else
            return false;
    }
    else
    {
        bool found = false;
        while (entry != NULL)
        {
            if (_map_->cmp(entry->key, key) == 0)
            {
                if (*head == entry)
                    *head = entry->next;
                if (*tail == entry)
                    *tail = entry->prev;
                if (entry->prev != NULL)
                    entry->prev->next = entry->next;
                if (entry->next != NULL)
                    entry->next->prev = entry->prev;
                *value = entry->value;
                found = true;
                break;
            }
            else
                entry = entry->next;
        }
        if (!found)
            return false;
    }
    free(entry);
    _map_->count--;
    return true;
}
size_t mm_remove_all(multimap *_map_, size_t key)
{
    size_t total = 0;
    size_t hash = _map_->hash(key);
    multimap_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);
    multimap_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);
    if (*head == NULL)
        return total;
    multimap_entry *entry = *head;
    if (entry->next == NULL)
    {
        *head = NULL;
        *tail = NULL;
        free(entry);
        total++;
    }
    else
    {
        while (entry != NULL)
        {
            if (_map_->cmp(entry->key, key) == 0)
            {
                if (*head == entry)
                    *head = entry->next;
                if (*tail == entry)
                    *tail = entry->prev;
                multimap_entry *temp = entry->next;
                if (entry->prev != NULL)
                    entry->prev->next = entry->next;
                if (entry->next != NULL)
                    entry->next->prev = entry->prev;
                free(entry);
                entry = temp;
                total++;
            }
            else
                entry = entry->next;
        }
    }
    _map_->count -= total;
    return total;
}
bool mm_insert_if(multimap *_map_, size_t key, size_t value, bool condition)
{
    if (condition)
        return mm_insert(_map_, key, value);
    return false;
}
bool mm_remove_if(multimap *_map_, size_t key, size_t *value, bool condition)
{
    if (condition)
        return mm_remove(_map_, key, value);
    return false;
}
bool mm_max(multimap *_map_, size_t *key, size_t *value)
{
    if (mm_empty(_map_))
        return false;
    multimap_iter iter;
    for (mm_iter_init(&iter, _map_); !mm_iter_end(&iter); mm_iter_next(&iter))
    {
        size_t result_key = mm_iter_key(&iter);
        size_t result_value = mm_iter_value(&iter);
        size_t index = mm_iter_index(&iter);
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
bool mm_min(multimap *_map_, size_t *key, size_t *value)
{
    if (mm_empty(_map_))
        return false;
    multimap_iter iter;
    for (mm_iter_init(&iter, _map_); !mm_iter_end(&iter); mm_iter_next(&iter))
    {
        size_t result_key = mm_iter_key(&iter);
        size_t result_value = mm_iter_value(&iter);
        size_t index = mm_iter_index(&iter);
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
size_t mm_get(multimap *_map_, size_t key)
{
    multimap_entry *entry = mm_impl_get_entry(_map_, key);
    if (!entry)
        return mm_impl_default_value();
    return entry->value;
}
size_t *mm_get_ref(multimap *_map_, size_t key)
{
    multimap_entry *entry = mm_impl_get_entry(_map_, key);
    if (!entry)
        return NULL;
    return &(entry->value);
}
bool mm_contains(multimap *_map_, size_t key) { return mm_impl_get_entry(_map_, key) != NULL; }
bool mm_empty(multimap *_map_) { return _map_->count == 0; }
size_t mm_count(multimap *_map_) { return _map_->count; }
size_t mm_capacity(multimap *_map_) { return _map_->capacity; }
multimap_iter *mm_iter_new(multimap *target)
{
    multimap_iter *iter = malloc(sizeof(multimap_iter));
    if (!iter)
        return NULL;
    mm_iter_init(iter, target);
    return iter;
}
void mm_iter_free(multimap_iter *iter) { free(iter); }
void mm_iter_init(multimap_iter *iter, multimap *target)
{
    iter->target = target;
    iter->cursor = 0;
    iter->index = 0;
    iter->start = true;
    iter->end = mm_empty(target);
    if (!mm_empty(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            if (target->buffer[i][0] != NULL)
            {
                iter->curr_entry = target->buffer[i][0];
                iter->first = i;
                break;
            }
        }
        iter->cursor = iter->first;
        for (size_t i = target->capacity; i > 0; i--)
        {
            if (target->buffer[i - 1][0] != NULL)
            {
                iter->last = i - 1;
                break;
            }
        }
    }
}
bool mm_iter_start(multimap_iter *iter) { return mm_empty(iter->target) || iter->start; }
bool mm_iter_end(multimap_iter *iter) { return mm_empty(iter->target) || iter->end; }
void mm_iter_to_start(multimap_iter *iter)
{
    iter->cursor = iter->first;
    iter->index = 0;
    iter->start = true;
    iter->end = mm_empty(iter->target);
    iter->curr_entry = iter->target->buffer[iter->first][0];
}
void mm_iter_to_end(multimap_iter *iter)
{
    iter->cursor = iter->last;
    iter->index = iter->target->count - 1;
    iter->start = mm_empty(iter->target);
    iter->end = true;
    iter->curr_entry = iter->target->buffer[iter->last][1];
}
bool mm_iter_next(multimap_iter *iter)
{
    if (iter->end)
        return false;
    if (iter->curr_entry->next != NULL)
    {
        iter->curr_entry = iter->curr_entry->next;
        iter->index++;
    }
    else
    {
        if (iter->cursor == iter->last)
        {
            iter->end = true;
            return false;
        }
        else
        {
            iter->cursor++;
            while (iter->target->buffer[iter->cursor][0] == NULL)
                iter->cursor++;
            iter->curr_entry = iter->target->buffer[iter->cursor][0];
            iter->index++;
        }
    }
    iter->start = mm_empty(iter->target);
    return true;
}
bool mm_iter_prev(multimap_iter *iter)
{
    if (iter->start)
        return false;
    if (iter->curr_entry->prev != NULL)
    {
        iter->curr_entry = iter->curr_entry->prev;
        iter->index--;
    }
    else
    {
        if (iter->cursor == iter->first)
        {
            iter->start = true;
            return false;
        }
        else
        {
            iter->cursor--;
            while (iter->target->buffer[iter->cursor][1] == NULL)
                iter->cursor--;
            iter->curr_entry = iter->target->buffer[iter->cursor][1];
            iter->index--;
        }
    }
    iter->end = mm_empty(iter->target);
    return true;
}
size_t mm_iter_key(multimap_iter *iter)
{
    if (mm_empty(iter->target))
        return mm_impl_default_key();
    return iter->curr_entry->key;
}
size_t mm_iter_value(multimap_iter *iter)
{
    if (mm_empty(iter->target))
        return mm_impl_default_value();
    return iter->curr_entry->value;
}
size_t *mm_iter_rvalue(multimap_iter *iter)
{
    if (mm_empty(iter->target))
        return NULL;
    return &(iter->curr_entry->value);
}
size_t mm_iter_index(multimap_iter *iter) { return iter->index; }
bool mm_impl_grow(multimap *_map_)
{
    size_t new_capacity = mm_impl_calculate_size(_map_->capacity * 1.5);
    multimap *_new_map_ = mm_new(new_capacity, _map_->load, _map_->cmp, _map_->hash);
    if (!_new_map_)
        return false;
    multimap_iter iter;
    for (mm_iter_init(&iter, _map_); !mm_iter_end(&iter); mm_iter_next(&iter))
    {
        size_t key = mm_iter_key(&iter);
        size_t value = mm_iter_value(&iter);
        mm_insert(_new_map_, key, value);
    }
    if (_map_->count != _new_map_->count)
    {
        mm_free(_new_map_);
        return false;
    }
    multimap_entry *(*tmp_b)[2] = _map_->buffer;
    _map_->buffer = _new_map_->buffer;
    _new_map_->buffer = tmp_b;
    size_t tmp_c = _map_->capacity;
    _map_->capacity = _new_map_->capacity;
    _new_map_->capacity = tmp_c;
    mm_free(_new_map_);
    return true;
}
multimap_entry *mm_impl_new_entry(size_t key, size_t value)
{
    multimap_entry *entry = malloc(sizeof(multimap_entry));
    if (!entry)
        return NULL;
    entry->key = key;
    entry->value = value;
    entry->next = NULL;
    entry->prev = NULL;
    return entry;
}
multimap_entry *mm_impl_get_entry(multimap *_map_, size_t key)
{
    size_t hash = _map_->hash(key);
    multimap_entry *entry = _map_->buffer[hash % _map_->capacity][0];
    if (entry == NULL)
        return NULL;
    if (entry->next == NULL && entry->prev == NULL)
    {
        if (_map_->cmp(entry->key, key) == 0)
            return entry;
        else
            return NULL;
    }
    while (entry != NULL)
    {
        if (_map_->cmp(entry->key, key) == 0)
            return entry;
        entry = entry->next;
    }
    return NULL;
}
size_t mm_impl_calculate_size(size_t required)
{
    const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);
    if (cmc_hashtable_primes[count - 1] < required)
        return required;
    size_t i = 0;
    while (cmc_hashtable_primes[i] < required)
        i++;
    return cmc_hashtable_primes[i];
}
static multimap_iter mm_impl_it_start(multimap *_map_)
{
    multimap_iter iter;
    mm_iter_init(&iter, _map_);
    return iter;
}
static multimap_iter mm_impl_it_end(multimap *_map_)
{
    multimap_iter iter;
    mm_iter_init(&iter, _map_);
    mm_iter_to_end(&iter);
    return iter;
}
