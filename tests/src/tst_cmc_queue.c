
#include "tst_cmc_queue.h"

struct queue *q_new(size_t capacity, struct queue_fval *f_val)
{
    return q_new_custom(capacity, f_val, ((void *)0), ((void *)0));
}
struct queue *q_new_custom(size_t capacity, struct queue_fval *f_val, struct cmc_alloc_node *alloc,
                           struct cmc_callbacks *callbacks)
{
    ;
    if (capacity < 1)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct queue *_queue_ = alloc->malloc(sizeof(struct queue));
    if (!_queue_)
        return ((void *)0);
    _queue_->buffer = alloc->calloc(capacity, sizeof(size_t));
    if (!_queue_->buffer)
    {
        alloc->free(_queue_);
        return ((void *)0);
    }
    _queue_->capacity = capacity;
    _queue_->count = 0;
    _queue_->front = 0;
    _queue_->back = 0;
    _queue_->flag = CMC_FLAG_OK;
    _queue_->f_val = f_val;
    _queue_->alloc = alloc;
    (_queue_)->callbacks = callbacks;
    return _queue_;
}
void q_clear(struct queue *_queue_)
{
    if (_queue_->f_val->free)
    {
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
        {
            _queue_->f_val->free(_queue_->buffer[i]);
            i = (i + 1) % _queue_->capacity;
        }
    }
    memset(_queue_->buffer, 0, sizeof(size_t) * _queue_->capacity);
    _queue_->count = 0;
    _queue_->front = 0;
    _queue_->back = 0;
    _queue_->flag = CMC_FLAG_OK;
}
void q_free(struct queue *_queue_)
{
    if (_queue_->f_val->free)
    {
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
        {
            _queue_->f_val->free(_queue_->buffer[i]);
            i = (i + 1) % _queue_->capacity;
        }
    }
    _queue_->alloc->free(_queue_->buffer);
    _queue_->alloc->free(_queue_);
}
void q_customize(struct queue *_queue_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    ;
    if (!alloc)
        _queue_->alloc = &cmc_alloc_node_default;
    else
        _queue_->alloc = alloc;
    (_queue_)->callbacks = callbacks;
    _queue_->flag = CMC_FLAG_OK;
}
_Bool q_enqueue(struct queue *_queue_, size_t value)
{
    if (q_full(_queue_))
    {
        if (!q_resize(_queue_, _queue_->capacity * 2))
            return 0;
    }
    _queue_->buffer[_queue_->back] = value;
    _queue_->back = (_queue_->back == _queue_->capacity - 1) ? 0 : _queue_->back + 1;
    _queue_->count++;
    _queue_->flag = CMC_FLAG_OK;
    if ((_queue_)->callbacks && (_queue_)->callbacks->create)
        (_queue_)->callbacks->create();
    ;
    return 1;
}
_Bool q_dequeue(struct queue *_queue_)
{
    if (q_empty(_queue_))
    {
        _queue_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    _queue_->buffer[_queue_->front] = (size_t){ 0 };
    _queue_->front = (_queue_->front == _queue_->capacity - 1) ? 0 : _queue_->front + 1;
    _queue_->count--;
    _queue_->flag = CMC_FLAG_OK;
    if ((_queue_)->callbacks && (_queue_)->callbacks->delete)
        (_queue_)->callbacks->delete ();
    ;
    return 1;
}
size_t q_peek(struct queue *_queue_)
{
    if (q_empty(_queue_))
    {
        _queue_->flag = CMC_FLAG_EMPTY;
        return (size_t){ 0 };
    }
    _queue_->flag = CMC_FLAG_OK;
    if ((_queue_)->callbacks && (_queue_)->callbacks->read)
        (_queue_)->callbacks->read();
    ;
    return _queue_->buffer[_queue_->front];
}
_Bool q_contains(struct queue *_queue_, size_t value)
{
    _queue_->flag = CMC_FLAG_OK;
    _Bool result = 0;
    for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
    {
        if (_queue_->f_val->cmp(_queue_->buffer[i], value) == 0)
        {
            result = 1;
            break;
        }
        i = (i + 1) % _queue_->capacity;
    }
    if ((_queue_)->callbacks && (_queue_)->callbacks->read)
        (_queue_)->callbacks->read();
    ;
    return result;
}
_Bool q_empty(struct queue *_queue_)
{
    return _queue_->count == 0;
}
_Bool q_full(struct queue *_queue_)
{
    return _queue_->count >= _queue_->capacity;
}
size_t q_count(struct queue *_queue_)
{
    return _queue_->count;
}
size_t q_capacity(struct queue *_queue_)
{
    return _queue_->capacity;
}
int q_flag(struct queue *_queue_)
{
    return _queue_->flag;
}
_Bool q_resize(struct queue *_queue_, size_t capacity)
{
    if (_queue_->capacity == capacity)
        goto success;
    if (capacity < _queue_->count)
    {
        _queue_->flag = CMC_FLAG_INVALID;
        return 0;
    }
    size_t *new_buffer = _queue_->alloc->malloc(sizeof(size_t) * capacity);
    if (!new_buffer)
    {
        _queue_->flag = CMC_FLAG_ALLOC;
        return 0;
    }
    for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
    {
        new_buffer[j] = _queue_->buffer[i];
        i = (i + 1) % _queue_->capacity;
    }
    _queue_->alloc->free(_queue_->buffer);
    _queue_->buffer = new_buffer;
    _queue_->capacity = capacity;
    _queue_->front = 0;
    _queue_->back = _queue_->count;
success:
    _queue_->flag = CMC_FLAG_OK;
    if ((_queue_)->callbacks && (_queue_)->callbacks->resize)
        (_queue_)->callbacks->resize();
    ;
    return 1;
}
struct queue *q_copy_of(struct queue *_queue_)
{
    struct queue *result = q_new_custom(_queue_->capacity, _queue_->f_val, _queue_->alloc, ((void *)0));
    if (!result)
    {
        _queue_->flag = CMC_FLAG_ERROR;
        return ((void *)0);
    }
    (result)->callbacks = _queue_->callbacks;
    if (_queue_->f_val->cpy)
    {
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
        {
            result->buffer[j] = _queue_->f_val->cpy(_queue_->buffer[i]);
            i = (i + 1) % _queue_->capacity;
        }
    }
    else
    {
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
        {
            result->buffer[j] = _queue_->buffer[i];
            i = (i + 1) % _queue_->capacity;
        }
    }
    result->count = _queue_->count;
    result->front = 0;
    result->back = _queue_->count;
    _queue_->flag = CMC_FLAG_OK;
    return result;
}
_Bool q_equals(struct queue *_queue1_, struct queue *_queue2_)
{
    _queue1_->flag = CMC_FLAG_OK;
    _queue2_->flag = CMC_FLAG_OK;
    if (_queue1_->count != _queue2_->count)
        return 0;
    size_t i, j, k;
    for (i = _queue1_->front, j = _queue2_->front, k = 0; k < _queue1_->count; k++)
    {
        if (_queue1_->f_val->cmp(_queue1_->buffer[i], _queue2_->buffer[j]) != 0)
            return 0;
        i = (i + 1) % _queue1_->capacity;
        j = (j + 1) % _queue2_->capacity;
    }
    return 1;
}
struct queue_iter q_iter_start(struct queue *target)
{
    struct queue_iter iter;
    iter.target = target;
    iter.cursor = target->front;
    iter.index = 0;
    iter.start = 1;
    iter.end = q_empty(target);
    return iter;
}
struct queue_iter q_iter_end(struct queue *target)
{
    struct queue_iter iter;
    iter.target = target;
    if (!q_empty(target))
    {
        if (iter.target->back == 0)
            iter.cursor = iter.target->capacity - 1;
        else
            iter.cursor = iter.target->back - 1;
        iter.index = iter.target->count - 1;
    }
    else
    {
        iter.cursor = 0;
        iter.index = 0;
    }
    iter.start = q_empty(target);
    iter.end = 1;
    return iter;
}
_Bool q_iter_at_start(struct queue_iter *iter)
{
    return q_empty(iter->target) || iter->start;
}
_Bool q_iter_at_end(struct queue_iter *iter)
{
    return q_empty(iter->target) || iter->end;
}
_Bool q_iter_to_start(struct queue_iter *iter)
{
    if (!q_empty(iter->target))
    {
        iter->cursor = iter->target->front;
        iter->index = 0;
        iter->start = 1;
        iter->end = 0;
        return 1;
    }
    return 0;
}
_Bool q_iter_to_end(struct queue_iter *iter)
{
    if (!q_empty(iter->target))
    {
        if (iter->target->back == 0)
            iter->cursor = iter->target->capacity - 1;
        else
            iter->cursor = iter->target->back - 1;
        iter->index = iter->target->count - 1;
        iter->start = 0;
        iter->end = 1;
        return 1;
    }
    return 0;
}
_Bool q_iter_next(struct queue_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = q_empty(iter->target);
    iter->cursor = (iter->cursor + 1) % (iter->target->capacity);
    iter->index++;
    return 1;
}
_Bool q_iter_prev(struct queue_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = q_empty(iter->target);
    iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1 : iter->cursor - 1;
    iter->index--;
    return 1;
}
_Bool q_iter_advance(struct queue_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == iter->target->count)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= iter->target->count)
        return 0;
    iter->start = q_empty(iter->target);
    iter->index += steps;
    iter->cursor = (iter->cursor + steps) % iter->target->capacity;
    return 1;
}
_Bool q_iter_rewind(struct queue_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->index == 0)
    {
        iter->start = 1;
        return 0;
    }
    if (steps == 0 || iter->index < steps)
        return 0;
    iter->end = q_empty(iter->target);
    iter->index -= steps;
    if (iter->cursor < steps)
        iter->cursor += iter->target->capacity;
    iter->cursor -= steps;
    return 1;
}
_Bool q_iter_go_to(struct queue_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return q_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return q_iter_advance(iter, index - iter->index);
    return 1;
}
size_t q_iter_value(struct queue_iter *iter)
{
    if (q_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor];
}
size_t *q_iter_rvalue(struct queue_iter *iter)
{
    if (q_empty(iter->target))
        return ((void *)0);
    return &(iter->target->buffer[iter->cursor]);
}
size_t q_iter_index(struct queue_iter *iter)
{
    return iter->index;
}
_Bool q_to_string(struct queue *_queue_, FILE *fptr)
{
    struct queue *q_ = _queue_;
    return 0 <= fprintf(fptr,
                        "struct %s<%s> "
                        "at %p { "
                        "buffer:%p, "
                        "capacity:%"
                        "I64u"
                        ", "
                        "count:%"
                        "I64u"
                        ", "
                        "front:%"
                        "I64u"
                        ", "
                        "back:%"
                        "I64u"
                        ", "
                        "flag:%d, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        "queue", "size_t", q_, q_->buffer, q_->capacity, q_->count, q_->front, q_->back, q_->flag,
                        q_->f_val, q_->alloc, (q_)->callbacks);
}
_Bool q_print(struct queue *_queue_, FILE *fptr, const char *start, const char *separator, const char *end)
{
    fprintf(fptr, "%s", start);
    for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
    {
        if (!_queue_->f_val->str(fptr, _queue_->buffer[i]))
            return 0;
        i = (i + 1) % _queue_->capacity;
        if (j + 1 < _queue_->count)
            fprintf(fptr, "%s", separator);
    }
    fprintf(fptr, "%s", end);
    return 1;
}
