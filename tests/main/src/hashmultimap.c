#ifndef CMC_TEST_SRC_HASHMULTIMAP
#define CMC_TEST_SRC_HASHMULTIMAP

#include "cmc/hashmultimap.h"

struct hashmultimap
{
    struct hashmultimap_entry *(*buffer)[2];
    size_t capacity;
    size_t count;
    double load;
    int flag;
    struct hashmultimap_fkey *f_key;
    struct hashmultimap_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct hashmultimap_entry
{
    size_t key;
    size_t value;
    struct hashmultimap_entry *next;
    struct hashmultimap_entry *prev;
};
struct hashmultimap_fkey
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashmultimap_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashmultimap *hmm_new(size_t capacity, double load, struct hashmultimap_fkey *f_key,
                             struct hashmultimap_fval *f_val);
struct hashmultimap *hmm_new_custom(size_t capacity, double load, struct hashmultimap_fkey *f_key,
                                    struct hashmultimap_fval *f_val, struct cmc_alloc_node *alloc,
                                    struct cmc_callbacks *callbacks);
void hmm_clear(struct hashmultimap *_map_);
void hmm_free(struct hashmultimap *_map_);
void hmm_customize(struct hashmultimap *_map_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool hmm_insert(struct hashmultimap *_map_, size_t key, size_t value);
_Bool hmm_update(struct hashmultimap *_map_, size_t key, size_t new_value, size_t *old_value);
size_t hmm_update_all(struct hashmultimap *_map_, size_t key, size_t new_value, size_t **old_values);
_Bool hmm_remove(struct hashmultimap *_map_, size_t key, size_t *out_value);
size_t hmm_remove_all(struct hashmultimap *_map_, size_t key, size_t **out_values);
_Bool hmm_max(struct hashmultimap *_map_, size_t *key, size_t *value);
_Bool hmm_min(struct hashmultimap *_map_, size_t *key, size_t *value);
size_t hmm_get(struct hashmultimap *_map_, size_t key);
size_t *hmm_get_ref(struct hashmultimap *_map_, size_t key);
_Bool hmm_contains(struct hashmultimap *_map_, size_t key);
_Bool hmm_empty(struct hashmultimap *_map_);
_Bool hmm_full(struct hashmultimap *_map_);
size_t hmm_count(struct hashmultimap *_map_);
size_t hmm_key_count(struct hashmultimap *_map_, size_t key);
size_t hmm_capacity(struct hashmultimap *_map_);
double hmm_load(struct hashmultimap *_map_);
int hmm_flag(struct hashmultimap *_map_);
_Bool hmm_resize(struct hashmultimap *_map_, size_t capacity);
struct hashmultimap *hmm_copy_of(struct hashmultimap *_map_);
_Bool hmm_equals(struct hashmultimap *_map1_, struct hashmultimap *_map2_);
struct hashmultimap_iter
{
    struct hashmultimap *target;
    struct hashmultimap_entry *curr_entry;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
};
struct hashmultimap_iter hmm_iter_start(struct hashmultimap *target);
struct hashmultimap_iter hmm_iter_end(struct hashmultimap *target);
_Bool hmm_iter_at_start(struct hashmultimap_iter *iter);
_Bool hmm_iter_at_end(struct hashmultimap_iter *iter);
_Bool hmm_iter_to_start(struct hashmultimap_iter *iter);
_Bool hmm_iter_to_end(struct hashmultimap_iter *iter);
_Bool hmm_iter_next(struct hashmultimap_iter *iter);
_Bool hmm_iter_prev(struct hashmultimap_iter *iter);
_Bool hmm_iter_advance(struct hashmultimap_iter *iter, size_t steps);
_Bool hmm_iter_rewind(struct hashmultimap_iter *iter, size_t steps);
_Bool hmm_iter_go_to(struct hashmultimap_iter *iter, size_t index);
size_t hmm_iter_key(struct hashmultimap_iter *iter);
size_t hmm_iter_value(struct hashmultimap_iter *iter);
size_t *hmm_iter_rvalue(struct hashmultimap_iter *iter);
size_t hmm_iter_index(struct hashmultimap_iter *iter);
_Bool hmm_to_string(struct hashmultimap *_map_, FILE *fptr);
_Bool hmm_print(struct hashmultimap *_map_, FILE *fptr, const char *start, const char *separator, const char *end,
                const char *key_val_sep);
struct hashmultimap_entry *hmm_impl_new_entry(struct hashmultimap *_map_, size_t key, size_t value);
struct hashmultimap_entry *hmm_impl_get_entry(struct hashmultimap *_map_, size_t key);
size_t hmm_impl_key_count(struct hashmultimap *_map_, size_t key);
size_t hmm_impl_calculate_size(size_t required);
struct hashmultimap *hmm_new(size_t capacity, double load, struct hashmultimap_fkey *f_key,
                             struct hashmultimap_fval *f_val)
{
    return hmm_new_custom(capacity, load, f_key, f_val, ((void *)0), ((void *)0));
}
struct hashmultimap *hmm_new_custom(size_t capacity, double load, struct hashmultimap_fkey *f_key,
                                    struct hashmultimap_fval *f_val, struct cmc_alloc_node *alloc,
                                    struct cmc_callbacks *callbacks)
{
    ;
    if (capacity == 0 || load <= 0)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffffULL * load)
        return ((void *)0);
    if (!f_key || !f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    size_t real_capacity = hmm_impl_calculate_size(capacity / load);
    struct hashmultimap *_map_ = alloc->malloc(sizeof(struct hashmultimap));
    if (!_map_)
        return ((void *)0);
    _map_->buffer = alloc->calloc(real_capacity, sizeof(struct hashmultimap_entry *[2]));
    if (!_map_->buffer)
    {
        alloc->free(_map_);
        return ((void *)0);
    }
    _map_->count = 0;
    _map_->capacity = real_capacity;
    _map_->load = load;
    _map_->flag = CMC_FLAG_OK;
    _map_->f_key = f_key;
    _map_->f_val = f_val;
    _map_->alloc = alloc;
    (_map_)->callbacks = callbacks;
    return _map_;
}
void hmm_clear(struct hashmultimap *_map_)
{
    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct hashmultimap_entry *scan = _map_->buffer[i][0];
        while (scan != ((void *)0))
        {
            struct hashmultimap_entry *next = scan->next;
            if (_map_->f_key->free)
                _map_->f_key->free(scan->key);
            if (_map_->f_val->free)
                _map_->f_val->free(scan->value);
            _map_->alloc->free(scan);
            scan = next;
        }
    }
    memset(_map_->buffer, 0, sizeof(struct hashmultimap_entry *[2]) * _map_->capacity);
    _map_->count = 0;
    _map_->flag = CMC_FLAG_OK;
}
void hmm_free(struct hashmultimap *_map_)
{
    size_t index = 0;
    while (index < _map_->capacity)
    {
        struct hashmultimap_entry *scan = _map_->buffer[index][0];
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
                    struct hashmultimap_entry *tmp = scan;
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
void hmm_customize(struct hashmultimap *_map_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    ;
    if (!alloc)
        _map_->alloc = &cmc_alloc_node_default;
    else
        _map_->alloc = alloc;
    (_map_)->callbacks = callbacks;
    _map_->flag = CMC_FLAG_OK;
}
_Bool hmm_insert(struct hashmultimap *_map_, size_t key, size_t value)
{
    if (hmm_full(_map_))
    {
        if (!hmm_resize(_map_, _map_->capacity + 1))
            return 0;
    }
    size_t hash = _map_->f_key->hash(key);
    size_t pos = hash % _map_->capacity;
    struct hashmultimap_entry *entry = hmm_impl_new_entry(_map_, key, value);
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
    _map_->flag = CMC_FLAG_OK;
    if ((_map_)->callbacks && (_map_)->callbacks->create)
        (_map_)->callbacks->create();
    ;
    return 1;
}
_Bool hmm_update(struct hashmultimap *_map_, size_t key, size_t new_value, size_t *old_value)
{
    if (hmm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct hashmultimap_entry *entry = hmm_impl_get_entry(_map_, key);
    if (!entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    if (old_value)
        *old_value = entry->value;
    entry->value = new_value;
    _map_->flag = CMC_FLAG_OK;
    if ((_map_)->callbacks && (_map_)->callbacks->update)
        (_map_)->callbacks->update();
    ;
    return 1;
}
size_t hmm_update_all(struct hashmultimap *_map_, size_t key, size_t new_value, size_t **old_values)
{
    if (hmm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    size_t hash = _map_->f_key->hash(key);
    struct hashmultimap_entry *entry = _map_->buffer[hash % _map_->capacity][0];
    if (entry == ((void *)0))
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    if (old_values)
    {
        size_t total = hmm_impl_key_count(_map_, key);
        if (total == 0)
        {
            _map_->flag = CMC_FLAG_NOT_FOUND;
            return 0;
        }
        *old_values = _map_->alloc->malloc(sizeof(size_t) * total);
        if (!(*old_values))
        {
            _map_->flag = CMC_FLAG_ALLOC;
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
    _map_->flag = CMC_FLAG_OK;
    if ((_map_)->callbacks && (_map_)->callbacks->update)
        (_map_)->callbacks->update();
    ;
    return index;
}
_Bool hmm_remove(struct hashmultimap *_map_, size_t key, size_t *out_value)
{
    if (hmm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    size_t hash = _map_->f_key->hash(key);
    struct hashmultimap_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);
    struct hashmultimap_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);
    if (*head == ((void *)0))
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    struct hashmultimap_entry *entry = *head;
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
            _map_->flag = CMC_FLAG_NOT_FOUND;
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
            _map_->flag = CMC_FLAG_NOT_FOUND;
            return 0;
        }
    }
    _map_->alloc->free(entry);
    _map_->count--;
    _map_->flag = CMC_FLAG_OK;
    if ((_map_)->callbacks && (_map_)->callbacks->delete)
        (_map_)->callbacks->delete ();
    ;
    return 1;
}
size_t hmm_remove_all(struct hashmultimap *_map_, size_t key, size_t **out_values)
{
    if (hmm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    size_t hash = _map_->f_key->hash(key);
    struct hashmultimap_entry **head = &(_map_->buffer[hash % _map_->capacity][0]);
    struct hashmultimap_entry **tail = &(_map_->buffer[hash % _map_->capacity][1]);
    if (*head == ((void *)0))
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    if (out_values)
    {
        size_t total = hmm_impl_key_count(_map_, key);
        if (total == 0)
        {
            _map_->flag = CMC_FLAG_NOT_FOUND;
            return 0;
        }
        *out_values = _map_->alloc->malloc(sizeof(size_t) * total);
        if (!(*out_values))
        {
            _map_->flag = CMC_FLAG_ALLOC;
            return 0;
        }
    }
    size_t index = 0;
    struct hashmultimap_entry *entry = *head;
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
                struct hashmultimap_entry *next = entry->next;
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
    _map_->flag = CMC_FLAG_OK;
    if ((_map_)->callbacks && (_map_)->callbacks->delete)
        (_map_)->callbacks->delete ();
    ;
    return index;
}
_Bool hmm_max(struct hashmultimap *_map_, size_t *key, size_t *value)
{
    if (hmm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    size_t max_key;
    size_t max_val;
    struct hashmultimap_iter iter = hmm_iter_start(_map_);
    for (; !hmm_iter_at_end(&iter); hmm_iter_next(&iter))
    {
        size_t result_key = hmm_iter_key(&iter);
        size_t result_value = hmm_iter_value(&iter);
        size_t index = hmm_iter_index(&iter);
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
    _map_->flag = CMC_FLAG_OK;
    if ((_map_)->callbacks && (_map_)->callbacks->read)
        (_map_)->callbacks->read();
    ;
    return 1;
}
_Bool hmm_min(struct hashmultimap *_map_, size_t *key, size_t *value)
{
    if (hmm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    size_t min_key;
    size_t min_val;
    struct hashmultimap_iter iter = hmm_iter_start(_map_);
    for (; !hmm_iter_at_end(&iter); hmm_iter_next(&iter))
    {
        size_t result_key = hmm_iter_key(&iter);
        size_t result_value = hmm_iter_value(&iter);
        size_t index = hmm_iter_index(&iter);
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
    _map_->flag = CMC_FLAG_OK;
    if ((_map_)->callbacks && (_map_)->callbacks->read)
        (_map_)->callbacks->read();
    ;
    return 1;
}
size_t hmm_get(struct hashmultimap *_map_, size_t key)
{
    if (hmm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct hashmultimap_entry *entry = hmm_impl_get_entry(_map_, key);
    if (!entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return (size_t){ 0 };
    }
    _map_->flag = CMC_FLAG_OK;
    if ((_map_)->callbacks && (_map_)->callbacks->read)
        (_map_)->callbacks->read();
    ;
    return entry->value;
}
size_t *hmm_get_ref(struct hashmultimap *_map_, size_t key)
{
    if (hmm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct hashmultimap_entry *entry = hmm_impl_get_entry(_map_, key);
    if (!entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return ((void *)0);
    }
    _map_->flag = CMC_FLAG_OK;
    if ((_map_)->callbacks && (_map_)->callbacks->read)
        (_map_)->callbacks->read();
    ;
    return &(entry->value);
}
_Bool hmm_contains(struct hashmultimap *_map_, size_t key)
{
    _map_->flag = CMC_FLAG_OK;
    _Bool result = hmm_impl_get_entry(_map_, key) != ((void *)0);
    if ((_map_)->callbacks && (_map_)->callbacks->read)
        (_map_)->callbacks->read();
    ;
    return result;
}
_Bool hmm_empty(struct hashmultimap *_map_)
{
    return _map_->count == 0;
}
_Bool hmm_full(struct hashmultimap *_map_)
{
    return (double)_map_->capacity * _map_->load <= (double)_map_->count;
}
size_t hmm_count(struct hashmultimap *_map_)
{
    return _map_->count;
}
size_t hmm_key_count(struct hashmultimap *_map_, size_t key)
{
    return hmm_impl_key_count(_map_, key);
}
size_t hmm_capacity(struct hashmultimap *_map_)
{
    return _map_->capacity;
}
double hmm_load(struct hashmultimap *_map_)
{
    return _map_->load;
}
int hmm_flag(struct hashmultimap *_map_)
{
    return _map_->flag;
}
_Bool hmm_resize(struct hashmultimap *_map_, size_t capacity)
{
    _map_->flag = CMC_FLAG_OK;
    if (_map_->capacity == capacity)
        goto success;
    if (_map_->capacity > capacity / _map_->load)
        goto success;
    if (capacity >= 0xffffffffffffffffULL * _map_->load)
    {
        _map_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    size_t theoretical_size = hmm_impl_calculate_size(capacity);
    if (theoretical_size < _map_->count / _map_->load)
    {
        _map_->flag = CMC_FLAG_INVALID;
        return 0;
    }
    struct hashmultimap *_new_map_ =
        hmm_new_custom(capacity, _map_->load, _map_->f_key, _map_->f_val, _map_->alloc, ((void *)0));
    if (!_new_map_)
    {
        _map_->flag = CMC_FLAG_ALLOC;
        return 0;
    }
    struct hashmultimap_iter iter = hmm_iter_start(_map_);
    for (; !hmm_iter_at_end(&iter); hmm_iter_next(&iter))
    {
        size_t key = hmm_iter_key(&iter);
        size_t value = hmm_iter_value(&iter);
        hmm_insert(_new_map_, key, value);
    }
    if (_map_->count != _new_map_->count)
    {
        hmm_free(_new_map_);
        _map_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    struct hashmultimap_entry *(*tmp_b)[2] = _map_->buffer;
    _map_->buffer = _new_map_->buffer;
    _new_map_->buffer = tmp_b;
    size_t tmp_c = _map_->capacity;
    _map_->capacity = _new_map_->capacity;
    _new_map_->capacity = tmp_c;
    _new_map_->f_key = &(struct hashmultimap_fkey){ ((void *)0) };
    _new_map_->f_val = &(struct hashmultimap_fval){ ((void *)0) };
    hmm_free(_new_map_);
success:
    if ((_map_)->callbacks && (_map_)->callbacks->resize)
        (_map_)->callbacks->resize();
    ;
    return 1;
}
struct hashmultimap *hmm_copy_of(struct hashmultimap *_map_)
{
    struct hashmultimap *result = hmm_new_custom(_map_->capacity * _map_->load, _map_->load, _map_->f_key, _map_->f_val,
                                                 _map_->alloc, ((void *)0));
    if (!result)
    {
        _map_->flag = CMC_FLAG_ERROR;
        return ((void *)0);
    }
    struct hashmultimap_iter iter = hmm_iter_start(_map_);
    if (!hmm_empty(_map_))
    {
        for (; !hmm_iter_at_end(&iter); hmm_iter_next(&iter))
        {
            size_t key = hmm_iter_key(&iter);
            size_t value = hmm_iter_value(&iter);
            if (_map_->f_key->cpy)
                key = _map_->f_key->cpy(key);
            if (_map_->f_val->cpy)
                value = _map_->f_val->cpy(value);
            hmm_insert(result, key, value);
        }
    }
    (result)->callbacks = _map_->callbacks;
    _map_->flag = CMC_FLAG_OK;
    return result;
}
_Bool hmm_equals(struct hashmultimap *_map1_, struct hashmultimap *_map2_)
{
    _map1_->flag = CMC_FLAG_OK;
    _map2_->flag = CMC_FLAG_OK;
    if (_map1_->count != _map2_->count)
        return 0;
    struct hashmultimap_iter iter = hmm_iter_start(_map1_);
    for (; !hmm_iter_at_end(&iter); hmm_iter_next(&iter))
    {
        size_t key = hmm_iter_key(&iter);
        if (hmm_impl_key_count(_map1_, key) != hmm_impl_key_count(_map2_, key))
            return 0;
    }
    return 1;
}
struct hashmultimap_entry *hmm_impl_new_entry(struct hashmultimap *_map_, size_t key, size_t value)
{
    struct hashmultimap_entry *entry = _map_->alloc->malloc(sizeof(struct hashmultimap_entry));
    if (!entry)
        return ((void *)0);
    entry->key = key;
    entry->value = value;
    entry->next = ((void *)0);
    entry->prev = ((void *)0);
    return entry;
}
struct hashmultimap_entry *hmm_impl_get_entry(struct hashmultimap *_map_, size_t key)
{
    size_t hash = _map_->f_key->hash(key);
    struct hashmultimap_entry *entry = _map_->buffer[hash % _map_->capacity][0];
    while (entry != ((void *)0))
    {
        if (_map_->f_key->cmp(entry->key, key) == 0)
            return entry;
        entry = entry->next;
    }
    return ((void *)0);
}
size_t hmm_impl_key_count(struct hashmultimap *_map_, size_t key)
{
    size_t hash = _map_->f_key->hash(key);
    struct hashmultimap_entry *entry = _map_->buffer[hash % _map_->capacity][0];
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
size_t hmm_impl_calculate_size(size_t required)
{
    const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);
    if (cmc_hashtable_primes[count - 1] < required)
        return required;
    size_t i = 0;
    while (cmc_hashtable_primes[i] < required)
        i++;
    return cmc_hashtable_primes[i];
}
struct hashmultimap_iter hmm_iter_start(struct hashmultimap *target)
{
    struct hashmultimap_iter iter;
    iter.target = target;
    iter.curr_entry = ((void *)0);
    iter.cursor = 0;
    iter.index = 0;
    iter.first = 0;
    iter.last = 0;
    iter.start = 1;
    iter.end = hmm_empty(target);
    if (!hmm_empty(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            struct hashmultimap_entry *entry = target->buffer[i][0];
            if (entry && entry != ((void *)1))
            {
                iter.curr_entry = target->buffer[i][0];
                iter.first = i;
                break;
            }
        }
        iter.cursor = iter.first;
        for (size_t i = target->capacity; i > 0; i--)
        {
            struct hashmultimap_entry *entry = target->buffer[i - 1][1];
            if (entry && entry != ((void *)1))
            {
                iter.last = i - 1;
                break;
            }
        }
    }
    return iter;
}
struct hashmultimap_iter hmm_iter_end(struct hashmultimap *target)
{
    struct hashmultimap_iter iter;
    iter.target = target;
    iter.curr_entry = ((void *)0);
    iter.cursor = 0;
    iter.index = 0;
    iter.first = 0;
    iter.last = 0;
    iter.start = hmm_empty(target);
    iter.end = 1;
    if (!hmm_empty(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            struct hashmultimap_entry *entry = target->buffer[i][0];
            if (entry && entry != ((void *)1))
            {
                iter.first = i;
                break;
            }
        }
        for (size_t i = target->capacity; i > 0; i--)
        {
            struct hashmultimap_entry *entry = target->buffer[i - 1][1];
            if (entry && entry != ((void *)1))
            {
                iter.curr_entry = target->buffer[i - 1][1];
                iter.last = i - 1;
                break;
            }
        }
        iter.cursor = iter.last;
        iter.index = target->count - 1;
    }
    return iter;
}
_Bool hmm_iter_at_start(struct hashmultimap_iter *iter)
{
    return hmm_empty(iter->target) || iter->start;
}
_Bool hmm_iter_at_end(struct hashmultimap_iter *iter)
{
    return hmm_empty(iter->target) || iter->end;
}
_Bool hmm_iter_to_start(struct hashmultimap_iter *iter)
{
    if (!hmm_empty(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = 1;
        iter->end = hmm_empty(iter->target);
        iter->curr_entry = iter->target->buffer[iter->first][0];
        return 1;
    }
    return 0;
}
_Bool hmm_iter_to_end(struct hashmultimap_iter *iter)
{
    if (!hmm_empty(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = hmm_empty(iter->target);
        iter->end = 1;
        iter->curr_entry = iter->target->buffer[iter->last][1];
        return 1;
    }
    return 0;
}
_Bool hmm_iter_next(struct hashmultimap_iter *iter)
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
    iter->start = hmm_empty(iter->target);
    return 1;
}
_Bool hmm_iter_prev(struct hashmultimap_iter *iter)
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
    iter->end = hmm_empty(iter->target);
    return 1;
}
_Bool hmm_iter_advance(struct hashmultimap_iter *iter, size_t steps)
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
        hmm_iter_next(iter);
    return 1;
}
_Bool hmm_iter_rewind(struct hashmultimap_iter *iter, size_t steps)
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
        hmm_iter_prev(iter);
    return 1;
}
_Bool hmm_iter_go_to(struct hashmultimap_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return hmm_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return hmm_iter_advance(iter, index - iter->index);
    return 1;
}
size_t hmm_iter_key(struct hashmultimap_iter *iter)
{
    if (hmm_empty(iter->target))
        return (size_t){ 0 };
    return iter->curr_entry->key;
}
size_t hmm_iter_value(struct hashmultimap_iter *iter)
{
    if (hmm_empty(iter->target))
        return (size_t){ 0 };
    return iter->curr_entry->value;
}
size_t *hmm_iter_rvalue(struct hashmultimap_iter *iter)
{
    if (hmm_empty(iter->target))
        return ((void *)0);
    return &(iter->curr_entry->value);
}
size_t hmm_iter_index(struct hashmultimap_iter *iter)
{
    return iter->index;
}
_Bool hmm_to_string(struct hashmultimap *_map_, FILE *fptr)
{
    struct hashmultimap *m_ = _map_;
    return 0 <= fprintf(fptr,
                        "struct %s<%s, %s> "
                        "at %p { "
                        "buffer:%p, "
                        "capacity:%"
                        "I64u"
                        ", "
                        "count:%"
                        "I64u"
                        ", "
                        "load:%lf, "
                        "flag:%d, "
                        "f_key:%p, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        "hashmultimap", "size_t", "size_t", m_, m_->buffer, m_->capacity, m_->count, m_->load, m_->flag,
                        m_->f_key, m_->f_val, m_->alloc, (m_)->callbacks);
}
_Bool hmm_print(struct hashmultimap *_map_, FILE *fptr, const char *start, const char *separator, const char *end,
                const char *key_val_sep)
{
    fprintf(fptr, "%s", start);
    size_t last = 0;
    for (size_t i = _map_->capacity; i > 0; i--)
    {
        struct hashmultimap_entry *entry = _map_->buffer[i - 1][1];
        if (entry && entry != ((void *)1))
        {
            last = i - 1;
            break;
        }
    }
    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct hashmultimap_entry *scan = _map_->buffer[i][0];
        while (scan != ((void *)0))
        {
            if (!_map_->f_key->str(fptr, scan->key))
                return 0;
            fprintf(fptr, "%s", key_val_sep);
            if (!_map_->f_val->str(fptr, scan->value))
                return 0;
            scan = scan->next;
            if (i + 1 < last)
                fprintf(fptr, "%s", separator);
        }
    }
    fprintf(fptr, "%s", end);
    return 1;
}

#endif /* CMC_TEST_SRC_HASHMULTIMAP */
