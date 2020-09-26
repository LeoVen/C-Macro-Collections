
#ifndef CMC_CMC_HASHMULTIMAP_TEST_H
#define CMC_CMC_HASHMULTIMAP_TEST_H

#include "macro_collections.h"

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

#endif /* CMC_CMC_HASHMULTIMAP_TEST_H */
