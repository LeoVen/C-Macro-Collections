
#ifndef CMC_CMC_HASHMAP_TEST_H
#define CMC_CMC_HASHMAP_TEST_H

#include "macro_collections.h"

struct hashmap
{
    struct hashmap_entry *buffer;
    size_t capacity;
    size_t count;
    double load;
    int flag;
    struct hashmap_fkey *f_key;
    struct hashmap_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct hashmap_entry
{
    size_t key;
    size_t value;
    size_t dist;
    enum cmc_entry_state state;
};
struct hashmap_fkey
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashmap_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct hashmap *hm_new(size_t capacity, double load, struct hashmap_fkey *f_key, struct hashmap_fval *f_val);
struct hashmap *hm_new_custom(size_t capacity, double load, struct hashmap_fkey *f_key, struct hashmap_fval *f_val,
                              struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
void hm_clear(struct hashmap *_map_);
void hm_free(struct hashmap *_map_);
void hm_customize(struct hashmap *_map_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool hm_insert(struct hashmap *_map_, size_t key, size_t value);
_Bool hm_update(struct hashmap *_map_, size_t key, size_t new_value, size_t *old_value);
_Bool hm_remove(struct hashmap *_map_, size_t key, size_t *out_value);
_Bool hm_max(struct hashmap *_map_, size_t *key, size_t *value);
_Bool hm_min(struct hashmap *_map_, size_t *key, size_t *value);
size_t hm_get(struct hashmap *_map_, size_t key);
size_t *hm_get_ref(struct hashmap *_map_, size_t key);
_Bool hm_contains(struct hashmap *_map_, size_t key);
_Bool hm_empty(struct hashmap *_map_);
_Bool hm_full(struct hashmap *_map_);
size_t hm_count(struct hashmap *_map_);
size_t hm_capacity(struct hashmap *_map_);
double hm_load(struct hashmap *_map_);
int hm_flag(struct hashmap *_map_);
_Bool hm_resize(struct hashmap *_map_, size_t capacity);
struct hashmap *hm_copy_of(struct hashmap *_map_);
_Bool hm_equals(struct hashmap *_map1_, struct hashmap *_map2_);
struct hashmap hm_init(size_t capacity, double load, struct hashmap_fkey *f_key, struct hashmap_fval *f_val);
struct hashmap hm_init_custom(size_t capacity, double load, struct hashmap_fkey *f_key, struct hashmap_fval *f_val,
                              struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
void hm_release(struct hashmap _map_);
struct hashmap_iter
{
    struct hashmap *target;
    size_t cursor;
    size_t index;
    size_t first;
    size_t last;
    _Bool start;
    _Bool end;
};
struct hashmap_iter hm_iter_start(struct hashmap *target);
struct hashmap_iter hm_iter_end(struct hashmap *target);
_Bool hm_iter_at_start(struct hashmap_iter *iter);
_Bool hm_iter_at_end(struct hashmap_iter *iter);
_Bool hm_iter_to_start(struct hashmap_iter *iter);
_Bool hm_iter_to_end(struct hashmap_iter *iter);
_Bool hm_iter_next(struct hashmap_iter *iter);
_Bool hm_iter_prev(struct hashmap_iter *iter);
_Bool hm_iter_advance(struct hashmap_iter *iter, size_t steps);
_Bool hm_iter_rewind(struct hashmap_iter *iter, size_t steps);
_Bool hm_iter_go_to(struct hashmap_iter *iter, size_t index);
size_t hm_iter_key(struct hashmap_iter *iter);
size_t hm_iter_value(struct hashmap_iter *iter);
size_t *hm_iter_rvalue(struct hashmap_iter *iter);
size_t hm_iter_index(struct hashmap_iter *iter);
_Bool hm_to_string(struct hashmap *_map_, FILE *fptr);
_Bool hm_print(struct hashmap *_map_, FILE *fptr, const char *start, const char *separator, const char *end,
               const char *key_val_sep);

#endif /* CMC_CMC_HASHMAP_TEST_H */
