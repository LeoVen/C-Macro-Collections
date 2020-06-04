#ifndef CMC_TEST_SRC_HASHBIDIMAP
#define CMC_TEST_SRC_HASHBIDIMAP

#include "cmc/hashbidimap.h"

struct hashbidimap
{
    struct hashbidimap_entry *(*buffer)[2];
    size_t capacity;
    size_t count;
    double load;
    int flag;
    struct hashbidimap_fkey *f_key;
    struct hashbidimap_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
    struct hashbidimap_iter (*it_start)(struct hashbidimap *);
    struct hashbidimap_iter (*it_end)(struct hashbidimap *);
};
struct hashbidimap_entry
{
    size_t key;
    size_t value;
    size_t dist[2];
    struct hashbidimap_entry **ref[2];
};
struct hashbidimap_fkey
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashbidimap_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashbidimap_iter
{
    struct hashbidimap *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
};
struct hashbidimap *hbm_new(size_t capacity, double load, struct hashbidimap_fkey *f_key,
                            struct hashbidimap_fval *f_val);
struct hashbidimap *hbm_new_custom(size_t capacity, double load, struct hashbidimap_fkey *f_key,
                                   struct hashbidimap_fval *f_val, struct cmc_alloc_node *alloc,
                                   struct cmc_callbacks *callbacks);
void hbm_clear(struct hashbidimap *_map_);
void hbm_free(struct hashbidimap *_map_);
void hbm_customize(struct hashbidimap *_map_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool hbm_insert(struct hashbidimap *_map_, size_t key, size_t value);
_Bool hbm_update_key(struct hashbidimap *_map_, size_t val, size_t new_key);
_Bool hbm_update_val(struct hashbidimap *_map_, size_t key, size_t new_val);
_Bool hbm_remove_by_key(struct hashbidimap *_map_, size_t key, size_t *out_key, size_t *out_val);
_Bool hbm_remove_by_val(struct hashbidimap *_map_, size_t val, size_t *out_key, size_t *out_val);
size_t hbm_get_key(struct hashbidimap *_map_, size_t val);
size_t hbm_get_val(struct hashbidimap *_map_, size_t key);
_Bool hbm_contains_key(struct hashbidimap *_map_, size_t key);
_Bool hbm_contains_val(struct hashbidimap *_map_, size_t val);
_Bool hbm_empty(struct hashbidimap *_map_);
_Bool hbm_full(struct hashbidimap *_map_);
size_t hbm_count(struct hashbidimap *_map_);
size_t hbm_capacity(struct hashbidimap *_map_);
double hbm_load(struct hashbidimap *_map_);
int hbm_flag(struct hashbidimap *_map_);
_Bool hbm_resize(struct hashbidimap *_map_, size_t capacity);
struct hashbidimap *hbm_copy_of(struct hashbidimap *_map_);
_Bool hbm_equals(struct hashbidimap *_map1_, struct hashbidimap *_map2_);
struct cmc_string hbm_to_string(struct hashbidimap *_map_);
_Bool hbm_print(struct hashbidimap *_map_, FILE *fptr);
struct hashbidimap_iter *hbm_iter_new(struct hashbidimap *target);
void hbm_iter_free(struct hashbidimap_iter *iter);
void hbm_iter_init(struct hashbidimap_iter *iter, struct hashbidimap *target);
_Bool hbm_iter_start(struct hashbidimap_iter *iter);
_Bool hbm_iter_end(struct hashbidimap_iter *iter);
void hbm_iter_to_start(struct hashbidimap_iter *iter);
void hbm_iter_to_end(struct hashbidimap_iter *iter);
_Bool hbm_iter_next(struct hashbidimap_iter *iter);
_Bool hbm_iter_prev(struct hashbidimap_iter *iter);
_Bool hbm_iter_advance(struct hashbidimap_iter *iter, size_t steps);
_Bool hbm_iter_rewind(struct hashbidimap_iter *iter, size_t steps);
_Bool hbm_iter_go_to(struct hashbidimap_iter *iter, size_t index);
size_t hbm_iter_key(struct hashbidimap_iter *iter);
size_t hbm_iter_value(struct hashbidimap_iter *iter);
size_t hbm_iter_index(struct hashbidimap_iter *iter);
static struct hashbidimap_entry *hbm_impl_new_entry(struct hashbidimap *_map_, size_t key, size_t value);
static struct hashbidimap_entry **hbm_impl_get_entry_by_key(struct hashbidimap *_map_, size_t key);
static struct hashbidimap_entry **hbm_impl_get_entry_by_val(struct hashbidimap *_map_, size_t val);
static struct hashbidimap_entry **hbm_impl_add_entry_to_key(struct hashbidimap *_map_, struct hashbidimap_entry *entry);
static struct hashbidimap_entry **hbm_impl_add_entry_to_val(struct hashbidimap *_map_, struct hashbidimap_entry *entry);
static size_t hbm_impl_calculate_size(size_t required);
static struct hashbidimap_iter hbm_impl_it_start(struct hashbidimap *_map_);
static struct hashbidimap_iter hbm_impl_it_end(struct hashbidimap *_map_);
struct hashbidimap *hbm_new(size_t capacity, double load, struct hashbidimap_fkey *f_key,
                            struct hashbidimap_fval *f_val)
{
    struct cmc_alloc_node *alloc = &cmc_alloc_node_default;
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffffULL * load)
        return ((void *)0);
    if (!f_key || !f_val)
        return ((void *)0);
    size_t real_capacity = hbm_impl_calculate_size(capacity / load);
    struct hashbidimap *_map_ = alloc->malloc(sizeof(struct hashbidimap));
    if (!_map_)
        return ((void *)0);
    _map_->buffer = alloc->calloc(real_capacity, sizeof(struct hashbidimap_entry *[2]));
    if (!_map_->buffer)
    {
        alloc->free(_map_->buffer);
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
    _map_->callbacks = ((void *)0);
    _map_->it_start = hbm_impl_it_start;
    _map_->it_end = hbm_impl_it_end;
    return _map_;
}
struct hashbidimap *hbm_new_custom(size_t capacity, double load, struct hashbidimap_fkey *f_key,
                                   struct hashbidimap_fval *f_val, struct cmc_alloc_node *alloc,
                                   struct cmc_callbacks *callbacks)
{
    if (capacity == 0 || load <= 0 || load >= 1)
        return ((void *)0);
    if (capacity >= 0xffffffffffffffffULL * load)
        return ((void *)0);
    if (!f_key || !f_val)
        return ((void *)0);
    size_t real_capacity = hbm_impl_calculate_size(capacity / load);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct hashbidimap *_map_ = alloc->malloc(sizeof(struct hashbidimap));
    if (!_map_)
        return ((void *)0);
    _map_->buffer = alloc->calloc(real_capacity, sizeof(struct hashbidimap_entry *[2]));
    if (!_map_->buffer)
    {
        alloc->free(_map_->buffer);
        alloc->free(_map_);
        return ((void *)0);
    }
    _map_->count = 0;
    _map_->capacity = real_capacity;
    _map_->load = load;
    _map_->flag = CMC_FLAG_OK;
    _map_->f_key = f_key;
    _map_->f_val = f_val;
    _map_->flag = CMC_FLAG_OK;
    _map_->alloc = alloc;
    _map_->callbacks = callbacks;
    _map_->it_start = hbm_impl_it_start;
    _map_->it_end = hbm_impl_it_end;
    return _map_;
}
void hbm_clear(struct hashbidimap *_map_)
{
    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct hashbidimap_entry *entry = _map_->buffer[i][0];
        if (entry && entry != ((void *)1))
        {
            if (_map_->f_key->free)
                _map_->f_key->free(entry->key);
            if (_map_->f_val->free)
                _map_->f_val->free(entry->value);
            _map_->alloc->free(entry);
        }
        _map_->buffer[i][0] = ((void *)0);
        _map_->buffer[i][1] = ((void *)0);
    }
    _map_->count = 0;
    _map_->flag = CMC_FLAG_OK;
}
void hbm_free(struct hashbidimap *_map_)
{
    hbm_clear(_map_);
    _map_->alloc->free(_map_->buffer);
    _map_->alloc->free(_map_);
}
void hbm_customize(struct hashbidimap *_map_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _map_->alloc = &cmc_alloc_node_default;
    else
        _map_->alloc = alloc;
    _map_->callbacks = callbacks;
    _map_->flag = CMC_FLAG_OK;
}
_Bool hbm_insert(struct hashbidimap *_map_, size_t key, size_t value)
{
    if (hbm_full(_map_))
    {
        if (!hbm_resize(_map_, _map_->capacity + 1))
            return 0;
    }
    if (hbm_impl_get_entry_by_key(_map_, key) != ((void *)0) || hbm_impl_get_entry_by_val(_map_, value) != ((void *)0))
    {
        _map_->flag = CMC_FLAG_DUPLICATE;
        return 0;
    }
    struct hashbidimap_entry *entry = hbm_impl_new_entry(_map_, key, value);
    struct hashbidimap_entry **key_entry = hbm_impl_add_entry_to_key(_map_, entry);
    struct hashbidimap_entry **val_entry = hbm_impl_add_entry_to_val(_map_, entry);
    if (!key_entry || !val_entry)
    {
        if (key_entry)
            *key_entry = ((void *)1);
        if (val_entry)
            *val_entry = ((void *)1);
        _map_->alloc->free(entry);
        _map_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    _map_->count++;
    _map_->flag = CMC_FLAG_OK;
    if (_map_->callbacks && _map_->callbacks->create)
        _map_->callbacks->create();
    return 1;
}
_Bool hbm_update_key(struct hashbidimap *_map_, size_t val, size_t new_key)
{
    if (hbm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct hashbidimap_entry **key_entry, **val_entry;
    val_entry = hbm_impl_get_entry_by_val(_map_, val);
    if (!val_entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    if (_map_->f_key->cmp(new_key, (*val_entry)->key) == 0)
        goto success;
    if (hbm_impl_get_entry_by_key(_map_, new_key) != ((void *)0))
    {
        _map_->flag = CMC_FLAG_DUPLICATE;
        return 0;
    }
    key_entry = (*val_entry)->ref[0];
    if (!key_entry || *key_entry != *val_entry)
    {
        _map_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    struct hashbidimap_entry *to_add = *key_entry;
    size_t tmp_key = to_add->key;
    to_add->key = new_key;
    *key_entry = ((void *)1);
    if (!hbm_impl_add_entry_to_key(_map_, to_add))
    {
        to_add->key = tmp_key;
        *key_entry = to_add;
        _map_->flag = CMC_FLAG_ERROR;
        return 0;
    }
success:
    _map_->flag = CMC_FLAG_OK;
    if (_map_->callbacks && _map_->callbacks->update)
        _map_->callbacks->update();
    return 1;
}
_Bool hbm_update_val(struct hashbidimap *_map_, size_t key, size_t new_val)
{
    if (hbm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct hashbidimap_entry **key_entry, **val_entry;
    key_entry = hbm_impl_get_entry_by_key(_map_, key);
    if (!key_entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    if (_map_->f_key->cmp(new_val, (*key_entry)->value) == 0)
        goto success;
    if (hbm_impl_get_entry_by_val(_map_, new_val) != ((void *)0))
    {
        _map_->flag = CMC_FLAG_DUPLICATE;
        return 0;
    }
    val_entry = (*key_entry)->ref[1];
    if (!val_entry || *val_entry != *key_entry)
    {
        _map_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    struct hashbidimap_entry *to_add = *val_entry;
    size_t tmp_val = to_add->value;
    to_add->value = new_val;
    *val_entry = ((void *)1);
    if (!hbm_impl_add_entry_to_val(_map_, to_add))
    {
        to_add->value = tmp_val;
        *val_entry = to_add;
        _map_->flag = CMC_FLAG_ERROR;
        return 0;
    }
success:
    _map_->flag = CMC_FLAG_OK;
    if (_map_->callbacks && _map_->callbacks->update)
        _map_->callbacks->update();
    return 1;
}
_Bool hbm_remove_by_key(struct hashbidimap *_map_, size_t key, size_t *out_key, size_t *out_val)
{
    if (hbm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct hashbidimap_entry **key_entry, **val_entry;
    key_entry = hbm_impl_get_entry_by_key(_map_, key);
    if (!key_entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    val_entry = (*key_entry)->ref[1];
    if (!val_entry || *val_entry != *key_entry)
    {
        _map_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    if (out_key)
        *out_key = (*key_entry)->key;
    if (out_val)
        *out_val = (*key_entry)->value;
    _map_->alloc->free(*key_entry);
    *key_entry = ((void *)1);
    *val_entry = ((void *)1);
    _map_->count--;
    _map_->flag = CMC_FLAG_OK;
    if (_map_->callbacks && _map_->callbacks->delete)
        _map_->callbacks->delete ();
    return 1;
}
_Bool hbm_remove_by_val(struct hashbidimap *_map_, size_t val, size_t *out_key, size_t *out_val)
{
    if (hbm_empty(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct hashbidimap_entry **key_entry, **val_entry;
    val_entry = hbm_impl_get_entry_by_val(_map_, val);
    if (!val_entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    key_entry = (*val_entry)->ref[0];
    if (!key_entry || *key_entry != *val_entry)
    {
        _map_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    if (out_key)
        *out_key = (*val_entry)->key;
    if (out_val)
        *out_val = (*val_entry)->value;
    _map_->alloc->free(*val_entry);
    *key_entry = ((void *)1);
    *val_entry = ((void *)1);
    _map_->count--;
    _map_->flag = CMC_FLAG_OK;
    if (_map_->callbacks && _map_->callbacks->delete)
        _map_->callbacks->delete ();
    return 1;
}
size_t hbm_get_key(struct hashbidimap *_map_, size_t val)
{
    struct hashbidimap_entry **entry = hbm_impl_get_entry_by_val(_map_, val);
    if (!entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return (size_t){ 0 };
    }
    _map_->flag = CMC_FLAG_OK;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return (*entry)->key;
}
size_t hbm_get_val(struct hashbidimap *_map_, size_t key)
{
    struct hashbidimap_entry **entry = hbm_impl_get_entry_by_key(_map_, key);
    if (!entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return (size_t){ 0 };
    }
    _map_->flag = CMC_FLAG_OK;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return (*entry)->value;
}
_Bool hbm_contains_key(struct hashbidimap *_map_, size_t key)
{
    _map_->flag = CMC_FLAG_OK;
    _Bool result = hbm_impl_get_entry_by_key(_map_, key) != ((void *)0);
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return result;
}
_Bool hbm_contains_val(struct hashbidimap *_map_, size_t val)
{
    _map_->flag = CMC_FLAG_OK;
    _Bool result = hbm_impl_get_entry_by_val(_map_, val) != ((void *)0);
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return result;
}
_Bool hbm_empty(struct hashbidimap *_map_)
{
    return _map_->count == 0;
}
_Bool hbm_full(struct hashbidimap *_map_)
{
    return (double)_map_->capacity * _map_->load <= (double)_map_->count;
}
size_t hbm_count(struct hashbidimap *_map_)
{
    return _map_->count;
}
size_t hbm_capacity(struct hashbidimap *_map_)
{
    return _map_->capacity;
}
double hbm_load(struct hashbidimap *_map_)
{
    return _map_->load;
}
int hbm_flag(struct hashbidimap *_map_)
{
    return _map_->flag;
}
_Bool hbm_resize(struct hashbidimap *_map_, size_t capacity)
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
    size_t new_cap = hbm_impl_calculate_size(capacity);
    if (new_cap < _map_->count / _map_->load)
    {
        _map_->flag = CMC_FLAG_INVALID;
        return 0;
    }
    struct hashbidimap *_new_map_ =
        hbm_new_custom(capacity, _map_->load, _map_->f_key, _map_->f_val, _map_->alloc, ((void *)0));
    if (!_new_map_)
    {
        _map_->flag = CMC_FLAG_ALLOC;
        return 0;
    }
    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct hashbidimap_entry *scan = _map_->buffer[i][0];
        if (scan && scan != ((void *)1))
        {
            struct hashbidimap_entry **e1 = hbm_impl_add_entry_to_key(_new_map_, scan);
            struct hashbidimap_entry **e2 = hbm_impl_add_entry_to_val(_new_map_, scan);
            if (!e1 || !e2)
            {
                _new_map_->f_key = &(struct hashbidimap_fkey){ ((void *)0) };
                _new_map_->f_val = &(struct hashbidimap_fval){ ((void *)0) };
                _map_->alloc->free(_new_map_->buffer);
                _map_->alloc->free(_new_map_);
                _map_->flag = CMC_FLAG_ERROR;
                return 0;
            }
            _new_map_->count++;
        }
    }
    if (_map_->count != _new_map_->count)
    {
        _new_map_->f_key = &(struct hashbidimap_fkey){ ((void *)0) };
        _new_map_->f_val = &(struct hashbidimap_fval){ ((void *)0) };
        _map_->alloc->free(_new_map_->buffer);
        _map_->alloc->free(_new_map_);
        _map_->flag = CMC_FLAG_ERROR;
        return 0;
    }
    struct hashbidimap_entry *(*tmp_buff)[2] = _map_->buffer;
    _map_->buffer = _new_map_->buffer;
    _map_->capacity = _new_map_->capacity;
    _map_->alloc->free(tmp_buff);
    _map_->alloc->free(_new_map_);
success:
    if (_map_->callbacks && _map_->callbacks->resize)
        _map_->callbacks->resize();
    return 1;
}
struct hashbidimap *hbm_copy_of(struct hashbidimap *_map_)
{
    struct hashbidimap *result = hbm_new_custom(_map_->capacity * _map_->load, _map_->load, _map_->f_key, _map_->f_val,
                                                _map_->alloc, _map_->callbacks);
    if (!result)
    {
        _map_->flag = CMC_FLAG_ERROR;
        return ((void *)0);
    }
    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct hashbidimap_entry *scan = _map_->buffer[i][0];
        if (scan && scan != ((void *)1))
        {
            size_t tmp_key;
            size_t tmp_val;
            if (_map_->f_key->cpy)
                tmp_key = _map_->f_key->cpy(scan->key);
            else
                tmp_key = scan->key;
            if (_map_->f_val->cpy)
                tmp_val = _map_->f_val->cpy(scan->value);
            else
                tmp_val = scan->value;
            struct hashbidimap_entry *entry = hbm_impl_new_entry(result, tmp_key, tmp_val);
            hbm_impl_add_entry_to_key(result, entry);
            hbm_impl_add_entry_to_val(result, entry);
            result->count++;
        }
    }
    _map_->flag = CMC_FLAG_OK;
    return result;
}
_Bool hbm_equals(struct hashbidimap *_map1_, struct hashbidimap *_map2_)
{
    _map1_->flag = CMC_FLAG_OK;
    _map2_->flag = CMC_FLAG_OK;
    if (_map1_->count != _map2_->count)
        return 0;
    struct hashbidimap *_mapA_;
    struct hashbidimap *_mapB_;
    if (_map1_->capacity < _map2_->capacity)
    {
        _mapA_ = _map1_;
        _mapB_ = _map2_;
    }
    else
    {
        _mapA_ = _map2_;
        _mapB_ = _map1_;
    }
    for (size_t i = 0; i < _mapA_->capacity; i++)
    {
        struct hashbidimap_entry *scan = _mapA_->buffer[i][0];
        if (scan && scan != ((void *)1))
        {
            struct hashbidimap_entry **entry_B = hbm_impl_get_entry_by_key(_mapB_, scan->key);
            if (!entry_B)
                return 0;
            if (_mapA_->f_val->cmp((*entry_B)->value, scan->value) != 0)
                return 0;
        }
    }
    return 1;
}
struct hashbidimap_iter *hbm_iter_new(struct hashbidimap *target)
{
    struct hashbidimap_iter *iter = target->alloc->malloc(sizeof(struct hashbidimap_iter));
    if (!iter)
        return ((void *)0);
    hbm_iter_init(iter, target);
    return iter;
}
void hbm_iter_free(struct hashbidimap_iter *iter)
{
    iter->target->alloc->free(iter);
}
void hbm_iter_init(struct hashbidimap_iter *iter, struct hashbidimap *target)
{
    memset(iter, 0, sizeof(struct hashbidimap_iter));
    iter->target = target;
    iter->start = 1;
    iter->end = hbm_empty(target);
    if (!hbm_empty(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            struct hashbidimap_entry *tmp = target->buffer[i][0];
            if (tmp != ((void *)0) && tmp != ((void *)1))
            {
                iter->first = i;
                break;
            }
        }
        iter->cursor = iter->first;
        for (size_t i = target->capacity; i > 0; i--)
        {
            struct hashbidimap_entry *tmp = target->buffer[i - 1][0];
            if (tmp != ((void *)0) && tmp != ((void *)1))
            {
                iter->last = i - 1;
                break;
            }
        }
    }
}
_Bool hbm_iter_start(struct hashbidimap_iter *iter)
{
    return hbm_empty(iter->target) || iter->start;
}
_Bool hbm_iter_end(struct hashbidimap_iter *iter)
{
    return hbm_empty(iter->target) || iter->end;
}
void hbm_iter_to_start(struct hashbidimap_iter *iter)
{
    if (!hbm_empty(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = 1;
        iter->end = 0;
    }
}
void hbm_iter_to_end(struct hashbidimap_iter *iter)
{
    if (!hbm_empty(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = 0;
        iter->end = 1;
    }
}
_Bool hbm_iter_next(struct hashbidimap_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = hbm_empty(iter->target);
    struct hashbidimap_entry *scan = iter->target->buffer[iter->cursor][0];
    iter->index++;
    while (1)
    {
        iter->cursor++;
        scan = iter->target->buffer[iter->cursor][0];
        if (scan != ((void *)0) && scan != ((void *)1))
            break;
    }
    return 1;
}
_Bool hbm_iter_prev(struct hashbidimap_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = hbm_empty(iter->target);
    struct hashbidimap_entry *scan = iter->target->buffer[iter->cursor][0];
    iter->index--;
    while (1)
    {
        iter->cursor--;
        scan = iter->target->buffer[iter->cursor][0];
        if (scan != ((void *)0) && scan != ((void *)1))
            break;
    }
    return 1;
}
_Bool hbm_iter_advance(struct hashbidimap_iter *iter, size_t steps)
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
        hbm_iter_next(iter);
    return 1;
}
_Bool hbm_iter_rewind(struct hashbidimap_iter *iter, size_t steps)
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
        hbm_iter_prev(iter);
    return 1;
}
_Bool hbm_iter_go_to(struct hashbidimap_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return hbm_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return hbm_iter_advance(iter, index - iter->index);
    return 1;
}
size_t hbm_iter_key(struct hashbidimap_iter *iter)
{
    if (hbm_empty(iter->target))
    {
        iter->target->flag = CMC_FLAG_EMPTY;
        return (size_t){ 0 };
    }
    return iter->target->buffer[iter->cursor][0]->key;
}
size_t hbm_iter_value(struct hashbidimap_iter *iter)
{
    if (hbm_empty(iter->target))
    {
        iter->target->flag = CMC_FLAG_EMPTY;
        return (size_t){ 0 };
    }
    return iter->target->buffer[iter->cursor][0]->value;
}
size_t hbm_iter_index(struct hashbidimap_iter *iter)
{
    return iter->index;
}
static struct hashbidimap_entry *hbm_impl_new_entry(struct hashbidimap *_map_, size_t key, size_t value)
{
    struct hashbidimap_entry *entry = _map_->alloc->malloc(sizeof(struct hashbidimap_entry));
    if (!entry)
        return ((void *)0);
    entry->key = key;
    entry->value = value;
    entry->dist[0] = 0;
    entry->dist[1] = 0;
    entry->ref[0] = ((void *)0);
    entry->ref[1] = ((void *)0);
    return entry;
}
static struct hashbidimap_entry **hbm_impl_get_entry_by_key(struct hashbidimap *_map_, size_t key)
{
    size_t hash = _map_->f_key->hash(key);
    size_t pos = hash % _map_->capacity;
    struct hashbidimap_entry *target = _map_->buffer[pos][0];
    while (target != ((void *)0))
    {
        if (target != ((void *)1) && _map_->f_key->cmp(target->key, key) == 0)
            return &(_map_->buffer[pos % _map_->capacity][0]);
        pos++;
        target = _map_->buffer[pos % _map_->capacity][0];
    }
    return ((void *)0);
}
static struct hashbidimap_entry **hbm_impl_get_entry_by_val(struct hashbidimap *_map_, size_t val)
{
    size_t hash = _map_->f_val->hash(val);
    size_t pos = hash % _map_->capacity;
    struct hashbidimap_entry *target = _map_->buffer[pos][1];
    while (target != ((void *)0))
    {
        if (target != ((void *)1) && _map_->f_val->cmp(target->value, val) == 0)
            return &(_map_->buffer[pos % _map_->capacity][1]);
        pos++;
        target = _map_->buffer[pos % _map_->capacity][1];
    }
    return ((void *)0);
}
static struct hashbidimap_entry **hbm_impl_add_entry_to_key(struct hashbidimap *_map_, struct hashbidimap_entry *entry)
{
    struct hashbidimap_entry **to_return = ((void *)0);
    size_t hash = _map_->f_key->hash(entry->key);
    size_t original_pos = hash % _map_->capacity;
    size_t pos = original_pos;
    struct hashbidimap_entry **scan = &(_map_->buffer[hash % _map_->capacity][0]);
    if (*scan == ((void *)0))
    {
        *scan = entry;
        entry->ref[0] = scan;
        return scan;
    }
    else
    {
        while (1)
        {
            pos++;
            scan = &(_map_->buffer[pos % _map_->capacity][0]);
            if (*scan == ((void *)0) || *scan == ((void *)1))
            {
                if (!to_return)
                    to_return = scan;
                *scan = entry;
                entry->ref[0] = scan;
                entry->dist[0] = pos - original_pos;
                return to_return;
            }
            else if ((*scan)->dist[0] < pos - original_pos)
            {
                if (!to_return)
                    to_return = scan;
                size_t tmp_dist = (*scan)->dist[0];
                entry->dist[0] = pos - original_pos;
                original_pos = pos - tmp_dist;
                entry->ref[0] = scan;
                struct hashbidimap_entry *_tmp_ = *scan;
                *scan = entry;
                entry = _tmp_;
            }
        }
    }
    return ((void *)0);
}
static struct hashbidimap_entry **hbm_impl_add_entry_to_val(struct hashbidimap *_map_, struct hashbidimap_entry *entry)
{
    struct hashbidimap_entry **to_return = ((void *)0);
    size_t hash = _map_->f_val->hash(entry->value);
    size_t original_pos = hash % _map_->capacity;
    size_t pos = original_pos;
    struct hashbidimap_entry **scan = &(_map_->buffer[hash % _map_->capacity][1]);
    if (*scan == ((void *)0))
    {
        *scan = entry;
        entry->ref[1] = scan;
        return scan;
    }
    else
    {
        while (1)
        {
            pos++;
            scan = &(_map_->buffer[pos % _map_->capacity][1]);
            if (*scan == ((void *)0) || *scan == ((void *)1))
            {
                if (!to_return)
                    to_return = scan;
                *scan = entry;
                entry->ref[1] = scan;
                entry->dist[1] = pos - original_pos;
                return to_return;
            }
            else if ((*scan)->dist[1] < pos - original_pos)
            {
                if (!to_return)
                    to_return = scan;
                size_t tmp_dist = (*scan)->dist[1];
                entry->dist[1] = pos - original_pos;
                original_pos = pos - tmp_dist;
                entry->ref[1] = scan;
                struct hashbidimap_entry *_tmp_ = *scan;
                *scan = entry;
                entry = _tmp_;
            }
        }
    }
    return ((void *)0);
}
static size_t hbm_impl_calculate_size(size_t required)
{
    const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);
    if (cmc_hashtable_primes[count - 1] < required)
        return required;
    size_t i = 0;
    while (cmc_hashtable_primes[i] < required)
        i++;
    return cmc_hashtable_primes[i];
}
static struct hashbidimap_iter hbm_impl_it_start(struct hashbidimap *_map_)
{
    struct hashbidimap_iter iter;
    hbm_iter_init(&iter, _map_);
    return iter;
}
static struct hashbidimap_iter hbm_impl_it_end(struct hashbidimap *_map_)
{
    struct hashbidimap_iter iter;
    hbm_iter_init(&iter, _map_);
    hbm_iter_to_end(&iter);
    return iter;
}

#endif /* CMC_TEST_SRC_HASHBIDIMAP */
