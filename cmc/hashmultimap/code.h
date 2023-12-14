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
struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_new_entry)(struct SNAME *_map_, K key, V value);
struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_get_entry)(struct SNAME *_map_, K key);
size_t CMC_(PFX, _impl_key_count)(struct SNAME *_map_, K key);
size_t CMC_(PFX, _impl_calculate_size)(size_t required);

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

    if (capacity == 0 || load <= 0)
        return NULL;

    /* Prevent integer overflow */
    if (capacity >= (double)UINTMAX_MAX * load)
        return NULL;

    if (!f_key || !f_val)
        return NULL;

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    size_t real_capacity = CMC_(PFX, _impl_calculate_size)(capacity / load);

    struct SNAME *_map_ = alloc->malloc(sizeof(struct SNAME));

    if (!_map_)
        return NULL;

    _map_->buffer = alloc->calloc(real_capacity, sizeof(struct CMC_DEF_ENTRY(SNAME) *[2]));

    if (!_map_->buffer)
    {
        alloc->free(_map_);
        return NULL;
    }

    _map_->count = 0;
    _map_->capacity = real_capacity;
    _map_->load = load;
    _map_->flag = CMC_FLAG_OK;
    _map_->f_key = f_key;
    _map_->f_val = f_val;
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
        struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0];

        while (scan)
        {
            struct CMC_DEF_ENTRY(SNAME) *next = scan->next;

            if (_map_->f_key->free)
                _map_->f_key->free(scan->key);
            if (_map_->f_val->free)
                _map_->f_val->free(scan->value);

            _map_->alloc->free(scan);

            scan = next;
        }
    }

    memset(_map_->buffer, 0, sizeof(struct CMC_DEF_ENTRY(SNAME) *[2]) * _map_->capacity);

    _map_->count = 0;
    _map_->flag = CMC_FLAG_OK;
}

void CMC_(PFX, _free)(struct SNAME *_map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    size_t index = 0;

    while (index < _map_->capacity)
    {
        struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[index][0];

        if (scan)
        {
            if (scan->next == NULL && scan->prev == NULL)
            {
                if (_map_->f_key->free)
                    _map_->f_key->free(scan->key);
                if (_map_->f_val->free)
                    _map_->f_val->free(scan->value);

                _map_->alloc->free(scan);
            }
            else
            {
                while (scan)
                {
                    struct CMC_DEF_ENTRY(SNAME) *tmp = scan;

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

// TODO
// struct SNAME CMC_(PFX, _init)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
//                               struct CMC_DEF_FVAL(SNAME) * f_val);
// struct SNAME CMC_(PFX, _init_custom)(size_t capacity, double load, struct CMC_DEF_FKEY(SNAME) * f_key,
//                                      struct CMC_DEF_FVAL(SNAME) * f_val, CMC_ALLOC_TYPE alloc,
//                                      CMC_CALLBACK_TYPE callbacks);
// void CMC_(PFX, _release)(struct SNAME _map_);

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

    size_t hash = _map_->f_key->hash(key);
    size_t pos = hash % _map_->capacity;

    struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_new_entry)(_map_, key, value);

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
    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

bool CMC_(PFX, _update)(struct SNAME *_map_, K key, V new_value, V *old_value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_get_entry)(_map_, key);

    if (!entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return false;
    }

    if (old_value)
        *old_value = entry->value;

    entry->value = new_value;

    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

size_t CMC_(PFX, _update_all)(struct SNAME *_map_, K key, V new_value, V **old_values)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return 0;
    }

    size_t hash = _map_->f_key->hash(key);

    struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[hash % _map_->capacity][0];

    if (entry == NULL)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }

    if (old_values)
    {
        size_t total = CMC_(PFX, _impl_key_count)(_map_, key);

        if (total == 0)
        {
            _map_->flag = CMC_FLAG_NOT_FOUND;
            return 0;
        }

        *old_values = _map_->alloc->malloc(sizeof(V) * total);

        if (!(*old_values))
        {
            _map_->flag = CMC_FLAG_ALLOC;
            return 0;
        }
    }

    size_t index = 0;

    while (entry)
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

    CMC_CALLBACKS_CALL(_map_);

    return index;
}

bool CMC_(PFX, _remove)(struct SNAME *_map_, K key, V *out_value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    size_t hash = _map_->f_key->hash(key);

    struct CMC_DEF_ENTRY(SNAME) **head = &(_map_->buffer[hash % _map_->capacity][0]);
    struct CMC_DEF_ENTRY(SNAME) **tail = &(_map_->buffer[hash % _map_->capacity][1]);

    if (*head == NULL)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return false;
    }

    struct CMC_DEF_ENTRY(SNAME) *entry = *head;

    if (entry->next == NULL && entry->prev == NULL)
    {
        if (_map_->f_key->cmp(entry->key, key) == 0)
        {
            *head = NULL;
            *tail = NULL;

            if (out_value)
                *out_value = entry->value;
        }
        else
        {
            _map_->flag = CMC_FLAG_NOT_FOUND;
            return false;
        }
    }
    else
    {
        bool found = false;

        while (entry)
        {
            if (_map_->f_key->cmp(entry->key, key) == 0)
            {
                if (*head == entry)
                    *head = entry->next;
                if (*tail == entry)
                    *tail = entry->prev;

                if (entry->prev)
                    entry->prev->next = entry->next;
                if (entry->next)
                    entry->next->prev = entry->prev;

                if (out_value)
                    *out_value = entry->value;

                found = true;

                break;
            }
            else
                entry = entry->next;
        }

        if (!found)
        {
            _map_->flag = CMC_FLAG_NOT_FOUND;
            return false;
        }
    }

    _map_->alloc->free(entry);

    _map_->count--;
    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

size_t CMC_(PFX, _remove_all)(struct SNAME *_map_, K key, V **out_values)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    size_t hash = _map_->f_key->hash(key);

    struct CMC_DEF_ENTRY(SNAME) **head = &(_map_->buffer[hash % _map_->capacity][0]);
    struct CMC_DEF_ENTRY(SNAME) **tail = &(_map_->buffer[hash % _map_->capacity][1]);

    if (*head == NULL)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }

    if (out_values)
    {
        size_t total = CMC_(PFX, _impl_key_count)(_map_, key);

        if (total == 0)
        {
            _map_->flag = CMC_FLAG_NOT_FOUND;
            return 0;
        }

        *out_values = _map_->alloc->malloc(sizeof(V) * total);

        if (!(*out_values))
        {
            _map_->flag = CMC_FLAG_ALLOC;
            return 0;
        }
    }

    size_t index = 0;
    struct CMC_DEF_ENTRY(SNAME) *entry = *head;

    if (entry->next == NULL)
    {
        *head = NULL;
        *tail = NULL;

        if (out_values)
            (*out_values)[index] = entry->value;

        index++;
        _map_->alloc->free(entry);
    }
    else
    {
        while (entry)
        {
            if (_map_->f_key->cmp(entry->key, key) == 0)
            {
                if (*head == entry)
                    *head = entry->next;
                if (*tail == entry)
                    *tail = entry->prev;

                struct CMC_DEF_ENTRY(SNAME) *next = entry->next;

                if (entry->prev)
                    entry->prev->next = entry->next;
                if (entry->next)
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

    CMC_CALLBACKS_CALL(_map_);

    return index;
}

bool CMC_(PFX, _max)(struct SNAME *_map_, K *key, V *value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    bool first = true;
    K max_key = (K){ 0 };
    V max_val = (V){ 0 };

    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0];

        while (scan)
        {
            if (first)
            {
                max_key = scan->key;
                max_val = scan->value;

                first = false;
            }
            else if (_map_->f_key->cmp(scan->key, max_key) > 0)
            {
                max_key = scan->key;
                max_val = scan->value;
            }

            scan = scan->next;
        }
    }

    if (key)
        *key = max_key;
    if (value)
        *value = max_val;

    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

bool CMC_(PFX, _min)(struct SNAME *_map_, K *key, V *value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    bool first = true;
    K min_key = (K){ 0 };
    V min_val = (V){ 0 };

    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0];

        while (scan)
        {
            if (first)
            {
                min_key = scan->key;
                min_val = scan->value;

                first = false;
            }
            else if (_map_->f_key->cmp(scan->key, min_key) < 0)
            {
                min_key = scan->key;
                min_val = scan->value;
            }

            scan = scan->next;
        }
    }

    if (key)
        *key = min_key;
    if (value)
        *value = min_val;

    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

V CMC_(PFX, _get)(struct SNAME *_map_, K key)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return (V){ 0 };
    }

    struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_get_entry)(_map_, key);

    if (!entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return (V){ 0 };
    }

    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return entry->value;
}

V *CMC_(PFX, _get_ref)(struct SNAME *_map_, K key)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_map_))
    {
        _map_->flag = CMC_FLAG_EMPTY;
        return NULL;
    }

    struct CMC_DEF_ENTRY(SNAME) *entry = CMC_(PFX, _impl_get_entry)(_map_, key);

    if (!entry)
    {
        _map_->flag = CMC_FLAG_NOT_FOUND;
        return NULL;
    }

    _map_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_map_);

    return &(entry->value);
}

bool CMC_(PFX, _contains)(struct SNAME *_map_, K key)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _map_->flag = CMC_FLAG_OK;

    bool result = CMC_(PFX, _impl_get_entry)(_map_, key) != NULL;

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

size_t CMC_(PFX, _key_count)(struct SNAME *_map_, K key)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _impl_key_count)(_map_, key);
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
    size_t theoretical_size = CMC_(PFX, _impl_calculate_size)(capacity);

    /* Not possible to shrink with current available prime numbers */
    if (theoretical_size < _map_->count / _map_->load)
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

        while (scan)
        {
            /* TODO check for errors */
            CMC_(PFX, _insert)(_new_map_, scan->key, scan->value);

            scan = scan->next;
        }
    }

    if (_map_->count != _new_map_->count)
    {
        CMC_(PFX, _free)(_new_map_);

        _map_->flag = CMC_FLAG_ERROR;
        return false;
    }

    struct CMC_DEF_ENTRY(SNAME) * (*tmp_b)[2] = _map_->buffer;
    _map_->buffer = _new_map_->buffer;
    _new_map_->buffer = tmp_b;

    size_t tmp_c = _map_->capacity;
    _map_->capacity = _new_map_->capacity;
    _new_map_->capacity = tmp_c;

    /* Prevent the map from freeing the data */
    _new_map_->f_key = &(struct CMC_DEF_FKEY(SNAME)){ 0 };
    _new_map_->f_val = &(struct CMC_DEF_FVAL(SNAME)){ 0 };

    CMC_(PFX, _free)(_new_map_);

success:

    CMC_CALLBACKS_CALL(_map_);

    return true;
}

struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_map_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    /* Callback will be added later */
    struct SNAME *result = CMC_(PFX, _new_custom)(_map_->capacity * _map_->load, _map_->load, _map_->f_key,
                                                  _map_->f_val, _map_->alloc, NULL);

    if (!result)
    {
        _map_->flag = CMC_FLAG_ERROR;
        return NULL;
    }

    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0];

        while (scan)
        {
            K key;
            V value;

            if (_map_->f_key->cpy)
                key = _map_->f_key->cpy(scan->key);
            else
                key = scan->key;
            if (_map_->f_val->cpy)
                value = _map_->f_val->cpy(scan->value);
            else
                value = scan->value;

            /* TODO check for errors */
            CMC_(PFX, _insert)(result, key, value);

            scan = scan->next;
        }
    }

    CMC_CALLBACKS_ASSIGN(result, _map_->callbacks);
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

    /* Optimize loop using the smallest hashtable */
    struct SNAME *_map_a_;
    struct SNAME *_map_b_;

    _map_a_ = _map1_->capacity < _map2_->capacity ? _map1_ : _map2_;
    _map_b_ = _map_a_ == _map1_ ? _map2_ : _map1_;

    for (size_t i = 0; i < _map_a_->capacity; i++)
    {
        struct CMC_DEF_ENTRY(SNAME) *scan = _map_a_->buffer[i][0];

        while (scan)
        {
            /* OPTIMIZE - This is calling key_count for repeating keys */
            if (CMC_(PFX, _impl_key_count)(_map_a_, scan->key) != CMC_(PFX, _impl_key_count)(_map_b_, scan->key))
                return false;

            scan = scan->next;
        }
    }

    return true;
}

struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_new_entry)(struct SNAME *_map_, K key, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ENTRY(SNAME) *entry = _map_->alloc->malloc(sizeof(struct CMC_DEF_ENTRY(SNAME)));

    if (!entry)
        return NULL;

    entry->key = key;
    entry->value = value;
    entry->next = NULL;
    entry->prev = NULL;

    return entry;
}

struct CMC_DEF_ENTRY(SNAME) * CMC_(PFX, _impl_get_entry)(struct SNAME *_map_, K key)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    size_t hash = _map_->f_key->hash(key);

    struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[hash % _map_->capacity][0];

    while (entry)
    {
        if (_map_->f_key->cmp(entry->key, key) == 0)
            return entry;

        entry = entry->next;
    }

    return NULL;
}

size_t CMC_(PFX, _impl_key_count)(struct SNAME *_map_, K key)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    size_t hash = _map_->f_key->hash(key);

    struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[hash % _map_->capacity][0];

    size_t total_count = 0;

    if (!entry)
        return total_count;

    while (entry)
    {
        if (_map_->f_key->cmp(entry->key, key) == 0)
            total_count++;

        entry = entry->next;
    }

    return total_count;
}

size_t CMC_(PFX, _impl_calculate_size)(size_t required)
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
 * List bi-directional iterator.
 */
#ifdef CMC_EXT_ITER

struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_start)(struct SNAME *target)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ITER(SNAME) iter;

    iter.target = target;
    iter.curr_entry = NULL;
    iter.cursor = 0;
    iter.index = 0;
    iter.first = 0;
    iter.last = 0;
    iter.start = true;
    iter.end = CMC_(PFX, _empty)(target);

    if (!CMC_(PFX, _empty)(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i][0];

            if (entry && entry != CMC_ENTRY_DELETED)
            {
                iter.curr_entry = target->buffer[i][0];
                iter.first = i;
                break;
            }
        }

        iter.cursor = iter.first;

        for (size_t i = target->capacity; i > 0; i--)
        {
            struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i - 1][1];

            if (entry && entry != CMC_ENTRY_DELETED)
            {
                iter.last = i - 1;
                break;
            }
        }
    }

    return iter;
}

struct CMC_DEF_ITER(SNAME) CMC_(PFX, _iter_end)(struct SNAME *target)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_ITER(SNAME) iter;

    iter.target = target;
    iter.curr_entry = NULL;
    iter.cursor = 0;
    iter.index = 0;
    iter.first = 0;
    iter.last = 0;
    iter.start = CMC_(PFX, _empty)(target);
    iter.end = true;

    if (!CMC_(PFX, _empty)(target))
    {
        for (size_t i = 0; i < target->capacity; i++)
        {
            struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i][0];

            if (entry && entry != CMC_ENTRY_DELETED)
            {
                iter.first = i;
                break;
            }
        }

        for (size_t i = target->capacity; i > 0; i--)
        {
            struct CMC_DEF_ENTRY(SNAME) *entry = target->buffer[i - 1][1];

            if (entry && entry != CMC_ENTRY_DELETED)
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

bool CMC_(PFX, _iter_at_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _empty)(iter->target) || iter->start;
}

bool CMC_(PFX, _iter_at_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _empty)(iter->target) || iter->end;
}

bool CMC_(PFX, _iter_to_start)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->cursor = iter->first;
        iter->index = 0;
        iter->start = true;
        iter->end = CMC_(PFX, _empty)(iter->target);
        iter->curr_entry = iter->target->buffer[iter->first][0];

        return true;
    }

    return false;
}

bool CMC_(PFX, _iter_to_end)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (!CMC_(PFX, _empty)(iter->target))
    {
        iter->cursor = iter->last;
        iter->index = iter->target->count - 1;
        iter->start = CMC_(PFX, _empty)(iter->target);
        iter->end = true;
        iter->curr_entry = iter->target->buffer[iter->last][1];

        return true;
    }

    return false;
}

bool CMC_(PFX, _iter_next)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

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

        iter->cursor++;

        while (iter->target->buffer[iter->cursor][0] == NULL)
            iter->cursor++;

        iter->curr_entry = iter->target->buffer[iter->cursor][0];

        iter->index++;
    }

    iter->start = CMC_(PFX, _empty)(iter->target);

    return true;
}

bool CMC_(PFX, _iter_prev)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

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

        iter->cursor--;

        while (iter->target->buffer[iter->cursor][1] == NULL)
            iter->cursor--;

        iter->curr_entry = iter->target->buffer[iter->cursor][1];

        iter->index--;
    }

    iter->end = CMC_(PFX, _empty)(iter->target);

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

/* Returns true only if the iterator was able to be positioned at the */
/* given index */
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
        return (K){ 0 };

    return iter->curr_entry->key;
}

V CMC_(PFX, _iter_value)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(iter->target))
        return (V){ 0 };

    return iter->curr_entry->value;
}

V *CMC_(PFX, _iter_rvalue)(struct CMC_DEF_ITER(SNAME) * iter)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(iter->target))
        return NULL;

    return &(iter->curr_entry->value);
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
        struct CMC_DEF_ENTRY(SNAME) *entry = _map_->buffer[i - 1][1];

        if (entry && entry != CMC_ENTRY_DELETED)
        {
            last = i - 1;
            break;
        }
    }

    for (size_t i = 0; i < _map_->capacity; i++)
    {
        struct CMC_DEF_ENTRY(SNAME) *scan = _map_->buffer[i][0];

        while (scan != NULL)
        {
            if (!_map_->f_key->str(fptr, scan->key))
                return false;

            fprintf(fptr, "%s", key_val_sep);

            if (!_map_->f_val->str(fptr, scan->value))
                return false;

            scan = scan->next;

            if (i + 1 < last)
                fprintf(fptr, "%s", separator);
        }
    }

    fprintf(fptr, "%s", end);

    return true;
}

#endif /* CMC_EXT_STR */
