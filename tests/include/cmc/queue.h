
#ifndef CMC_CMC_QUEUE_TEST_H
#define CMC_CMC_QUEUE_TEST_H

#include "macro_collections.h"

struct queue
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    size_t front;
    size_t back;
    int flag;
    struct queue_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct queue_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct queue_iter
{
    struct queue *target;
    size_t cursor;
    size_t index;
    _Bool start;
    _Bool end;
};
struct queue *q_new(size_t capacity, struct queue_fval *f_val);
struct queue *q_new_custom(size_t capacity, struct queue_fval *f_val, struct cmc_alloc_node *alloc,
                           struct cmc_callbacks *callbacks);
void q_clear(struct queue *_queue_);
void q_free(struct queue *_queue_);
void q_customize(struct queue *_queue_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks);
_Bool q_enqueue(struct queue *_queue_, size_t value);
_Bool q_dequeue(struct queue *_queue_);
size_t q_peek(struct queue *_queue_);
_Bool q_contains(struct queue *_queue_, size_t value);
_Bool q_empty(struct queue *_queue_);
_Bool q_full(struct queue *_queue_);
size_t q_count(struct queue *_queue_);
size_t q_capacity(struct queue *_queue_);
int q_flag(struct queue *_queue_);
_Bool q_resize(struct queue *_queue_, size_t capacity);
struct queue *q_copy_of(struct queue *_queue_);
_Bool q_equals(struct queue *_queue1_, struct queue *_queue2_);
struct queue_iter q_iter_start(struct queue *target);
struct queue_iter q_iter_end(struct queue *target);
_Bool q_iter_at_start(struct queue_iter *iter);
_Bool q_iter_at_end(struct queue_iter *iter);
_Bool q_iter_to_start(struct queue_iter *iter);
_Bool q_iter_to_end(struct queue_iter *iter);
_Bool q_iter_next(struct queue_iter *iter);
_Bool q_iter_prev(struct queue_iter *iter);
_Bool q_iter_advance(struct queue_iter *iter, size_t steps);
_Bool q_iter_rewind(struct queue_iter *iter, size_t steps);
_Bool q_iter_go_to(struct queue_iter *iter, size_t index);
size_t q_iter_value(struct queue_iter *iter);
size_t *q_iter_rvalue(struct queue_iter *iter);
size_t q_iter_index(struct queue_iter *iter);
_Bool q_to_string(struct queue *_queue_, FILE *fptr);
_Bool q_print(struct queue *_queue_, FILE *fptr, const char *start, const char *separator, const char *end);

#endif /* CMC_CMC_QUEUE_TEST_H */
