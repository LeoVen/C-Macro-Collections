#include <cmc/queue.h>

//CMC_GENERATE_QUEUE(q, queue, size_t)

typedef struct queue_s
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    size_t front;
    size_t back;
    struct queue_iter_s (*it_start)(struct queue_s *);
    struct queue_iter_s (*it_end)(struct queue_s *);
} queue, *queue_ptr;
typedef struct queue_iter_s
{
    struct queue_s *target;
    size_t cursor;
    size_t index;
    _Bool start;
    _Bool end;
} queue_iter, *queue_iter_ptr;
queue *q_new(size_t capacity);
void q_clear(queue *_queue_, void (*deallocator)(size_t));
void q_free(queue *_queue_, void (*deallocator)(size_t));
_Bool q_enqueue(queue *_queue_, size_t element);
_Bool q_dequeue(queue *_queue_);
size_t q_peek(queue *_queue_);
_Bool q_contains(queue *_queue_, size_t element, int (*comparator)(size_t, size_t));
_Bool q_empty(queue *_queue_);
_Bool q_full(queue *_queue_);
size_t q_count(queue *_queue_);
size_t q_capacity(queue *_queue_);
_Bool q_resize(queue *_queue_, size_t capacity);
queue *q_copy_of(queue *_queue_, size_t (*copy_func)(size_t));
_Bool q_equals(queue *_queue1_, queue *_queue2_, int (*comparator)(size_t, size_t));
struct cmc_string q_to_string(queue *_queue_);
queue_iter *q_iter_new(queue *target);
void q_iter_free(queue_iter *iter);
void q_iter_init(queue_iter *iter, queue *target);
_Bool q_iter_start(queue_iter *iter);
_Bool q_iter_end(queue_iter *iter);
void q_iter_to_start(queue_iter *iter);
void q_iter_to_end(queue_iter *iter);
_Bool q_iter_next(queue_iter *iter);
_Bool q_iter_prev(queue_iter *iter);
_Bool q_iter_advance(queue_iter *iter, size_t steps);
_Bool q_iter_rewind(queue_iter *iter, size_t steps);
_Bool q_iter_go_to(queue_iter *iter, size_t index);
size_t q_iter_value(queue_iter *iter);
size_t *q_iter_rvalue(queue_iter *iter);
size_t q_iter_index(queue_iter *iter);
static queue_iter q_impl_it_start(queue *_queue_);
static queue_iter q_impl_it_end(queue *_queue_);
queue *q_new(size_t capacity)
{
    if (capacity < 1)
        return ((void *)0);
    queue *_queue_ = malloc(sizeof(queue));
    if (!_queue_)
        return ((void *)0);
    _queue_->buffer = malloc(sizeof(size_t) * capacity);
    if (!_queue_->buffer)
    {
        free(_queue_);
        return ((void *)0);
    }
    memset(_queue_->buffer, 0, sizeof(size_t) * capacity);
    _queue_->capacity = capacity;
    _queue_->count = 0;
    _queue_->front = 0;
    _queue_->back = 0;
    _queue_->it_start = q_impl_it_start;
    _queue_->it_end = q_impl_it_end;
    return _queue_;
}
void q_clear(queue *_queue_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
        {
            deallocator(_queue_->buffer[i]);
            i = (i + 1) % _queue_->capacity;
        }
    }
    memset(_queue_->buffer, 0, sizeof(size_t) * _queue_->capacity);
    _queue_->count = 0;
    _queue_->front = 0;
    _queue_->back = 0;
}
void q_free(queue *_queue_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
        {
            deallocator(_queue_->buffer[i]);
            i = (i + 1) % _queue_->capacity;
        }
    }
    free(_queue_->buffer);
    free(_queue_);
}
_Bool q_enqueue(queue *_queue_, size_t element)
{
    if (q_full(_queue_))
    {
        if (!q_resize(_queue_, q_capacity(_queue_) * 2))
            return 0;
    }
    _queue_->buffer[_queue_->back] = element;
    _queue_->back = (_queue_->back == _queue_->capacity - 1) ? 0 : _queue_->back + 1;
    _queue_->count++;
    return 1;
}
_Bool q_dequeue(queue *_queue_)
{
    if (q_empty(_queue_))
        return 0;
    _queue_->buffer[_queue_->front] = (size_t){0};
    _queue_->front = (_queue_->front == _queue_->capacity - 1) ? 0 : _queue_->front + 1;
    _queue_->count--;
    return 1;
}
size_t q_peek(queue *_queue_)
{
    if (q_empty(_queue_))
        return (size_t){0};
    return _queue_->buffer[_queue_->front];
}
_Bool q_contains(queue *_queue_, size_t element, int (*comparator)(size_t, size_t))
{
    for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
    {
        if (comparator(_queue_->buffer[i], element) == 0)
            return 1;
        i = (i + 1) % _queue_->capacity;
    }
    return 0;
}
_Bool q_empty(queue *_queue_) { return _queue_->count == 0; }
_Bool q_full(queue *_queue_) { return _queue_->count >= _queue_->capacity; }
size_t q_count(queue *_queue_) { return _queue_->count; }
size_t q_capacity(queue *_queue_) { return _queue_->capacity; }
_Bool q_resize(queue *_queue_, size_t capacity)
{
    if (q_capacity(_queue_) == capacity)
        return 1;
    if (capacity < q_count(_queue_))
        return 0;
    size_t *new_buffer = malloc(sizeof(size_t) * capacity);
    if (!new_buffer)
        return 0;
    for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
    {
        new_buffer[j] = _queue_->buffer[i];
        i = (i + 1) % q_capacity(_queue_);
    }
    free(_queue_->buffer);
    _queue_->buffer = new_buffer;
    _queue_->capacity = capacity;
    _queue_->front = 0;
    _queue_->back = _queue_->count;
    return 1;
}
queue *q_copy_of(queue *_queue_, size_t (*copy_func)(size_t))
{
    queue *result = q_new(_queue_->capacity);
    if (!result)
        return ((void *)0);
    if (copy_func)
    {
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; j++)
        {
            result->buffer[j] = copy_func(_queue_->buffer[i]);
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
    return result;
}
_Bool q_equals(queue *_queue1_, queue *_queue2_, int (*comparator)(size_t, size_t))
{
    if (q_count(_queue1_) != q_count(_queue2_))
        return 0;
    size_t i, j, k;
    for (i = _queue1_->front, j = _queue2_->front, k = 0; k < q_count(_queue1_); k++)
    {
        if (comparator(_queue1_->buffer[i], _queue2_->buffer[j]) != 0)
            return 0;
        i = (i + 1) % _queue1_->capacity;
        j = (j + 1) % _queue2_->capacity;
    }
    return 1;
}
struct cmc_string q_to_string(queue *_queue_)
{
    struct cmc_string str;
    queue *q_ = _queue_;
    const char *name = "queue";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_queue, name, q_, q_->buffer, q_->capacity, q_->count, q_->front, q_->back);
    return str;
}
queue_iter *q_iter_new(queue *target)
{
    queue_iter *iter = malloc(sizeof(queue_iter));
    if (!iter)
        return ((void *)0);
    q_iter_init(iter, target);
    return iter;
}
void q_iter_free(queue_iter *iter) { free(iter); }
void q_iter_init(queue_iter *iter, queue *target)
{
    iter->target = target;
    iter->cursor = target->front;
    iter->index = 0;
    iter->start = 1;
    iter->end = q_empty(target);
}
_Bool q_iter_start(queue_iter *iter) { return q_empty(iter->target) || iter->start; }
_Bool q_iter_end(queue_iter *iter) { return q_empty(iter->target) || iter->end; }
void q_iter_to_start(queue_iter *iter)
{
    iter->cursor = iter->target->front;
    iter->index = 0;
    iter->start = 1;
    iter->end = q_empty(iter->target);
}
void q_iter_to_end(queue_iter *iter)
{
    if (q_empty(iter->target))
        iter->cursor = 0;
    else
    {
        if (iter->target->back == 0)
            iter->cursor = iter->target->capacity - 1;
        else
            iter->cursor = iter->target->back - 1;
    }
    iter->index = iter->target->count - 1;
    iter->start = q_empty(iter->target);
    iter->end = 1;
}
_Bool q_iter_next(queue_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == q_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    iter->start = q_empty(iter->target);
    iter->cursor = (iter->cursor + 1) % (iter->target->capacity);
    iter->index++;
    return 1;
}
_Bool q_iter_prev(queue_iter *iter)
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
_Bool q_iter_advance(queue_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->index + 1 == q_count(iter->target))
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= q_count(iter->target))
        return 0;
    iter->start = q_empty(iter->target);
    iter->index += steps;
    iter->cursor = (iter->cursor + steps) % iter->target->capacity;
    return 1;
}
_Bool q_iter_rewind(queue_iter *iter, size_t steps)
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
        iter->cursor += q_capacity(iter->target);
    iter->cursor -= steps;
    return 1;
}
_Bool q_iter_go_to(queue_iter *iter, size_t index)
{
    if (index >= q_count(iter->target))
        return 0;
    if (iter->index > index)
        return q_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return q_iter_advance(iter, index - iter->index);
    return 1;
}
size_t q_iter_value(queue_iter *iter)
{
    if (q_empty(iter->target))
        return (size_t){0};
    return iter->target->buffer[iter->cursor];
}
size_t *q_iter_rvalue(queue_iter *iter)
{
    if (q_empty(iter->target))
        return ((void *)0);
    return &(iter->target->buffer[iter->cursor]);
}
size_t q_iter_index(queue_iter *iter) { return iter->index; }
static queue_iter q_impl_it_start(queue *_queue_)
{
    queue_iter iter;
    q_iter_init(&iter, _queue_);
    q_iter_to_start(&iter);
    return iter;
}
static queue_iter q_impl_it_end(queue *_queue_)
{
    queue_iter iter;
    q_iter_init(&iter, _queue_);
    q_iter_to_end(&iter);
    return iter;
}
