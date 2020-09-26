
#ifndef CMC_CMC_STACK_TEST_H
#define CMC_CMC_STACK_TEST_H

#include "macro_collections.h"

struct stack
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    int flag;
    struct stack_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct stack_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct stack *s_new(size_t capacity, struct stack_fval *f_val);
struct stack *s_new_custom(size_t capacity, struct stack_fval *f_val, struct cmc_alloc_node *alloc,
                           struct cmc_callbacks *callbacks);
void s_clear(struct stack *_stack_);
void s_free(struct stack *_stack_);
void s_customize(struct stack *_stack_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool s_push(struct stack *_stack_, size_t value);
_Bool s_pop(struct stack *_stack_);
size_t s_top(struct stack *_stack_);
_Bool s_contains(struct stack *_stack_, size_t value);
_Bool s_empty(struct stack *_stack_);
_Bool s_full(struct stack *_stack_);
size_t s_count(struct stack *_stack_);
size_t s_capacity(struct stack *_stack_);
int s_flag(struct stack *_stack_);
_Bool s_resize(struct stack *_stack_, size_t capacity);
struct stack *s_copy_of(struct stack *_stack_);
_Bool s_equals(struct stack *_stack1_, struct stack *_stack2_);
struct stack_iter
{
    struct stack *target;
    size_t cursor;
    _Bool start;
    _Bool end;
};
struct stack_iter s_iter_start(struct stack *target);
struct stack_iter s_iter_end(struct stack *target);
_Bool s_iter_at_start(struct stack_iter *iter);
_Bool s_iter_at_end(struct stack_iter *iter);
_Bool s_iter_to_start(struct stack_iter *iter);
_Bool s_iter_to_end(struct stack_iter *iter);
_Bool s_iter_next(struct stack_iter *iter);
_Bool s_iter_prev(struct stack_iter *iter);
_Bool s_iter_advance(struct stack_iter *iter, size_t steps);
_Bool s_iter_rewind(struct stack_iter *iter, size_t steps);
_Bool s_iter_go_to(struct stack_iter *iter, size_t index);
size_t s_iter_value(struct stack_iter *iter);
size_t *s_iter_rvalue(struct stack_iter *iter);
size_t s_iter_index(struct stack_iter *iter);
_Bool s_to_string(struct stack *_stack_, FILE *fptr);
_Bool s_print(struct stack *_stack_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_CMC_STACK_TEST_H */
