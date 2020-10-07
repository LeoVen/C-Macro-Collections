
#ifndef CMC_CMC_LINKEDLIST_TEST_H
#define CMC_CMC_LINKEDLIST_TEST_H

#include "macro_collections.h"

struct linkedlist
{
    struct linkedlist_node *head;
    struct linkedlist_node *tail;
    size_t count;
    int flag;
    struct linkedlist_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct linkedlist_node
{
    size_t value;
    struct linkedlist_node *next;
    struct linkedlist_node *prev;
};
struct linkedlist_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct linkedlist *ll_new(struct linkedlist_fval *f_val);
struct linkedlist *ll_new_custom(struct linkedlist_fval *f_val, struct cmc_alloc_node *alloc,
                                 struct cmc_callbacks *callbacks);
void ll_clear(struct linkedlist *_list_);
void ll_free(struct linkedlist *_list_);
void ll_customize(struct linkedlist *_list_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool ll_push_front(struct linkedlist *_list_, size_t value);
_Bool ll_push_at(struct linkedlist *_list_, size_t value, size_t index);
_Bool ll_push_back(struct linkedlist *_list_, size_t value);
_Bool ll_pop_front(struct linkedlist *_list_);
_Bool ll_pop_at(struct linkedlist *_list_, size_t index);
_Bool ll_pop_back(struct linkedlist *_list_);
size_t ll_front(struct linkedlist *_list_);
size_t ll_get(struct linkedlist *_list_, size_t index);
size_t *ll_get_ref(struct linkedlist *_list_, size_t index);
size_t ll_back(struct linkedlist *_list_);
_Bool ll_contains(struct linkedlist *_list_, size_t value);
_Bool ll_empty(struct linkedlist *_list_);
size_t ll_count(struct linkedlist *_list_);
int ll_flag(struct linkedlist *_list_);
struct linkedlist *ll_copy_of(struct linkedlist *_list_);
_Bool ll_equals(struct linkedlist *_list1_, struct linkedlist *_list2_);
struct linkedlist_iter
{
    struct linkedlist *target;
    struct linkedlist_node *cursor;
    size_t index;
    _Bool start;
    _Bool end;
};
struct linkedlist_iter ll_iter_start(struct linkedlist *target);
struct linkedlist_iter ll_iter_end(struct linkedlist *target);
_Bool ll_iter_at_start(struct linkedlist_iter *iter);
_Bool ll_iter_at_end(struct linkedlist_iter *iter);
_Bool ll_iter_to_start(struct linkedlist_iter *iter);
_Bool ll_iter_to_end(struct linkedlist_iter *iter);
_Bool ll_iter_next(struct linkedlist_iter *iter);
_Bool ll_iter_prev(struct linkedlist_iter *iter);
_Bool ll_iter_advance(struct linkedlist_iter *iter, size_t steps);
_Bool ll_iter_rewind(struct linkedlist_iter *iter, size_t steps);
_Bool ll_iter_go_to(struct linkedlist_iter *iter, size_t index);
size_t ll_iter_value(struct linkedlist_iter *iter);
size_t *ll_iter_rvalue(struct linkedlist_iter *iter);
size_t ll_iter_index(struct linkedlist_iter *iter);
struct linkedlist_node *ll_iter_node(struct linkedlist_iter *iter);
struct linkedlist_node *ll_new_node(struct linkedlist *_list_, size_t value);
void ll_free_node(struct linkedlist *_list_, struct linkedlist_node *_node_);
struct linkedlist_node *ll_head(struct linkedlist *_list_);
struct linkedlist_node *ll_get_node(struct linkedlist *_list_, size_t index);
struct linkedlist_node *ll_tail(struct linkedlist *_list_);
_Bool ll_add_next(struct linkedlist *_owner_, struct linkedlist_node *_node_, size_t value);
_Bool ll_add_prev(struct linkedlist *_owner_, struct linkedlist_node *_node_, size_t value);
_Bool ll_del_next(struct linkedlist *_owner_, struct linkedlist_node *_node_);
_Bool ll_del_curr(struct linkedlist *_owner_, struct linkedlist_node *_node_);
_Bool ll_del_prev(struct linkedlist *_owner_, struct linkedlist_node *_node_);
struct linkedlist_node *ll_next_node(struct linkedlist_node *_node_);
struct linkedlist_node *ll_prev_node(struct linkedlist_node *_node_);
_Bool ll_to_string(struct linkedlist *_list_, FILE *fptr);
_Bool ll_print(struct linkedlist *_list_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_CMC_LINKEDLIST_TEST_H */
