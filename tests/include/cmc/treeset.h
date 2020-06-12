
#ifndef CMC_CMC_TREESET_TEST_H
#define CMC_CMC_TREESET_TEST_H

#include "macro_collections.h"

struct treeset
{
    struct treeset_node *root;
    size_t count;
    int flag;
    struct treeset_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct treeset_node
{
    size_t value;
    unsigned char height;
    struct treeset_node *right;
    struct treeset_node *left;
    struct treeset_node *parent;
};
struct treeset_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct treeset_iter
{
    struct treeset *target;
    struct treeset_node *cursor;
    struct treeset_node *first;
    struct treeset_node *last;
    size_t index;
    _Bool start;
    _Bool end;
};
struct treeset *ts_new(struct treeset_fval *f_val);
struct treeset *ts_new_custom(struct treeset_fval *f_val, struct cmc_alloc_node *alloc,
                              struct cmc_callbacks *callbacks);
void ts_clear(struct treeset *_set_);
void ts_free(struct treeset *_set_);
void ts_customize(struct treeset *_set_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool ts_insert(struct treeset *_set_, size_t value);
_Bool ts_remove(struct treeset *_set_, size_t value);
_Bool ts_max(struct treeset *_set_, size_t *value);
_Bool ts_min(struct treeset *_set_, size_t *value);
_Bool ts_contains(struct treeset *_set_, size_t value);
_Bool ts_empty(struct treeset *_set_);
size_t ts_count(struct treeset *_set_);
int ts_flag(struct treeset *_set_);
struct treeset *ts_copy_of(struct treeset *_set_);
_Bool ts_equals(struct treeset *_set1_, struct treeset *_set2_);
struct treeset *ts_union(struct treeset *_set1_, struct treeset *_set2_);
struct treeset *ts_intersection(struct treeset *_set1_, struct treeset *_set2_);
struct treeset *ts_difference(struct treeset *_set1_, struct treeset *_set2_);
struct treeset *ts_symmetric_difference(struct treeset *_set1_, struct treeset *_set2_);
_Bool ts_is_subset(struct treeset *_set1_, struct treeset *_set2_);
_Bool ts_is_superset(struct treeset *_set1_, struct treeset *_set2_);
_Bool ts_is_proper_subset(struct treeset *_set1_, struct treeset *_set2_);
_Bool ts_is_proper_superset(struct treeset *_set1_, struct treeset *_set2_);
_Bool ts_is_disjointset(struct treeset *_set1_, struct treeset *_set2_);
struct treeset_iter ts_iter_start(struct treeset *target);
struct treeset_iter ts_iter_end(struct treeset *target);
_Bool ts_iter_at_start(struct treeset_iter *iter);
_Bool ts_iter_at_end(struct treeset_iter *iter);
_Bool ts_iter_to_start(struct treeset_iter *iter);
_Bool ts_iter_to_end(struct treeset_iter *iter);
_Bool ts_iter_next(struct treeset_iter *iter);
_Bool ts_iter_prev(struct treeset_iter *iter);
_Bool ts_iter_advance(struct treeset_iter *iter, size_t steps);
_Bool ts_iter_rewind(struct treeset_iter *iter, size_t steps);
_Bool ts_iter_go_to(struct treeset_iter *iter, size_t index);
size_t ts_iter_value(struct treeset_iter *iter);
size_t ts_iter_index(struct treeset_iter *iter);
_Bool ts_to_string(struct treeset *_set_, FILE *fptr);
_Bool ts_print(struct treeset *_set_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_CMC_TREESET_TEST_H */
