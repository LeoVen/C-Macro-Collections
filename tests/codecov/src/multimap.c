#include <cmc/multimap.h>

//CMC_GENERATE_MULTIMAP(mm, multimap, size_t, size_t)

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
    _Bool start;
    _Bool end;
} multimap_iter, *multimap_iter_ptr;
multimap *mm_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t));
void mm_clear(multimap *_map_, void (*deallocator)(size_t, size_t));
void mm_free(multimap *_map_, void (*deallocator)(size_t, size_t));
_Bool mm_insert(multimap *_map_, size_t key, size_t value);
_Bool mm_update(multimap *_map_, size_t key, size_t new_value, size_t *old_value);
size_t mm_update_all(multimap *_map_, size_t key, size_t new_value, size_t **old_values);
_Bool mm_remove(multimap *_map_, size_t key, size_t *out_value);
size_t mm_remove_all(multimap *_map_, size_t key, size_t **out_values);
_Bool mm_max(multimap *_map_, size_t *key, size_t *value);
_Bool mm_min(multimap *_map_, size_t *key, size_t *value);
size_t mm_get(multimap *_map_, size_t key);
size_t *mm_get_ref(multimap *_map_, size_t key);
_Bool mm_contains(multimap *_map_, size_t key);
_Bool mm_empty(multimap *_map_);
_Bool mm_full(multimap *_map_);
size_t mm_count(multimap *_map_);
size_t mm_key_count(multimap *_map_, size_t key);
size_t mm_capacity(multimap *_map_);
double mm_load(multimap *_map_);
_Bool mm_resize(multimap *_map_, size_t capacity);
multimap *mm_copy_of(multimap *_map_, size_t (*key_copy_func)(size_t), size_t (*value_copy_func)(size_t));
_Bool mm_equals(multimap *_map1_, multimap *_map2_, _Bool ignore_key_count);
struct cmc_string mm_to_string(multimap *_map_);
multimap_iter *mm_iter_new(multimap *target);
void mm_iter_free(multimap_iter *iter);
void mm_iter_init(multimap_iter *iter, multimap *target);
_Bool mm_iter_start(multimap_iter *iter);
_Bool mm_iter_end(multimap_iter *iter);
void mm_iter_to_start(multimap_iter *iter);
void mm_iter_to_end(multimap_iter *iter);
_Bool mm_iter_next(multimap_iter *iter);
_Bool mm_iter_prev(multimap_iter *iter);
_Bool mm_iter_advance(multimap_iter *iter, size_t steps);
_Bool mm_iter_rewind(multimap_iter *iter, size_t steps);
_Bool mm_iter_go_to(multimap_iter *iter, size_t index);
size_t mm_iter_key(multimap_iter *iter);
size_t mm_iter_value(multimap_iter *iter);
size_t *mm_iter_rvalue(multimap_iter *iter);
size_t mm_iter_index(multimap_iter *iter);
multimap_entry *mm_impl_new_entry(size_t key, size_t value);
multimap_entry *mm_impl_get_entry(multimap *_map_, size_t key);
size_t mm_impl_calculate_size(size_t required);
static multimap_iter mm_impl_it_start(multimap *_map_);
static multimap_iter mm_impl_it_end(multimap *_map_);
multimap *mm_new(size_t capacity, double load, int (*compare)(size_t, size_t), size_t (*hash)(size_t))
{
    if (capacity == 0 || load <= 0)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffff * load)
        return ((void *)0);
    size_t real_capacity = mm_impl_calculate_size(capacity / load);
    multimap *_map_ = malloc(sizeof(multimap));
    if (!_map_)
        return ((void *)0);
    _map_->buffer = malloc(sizeof(multimap_entry_ptr[2]) * real_capacity);
    if (!_map_->buffer)
    {
        free(_map_);
        return ((void *)0);
    }
    memset(_map_->buffer, 0, sizeof(multimap_entry_ptr[2]) * real_capacity);
    _map_->count = 0;
    _map_->capacity = real_capacity;
    _map_->load = load;
    _map_->cmp = compare;
    _map_->hash = hash;
    _map_->it_start = mm_impl_it_start;
    _map_->it_end = mm_impl_it_end;
    return _map_;
}
void mm_clear(multimap *_map_, void (*deallocator)(size_t, size_t))
{
    size_t index = 0;
    multimap_entry *scan;
    while (index < _map_->capacity)
    {
        scan = _map_->buffer[index][0];
        if (scan != ((void *)0))
        {
            if (scan->next == ((void *)0) && scan->prev == ((void *)0))
            {
                if (deallocator)
                    deallocator(scan->key, scan->value);
                free(scan);
            }
            else
            {
                while (scan != ((void *)0))
                {
                    multimap_entry *tmp = scan;
                    scan = scan->next;
                    if (deallocator)
                        deallocator(tmp->key, tmp->value);
                    free(tmp);
                }
            }
        }
        index++;
    }
    memset(_map_->buffer, 0, sizeof(multimap_entry_ptr[2]) * _map_->capacity);
    _map_->count = 0;
}
void mm_free(multimap *_map_, void (*deallocator)(size_t, size_t))
{
    size_t index = 0;
    multimap_entry *scan;
    while (index < _map_->capacity)
    {
        scan = _map_->buffer[index][0];
        if (scan != ((void *)0))
        {
            if (scan->next == ((void *)0) && scan->prev == ((void *)0))
                free(scan);
            else
            {
                while (scan != ((void *)0))
                {
                    multimap_entry *tmp = scan;
                    scan = scan->next;
                    if (deallocator)
                        deallocator(tmp->key, tmp->value);
                    free(tmp);
                }
            }
        }
        index++;
    }
    free(_map_->buffer);
    free(_map_);
}
_Bool mm_insert(multimap *_map_, size_t key, size_t value)
{
    if (mm_full(_map_))
    {
        if (!mm_resize(_map_, mm_capacity(_map_) + 1))
            return 0;
    }
    size_t hash = _map_->hash(key);
    size_t pos = hash % _map_->capacity;
    multimap_entry *entry = mm_impl_new_entry(key, value);
    if (_map_->buffer[pos][0] == ((void *)0))
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
    return 1;
}
_Bool mm_update(multimap *_map_, size_t key, size_t new_value, size_t *old_value)
{
    multimap_entry *entry = mm_impl_get_entry(_map_, key);
    if (!entry)
        return 0;
    if (old_value)
        *old_value = entry->value;
    entry->value = new_value;
    return 0;
}
size_t mm_update_all(multimap *_map_, size_t key, size_t new_value, size_t **old_values)
{
    size_t total = mm_key_count(_map_, key);
    size_t index = 0;
    if (total == 0)
        return total;
    if (old_values)
    {
        *old_values = malloc(sizeof(size_t) * total);
        if (!(*old_values))
            return 0;
    }
    size_t hash = _map_->hash(key);
    multimap_entry *entry = _map_->buffer[hash % _map_->capacity][0];
    while (entry != ((void *)0))
    {
        if (_map_->cmp(entry->key, key) == 0)
        {
            if (old_values)
                (*old_values)[index++] = entry->value;
            entry->value = new_value;
        }
        entry = entry->next;
    }
    return index;
}
_Bool mm_remove(multimap *_map_, size_t key, size_t *out_value)
{
    size_t hash = _map_->hash(key);
    multimap_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);
    multimap_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);
    if (*head == ((void *)0))
        return 0;
    multimap_entry *entry = *head;
    if (entry->next == ((void *)0) && entry->prev == ((void *)0))
    {
        if (_map_->cmp(entry->key, key) == 0)
        {
            *head = ((void *)0);
            *tail = ((void *)0);
            if (out_value)
                *out_value = entry->value;
        }
        else
            return 0;
    }
    else
    {
        _Bool found = 0;
        while (entry != ((void *)0))
        {
            if (_map_->cmp(entry->key, key) == 0)
            {
                if (*head == entry)
                    *head = entry->next;
                if (*tail == entry)
                    *tail = entry->prev;
                if (entry->prev != ((void *)0))
                    entry->prev->next = entry->next;
                if (entry->next != ((void *)0))
                    entry->next->prev = entry->prev;
                if (out_value)
                    *out_value = entry->value;
                found = 1;
                break;
            }
            else
                entry = entry->next;
        }
        if (!found)
            return 0;
    }
    free(entry);
    _map_->count--;
    return 1;
}
size_t mm_remove_all(multimap *_map_, size_t key, size_t **out_values)
{
    size_t hash = _map_->hash(key);
    multimap_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);
    multimap_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);
    if (*head == ((void *)0))
        return 0;
    size_t total = mm_key_count(_map_, key);
    size_t index = 0;
    if (out_values)
        *out_values = malloc(sizeof(multimap_entry) * total);
    multimap_entry *entry = *head;
    if (entry->next == ((void *)0))
    {
        *head = ((void *)0);
        *tail = ((void *)0);
        if (out_values)
            (*out_values)[index++] = entry->value;
        free(entry);
    }
    else
    {
        while (entry != ((void *)0))
        {
            if (_map_->cmp(entry->key, key) == 0)
            {
                if (*head == entry)
                    *head = entry->next;
                if (*tail == entry)
                    *tail = entry->prev;
                multimap_entry *temp = entry->next;
                if (entry->prev != ((void *)0))
                    entry->prev->next = entry->next;
                if (entry->next != ((void *)0))
                    entry->next->prev = entry->prev;
                if (out_values)
                    (*out_values)[index++] = entry->value;
                free(entry);
                entry = temp;
            }
            else
                entry = entry->next;
        }
    }
    _map_->count -= index;
    return index;
}
_Bool mm_max(multimap *_map_, size_t *key, size_t *value)
{
    if (mm_empty(_map_))
        return 0;
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
    return 1;
}
_Bool mm_min(multimap *_map_, size_t *key, size_t *value)
{
    if (mm_empty(_map_))
        return 0;
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
    return 1;
}
size_t mm_get(multimap *_map_, size_t key)
{
    multimap_entry *entry = mm_impl_get_entry(_map_, key);
    if (!entry)
        return (size_t){0};
    return entry->value;
}
size_t *mm_get_ref(multimap *_map_, size_t key)
{
    multimap_entry *entry = mm_impl_get_entry(_map_, key);
    if (!entry)
        return ((void *)0);
    return &(entry->value);
}
_Bool mm_contains(multimap *_map_, size_t key) { return mm_impl_get_entry(_map_, key) != ((void *)0); }
_Bool mm_empty(multimap *_map_) { return _map_->count == 0; }
_Bool mm_full(multimap *_map_) { return (double)mm_capacity(_map_) * mm_load(_map_) <= (double)mm_count(_map_); }
size_t mm_count(multimap *_map_) { return _map_->count; }
size_t mm_key_count(multimap *_map_, size_t key)
{
    size_t hash = _map_->hash(key);
    multimap_entry *entry = _map_->buffer[hash % _map_->capacity][0];
    size_t total_count = 0;
    if (!entry)
        return total_count;
    while (entry != ((void *)0))
    {
        if (_map_->cmp(entry->key, key) == 0)
            total_count++;
        entry = entry->next;
    }
    return total_count;
}
size_t mm_capacity(multimap *_map_) { return _map_->capacity; }
double mm_load(multimap *_map_) { return _map_->load; }
_Bool mm_resize(multimap *_map_, size_t capacity)
{
    if (mm_capacity(_map_) == capacity)
        return 1;
    if (mm_capacity(_map_) > capacity / mm_load(_map_))
        return 1;
    if (capacity >= 0xffffffffffffffff * mm_load(_map_))
        return 0;
    size_t theoretical_size = mm_impl_calculate_size(capacity);
    if (theoretical_size < mm_count(_map_) / mm_load(_map_))
        return 0;
    multimap *_new_map_ = mm_new(capacity, mm_load(_map_), _map_->cmp, _map_->hash);
    if (!_new_map_)
        return 0;
    multimap_iter iter;
    for (mm_iter_init(&iter, _map_); !mm_iter_end(&iter); mm_iter_next(&iter))
    {
        size_t key = mm_iter_key(&iter);
        size_t value = mm_iter_value(&iter);
        mm_insert(_new_map_, key, value);
    }
    if (mm_count(_map_) != mm_count(_new_map_))
    {
        mm_free(_new_map_, ((void *)0));
        return 0;
    }
    multimap_entry *(*tmp_b)[2] = _map_->buffer;
    _map_->buffer = _new_map_->buffer;
    _new_map_->buffer = tmp_b;
    size_t tmp_c = _map_->capacity;
    _map_->capacity = _new_map_->capacity;
    _new_map_->capacity = tmp_c;
    mm_free(_new_map_, ((void *)0));
    return 1;
}
multimap *mm_copy_of(multimap *_map_, size_t (*key_copy_func)(size_t), size_t (*value_copy_func)(size_t))
{
    multimap *result = mm_new(_map_->capacity, _map_->load, _map_->cmp, _map_->hash);
    if (!result)
        return ((void *)0);
    multimap_iter iter;
    mm_iter_init(&iter, _map_);
    if (!mm_empty(_map_))
    {
        for (mm_iter_to_start(&iter); !mm_iter_end(&iter); mm_iter_next(&iter))
        {
            size_t key = mm_iter_key(&iter);
            size_t value = mm_iter_value(&iter);
            if (key_copy_func)
                key = key_copy_func(key);
            if (value_copy_func)
                value = value_copy_func(value);
            mm_insert(result, key, value);
        }
    }
    return result;
}
_Bool mm_equals(multimap *_map1_, multimap *_map2_, _Bool ignore_key_count)
{
    if (ignore_key_count)
    {
        multimap_iter iter;
        mm_iter_init(&iter, _map1_);
        for (mm_iter_to_start(&iter); !mm_iter_end(&iter); mm_iter_next(&iter))
        {
            if (mm_impl_get_entry(_map2_, mm_iter_key(&iter)) == ((void *)0))
                return 0;
        }
    }
    else
    {
        if (mm_count(_map1_) != mm_count(_map2_))
            return 0;
        multimap_iter iter;
        mm_iter_init(&iter, _map1_);
        for (mm_iter_to_start(&iter); !mm_iter_end(&iter); mm_iter_next(&iter))
        {
            size_t key = mm_iter_key(&iter);
            if (mm_key_count(_map1_, key) != mm_key_count(_map2_, key))
                return 0;
        }
    }
    return 1;
}
struct cmc_string mm_to_string(multimap *_map_)
{
    struct cmc_string str;
    multimap *m_ = _map_;
    const char *name = "multimap";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_multimap, name, m_, m_->buffer, m_->capacity, m_->count, m_->load, m_->cmp, m_->hash);
    return str;
}
multimap_iter *mm_iter_new(multimap *target)
{
    multimap_iter *iter = malloc(sizeof(multimap_iter));
    if (!iter)
        return ((void *)0);
    mm_iter_init(iter, target);
    return iter;
}
void mm_iter_free(multimap_iter *iter) { free(iter); }
void mm_iter_init(multimap_iter *iter, multimap *target)
{
    memset(iter, 0, sizeof(multimap_iter));
    iter->target = target;
    iter->start = 1;
    iter->end = mm_empty(target);
    if (!mm_empty(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            if (target->buffer[i][0] != ((void *)0))
            {
                iter->curr_entry = target->buffer[i][0];
                iter->first = i;
                break;
            }
        }
        iter->cursor = iter->first;
        for (size_t i = target->capacity; i > 0; i--)
        {
            if (target->buffer[i - 1][0] != ((void *)0))
            {
                iter->last = i - 1;
                break;
            }
        }
    }
}
_Bool mm_iter_start(multimap_iter *iter) { return mm_empty(iter->target) || iter->start; }
_Bool mm_iter_end(multimap_iter *iter) { return mm_empty(iter->target) || iter->end; }
void mm_iter_to_start(multimap_iter *iter)
{
    if (!mm_empty(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = 1;
        iter->end = mm_empty(iter->target);
        iter->curr_entry = iter->target->buffer[iter->first][0];
    }
}
void mm_iter_to_end(multimap_iter *iter)
{
    if (!mm_empty(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = mm_empty(iter->target);
        iter->end = 1;
        iter->curr_entry = iter->target->buffer[iter->last][1];
    }
}
_Bool mm_iter_next(multimap_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->curr_entry->next != ((void *)0))
    {
        iter->curr_entry = iter->curr_entry->next;
        iter->index++;
    }
    else
    {
        if (iter->cursor == iter->last)
        {
            iter->end = 1;
            return 0;
        }
        iter->cursor++;
        while (iter->target->buffer[iter->cursor][0] == ((void *)0))
            iter->cursor++;
        iter->curr_entry = iter->target->buffer[iter->cursor][0];
        iter->index++;
    }
    iter->start = mm_empty(iter->target);
    return 1;
}
_Bool mm_iter_prev(multimap_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->curr_entry->prev != ((void *)0))
    {
        iter->curr_entry = iter->curr_entry->prev;
        iter->index--;
    }
    else
    {
        if (iter->cursor == iter->first)
        {
            iter->start = 1;
            return 0;
        }
        iter->cursor--;
        while (iter->target->buffer[iter->cursor][1] == ((void *)0))
            iter->cursor--;
        iter->curr_entry = iter->target->buffer[iter->cursor][1];
        iter->index--;
    }
    iter->end = mm_empty(iter->target);
    return 1;
}
_Bool mm_iter_advance(multimap_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == mm_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= mm_count(iter->target))
        return 0;
    for (size_t i = 0; i < steps; i++)
        mm_iter_next(iter);
    return 1;
}
_Bool mm_iter_rewind(multimap_iter *iter, size_t steps)
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
        mm_iter_prev(iter);
    return 1;
}
_Bool mm_iter_go_to(multimap_iter *iter, size_t index)
{
    if (index >= mm_count(iter->target))
        return 0;
    if (iter->index > index)
        return mm_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return mm_iter_advance(iter, index - iter->index);
    return 1;
}
size_t mm_iter_key(multimap_iter *iter)
{
    if (mm_empty(iter->target))
        return (size_t){0};
    return iter->curr_entry->key;
}
size_t mm_iter_value(multimap_iter *iter)
{
    if (mm_empty(iter->target))
        return (size_t){0};
    return iter->curr_entry->value;
}
size_t *mm_iter_rvalue(multimap_iter *iter)
{
    if (mm_empty(iter->target))
        return ((void *)0);
    return &(iter->curr_entry->value);
}
size_t mm_iter_index(multimap_iter *iter) { return iter->index; }
multimap_entry *mm_impl_new_entry(size_t key, size_t value)
{
    multimap_entry *entry = malloc(sizeof(multimap_entry));
    if (!entry)
        return ((void *)0);
    entry->key = key;
    entry->value = value;
    entry->next = ((void *)0);
    entry->prev = ((void *)0);
    return entry;
}
multimap_entry *mm_impl_get_entry(multimap *_map_, size_t key)
{
    size_t hash = _map_->hash(key);
    multimap_entry *entry = _map_->buffer[hash % _map_->capacity][0];
    while (entry != ((void *)0))
    {
        if (_map_->cmp(entry->key, key) == 0)
            return entry;
        entry = entry->next;
    }
    return ((void *)0);
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
