#include "cmc/multimap.h"

struct multimap
{
    struct multimap_entry *(*buffer)[2];
    size_t capacity;
    size_t count;
    double load;
    int flag;
    struct multimap_fkey *f_key;
    struct multimap_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
    struct multimap_iter (*it_start)(struct multimap *);
    struct multimap_iter (*it_end)(struct multimap *);
};
struct multimap_entry
{
    size_t key;
    size_t value;
    struct multimap_entry *next;
    struct multimap_entry *prev;
};
struct multimap_fkey
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct multimap_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct multimap_iter
{
    struct multimap *target;
    struct multimap_entry *curr_entry;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
};
struct multimap *mm_new(size_t capacity, double load,
                        struct multimap_fkey *f_key,
                        struct multimap_fval *f_val);
struct multimap *mm_new_custom(size_t capacity, double load,
                               struct multimap_fkey *f_key,
                               struct multimap_fval *f_val,
                               struct cmc_alloc_node *alloc,
                               struct cmc_callbacks *callbacks);
void mm_clear(struct multimap *_map_);
void mm_free(struct multimap *_map_);
void mm_customize(struct multimap *_map_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks);
_Bool mm_insert(struct multimap *_map_, size_t key, size_t value);
_Bool mm_update(struct multimap *_map_, size_t key, size_t new_value,
                size_t *old_value);
size_t mm_update_all(struct multimap *_map_, size_t key, size_t new_value,
                     size_t **old_values);
_Bool mm_remove(struct multimap *_map_, size_t key, size_t *out_value);
size_t mm_remove_all(struct multimap *_map_, size_t key, size_t **out_values);
_Bool mm_max(struct multimap *_map_, size_t *key, size_t *value);
_Bool mm_min(struct multimap *_map_, size_t *key, size_t *value);
size_t mm_get(struct multimap *_map_, size_t key);
size_t *mm_get_ref(struct multimap *_map_, size_t key);
_Bool mm_contains(struct multimap *_map_, size_t key);
_Bool mm_empty(struct multimap *_map_);
_Bool mm_full(struct multimap *_map_);
size_t mm_count(struct multimap *_map_);
size_t mm_key_count(struct multimap *_map_, size_t key);
size_t mm_capacity(struct multimap *_map_);
double mm_load(struct multimap *_map_);
int mm_flag(struct multimap *_map_);
_Bool mm_resize(struct multimap *_map_, size_t capacity);
struct multimap *mm_copy_of(struct multimap *_map_);
_Bool mm_equals(struct multimap *_map1_, struct multimap *_map2_);
struct cmc_string mm_to_string(struct multimap *_map_);
_Bool mm_print(struct multimap *_map_, FILE *fptr);
struct multimap_iter *mm_iter_new(struct multimap *target);
void mm_iter_free(struct multimap_iter *iter);
void mm_iter_init(struct multimap_iter *iter, struct multimap *target);
_Bool mm_iter_start(struct multimap_iter *iter);
_Bool mm_iter_end(struct multimap_iter *iter);
void mm_iter_to_start(struct multimap_iter *iter);
void mm_iter_to_end(struct multimap_iter *iter);
_Bool mm_iter_next(struct multimap_iter *iter);
_Bool mm_iter_prev(struct multimap_iter *iter);
_Bool mm_iter_advance(struct multimap_iter *iter, size_t steps);
_Bool mm_iter_rewind(struct multimap_iter *iter, size_t steps);
_Bool mm_iter_go_to(struct multimap_iter *iter, size_t index);
size_t mm_iter_key(struct multimap_iter *iter);
size_t mm_iter_value(struct multimap_iter *iter);
size_t *mm_iter_rvalue(struct multimap_iter *iter);
size_t mm_iter_index(struct multimap_iter *iter);
struct multimap_entry *mm_impl_new_entry(struct multimap *_map_, size_t key,
                                         size_t value);
struct multimap_entry *mm_impl_get_entry(struct multimap *_map_, size_t key);
size_t mm_impl_key_count(struct multimap *_map_, size_t key);
size_t mm_impl_calculate_size(size_t required);
static struct multimap_iter mm_impl_it_start(struct multimap *_map_);
static struct multimap_iter mm_impl_it_end(struct multimap *_map_);
struct multimap *mm_new(size_t capacity, double load,
                        struct multimap_fkey *f_key,
                        struct multimap_fval *f_val)
{
    struct cmc_alloc_node *alloc = &cmc_alloc_node_default;
    if (capacity == 0 || load <= 0)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffffULL * load)
        return ((void *)0);
    if (!f_key || !f_val)
        return ((void *)0);
    size_t real_capacity = mm_impl_calculate_size(capacity / load);
    struct multimap *_map_ = alloc->malloc(sizeof(struct multimap));
    if (!_map_)
        return ((void *)0);
    _map_->buffer =
        alloc->calloc(real_capacity, sizeof(struct multimap_entry *[2]));
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
    _map_->it_end = mm_impl_it_end;
    _map_->it_start = mm_impl_it_start;
    return _map_;
}
struct multimap *mm_new_custom(size_t capacity, double load,
                               struct multimap_fkey *f_key,
                               struct multimap_fval *f_val,
                               struct cmc_alloc_node *alloc,
                               struct cmc_callbacks *callbacks)
{
    if (capacity == 0 || load <= 0)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffffULL * load)
        return ((void *)0);
    if (!f_key || !f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    size_t real_capacity = mm_impl_calculate_size(capacity / load);
    struct multimap *_map_ = alloc->malloc(sizeof(struct multimap));
    if (!_map_)
        return ((void *)0);
    _map_->buffer =
        alloc->calloc(real_capacity, sizeof(struct multimap_entry *[2]));
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
    _map_->it_start = mm_impl_it_start;
    _map_->it_end = mm_impl_it_end;
    return _map_;
}
void mm_clear(struct multimap *_map_)
{
    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct multimap_entry *scan = _map_->buffer[i][0];
        while (scan != ((void *)0))
        {
            struct multimap_entry *next = scan->next;
            if (_map_->f_key->free)
                _map_->f_key->free(scan->key);
            if (_map_->f_val->free)
                _map_->f_val->free(scan->value);
            _map_->alloc->free(scan);
            scan = next;
        }
    }
    memset(_map_->buffer, 0,
           sizeof(struct multimap_entry *[2]) * _map_->capacity);
    _map_->count = 0;
    _map_->flag = cmc_flags.OK;
}
void mm_free(struct multimap *_map_)
{
    size_t index = 0;
    while (index < _map_->capacity)
    {
        struct multimap_entry *scan = _map_->buffer[index][0];
        if (scan != ((void *)0))
        {
            if (scan->next == ((void *)0) && scan->prev == ((void *)0))
            {
                if (_map_->f_key->free)
                    _map_->f_key->free(scan->key);
                if (_map_->f_val->free)
                    _map_->f_val->free(scan->value);
                _map_->alloc->free(scan);
            }
            else
            {
                while (scan != ((void *)0))
                {
                    struct multimap_entry *tmp = scan;
                    scan = scan->next;
                    if (_map_->f_key->free)
                        _map_->f_key->free(tmp->key);
                    if (_map_->f_val->free)
                        _map_->f_val->free(tmp->value);
                    _map_->alloc->free(tmp);
                }
            }
        }
        index++;
    }
    _map_->alloc->free(_map_->buffer);
    _map_->alloc->free(_map_);
}
void mm_customize(struct multimap *_map_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _map_->alloc = &cmc_alloc_node_default;
    else
        _map_->alloc = alloc;
    _map_->callbacks = callbacks;
    _map_->flag = cmc_flags.OK;
}
_Bool mm_insert(struct multimap *_map_, size_t key, size_t value)
{
    if (mm_full(_map_))
    {
        if (!mm_resize(_map_, _map_->capacity + 1))
            return 0;
    }
    size_t hash = _map_->f_key->hash(key);
    size_t pos = hash % _map_->capacity;
    struct multimap_entry *entry = mm_impl_new_entry(_map_, key, value);
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
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->create)
        _map_->callbacks->create();
    return 1;
}
_Bool mm_update(struct multimap *_map_, size_t key, size_t new_value,
                size_t *old_value)
{
    if (mm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct multimap_entry *entry = mm_impl_get_entry(_map_, key);
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
size_t mm_update_all(struct multimap *_map_, size_t key, size_t new_value,
                     size_t **old_values)
{
    if (mm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    size_t hash = _map_->f_key->hash(key);
    struct multimap_entry *entry = _map_->buffer[hash % _map_->capacity][0];
    if (entry == ((void *)0))
    {
        _map_->flag = cmc_flags.NOT_FOUND;
        return 0;
    }
    if (old_values)
    {
        size_t total = mm_impl_key_count(_map_, key);
        if (total == 0)
        {
            _map_->flag = cmc_flags.NOT_FOUND;
            return 0;
        }
        *old_values = _map_->alloc->malloc(sizeof(size_t) * total);
        if (!(*old_values))
        {
            _map_->flag = cmc_flags.ALLOC;
            return 0;
        }
    }
    size_t index = 0;
    while (entry != ((void *)0))
    {
        if (_map_->f_key->cmp(entry->key, key) == 0)
        {
            if (old_values)
                (*old_values)[index] = entry->value;
            index++;
            entry->value = new_value;
        }
        entry = entry->next;
    }
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->update)
        _map_->callbacks->update();
    return index;
}
_Bool mm_remove(struct multimap *_map_, size_t key, size_t *out_value)
{
    if (mm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    size_t hash = _map_->f_key->hash(key);
    struct multimap_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);
    struct multimap_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);
    if (*head == ((void *)0))
    {
        _map_->flag = cmc_flags.NOT_FOUND;
        return 0;
    }
    struct multimap_entry *entry = *head;
    if (entry->next == ((void *)0) && entry->prev == ((void *)0))
    {
        if (_map_->f_key->cmp(entry->key, key) == 0)
        {
            *head = ((void *)0);
            *tail = ((void *)0);
            if (out_value)
                *out_value = entry->value;
        }
        else
        {
            _map_->flag = cmc_flags.NOT_FOUND;
            return 0;
        }
    }
    else
    {
        _Bool found = 0;
        while (entry != ((void *)0))
        {
            if (_map_->f_key->cmp(entry->key, key) == 0)
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
        {
            _map_->flag = cmc_flags.NOT_FOUND;
            return 0;
        }
    }
    _map_->alloc->free(entry);
    _map_->count--;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->delete)
        _map_->callbacks->delete ();
    return 1;
}
size_t mm_remove_all(struct multimap *_map_, size_t key, size_t **out_values)
{
    if (mm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    size_t hash = _map_->f_key->hash(key);
    struct multimap_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);
    struct multimap_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);
    if (*head == ((void *)0))
    {
        _map_->flag = cmc_flags.NOT_FOUND;
        return 0;
    }
    if (out_values)
    {
        size_t total = mm_impl_key_count(_map_, key);
        if (total == 0)
        {
            _map_->flag = cmc_flags.NOT_FOUND;
            return 0;
        }
        *out_values = _map_->alloc->malloc(sizeof(size_t) * total);
        if (!(*out_values))
        {
            _map_->flag = cmc_flags.ALLOC;
            return 0;
        }
    }
    size_t index = 0;
    struct multimap_entry *entry = *head;
    if (entry->next == ((void *)0))
    {
        *head = ((void *)0);
        *tail = ((void *)0);
        if (out_values)
            (*out_values)[index] = entry->value;
        index++;
        _map_->alloc->free(entry);
    }
    else
    {
        while (entry != ((void *)0))
        {
            if (_map_->f_key->cmp(entry->key, key) == 0)
            {
                if (*head == entry)
                    *head = entry->next;
                if (*tail == entry)
                    *tail = entry->prev;
                struct multimap_entry *next = entry->next;
                if (entry->prev != ((void *)0))
                    entry->prev->next = entry->next;
                if (entry->next != ((void *)0))
                    entry->next->prev = entry->prev;
                if (out_values)
                    (*out_values)[index] = entry->value;
                index++;
                _map_->alloc->free(entry);
                entry = next;
            }
            else
                entry = entry->next;
        }
    }
    _map_->count -= index;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->delete)
        _map_->callbacks->delete ();
    return index;
}
_Bool mm_max(struct multimap *_map_, size_t *key, size_t *value)
{
    if (mm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    size_t max_key;
    size_t max_val;
    struct multimap_iter iter;
    for (mm_iter_init(&iter, _map_); !mm_iter_end(&iter); mm_iter_next(&iter))
    {
        size_t result_key = mm_iter_key(&iter);
        size_t result_value = mm_iter_value(&iter);
        size_t index = mm_iter_index(&iter);
        if (index == 0)
        {
            max_key = result_key;
            max_val = result_value;
        }
        else if (_map_->f_key->cmp(result_key, max_key) > 0)
        {
            max_key = result_key;
            max_val = result_value;
        }
    }
    if (key)
        *key = max_key;
    if (value)
        *value = max_val;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return 1;
}
_Bool mm_min(struct multimap *_map_, size_t *key, size_t *value)
{
    if (mm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    size_t min_key;
    size_t min_val;
    struct multimap_iter iter;
    for (mm_iter_init(&iter, _map_); !mm_iter_end(&iter); mm_iter_next(&iter))
    {
        size_t result_key = mm_iter_key(&iter);
        size_t result_value = mm_iter_value(&iter);
        size_t index = mm_iter_index(&iter);
        if (index == 0)
        {
            min_key = result_key;
            min_val = result_value;
        }
        else if (_map_->f_key->cmp(result_key, min_key) < 0)
        {
            min_key = result_key;
            min_val = result_value;
        }
    }
    if (key)
        *key = min_key;
    if (value)
        *value = min_val;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return 1;
}
size_t mm_get(struct multimap *_map_, size_t key)
{
    if (mm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct multimap_entry *entry = mm_impl_get_entry(_map_, key);
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
size_t *mm_get_ref(struct multimap *_map_, size_t key)
{
    if (mm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct multimap_entry *entry = mm_impl_get_entry(_map_, key);
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
_Bool mm_contains(struct multimap *_map_, size_t key)
{
    _map_->flag = cmc_flags.OK;
    _Bool result = mm_impl_get_entry(_map_, key) != ((void *)0);
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return result;
}
_Bool mm_empty(struct multimap *_map_)
{
    return _map_->count == 0;
}
_Bool mm_full(struct multimap *_map_)
{
    return (double)_map_->capacity * _map_->load <= (double)_map_->count;
}
size_t mm_count(struct multimap *_map_)
{
    return _map_->count;
}
size_t mm_key_count(struct multimap *_map_, size_t key)
{
    return mm_impl_key_count(_map_, key);
}
size_t mm_capacity(struct multimap *_map_)
{
    return _map_->capacity;
}
double mm_load(struct multimap *_map_)
{
    return _map_->load;
}
int mm_flag(struct multimap *_map_)
{
    return _map_->flag;
}
_Bool mm_resize(struct multimap *_map_, size_t capacity)
{
    _map_->flag = cmc_flags.OK;
    if (_map_->capacity == capacity)
        goto success;
    if (_map_->capacity > capacity / _map_->load)
        goto success;
    if (capacity >= 0xffffffffffffffffULL * _map_->load)
    {
        _map_->flag = cmc_flags.ERROR;
        return 0;
    }
    size_t theoretical_size = mm_impl_calculate_size(capacity);
    if (theoretical_size < _map_->count / _map_->load)
    {
        _map_->flag = cmc_flags.INVALID;
        return 0;
    }
    struct multimap *_new_map_ =
        mm_new_custom(capacity, _map_->load, _map_->f_key, _map_->f_val,
                      _map_->alloc, ((void *)0));
    if (!_new_map_)
    {
        _map_->flag = cmc_flags.ALLOC;
        return 0;
    }
    struct multimap_iter iter;
    for (mm_iter_init(&iter, _map_); !mm_iter_end(&iter); mm_iter_next(&iter))
    {
        size_t key = mm_iter_key(&iter);
        size_t value = mm_iter_value(&iter);
        mm_insert(_new_map_, key, value);
    }
    if (_map_->count != _new_map_->count)
    {
        mm_free(_new_map_);
        _map_->flag = cmc_flags.ERROR;
        return 0;
    }
    struct multimap_entry *(*tmp_b)[2] = _map_->buffer;
    _map_->buffer = _new_map_->buffer;
    _new_map_->buffer = tmp_b;
    size_t tmp_c = _map_->capacity;
    _map_->capacity = _new_map_->capacity;
    _new_map_->capacity = tmp_c;
    mm_free(_new_map_);
success:
    if (_map_->callbacks && _map_->callbacks->resize)
        _map_->callbacks->resize();
    return 1;
}
struct multimap *mm_copy_of(struct multimap *_map_)
{
    struct multimap *result =
        mm_new_custom(_map_->capacity * _map_->load, _map_->load, _map_->f_key,
                      _map_->f_val, _map_->alloc, ((void *)0));
    if (!result)
    {
        _map_->flag = cmc_flags.ERROR;
        return ((void *)0);
    }
    struct multimap_iter iter;
    mm_iter_init(&iter, _map_);
    if (!mm_empty(_map_))
    {
        for (mm_iter_to_start(&iter); !mm_iter_end(&iter); mm_iter_next(&iter))
        {
            size_t key = mm_iter_key(&iter);
            size_t value = mm_iter_value(&iter);
            if (_map_->f_key->cpy)
                key = _map_->f_key->cpy(key);
            if (_map_->f_val->cpy)
                value = _map_->f_val->cpy(value);
            mm_insert(result, key, value);
        }
    }
    result->callbacks = _map_->callbacks;
    _map_->flag = cmc_flags.OK;
    return result;
}
_Bool mm_equals(struct multimap *_map1_, struct multimap *_map2_)
{
    _map1_->flag = cmc_flags.OK;
    _map2_->flag = cmc_flags.OK;
    if (_map1_->count != _map2_->count)
        return 0;
    struct multimap_iter iter;
    mm_iter_init(&iter, _map1_);
    for (mm_iter_to_start(&iter); !mm_iter_end(&iter); mm_iter_next(&iter))
    {
        size_t key = mm_iter_key(&iter);
        if (mm_impl_key_count(_map1_, key) != mm_impl_key_count(_map2_, key))
            return 0;
    }
    return 1;
}
struct cmc_string mm_to_string(struct multimap *_map_)
{
    struct cmc_string str;
    struct multimap *m_ = _map_;
    int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_multimap, "multimap",
                     "size_t", "size_t", m_, m_->buffer, m_->capacity,
                     m_->count, m_->load, m_->flag, m_->f_key, m_->f_val,
                     m_->alloc, m_->callbacks);
    return n >= 0 ? str : (struct cmc_string){ 0 };
}
_Bool mm_print(struct multimap *_map_, FILE *fptr)
{
    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct multimap_entry *scan = _map_->buffer[i][0];
        while (scan != ((void *)0))
        {
            if (!_map_->f_key->str(fptr, scan->key) ||
                !_map_->f_val->str(fptr, scan->value))
                return 0;
            scan = scan->next;
        }
    }
    return 1;
}
struct multimap_iter *mm_iter_new(struct multimap *target)
{
    struct multimap_iter *iter =
        target->alloc->malloc(sizeof(struct multimap_iter));
    if (!iter)
        return ((void *)0);
    mm_iter_init(iter, target);
    return iter;
}
void mm_iter_free(struct multimap_iter *iter)
{
    iter->target->alloc->free(iter);
}
void mm_iter_init(struct multimap_iter *iter, struct multimap *target)
{
    memset(iter, 0, sizeof(struct multimap_iter));
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
_Bool mm_iter_start(struct multimap_iter *iter)
{
    return mm_empty(iter->target) || iter->start;
}
_Bool mm_iter_end(struct multimap_iter *iter)
{
    return mm_empty(iter->target) || iter->end;
}
void mm_iter_to_start(struct multimap_iter *iter)
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
void mm_iter_to_end(struct multimap_iter *iter)
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
_Bool mm_iter_next(struct multimap_iter *iter)
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
_Bool mm_iter_prev(struct multimap_iter *iter)
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
_Bool mm_iter_advance(struct multimap_iter *iter, size_t steps)
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
        mm_iter_next(iter);
    return 1;
}
_Bool mm_iter_rewind(struct multimap_iter *iter, size_t steps)
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
_Bool mm_iter_go_to(struct multimap_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return mm_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return mm_iter_advance(iter, index - iter->index);
    return 1;
}
size_t mm_iter_key(struct multimap_iter *iter)
{
    if (mm_empty(iter->target))
        return (size_t){ 0 };
    return iter->curr_entry->key;
}
size_t mm_iter_value(struct multimap_iter *iter)
{
    if (mm_empty(iter->target))
        return (size_t){ 0 };
    return iter->curr_entry->value;
}
size_t *mm_iter_rvalue(struct multimap_iter *iter)
{
    if (mm_empty(iter->target))
        return ((void *)0);
    return &(iter->curr_entry->value);
}
size_t mm_iter_index(struct multimap_iter *iter)
{
    return iter->index;
}
struct multimap_entry *mm_impl_new_entry(struct multimap *_map_, size_t key,
                                         size_t value)
{
    struct multimap_entry *entry =
        _map_->alloc->malloc(sizeof(struct multimap_entry));
    if (!entry)
        return ((void *)0);
    entry->key = key;
    entry->value = value;
    entry->next = ((void *)0);
    entry->prev = ((void *)0);
    return entry;
}
struct multimap_entry *mm_impl_get_entry(struct multimap *_map_, size_t key)
{
    size_t hash = _map_->f_key->hash(key);
    struct multimap_entry *entry = _map_->buffer[hash % _map_->capacity][0];
    while (entry != ((void *)0))
    {
        if (_map_->f_key->cmp(entry->key, key) == 0)
            return entry;
        entry = entry->next;
    }
    return ((void *)0);
}
size_t mm_impl_key_count(struct multimap *_map_, size_t key)
{
    size_t hash = _map_->f_key->hash(key);
    struct multimap_entry *entry = _map_->buffer[hash % _map_->capacity][0];
    size_t total_count = 0;
    if (!entry)
        return total_count;
    while (entry != ((void *)0))
    {
        if (_map_->f_key->cmp(entry->key, key) == 0)
            total_count++;
        entry = entry->next;
    }
    return total_count;
}
size_t mm_impl_calculate_size(size_t required)
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
static struct multimap_iter mm_impl_it_start(struct multimap *_map_)
{
    struct multimap_iter iter;
    mm_iter_init(&iter, _map_);
    return iter;
}
static struct multimap_iter mm_impl_it_end(struct multimap *_map_)
{
    struct multimap_iter iter;
    mm_iter_init(&iter, _map_);
    mm_iter_to_end(&iter);
    return iter;
}
