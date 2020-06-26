
#ifndef CMC_CMC_HEAP_TEST_H
#define CMC_CMC_HEAP_TEST_H

#include "macro_collections.h"

struct heap
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    enum cmc_heap_order HO;
    int flag;
    struct heap_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct heap_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct heap *h_new(size_t capacity, enum cmc_heap_order HO, struct heap_fval *f_val);
struct heap *h_new_custom(size_t capacity, enum cmc_heap_order HO, struct heap_fval *f_val,
                          struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
void h_clear(struct heap *_heap_);
void h_free(struct heap *_heap_);
void h_customize(struct heap *_heap_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool h_insert(struct heap *_heap_, size_t value);
_Bool h_remove(struct heap *_heap_);
size_t h_peek(struct heap *_heap_);
_Bool h_contains(struct heap *_heap_, size_t value);
_Bool h_empty(struct heap *_heap_);
_Bool h_full(struct heap *_heap_);
size_t h_count(struct heap *_heap_);
size_t h_capacity(struct heap *_heap_);
int h_flag(struct heap *_heap_);
_Bool h_resize(struct heap *_heap_, size_t capacity);
struct heap *h_copy_of(struct heap *_heap_);
_Bool h_equals(struct heap *_heap1_, struct heap *_heap2_);
struct heap_iter
{
    struct heap *target;
    size_t cursor;
    _Bool start;
    _Bool end;
};
struct heap_iter h_iter_start(struct heap *target);
struct heap_iter h_iter_end(struct heap *target);
_Bool h_iter_at_start(struct heap_iter *iter);
_Bool h_iter_at_end(struct heap_iter *iter);
_Bool h_iter_to_start(struct heap_iter *iter);
_Bool h_iter_to_end(struct heap_iter *iter);
_Bool h_iter_next(struct heap_iter *iter);
_Bool h_iter_prev(struct heap_iter *iter);
_Bool h_iter_advance(struct heap_iter *iter, size_t steps);
_Bool h_iter_rewind(struct heap_iter *iter, size_t steps);
_Bool h_iter_go_to(struct heap_iter *iter, size_t index);
size_t h_iter_value(struct heap_iter *iter);
size_t h_iter_index(struct heap_iter *iter);
_Bool h_to_string(struct heap *_heap_, FILE *fptr);
_Bool h_print(struct heap *_heap_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_CMC_HEAP_TEST_H */
