
#ifndef CMC_CMC_BITSET_TEST_H
#define CMC_CMC_BITSET_TEST_H

#include "macro_collections.h"

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
    size_t cursor;
    _Bool start;
    _Bool end;
};
struct bitset *bs_new(size_t n_bits);
struct bitset *bs_new_custom(size_t n_bits, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
struct bitset bs_init(size_t n_bits);
struct bitset bs_init_custom(size_t n_bits, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
void bs_free(struct bitset *_bitset_);
void bs_release(struct bitset _bitset_);
void bs_customize(struct bitset *_bitset_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool bs_set(struct bitset *_bitset_, size_t bit_index);
_Bool bs_set_range(struct bitset *_bitset_, size_t from, size_t to);
_Bool bs_clear(struct bitset *_bitset_, size_t bit_index);
_Bool bs_clear_range(struct bitset *_bitset_, size_t from, size_t to);
_Bool bs_flip(struct bitset *_bitset_, size_t bit_index);
_Bool bs_flip_range(struct bitset *_bitset_, size_t from, size_t to);
_Bool bs_put(struct bitset *_bitset_, size_t bit_index, _Bool state);
_Bool bs_put_range(struct bitset *_bitset_, size_t from, size_t to, _Bool state);
_Bool bs_set_all(struct bitset *_bitset_);
_Bool bs_clear_all(struct bitset *_bitset_);
_Bool bs_flip_all(struct bitset *_bitset_);
_Bool bs_get(struct bitset *_bitset_, size_t bit_index);
_Bool bs_resize(struct bitset *_bitset_, size_t n_bits);
static inline size_t bs_bit_to_index(size_t idx)
{
    static const size_t shift =
        ((sizeof(cmc_bitset_word) * 8) >> 6) > 0
            ? 6
            : ((sizeof(cmc_bitset_word) * 8) >> 5) > 0 ? 5 : ((sizeof(cmc_bitset_word) * 8) >> 4) > 0 ? 4 : 3;
    return idx >> shift;
}

#endif /* CMC_CMC_BITSET_TEST_H */
