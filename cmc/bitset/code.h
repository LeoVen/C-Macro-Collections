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
bool CMC_(PFX, _impl_resize)(struct SNAME *_bitset_, size_t n_bits, bool do_resize);

struct SNAME *CMC_(PFX, _new)(size_t n_bits)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _new_custom)(n_bits, NULL, NULL);
}

struct SNAME *CMC_(PFX, _new_custom)(size_t n_bits, CMC_ALLOC_TYPE alloc, CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (n_bits < 1)
        return NULL;

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    size_t capacity = cmc_bidx_to_widx(n_bits - 1) + 1;

    struct SNAME *_bitset_ = alloc->malloc(sizeof(struct SNAME));

    if (!_bitset_)
        return NULL;

    _bitset_->buffer = alloc->calloc(capacity, sizeof(cmc_bitset_word));

    if (!_bitset_->buffer)
    {
        alloc->free(_bitset_);
        return NULL;
    }

    _bitset_->capacity = capacity;
    _bitset_->flag = CMC_FLAG_OK;
    _bitset_->alloc = alloc;
    CMC_CALLBACKS_ASSIGN(_bitset_, callbacks);

    return _bitset_;
}

struct SNAME CMC_(PFX, _init)(size_t n_bits)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _init_custom)(n_bits, NULL, NULL);
}

struct SNAME CMC_(PFX, _init_custom)(size_t n_bits, CMC_ALLOC_TYPE alloc, CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    struct SNAME _bitset_ = { 0 };

    if (n_bits < 1)
        return _bitset_;

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    size_t capacity = cmc_bidx_to_widx(n_bits - 1) + 1;

    _bitset_.buffer = alloc->calloc(capacity, sizeof(cmc_bitset_word));

    if (!_bitset_.buffer)
        return _bitset_;

    _bitset_.capacity = capacity;
    _bitset_.flag = CMC_FLAG_OK;
    _bitset_.alloc = alloc;
    CMC_CALLBACKS_ASSIGN(&_bitset_, callbacks);

    return _bitset_;
}

void CMC_(PFX, _free)(struct SNAME *_bitset_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    free(_bitset_->buffer);
    free(_bitset_);
}

void CMC_(PFX, _release)(struct SNAME _bitset_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    free(_bitset_.buffer);
}

void CMC_(PFX, _customize)(struct SNAME *_bitset_, CMC_ALLOC_TYPE alloc, CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (!alloc)
        _bitset_->alloc = &cmc_alloc_node_default;
    else
        _bitset_->alloc = alloc;

    CMC_CALLBACKS_ASSIGN(_bitset_, callbacks);

    _bitset_->flag = CMC_FLAG_OK;
}

bool CMC_(PFX, _set)(struct SNAME *_bitset_, size_t bit_index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (!CMC_(PFX, _impl_resize)(_bitset_, bit_index + 1, false))
        return false;

    /* How many bits in a word */
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT;

    size_t i = cmc_bidx_to_widx(bit_index);

    _bitset_->buffer[i] |= ((cmc_bitset_word)1) << (bit_index % bits);

    _bitset_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_bitset_);

    return true;
}

bool CMC_(PFX, _set_range)(struct SNAME *_bitset_, size_t from, size_t to)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (to < from)
    {
        _bitset_->flag = CMC_FLAG_INVALID;
        return false;
    }

    if (!CMC_(PFX, _impl_resize)(_bitset_, to + 1, false))
        return false;

    size_t start_index = cmc_bidx_to_widx(from);
    size_t end_index = cmc_bidx_to_widx(to);

    /* How many bits in a word */
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT;

    /* 1111 ... 1111 */
    const cmc_bitset_word ones = ~((cmc_bitset_word)0);

    cmc_bitset_word shift_start = from % bits;
    cmc_bitset_word shift_end = (cmc_bitset_word)(-(to + 1)) % bits;

    cmc_bitset_word start_mask = ones << shift_start;
    cmc_bitset_word end_mask = ones >> shift_end;

    if (start_index == end_index)
    {
        _bitset_->buffer[end_index] |= (start_mask & end_mask);
    }
    else
    {
        _bitset_->buffer[start_index] |= start_mask;

        for (size_t i = start_index + 1; i < end_index; i++)
            _bitset_->buffer[i] |= ~((cmc_bitset_word)0);

        _bitset_->buffer[end_index] |= end_mask;
    }

    _bitset_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_bitset_);

    return true;
}

bool CMC_(PFX, _clear)(struct SNAME *_bitset_, size_t bit_index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (!CMC_(PFX, _impl_resize)(_bitset_, bit_index + 1, false))
        return false;

    /* How many bits in a word */
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT;

    size_t i = cmc_bidx_to_widx(bit_index);

    _bitset_->buffer[i] &= ~(((cmc_bitset_word)1) << (bit_index % bits));

    _bitset_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_bitset_);

    return true;
}

bool CMC_(PFX, _clear_range)(struct SNAME *_bitset_, size_t from, size_t to)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (to < from)
    {
        _bitset_->flag = CMC_FLAG_INVALID;
        return false;
    }

    if (!CMC_(PFX, _impl_resize)(_bitset_, to + 1, false))
        return false;

    size_t start_index = cmc_bidx_to_widx(from);
    size_t end_index = cmc_bidx_to_widx(to);

    /* How many bits in a word */
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT;

    /* 1111 ... 1111 */
    const cmc_bitset_word ones = ~((cmc_bitset_word)0);

    cmc_bitset_word shift_start = from % bits;
    cmc_bitset_word shift_end = (cmc_bitset_word)(-(to + 1)) % bits;

    cmc_bitset_word start_mask = ones << shift_start;
    cmc_bitset_word end_mask = ones >> shift_end;

    if (start_index == end_index)
    {
        _bitset_->buffer[end_index] &= ~(start_mask & end_mask);
    }
    else
    {
        _bitset_->buffer[start_index] &= ~start_mask;

        for (size_t i = start_index + 1; i < end_index; i++)
            _bitset_->buffer[i] = 0;

        _bitset_->buffer[end_index] &= ~end_mask;
    }

    _bitset_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_bitset_);

    return true;
}

bool CMC_(PFX, _flip)(struct SNAME *_bitset_, size_t bit_index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (!CMC_(PFX, _impl_resize)(_bitset_, bit_index + 1, false))
        return false;

    /* How many bits in a word */
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT;

    size_t i = cmc_bidx_to_widx(bit_index);

    _bitset_->buffer[i] ^= ((cmc_bitset_word)1) << (bit_index % bits);

    _bitset_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_bitset_);

    return true;
}

bool CMC_(PFX, _flip_range)(struct SNAME *_bitset_, size_t from, size_t to)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (to < from)
    {
        _bitset_->flag = CMC_FLAG_INVALID;
        return false;
    }

    if (!CMC_(PFX, _impl_resize)(_bitset_, to + 1, false))
        return false;

    size_t start_index = cmc_bidx_to_widx(from);
    size_t end_index = cmc_bidx_to_widx(to);

    /* How many bits in a word */
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT;

    /* 1111 ... 1111 */
    const cmc_bitset_word ones = ~((cmc_bitset_word)0);

    cmc_bitset_word shift_start = from % bits;
    cmc_bitset_word shift_end = (cmc_bitset_word)(-(to + 1)) % bits;

    cmc_bitset_word start_mask = ones << shift_start;
    cmc_bitset_word end_mask = ones >> shift_end;

    if (start_index == end_index)
    {
        _bitset_->buffer[end_index] ^= (start_mask & end_mask);
    }
    else
    {
        _bitset_->buffer[start_index] ^= start_mask;

        for (size_t i = start_index + 1; i < end_index; i++)
            _bitset_->buffer[i] ^= ones;

        _bitset_->buffer[end_index] ^= end_mask;
    }

    _bitset_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_bitset_);

    return true;
}

bool CMC_(PFX, _put)(struct SNAME *_bitset_, size_t bit_index, bool state)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (state)
        return CMC_(PFX, _set)(_bitset_, bit_index);
    else
        return CMC_(PFX, _clear)(_bitset_, bit_index);
}

bool CMC_(PFX, _put_range)(struct SNAME *_bitset_, size_t from, size_t to, bool state)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (state)
        return CMC_(PFX, _set_range)(_bitset_, from, to);
    else
        return CMC_(PFX, _clear_range)(_bitset_, from, to);
}

bool CMC_(PFX, _set_all)(struct SNAME *_bitset_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    for (size_t i = 0; i < _bitset_->capacity; i++)
        _bitset_->buffer[i] = ~((cmc_bitset_word)0);

    _bitset_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_bitset_);

    return true;
}

bool CMC_(PFX, _clear_all)(struct SNAME *_bitset_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    for (size_t i = 0; i < _bitset_->capacity; i++)
        _bitset_->buffer[i] = 0;

    _bitset_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_bitset_);

    return true;
}

bool CMC_(PFX, _flip_all)(struct SNAME *_bitset_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    for (size_t i = 0; i < _bitset_->capacity; i++)
        _bitset_->buffer[i] ^= ~((cmc_bitset_word)0);

    _bitset_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_bitset_);

    return true;
}

bool CMC_(PFX, _get)(struct SNAME *_bitset_, size_t bit_index)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    /* How many bits in a word */
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * CHAR_BIT;

    cmc_bitset_word w = _bitset_->buffer[cmc_bidx_to_widx(bit_index)];

    return w & ((cmc_bitset_word)1 << (bit_index % bits));
}

bool CMC_(PFX, _resize)(struct SNAME *_bitset_, size_t n_bits)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _impl_resize)(_bitset_, n_bits, true);
}

/* When do_resize is false, then we just want to assure that the bitset */
/* can accommodate n_bits */
bool CMC_(PFX, _impl_resize)(struct SNAME *_bitset_, size_t n_bits, bool do_resize)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (n_bits == 0)
    {
        _bitset_->flag = CMC_FLAG_INVALID;
        return false;
    }

    /* -1 because n_bits is 1-based and we need to pass a 0-based index */
    /* +1 to have a 1-based result */
    size_t words = cmc_bidx_to_widx(n_bits - 1) + 1;

    /* Be sure we are not always shrinking when we just want to make */
    /* sure that we have enough size for n_bits */
    if (!do_resize && words <= _bitset_->capacity)
        return true;

    cmc_bitset_word *new_buffer = realloc(_bitset_->buffer, words * sizeof(cmc_bitset_word));

    if (!new_buffer)
    {
        _bitset_->flag = CMC_FLAG_ALLOC;
        return false;
    }

    _bitset_->buffer = new_buffer;

    /* Make new bits be all zeroes */
    if (_bitset_->capacity < words)
        memset(_bitset_->buffer + _bitset_->capacity, 0, (words - _bitset_->capacity) * sizeof(cmc_bitset_word));

    _bitset_->capacity = words;

    CMC_CALLBACKS_CALL(_bitset_);

    _bitset_->flag = CMC_FLAG_OK;

    return true;
}
