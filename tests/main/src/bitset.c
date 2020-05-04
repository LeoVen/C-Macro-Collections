#include "cmc/bitset.h"

struct bitset
{
    cmc_bitset_word *buffer;
    size_t capacity;
    int flag;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct bitset_iter
{
    struct bitset *target;
    cmc_bitset_word cursor;
    _Bool start;
    _Bool end;
};
struct bitset *bs_new(size_t n_bits);
struct bitset *bs_new_custom(size_t n_bits, struct cmc_alloc_node *alloc,
                             struct cmc_callbacks *callbacks);
struct bitset bs_init(size_t n_bits);
struct bitset bs_init_custom(size_t n_bits, struct cmc_alloc_node *alloc,
                             struct cmc_callbacks *callbacks);
void bs_free(struct bitset *_bitset_);
void bs_release(struct bitset _bitset_);
void bs_customize(struct bitset *_bitset_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks);
_Bool bs_set(struct bitset *_bitset_, size_t bit_index);
_Bool bs_set_range(struct bitset *_bitset_, size_t from, size_t to);
_Bool bs_clear(struct bitset *_bitset_, size_t bit_index);
_Bool bs_clear_range(struct bitset *_bitset_, size_t from, size_t to);
_Bool bs_flip(struct bitset *_bitset_, size_t bit_index);
_Bool bs_flip_range(struct bitset *_bitset_, size_t from, size_t to);
_Bool bs_put(struct bitset *_bitset_, size_t bit_index);
_Bool bs_put_range(struct bitset *_bitset_, size_t from, size_t to);
_Bool bs_fill(struct bitset *_bitset_);
_Bool bs_empty(struct bitset *_bitset_);
_Bool bs_get(struct bitset *_bitset_, size_t bit_index);
_Bool bs_resize(struct bitset *_bitset_, size_t n_bits);
static inline size_t bs_bit_to_index(size_t idx)
{
    static const size_t shift =
        ((sizeof(cmc_bitset_word) * 8) >> 6) > 0
            ? 6
            : ((sizeof(cmc_bitset_word) * 8) >> 5) > 0
                  ? 5
                  : ((sizeof(cmc_bitset_word) * 8) >> 4) > 0 ? 4 : 3;
    return idx >> shift;
}
_Bool bs_impl_resize(struct bitset *_bitset_, size_t n_bits, _Bool do_resize);
struct bitset *bs_new(size_t n_bits)
{
    return bs_new_custom(n_bits, ((void *)0), ((void *)0));
}
struct bitset *bs_new_custom(size_t n_bits, struct cmc_alloc_node *alloc,
                             struct cmc_callbacks *callbacks)
{
    if (n_bits < 1)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    size_t capacity = bs_bit_to_index(n_bits - 1) + 1;
    struct bitset *_bitset_ = alloc->malloc(sizeof(struct bitset));
    if (!_bitset_)
        return ((void *)0);
    _bitset_->buffer = alloc->calloc(capacity, sizeof(cmc_bitset_word));
    if (!_bitset_->buffer)
    {
        alloc->free(_bitset_);
        return ((void *)0);
    }
    _bitset_->capacity = capacity;
    _bitset_->flag = cmc_flags.OK;
    _bitset_->alloc = alloc;
    _bitset_->callbacks = callbacks;
    return _bitset_;
}
struct bitset bs_init(size_t n_bits)
{
    return bs_init_custom(n_bits, ((void *)0), ((void *)0));
}
struct bitset bs_init_custom(size_t n_bits, struct cmc_alloc_node *alloc,
                             struct cmc_callbacks *callbacks)
{
    struct bitset _bitset_ = { 0 };
    if (n_bits < 1)
        return _bitset_;
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    size_t capacity = bs_bit_to_index(n_bits - 1) + 1;
    _bitset_.buffer = alloc->calloc(capacity, sizeof(cmc_bitset_word));
    if (!_bitset_.buffer)
        return _bitset_;
    _bitset_.capacity = capacity;
    _bitset_.flag = cmc_flags.OK;
    _bitset_.alloc = alloc;
    _bitset_.callbacks = callbacks;
    return _bitset_;
}
void bs_free(struct bitset *_bitset_)
{
    free(_bitset_->buffer);
    free(_bitset_);
}
void bs_release(struct bitset _bitset_)
{
    free(_bitset_.buffer);
}
void bs_customize(struct bitset *_bitset_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _bitset_->alloc = &cmc_alloc_node_default;
    else
        _bitset_->alloc = alloc;
    _bitset_->callbacks = callbacks;
    _bitset_->flag = cmc_flags.OK;
}
_Bool bs_set(struct bitset *_bitset_, size_t bit_index)
{
    if (!bs_impl_resize(_bitset_, bit_index + 1, 0))
        return 0;
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * 8;
    size_t i = bs_bit_to_index(bit_index);
    _bitset_->buffer[i] |= ((cmc_bitset_word)1) << (bit_index % bits);
    _bitset_->flag = cmc_flags.OK;
    if (_bitset_->callbacks && _bitset_->callbacks->create)
        _bitset_->callbacks->create();
    return 1;
}
_Bool bs_set_range(struct bitset *_bitset_, size_t from, size_t to)
{
    if (to < from)
    {
        _bitset_->flag = cmc_flags.INVALID;
        return 0;
    }
    if (!bs_impl_resize(_bitset_, to + 1, 0))
        return 0;
    size_t start_index = bs_bit_to_index(from);
    size_t end_index = bs_bit_to_index(to);
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * 8;
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
    _bitset_->flag = cmc_flags.OK;
    if (_bitset_->callbacks && _bitset_->callbacks->create)
        _bitset_->callbacks->create();
    return 1;
}
_Bool bs_clear(struct bitset *_bitset_, size_t bit_index)
{
    if (!bs_impl_resize(_bitset_, bit_index + 1, 0))
        return 0;
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * 8;
    size_t i = bs_bit_to_index(bit_index);
    _bitset_->buffer[i] &= ~(((cmc_bitset_word)1) << (bit_index % bits));
    _bitset_->flag = cmc_flags.OK;
    if (_bitset_->callbacks && _bitset_->callbacks->delete)
        _bitset_->callbacks->delete ();
    return 1;
}
_Bool bs_clear_range(struct bitset *_bitset_, size_t from, size_t to)
{
    if (to < from)
    {
        _bitset_->flag = cmc_flags.INVALID;
        return 0;
    }
    if (!bs_impl_resize(_bitset_, to + 1, 0))
        return 0;
    size_t start_index = bs_bit_to_index(from);
    size_t end_index = bs_bit_to_index(to);
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * 8;
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
        for (size_t i = start_index; i < end_index; i++)
            _bitset_->buffer[i] = 0;
        _bitset_->buffer[end_index] &= ~end_mask;
    }
    _bitset_->flag = cmc_flags.OK;
    if (_bitset_->callbacks && _bitset_->callbacks->delete)
        _bitset_->callbacks->delete ();
    return 1;
}
_Bool bs_flip(struct bitset *_bitset_, size_t bit_index)
{
    return 1;
}
_Bool bs_flip_range(struct bitset *_bitset_, size_t from, size_t to)
{
    return 1;
}
_Bool bs_put(struct bitset *_bitset_, size_t bit_index)
{
    return 1;
}
_Bool bs_put_range(struct bitset *_bitset_, size_t from, size_t to)
{
    return 1;
}
_Bool bs_fill(struct bitset *_bitset_)
{
    return 1;
}
_Bool bs_empty(struct bitset *_bitset_)
{
    return 1;
}
_Bool bs_get(struct bitset *_bitset_, size_t bit_index)
{
    const cmc_bitset_word bits = sizeof(cmc_bitset_word) * 8;
    cmc_bitset_word w = _bitset_->buffer[bs_bit_to_index(bit_index)];
    return w & ((cmc_bitset_word)1 << (bit_index % bits));
}
_Bool bs_resize(struct bitset *_bitset_, size_t n_bits)
{
    return bs_impl_resize(_bitset_, n_bits, 1);
}
_Bool bs_impl_resize(struct bitset *_bitset_, size_t n_bits, _Bool do_resize)
{
    if (n_bits == 0)
    {
        _bitset_->flag = cmc_flags.INVALID;
        return 0;
    }
    size_t words = bs_bit_to_index(n_bits - 1) + 1;
    if (!do_resize && words <= _bitset_->capacity)
        return 1;
    cmc_bitset_word *new_buffer =
        realloc(_bitset_->buffer, words * sizeof(cmc_bitset_word));
    if (!new_buffer)
    {
        _bitset_->flag = cmc_flags.ALLOC;
        return 0;
    }
    _bitset_->buffer = new_buffer;
    if (_bitset_->capacity < words)
        memset(_bitset_->buffer + _bitset_->capacity, 0,
               (words - _bitset_->capacity) * sizeof(cmc_bitset_word));
    _bitset_->capacity = words;
    if (_bitset_->callbacks && _bitset_->callbacks->resize)
        _bitset_->callbacks->resize();
    _bitset_->flag = cmc_flags.OK;
    return 1;
}
