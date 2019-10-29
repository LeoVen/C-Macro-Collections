#include <cmc/bidimap.h>

//CMC_GENERATE_BIDIMAP(bm, bidimap, size_t, size_t)

typedef struct bidimap_s
{
    struct bidimap_entry_s **key_buffer;
    struct bidimap_entry_s **val_buffer;
    size_t capacity;
    size_t count;
    double load;
    int (*key_cmp)(size_t, size_t);
    int (*val_cmp)(size_t, size_t);
    size_t (*key_hash)(size_t);
    size_t (*val_hash)(size_t);
    struct bidimap_iter_s (*it_start)(struct bidimap_s *);
    struct bidimap_iter_s (*it_end)(struct bidimap_s *);
} bidimap, *bidimap_ptr;
typedef struct bidimap_entry_s
{
    size_t key;
    size_t value;
    size_t key_dist;
    size_t val_dist;
} bidimap_entry, *bidimap_entry_ptr;
typedef struct bidimap_iter_s
{
    struct bidimap_s *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
} bidimap_iter, *bidimap_iter_ptr;
bidimap *bm_new(size_t capacity, double load, int (*key_cmp)(size_t, size_t), size_t (*key_hash)(size_t), int (*val_cmp)(size_t, size_t), size_t (*val_hash)(size_t));
void bm_clear(bidimap *_map_, void (*deallocator)(size_t, size_t));
void bm_free(bidimap *_map_, void (*deallocator)(size_t, size_t));
_Bool bm_insert(bidimap *_map_, size_t key, size_t value);
_Bool bm_update_key(bidimap *_map_, size_t key, size_t *old_key);
_Bool bm_update_val(bidimap *_map_, size_t val, size_t *old_val);
_Bool bm_remove_by_key(bidimap *_map_, size_t key, size_t *out_value);
_Bool bm_remove_by_val(bidimap *_map_, size_t value, size_t *out_key);
size_t bm_get_key(bidimap *_map_, size_t val);
size_t bm_get_val(bidimap *_map_, size_t key);
_Bool bm_contains_key(bidimap *_map_, size_t key);
_Bool bm_contains_val(bidimap *_map_, size_t val);
_Bool bm_empty(bidimap *_map_);
_Bool bm_full(bidimap *_map_);
size_t bm_count(bidimap *_map_);
size_t bm_capacity(bidimap *_map_);
double bm_load(bidimap *_map_);
bidimap *bm_copy_of(bidimap *_map_, size_t (*key_copy_func)(size_t), size_t (*value_copy_func)(size_t));
_Bool bm_equals(bidimap *_map1_, bidimap *_map2_);
struct cmc_string bm_to_string(bidimap *_map_);
bidimap_iter *bm_iter_new(bidimap *target);
void bm_iter_free(bidimap_iter *iter);
void bm_iter_init(bidimap_iter *iter, bidimap *target);
_Bool bm_iter_start(bidimap_iter *iter);
_Bool bm_iter_end(bidimap_iter *iter);
void bm_iter_to_start(bidimap_iter *iter);
void bm_iter_to_end(bidimap_iter *iter);
_Bool bm_iter_next(bidimap_iter *iter);
_Bool bm_iter_prev(bidimap_iter *iter);
_Bool bm_iter_advance(bidimap_iter *iter, size_t steps);
_Bool bm_iter_rewind(bidimap_iter *iter, size_t steps);
_Bool bm_iter_go_to(bidimap_iter *iter, size_t index);
size_t bm_iter_key(bidimap_iter *iter);
size_t bm_iter_value(bidimap_iter *iter);
size_t bm_iter_index(bidimap_iter *iter);
static _Bool bm_impl_grow(bidimap *_map_);
static bidimap_entry *bm_impl_new_entry(size_t key, size_t value);
static bidimap_entry **bm_impl_get_entry_by_key(bidimap *_map_, size_t key);
static bidimap_entry **bm_impl_get_entry_by_val(bidimap *_map_, size_t val);
static bidimap_entry **bm_impl_add_entry_to_key(bidimap *_map_, bidimap_entry *entry);
static bidimap_entry **bm_impl_add_entry_to_val(bidimap *_map_, bidimap_entry *entry);
static size_t bm_impl_calculate_size(size_t required);
static bidimap_iter bm_impl_it_start(bidimap *_map_);
static bidimap_iter bm_impl_it_end(bidimap *_map_);
bidimap *bm_new(size_t capacity, double load, int (*key_cmp)(size_t, size_t), size_t (*key_hash)(size_t), int (*val_cmp)(size_t, size_t), size_t (*val_hash)(size_t))
{
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffff * load)
        return ((void *)0);
    size_t real_capacity = bm_impl_calculate_size(capacity / load);
    bidimap *_map_ = malloc(sizeof(bidimap));
    if (!_map_)
        return ((void *)0);
    _map_->key_buffer = calloc(real_capacity, sizeof(bidimap_entry *));
    _map_->val_buffer = calloc(real_capacity, sizeof(bidimap_entry *));
    if (!_map_->key_buffer || !_map_->val_buffer)
    {
        free(_map_->key_buffer);
        free(_map_->val_buffer);
        free(_map_);
        return ((void *)0);
    }
    _map_->count = 0;
    _map_->capacity = real_capacity;
    _map_->load = load;
    _map_->key_cmp = key_cmp;
    _map_->val_cmp = val_cmp;
    _map_->key_hash = key_hash;
    _map_->val_hash = val_hash;
    _map_->it_start = bm_impl_it_start;
    _map_->it_end = bm_impl_it_end;
    return _map_;
}
void bm_clear(bidimap *_map_, void (*deallocator)(size_t, size_t))
{
    for (size_t i = 0; i < _map_->capacity; i++)
    {
        bidimap_entry *entry = _map_->key_buffer[i];
        if (entry != ((void *)0) && entry != ((void *)1))
        {
            if (deallocator)
                deallocator(entry->key, entry->value);
            free(entry);
        }
        _map_->key_buffer[i] = ((void *)0);
        _map_->val_buffer[i] = ((void *)0);
    }
    _map_->count = 0;
}
void bm_free(bidimap *_map_, void (*deallocator)(size_t, size_t))
{
    bm_clear(_map_, deallocator);
    free(_map_->key_buffer);
    free(_map_->val_buffer);
    free(_map_);
}
_Bool bm_insert(bidimap *_map_, size_t key, size_t value)
{
    if (bm_full(_map_))
    {
        if (!bm_impl_grow(_map_))
            return 0;
    }
    if (bm_impl_get_entry_by_key(_map_, key) != ((void *)0) || bm_impl_get_entry_by_val(_map_, value) != ((void *)0))
        return 0;
    bidimap_entry *entry = bm_impl_new_entry(key, value);
    bidimap_entry **key_entry = bm_impl_add_entry_to_key(_map_, entry);
    bidimap_entry **val_entry = bm_impl_add_entry_to_val(_map_, entry);
    if (!key_entry || !val_entry)
    {
        if (key_entry)
            *key_entry = ((void *)1);
        if (val_entry)
            *val_entry = ((void *)1);
        free(entry);
        return 0;
    }
    _map_->count++;
    return 1;
}
_Bool bm_update_key(bidimap *_map_, size_t key, size_t *old_key)
{
    if (bm_empty(_map_))
        return 0;
    return 1;
}
_Bool bm_update_val(bidimap *_map_, size_t val, size_t *old_val)
{
    if (bm_empty(_map_))
        return 0;
    return 1;
}
_Bool bm_remove_by_key(bidimap *_map_, size_t key, size_t *out_value)
{
    if (bm_empty(_map_))
        return 0;
    return 1;
}
_Bool bm_remove_by_val(bidimap *_map_, size_t value, size_t *out_key)
{
    if (bm_empty(_map_))
        return 0;
    return 1;
}
size_t bm_get_key(bidimap *_map_, size_t val)
{
    bidimap_entry **entry = bm_impl_get_entry_by_val(_map_, val);
    if (!entry)
        return (size_t){0};
    return (*entry)->key;
}
size_t bm_get_val(bidimap *_map_, size_t key)
{
    bidimap_entry **entry = bm_impl_get_entry_by_key(_map_, key);
    if (!entry)
        return (size_t){0};
    return (*entry)->value;
}
_Bool bm_contains_key(bidimap *_map_, size_t key) { return bm_impl_get_entry_by_key(_map_, key) != ((void *)0); }
_Bool bm_contains_val(bidimap *_map_, size_t val) { return bm_impl_get_entry_by_val(_map_, val) != ((void *)0); }
_Bool bm_empty(bidimap *_map_) { return _map_->count == 0; }
_Bool bm_full(bidimap *_map_) { return (double)bm_capacity(_map_) * bm_load(_map_) <= (double)bm_count(_map_); }
size_t bm_count(bidimap *_map_) { return _map_->count; }
size_t bm_capacity(bidimap *_map_) { return _map_->capacity; }
double bm_load(bidimap *_map_) { return _map_->load; }
bidimap *bm_copy_of(bidimap *_map_, size_t (*key_copy_func)(size_t), size_t (*value_copy_func)(size_t)) { return ((void *)0); }
_Bool bm_equals(bidimap *_map1_, bidimap *_map2_) { return 0; }
struct cmc_string bm_to_string(bidimap *_map_)
{
    struct cmc_string str;
    bidimap *m_ = _map_;
    const char *name = "bidimap";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_bidimap, name, m_, m_->key_buffer, m_->val_buffer, m_->capacity, m_->count, m_->load, m_->key_cmp, m_->val_cmp, m_->key_hash, m_->val_hash);
    return str;
}
bidimap_iter *bm_iter_new(bidimap *target)
{
    bidimap_iter *iter = malloc(sizeof(bidimap_iter));
    if (!iter)
        return ((void *)0);
    bm_iter_init(iter, target);
    return iter;
}
void bm_iter_free(bidimap_iter *iter) { free(iter); }
void bm_iter_init(bidimap_iter *iter, bidimap *target)
{
    memset(iter, 0, sizeof(bidimap_iter));
    iter->target = target;
    iter->start = 1;
    iter->end = bm_empty(target);
    if (!bm_empty(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            bidimap_entry *tmp = target->key_buffer[i];
            if (tmp != ((void *)0) && tmp != ((void *)1))
            {
                iter->first = i;
                break;
            }
        }
        iter->cursor = iter->first;
        for (size_t i = target->capacity; i > 0; i--)
        {
            bidimap_entry *tmp = target->key_buffer[i - 1];
            if (tmp != ((void *)0) && tmp != ((void *)1))
            {
                iter->last = i - 1;
                break;
            }
        }
    }
}
_Bool bm_iter_start(bidimap_iter *iter) { return bm_empty(iter->target) || iter->start; }
_Bool bm_iter_end(bidimap_iter *iter) { return bm_empty(iter->target) || iter->end; }
void bm_iter_to_start(bidimap_iter *iter)
{
    if (!bm_empty(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = 1;
        iter->end = 0;
    }
}
void bm_iter_to_end(bidimap_iter *iter)
{
    if (!bm_empty(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = bm_count(iter->target) - 1;
        iter->start = 0;
        iter->end = 1;
    }
}
_Bool bm_iter_next(bidimap_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == bm_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    iter->start = bm_empty(iter->target);
    bidimap_entry *scan = iter->target->key_buffer[iter->cursor];
    iter->index++;
    while (1)
    {
        iter->cursor++;
        scan = iter->target->key_buffer[iter->cursor];
        if (scan != ((void *)0) && scan != ((void *)1))
            break;
    }
    return 1;
}
_Bool bm_iter_prev(bidimap_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = bm_empty(iter->target);
    bidimap_entry *scan = iter->target->key_buffer[iter->cursor];
    iter->index--;
    while (1)
    {
        iter->cursor--;
        scan = iter->target->key_buffer[iter->cursor];
        if (scan != ((void *)0) && scan != ((void *)1))
            break;
    }
    return 1;
}
_Bool bm_iter_advance(bidimap_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == bm_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= bm_count(iter->target))
        return 0;
    for (size_t i = 0; i < steps; i++)
        bm_iter_next(iter);
    return 1;
}
_Bool bm_iter_rewind(bidimap_iter *iter, size_t steps)
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
        bm_iter_prev(iter);
    return 1;
}
_Bool bm_iter_go_to(bidimap_iter *iter, size_t index)
{
    if (index >= bm_count(iter->target))
        return 0;
    if (iter->index > index)
        return bm_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return bm_iter_advance(iter, index - iter->index);
    return 1;
}
size_t bm_iter_key(bidimap_iter *iter)
{
    if (bm_empty(iter->target))
        return (size_t){0};
    return iter->target->key_buffer[iter->cursor]->key;
}
size_t bm_iter_value(bidimap_iter *iter)
{
    if (bm_empty(iter->target))
        return (size_t){0};
    return iter->target->key_buffer[iter->cursor]->value;
}
size_t bm_iter_index(bidimap_iter *iter) { return iter->index; }
static _Bool bm_impl_grow(bidimap *_map_) { return 0; }
static bidimap_entry *bm_impl_new_entry(size_t key, size_t value)
{
    bidimap_entry *entry = malloc(sizeof(bidimap_entry));
    if (!entry)
        return ((void *)0);
    entry->key = key;
    entry->value = value;
    entry->key_dist = 0;
    entry->val_dist = 0;
    return entry;
}
static bidimap_entry **bm_impl_get_entry_by_key(bidimap *_map_, size_t key)
{
    size_t hash = _map_->key_hash(key);
    size_t pos = hash % _map_->capacity;
    bidimap_entry *target = _map_->key_buffer[pos];
    while (target != ((void *)0))
    {
        if (target != ((void *)1) && _map_->key_cmp(target->key, key) == 0)
            return &(_map_->key_buffer[pos % _map_->capacity]);
        pos++;
        target = _map_->key_buffer[pos % _map_->capacity];
    }
    return ((void *)0);
}
static bidimap_entry **bm_impl_get_entry_by_val(bidimap *_map_, size_t val)
{
    size_t hash = _map_->val_hash(val);
    size_t pos = hash % _map_->capacity;
    bidimap_entry *target = _map_->val_buffer[pos];
    while (target != ((void *)0))
    {
        if (target != ((void *)1) && _map_->val_cmp(target->value, val) == 0)
            return &(_map_->val_buffer[pos % _map_->capacity]);
        pos++;
        target = _map_->val_buffer[pos % _map_->capacity];
    }
    return ((void *)0);
}
static bidimap_entry **bm_impl_add_entry_to_key(bidimap *_map_, bidimap_entry *entry)
{
    bidimap_entry **to_return = ((void *)0);
    size_t hash = _map_->key_hash(entry->key);
    size_t original_pos = hash % _map_->capacity;
    size_t pos = original_pos;
    bidimap_entry **scan = &(_map_->key_buffer[hash % _map_->capacity]);
    if (*scan == ((void *)0))
    {
        *scan = entry;
        return scan;
    }
    else
    {
        while (1)
        {
            pos++;
            scan = &(_map_->key_buffer[pos % _map_->capacity]);
            if (*scan == ((void *)0) || *scan == ((void *)1))
            {
                if (!to_return)
                    to_return = scan;
                *scan = entry;
                entry->key_dist = pos - original_pos;
                return to_return;
            }
            else if ((*scan)->key_dist < pos - original_pos)
            {
                if (!to_return)
                    to_return = scan;
                size_t tmp_dist = (*scan)->key_dist;
                entry->key_dist = pos - original_pos;
                original_pos = pos - tmp_dist;
                bidimap_entry *_tmp_ = *scan;
                *scan = entry;
                entry = _tmp_;
            }
        }
    }
    return ((void *)0);
}
static bidimap_entry **bm_impl_add_entry_to_val(bidimap *_map_, bidimap_entry *entry)
{
    bidimap_entry **to_return = ((void *)0);
    size_t hash = _map_->val_hash(entry->value);
    size_t original_pos = hash % _map_->capacity;
    size_t pos = original_pos;
    bidimap_entry **scan = &(_map_->val_buffer[hash % _map_->capacity]);
    if (*scan == ((void *)0))
    {
        *scan = entry;
        return scan;
    }
    else
    {
        while (1)
        {
            pos++;
            scan = &(_map_->val_buffer[pos % _map_->capacity]);
            if (*scan == ((void *)0) || *scan == ((void *)1))
            {
                if (!to_return)
                    to_return = scan;
                *scan = entry;
                entry->val_dist = pos - original_pos;
                return to_return;
            }
            else if ((*scan)->val_dist < pos - original_pos)
            {
                if (!to_return)
                    to_return = scan;
                size_t tmp_dist = (*scan)->val_dist;
                entry->val_dist = pos - original_pos;
                original_pos = pos - tmp_dist;
                bidimap_entry *_tmp_ = *scan;
                *scan = entry;
                entry = _tmp_;
            }
        }
    }
    return ((void *)0);
}
static size_t bm_impl_calculate_size(size_t required)
{
    const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);
    if (cmc_hashtable_primes[count - 1] < required)
        return required;
    size_t i = 0;
    while (cmc_hashtable_primes[i] < required)
        i++;
    return cmc_hashtable_primes[i];
}
static bidimap_iter bm_impl_it_start(bidimap *_map_)
{
    bidimap_iter iter;
    bm_iter_init(&iter, _map_);
    return iter;
}
static bidimap_iter bm_impl_it_end(bidimap *_map_)
{
    bidimap_iter iter;
    bm_iter_init(&iter, _map_);
    bm_iter_to_end(&iter);
    return iter;
}
