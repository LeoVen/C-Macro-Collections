/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Library.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* Implementation Detail Functions */
static struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_new_entry)(struct SNAME *_map_, K key, V value);
static struct CMC_DEF_ENTRY(SNAME) * *CMC_(PFX, _impl_get_entry_by_key)(struct SNAME *_map_, K key);
static struct CMC_DEF_ENTRY(SNAME) * *CMC_(PFX, _impl_get_entry_by_val)(struct SNAME *_map_, V val);
static struct CMC_DEF_ENTRY(SNAME) *
    *CMC_(PFX, _impl_add_entry_to_key)(struct SNAME *_map_, struct CMC_DEF_ENTRY(SNAME) * entry);
static struct CMC_DEF_ENTRY(SNAME) *
    *CMC_(PFX, _impl_add_entry_to_val)(struct SNAME *_map_, struct CMC_DEF_ENTRY(SNAME) * entry);
static size_t CMC_(PFX, _impl_calculate_size)(size_t required);

struct SNAME *CMC_(PFX, _new)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
                              struct CMC_DEF_FVAL(SNAME) * f_val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _new_custom)(capacity, load, f_key, f_val, NULL, NULL);
}

struct SNAME *CMC_(PFX, _new_custom)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
                                     struct CMC_DEF_FVAL(SNAME) * f_val, CMC_ALLOC_TYPE alloc,
                                     CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (capacity == 0 || load <= 0 || load >= 1)
        return NULL;

    /* Prevent integer overflow */
    if (capacity >= (double)UINTMAX_MAX * load)
        return NULL;

    if (!f_key || !f_val)
        return NULL;

    size_t real_capacity = CMC_(PFX, _impl_calculate_size)(capacity / load);

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME));

    if (!_map_)
        return NULL;

    _map_->buffer = alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME) *[2]));

    if (!_map_->buffer)
    {
        alloc->free(_map_->buffer);
        alloc->free(_map_);
        return NULL;
    }

    _map_->count = 0;
    _map_->capacity = real_capacity;
    _map_->load = load;
    _map_->flag = CMC_FLAG_OK;
    _map_->f_key = f_key;
    _map_->f_val = f_val;
    _map_->flag = CMC_FLAG_OK;
    _map_->alloc = alloc;
    CMC_CALLBACKS_ASSIGN(_map_, callbacks);

    return _map_;
}

void CMC_(PFX, _clear)(struct SNAME *_map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[i][0];

        if (entry && entry != CMC_ENTRY_DELETED)
        {
            if (_map_->f_key->free)
                _map_->f_key->free(entry->key);
            if (_map_->f_val->free)
                _map_->f_val->free(entry->value);

            _map_->alloc->free(entry);
        }

        _map_->buffer[i][0] = NULL;
        _map_->buffer[i][1] = NULL;
    }

    _map_->count = 0;
    _map_->flag = CMC_FLAG_OK;
}

void CMC_(PFX, _free)(struct SNAME *_map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_(PFX, _clear)(_map_);

    _map_->alloc->free(_map_->buffer);
    _map_->alloc->free(_map_);
}

struct SNAME CMC_(PFX, _init)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
                              struct CMC_DEF_FVAL(SNAME) * f_val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _init_custom)(capacity, load, f_key, f_val, NULL, NULL);
}

struct SNAME CMC_(PFX, _init_custom)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
                                     struct CMC_DEF_FVAL(SNAME) * f_val, CMC_ALLOC_TYPE alloc,
                                     CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    struct SNAME _map_ = { 0 };

    if (capacity == 0 || load <= 0 || load >= 1)
        return _map_;

    /* Prevent integer overflow */
    if (capacity >= (double)UINTMAX_MAX * load)
        return _map_;

    if (!f_key || !f_val)
        return _map_;

    size_t real_capacity = CMC_(PFX, _impl_calculate_size)(capacity / load);

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    _map_.buffer = alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME) *[2]));

    if (!_map_.buffer)
        return _map_;

    _map_.count = 0;
    _map_.capacity = real_capacity;
    _map_.load = load;
    _map_.flag = CMC_FLAG_OK;
    _map_.f_key = f_key;
    _map_.f_val = f_val;
    _map_.flag = CMC_FLAG_OK;
    _map_.alloc = alloc;
    CMC_CALLBACKS_ASSIGN(&_map_, callbacks);

    return _map_;
}

void CMC_(PFX, _release)(struct SNAME _map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_(PFX, _clear)(&_map_);

    _map_.alloc->free(_map_.buffer);
}

void CMC_(PFX, _customize)(struct SNAME *_map_, CMC_ALLOC_TYPE alloc, CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (!alloc)
        _map_->alloc = &cmc_alloc_node_default;
    else
        _map_->alloc = alloc;

    CMC_CALLBACKS_ASSIGN(_map_, callbacks);

    _map_->flag = CMC_FLAG_OK;
}

bool CMC_(PFX, _insert)(struct SNAME *_map_, K key, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _full)(_map_))
    {
        if (!CMC_(PFX, _resize)(_map_, _map_->capacity + 1))
            return false;
    }

    if (CMC_(PFX, _impl_get_entry_by_key)(_map_, key) != NULL ||
        CMC_(PFX, _impl_get_entry_by_val)(_map_, value) != NULL)
    {
        _map_->flag = CMC_FLAG_DUPLICATE;
        return false;
    }

    struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_new_entry)(_map_, key, value);

    struct CMC_DEF_ENTRY(SNAME) **key_entry = CMC_(PFX, _impl_add_entry_to_key)(_map_, entry);
    struct CMC_DEF_ENTRY(SNAME) **val_entry = CMC_(PFX, _impl_add_entry_to_val)(_map_, entry);

    if (!key_entry || !val_entry)
    {
        if (key_entry)
            *key_entry = CMC_ENTRY_DELETED;
        if (val_entry)
            *val_entry = CMC_ENTRY_DELETED;

        _map_->alloc->free(entry);

        _map_->flag = CMC_FLAG_ERROR;

        return false;
    }

    _map_->count++;
    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

bool CMC_(PFX, _update_key)(struct SNAME *_map_, V val, K new_key)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    struct CMC_DEF_ENTRY(SNAME) * *key_entry, **val_entry;

    val_entry = CMC_(PFX, _impl_get_entry_by_val)(_map_, val);

    if (!val_entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return false;
    }

    /* The mapping val -> new_key is already true */
    if (_map_->f_key->cmp(new_key, (*val_entry)->key) == 0)
        goto success;

    if (CMC_(PFX, _impl_get_entry_by_key)(_map_, new_key) != NULL)
    {
        _map_->flag = CMC_FLAG_DUPLICATE;
        return false;
    }

    key_entry = (*val_entry)->ref[0];

    if (!key_entry || *key_entry != *val_entry)
    {
        /* Should never happen */
        _map_->flag = CMC_FLAG_ERROR;
        return false;
    }

    /* Remove entry from key buffer and add it again with new key */
    struct CMC_DEF_ENTRY(SNAME) *to_add = *key_entry;
    K tmp_key = to_add->key;
    to_add->key = new_key;

    *key_entry = CMC_ENTRY_DELETED;

    if (!CMC_(PFX, _impl_add_entry_to_key)(_map_, to_add))
    {
        /* Revert changes */
        to_add->key = tmp_key;
        *key_entry = to_add;

        _map_->flag = CMC_FLAG_ERROR;
        return false;
    }

success:

    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

bool CMC_(PFX, _update_val)(struct SNAME *_map_, K key, V new_val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    struct CMC_DEF_ENTRY(SNAME) * *key_entry, **val_entry;

    key_entry = CMC_(PFX, _impl_get_entry_by_key)(_map_, key);

    if (!key_entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return false;
    }

    /* The mapping key -> new_val is already true */
    if (_map_->f_val->cmp(new_val, (*key_entry)->value) == 0)
        goto success;

    if (CMC_(PFX, _impl_get_entry_by_val)(_map_, new_val) != NULL)
    {
        _map_->flag = CMC_FLAG_DUPLICATE;
        return false;
    }

    val_entry = (*key_entry)->ref[1];

    if (!val_entry || *val_entry != *key_entry)
    {
        /* Should never happen */
        _map_->flag = CMC_FLAG_ERROR;
        return false;
    }

    /* Remove entry from value buffer and add it again with new value */
    struct CMC_DEF_ENTRY(SNAME) *to_add = *val_entry;
    V tmp_val = to_add->value;
    to_add->value = new_val;

    *val_entry = CMC_ENTRY_DELETED;

    if (!CMC_(PFX, _impl_add_entry_to_val)(_map_, to_add))
    {
        /* Revert changes */
        to_add->value = tmp_val;
        *val_entry = to_add;

        _map_->flag = CMC_FLAG_ERROR;

        return false;
    }

success:

    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

bool CMC_(PFX, _remove_by_key)(struct SNAME *_map_, K key, K *out_key, V *out_val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    struct CMC_DEF_ENTRY(SNAME) * *key_entry, **val_entry;

    key_entry = CMC_(PFX, _impl_get_entry_by_key)(_map_, key);

    if (!key_entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return false;
    }

    val_entry = (*key_entry)->ref[1];

    if (!val_entry || *val_entry != *key_entry)
    {
        /* Should never happen */
        _map_->flag = CMC_FLAG_ERROR;
        return false;
    }

    if (out_key)
        *out_key = (*key_entry)->key;
    if (out_val)
        *out_val = (*key_entry)->value;

    _map_->alloc->free(*key_entry);

    *key_entry = CMC_ENTRY_DELETED;
    *val_entry = CMC_ENTRY_DELETED;

    _map_->count--;
    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

bool CMC_(PFX, _remove_by_val)(struct SNAME *_map_, V val, K *out_key, V *out_val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    struct CMC_DEF_ENTRY(SNAME) * *key_entry, **val_entry;

    val_entry = CMC_(PFX, _impl_get_entry_by_val)(_map_, val);

    if (!val_entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return false;
    }

    key_entry = (*val_entry)->ref[0];

    if (!key_entry || *key_entry != *val_entry)
    {
        /* Should never happen */
        _map_->flag = CMC_FLAG_ERROR;
        return false;
    }

    if (out_key)
        *out_key = (*val_entry)->key;
    if (out_val)
        *out_val = (*val_entry)->value;

    _map_->alloc->free(*val_entry);

    *key_entry = CMC_ENTRY_DELETED;
    *val_entry = CMC_ENTRY_DELETED;

    _map_->count--;
    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

K CMC_(PFX, _get_key)(struct SNAME *_map_, V val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ENTRY(SNAME) **entry = CMC_(PFX, _impl_get_entry_by_val)(_map_, val);

    if (!entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return (K){ 0 };
    }

    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return (*entry)->key;
}

V CMC_(PFX, _get_val)(struct SNAME *_map_, K key)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ENTRY(SNAME) **entry = CMC_(PFX, _impl_get_entry_by_key)(_map_, key);

    if (!entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return (V){ 0 };
    }

    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return (*entry)->value;
}

bool CMC_(PFX, _contains_key)(struct SNAME *_map_, K key)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _map_->flag = CMC_FLAG_OK;

    bool result = CMC_(PFX, _impl_get_entry_by_key)(_map_, key) != NULL;

    CMC_CALLBACKS_CALL(_map_);

    return result;
}

bool CMC_(PFX, _contains_val)(struct SNAME *_map_, V val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _map_->flag = CMC_FLAG_OK;

    bool result = CMC_(PFX, _impl_get_entry_by_val)(_map_, val) != NULL;

    CMC_CALLBACKS_CALL(_map_);

    return result;
}

bool CMC_(PFX, _empty)(struct SNAME *_map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _map_->count == 0;
}

bool CMC_(PFX, _full)(struct SNAME *_map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return (double)_map_->capacity * _map_->load <= (double)_map_->count;
}

size_t CMC_(PFX, _count)(struct SNAME *_map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _map_->count;
}

size_t CMC_(PFX, _capacity)(struct SNAME *_map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _map_->capacity;
}

double CMC_(PFX, _load)(struct SNAME *_map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _map_->load;
}

int CMC_(PFX, _flag)(struct SNAME *_map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _map_->flag;
}

bool CMC_(PFX, _resize)(struct SNAME *_map_, size_t capacity)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _map_->flag = CMC_FLAG_OK;

    if (_map_->capacity == capacity)
        goto success;

    if (_map_->capacity > capacity / _map_->load)
        goto success;

    /* Prevent integer overflow */
    if (capacity >= (double)UINTMAX_MAX * _map_->load)
    {
        _map_->flag = CMC_FLAG_ERROR;
        return false;
    }

    /* Calculate required capacity based on the prime numbers */
    size_t new_cap = CMC_(PFX, _impl_calculate_size)(capacity);

    /* Not possible to shrink with current available prime numbers */
    if (new_cap < _map_->count / _map_->load)
    {
        _map_->flag = CMC_FLAG_INVALID;
        return false;
    }

    /* No callbacks since _new_map_ is just a temporary hashtable */
    struct SNAME *_new_map_ =
        CMC_(PFX, _new_custom)(capacity, _map_->load, _map_->f_key, _map_->f_val, _map_->alloc, NULL);

    if (!_new_map_)
    {
        _map_->flag = CMC_FLAG_ALLOC;
        return false;
    }

    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0];

        if (scan && scan != CMC_ENTRY_DELETED)
        {
            struct CMC_DEF_ENTRY(SNAME) **e1 = CMC_(PFX, _impl_add_entry_to_key)(_new_map_, scan);
            struct CMC_DEF_ENTRY(SNAME) **e2 = CMC_(PFX, _impl_add_entry_to_val)(_new_map_, scan);

            if (!e1 || !e2)
            {
                /* Prevent the map from freeing the data */
                _new_map_->f_key = &(struct CMC_DEF_FKEY(SNAME)){ 0 };
                _new_map_->f_val = &(struct CMC_DEF_FVAL(SNAME)){ 0 };

                _map_->alloc->free(_new_map_->buffer);
                _map_->alloc->free(_new_map_);

                _map_->flag = CMC_FLAG_ERROR;

                return false;
            }

            _new_map_->count++;
        }
    }

    if (_map_->count != _new_map_->count)
    {
        /* Prevent the map from freeing the data */
        _new_map_->f_key = &(struct CMC_DEF_FKEY(SNAME)){ 0 };
        _new_map_->f_val = &(struct CMC_DEF_FVAL(SNAME)){ 0 };

        _map_->alloc->free(_new_map_->buffer);
        _map_->alloc->free(_new_map_);

        _map_->flag = CMC_FLAG_ERROR;

        return false;
    }

    struct CMC_DEF_ENTRY(SNAME) * (*tmp_buff)[2] = _map_->buffer;

    _map_->buffer = _new_map_->buffer;
    _map_->capacity = _new_map_->capacity;

    _map_->alloc->free(tmp_buff);
    _map_->alloc->free(_new_map_);

success:

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    /* TODO this function can be optimized */
    struct SNAME *result = CMC_(PFX, _new_custom)(_map_->capacity * _map_->load, _map_->load, _map_->f_key,
                                                  _map_->f_val, _map_->alloc, NULL);

    if (!result)
    {
        _map_->flag = CMC_FLAG_ERROR;
        return NULL;
    }

    CMC_CALLBACKS_ASSIGN(result, _map_->callbacks);

    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0];

        if (scan && scan != CMC_ENTRY_DELETED)
        {
            K tmp_key;
            V tmp_val;

            if (_map_->f_key->cpy)
                tmp_key = _map_->f_key->cpy(scan->key);
            else
                tmp_key = scan->key;

            if (_map_->f_val->cpy)
                tmp_val = _map_->f_val->cpy(scan->value);
            else
                tmp_val = scan->value;

            /* TODO treat the possible errors */
            struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_new_entry)(result, tmp_key, tmp_val);

            CMC_(PFX, _impl_add_entry_to_key)(result, entry);
            CMC_(PFX, _impl_add_entry_to_val)(result, entry);

            result->count++;
        }
    }

    _map_->flag = CMC_FLAG_OK;

    return result;
}

bool CMC_(PFX, _equals)(struct SNAME *_map1_, struct SNAME *_map2_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _map1_->flag = CMC_FLAG_OK;
    _map2_->flag = CMC_FLAG_OK;

    if (_map1_->count != _map2_->count)
        return false;

    struct SNAME *_mapA_;
    struct SNAME *_mapB_;

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
        struct CMC_DEF_ENTRY(SNAME) *scan = _mapA_->buffer[i][0];

        if (scan && scan != CMC_ENTRY_DELETED)
        {
            struct CMC_DEF_ENTRY(SNAME) **entry_B = CMC_(PFX, _impl_get_entry_by_key)(_mapB_, scan->key);

            if (!entry_B)
                return false;

            if (_mapA_->f_val->cmp((*entry_B)->value, scan->value) != 0)
                return false;
        }
    }

    return true;
}

static struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_new_entry)(struct SNAME *_map_, K key, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ENTRY(SNAME) *entry = _map_->alloc->malloc(sizeof(struct CMC_DEF_ENTRY(SNAME)));

    if (!entry)
        return NULL;

    entry->key = key;
    entry->value = value;
    entry->dist[0] = 0;
    entry->dist[1] = 0;
    entry->ref[0] = NULL;
    entry->ref[1] = NULL;

    return entry;
}

static struct CMC_DEF_ENTRY(SNAME) * *CMC_(PFX, _impl_get_entry_by_key)(struct SNAME *_map_, K key)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    size_t hash = _map_->f_key->hash(key);
    size_t pos = hash % _map_->capacity;

    struct CMC_DEF_ENTRY(SNAME) *target = _map_->buffer[pos][0];

    while (target != NULL)
    {
        if (target != CMC_ENTRY_DELETED && _map_->f_key->cmp(target->key, key) == 0)
            return &(_map_->buffer[pos % _map_->capacity][0]);

        pos++;
        target = _map_->buffer[pos % _map_->capacity][0];
    }

    return NULL;
}

static struct CMC_DEF_ENTRY(SNAME) * *CMC_(PFX, _impl_get_entry_by_val)(struct SNAME *_map_, V val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    size_t hash = _map_->f_val->hash(val);
    size_t pos = hash % _map_->capacity;

    struct CMC_DEF_ENTRY(SNAME) *target = _map_->buffer[pos][1];

    while (target != NULL)
    {
        if (target != CMC_ENTRY_DELETED && _map_->f_val->cmp(target->value, val) == 0)
            return &(_map_->buffer[pos % _map_->capacity][1]);

        pos++;
        target = _map_->buffer[pos % _map_->capacity][1];
    }

    return NULL;
}

static struct CMC_DEF_ENTRY(SNAME) *
    *CMC_(PFX, _impl_add_entry_to_key)(struct SNAME *_map_, struct CMC_DEF_ENTRY(SNAME) * entry)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ENTRY(SNAME) **to_return = NULL;

    size_t hash = _map_->f_key->hash(entry->key);
    size_t original_pos = hash % _map_->capacity;
    size_t pos = original_pos;

    struct CMC_DEF_ENTRY(SNAME) **scan = &(_map_->buffer[hash % _map_->capacity][0]);

    if (*scan == NULL)
    {
        *scan = entry;

        entry->ref[0] = scan;

        return scan;
    }
    else
    {
        while (true)
        {
            pos++;
            scan = &(_map_->buffer[pos % _map_->capacity][0]);

            if (*scan == NULL || *scan == CMC_ENTRY_DELETED)
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

                struct CMC_DEF_ENTRY(SNAME) *_tmp_ = *scan;
                *scan = entry;
                entry = _tmp_;
            }
        }
    }

    return NULL;
}

static struct CMC_DEF_ENTRY(SNAME) *
    *CMC_(PFX, _impl_add_entry_to_val)(struct SNAME *_map_, struct CMC_DEF_ENTRY(SNAME) * entry)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ENTRY(SNAME) **to_return = NULL;

    size_t hash = _map_->f_val->hash(entry->value);
    size_t original_pos = hash % _map_->capacity;
    size_t pos = original_pos;

    struct CMC_DEF_ENTRY(SNAME) **scan = &(_map_->buffer[hash % _map_->capacity][1]);

    if (*scan == NULL)
    {
        *scan = entry;

        entry->ref[1] = scan;

        return scan;
    }
    else
    {
        while (true)
        {
            pos++;
            scan = &(_map_->buffer[pos % _map_->capacity][1]);

            if (*scan == NULL || *scan == CMC_ENTRY_DELETED)
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

                struct CMC_DEF_ENTRY(SNAME) *_tmp_ = *scan;
                *scan = entry;
                entry = _tmp_;
            }
        }
    }

    return NULL;
}

static size_t CMC_(PFX, _impl_calculate_size)(size_t required)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    const size_t count = sizeof(cmc_hashtable_primes) / sizeof(cmc_hashtable_primes[0]);

    if (cmc_hashtable_primes[count - 1] < required)
        return required;

    size_t i = 0;
    while (cmc_hashtable_primes[i] < required)
        i++;

    return cmc_hashtable_primes[i];
}

/**
 * ITER
 *
 * Hashbidimap bi-directional iterator.
 */
#ifdef CMC_EXT_ITER

struct CMC_DEF_ITER(SNAME) * CMC_(PFX, _iter_new)(struct SNAME *target)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ITER(SNAME) *iter = target->alloc->malloc(sizeof(struct CMC_DEF_ITER(SNAME)));

    if (!iter)
        return NULL;

    CMC_(PFX, _iter_init)(iter, target);

    return iter;
}

void CMC_(PFX, _iter_free)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    iter->target->alloc->free(iter);
}

void CMC_(PFX, _iter_init)(struct CMC_DEF_ITER(SNAME) * iter, struct SNAME *target)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    memset(iter, 0, sizeof(struct CMC_DEF_ITER(SNAME)));

    iter->target = target;
    iter->start = true;
    iter->end = CMC_(PFX, _empty)(target);

    if (!CMC_(PFX, _empty)(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i][0];

            if (entry && entry != CMC_ENTRY_DELETED)
            {
                iter->first = i;
                break;
            }
        }

        iter->cursor = iter->first;

        for (size_t i = target->capacity; i > 0; i--)
        {
            struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i - 1][0];

            if (entry && entry != CMC_ENTRY_DELETED)
            {
                iter->last = i - 1;
                break;
            }
        }
    }
}

bool CMC_(PFX, _iter_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _empty)(iter->target) || iter->start;
}

bool CMC_(PFX, _iter_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _empty)(iter->target) || iter->end;
}

void CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = true;
        iter->end = false;
    }
}

void CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = false;
        iter->end = true;
    }
}

bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->end)
        return false;

    if (iter->index + 1 == iter->target->count)
    {
        iter->end = true;
        return false;
    }

    iter->start = CMC_(PFX, _empty)(iter->target);

    struct CMC_DEF_ENTRY(SNAME) *scan = iter->target->buffer[iter->cursor][0];

    iter->index++;

    while (1)
    {
        iter->cursor++;
        scan = iter->target->buffer[iter->cursor][0];

        if (scan != NULL && scan != CMC_ENTRY_DELETED)
            break;
    }

    return true;
}

bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->start)
        return false;

    if (iter->index == 0)
    {
        iter->start = true;
        return false;
    }

    iter->end = CMC_(PFX, _empty)(iter->target);

    struct CMC_DEF_ENTRY(SNAME) *scan = iter->target->buffer[iter->cursor][0];

    iter->index--;

    while (1)
    {
        iter->cursor--;
        scan = iter->target->buffer[iter->cursor][0];

        if (scan != NULL && scan != CMC_ENTRY_DELETED)
            break;
    }

    return true;
}

/* Returns true only if the iterator moved */
bool CMC_(PFX, _iter_advance)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->end)
        return false;

    if (iter->index + 1 == iter->target->count)
    {
        iter->end = true;
        return false;
    }

    if (steps == 0 || iter->index + steps >= iter->target->count)
        return false;

    for (size_t i = 0; i < steps; i++)
        CMC_(PFX, _iter_next)(iter);

    return true;
}

/* Returns true only if the iterator moved */
bool CMC_(PFX, _iter_rewind)(struct CMC_DEF_ITER(SNAME) * iter, size_t steps)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (iter->start)
        return false;

    if (iter->index == 0)
    {
        iter->start = true;
        return false;
    }

    if (steps == 0 || iter->index < steps)
        return false;

    for (size_t i = 0; i < steps; i++)
        CMC_(PFX, _iter_prev)(iter);

    return true;
}

/* Returns true only if the iterator was able to be positioned at */
/* the given index */
bool CMC_(PFX, _iter_go_to)(struct CMC_DEF_ITER(SNAME) * iter, size_t index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (index >= iter->target->count)
        return false;

    if (iter->index > index)
        return CMC_(PFX, _iter_rewind)(iter, iter->index - index);
    else if (iter->index < index)
        return CMC_(PFX, _iter_advance)(iter, index - iter->index);

    return true;
}

K CMC_(PFX, _iter_key)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(iter->target))
    {
        iter->target->flag = CMC_FLAG_EMPTY;
        return (K){ 0 };
    }

    return iter->target->buffer[iter->cursor][0]->key;
}

V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(iter->target))
    {
        iter->target->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    return iter->target->buffer[iter->cursor][0]->value;
}

size_t CMC_(PFX, _iter_index)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return iter->index;
}

#endif /* CMC_EXT_ITER */

/**
 * STR
 *
 * Print helper functions.
 */
#ifdef CMC_EXT_STR

bool CMC_(PFX, _to_string)(struct SNAME *_map_, FILE *fptr)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct SNAME *m_ = _map_;

    return 0 <= fprintf(fptr,
                        "struct %s<%s, %s> "
                        "at %p { "
                        "buffer:%p, "
                        "capacity:%" PRIuMAX ", "
                        "count:%" PRIuMAX ", "
                        "load:%lf, "
                        "flag:%d, "
                        "f_key:%p, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        CMC_TO_STRING(SNAME), CMC_TO_STRING(K), CMC_TO_STRING(V), m_, m_->buffer, m_->capacity,
                        m_->count, m_->load, m_->flag, m_->f_key, m_->f_val, m_->alloc, CMC_CALLBACKS_GET(m_));
}

bool CMC_(PFX, _print)(struct SNAME *_map_, FILE *fptr, const char *start, const char *separator, const char *end,
                       const char *key_val_sep)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    fprintf(fptr, "%s", start);

    size_t last = 0;
    for (size_t i = _map_->capacity; i > 0; i--)
    {
        struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[i - 1][0];

        if (entry && entry != CMC_ENTRY_DELETED)
        {
            last = i - 1;
            break;
        }
    }

    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[i][0];

        if (entry && entry != CMC_ENTRY_DELETED)
        {
            if (!_map_->f_key->str(fptr, entry->key))
                return false;

            fprintf(fptr, "%s", key_val_sep);

            if (!_map_->f_val->str(fptr, entry->value))
                return false;

            if (i + 1 < last)
                fprintf(fptr, "%s", separator);
        }
    }

    fprintf(fptr, "%s", end);

    return true;
}

#endif /* CMC_EXT_STR */
