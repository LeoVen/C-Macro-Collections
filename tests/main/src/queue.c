#include "cmc/queue.h"

typedef struct queue_s
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    size_t front;
    size_t rear;
    struct queue_iter_s (*it_start)(struct queue_s *);
    struct queue_iter_s (*it_end)(struct queue_s *);
} queue, *queue_ptr;
typedef struct queue_iter_s
{
    struct queue_s *target;
    size_t cursor;
    size_t index;
    bool start;
    bool end;
} queue_iter, *queue_iter_ptr;
queue *q_new(size_t capacity);
void q_clear(queue *_queue_);
void q_free(queue *_queue_);
bool q_enqueue(queue *_queue_, size_t element);
bool q_dequeue(queue *_queue_);
bool q_enqueue_if(queue *_queue_, size_t element, bool condition);
bool q_dequeue_if(queue *_queue_, bool condition);
size_t q_peek(queue *_queue_);
bool q_contains(queue *_queue_, size_t element, int (*comparator)(size_t, size_t));
bool q_empty(queue *_queue_);
bool q_full(queue *_queue_);
size_t q_count(queue *_queue_);
size_t q_capacity(queue *_queue_);
queue_iter *q_iter_new(queue *target);
void q_iter_free(queue_iter *iter);
void q_iter_init(queue_iter *iter, queue *target);
bool q_iter_start(queue_iter *iter);
bool q_iter_end(queue_iter *iter);
void q_iter_to_start(queue_iter *iter);
void q_iter_to_end(queue_iter *iter);
bool q_iter_next(queue_iter *iter);
bool q_iter_prev(queue_iter *iter);
size_t q_iter_value(queue_iter *iter);
size_t *q_iter_rvalue(queue_iter *iter);
size_t q_iter_index(queue_iter *iter);
static inline size_t q_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
static bool q_impl_grow(queue *_queue_);
static queue_iter q_impl_it_start(queue *_queue_);
static queue_iter q_impl_it_end(queue *_queue_);
queue *q_new(size_t capacity)
{
    if (capacity < 1)
        return NULL;
    queue *_queue_ = malloc(sizeof(queue));
    if (!_queue_)
        return NULL;
    _queue_->buffer = malloc(sizeof(size_t) * capacity);
    if (!_queue_->buffer)
    {
        free(_queue_);
        return NULL;
    }
    memset(_queue_->buffer, 0, sizeof(size_t) * capacity);
    _queue_->capacity = capacity;
    _queue_->count = 0;
    _queue_->front = 0;
    _queue_->rear = 0;
    _queue_->it_start = q_impl_it_start;
    _queue_->it_end = q_impl_it_end;
    return _queue_;
}
void q_clear(queue *_queue_)
{
    memset(_queue_->buffer, 0, sizeof(size_t) * _queue_->capacity);
    _queue_->count = 0;
    _queue_->front = 0;
    _queue_->rear = 0;
}
void q_free(queue *_queue_)
{
    free(_queue_->buffer);
    free(_queue_);
}
bool q_enqueue(queue *_queue_, size_t element)
{
    if (q_full(_queue_))
    {
        if (!q_impl_grow(_queue_))
            return false;
    }
    _queue_->buffer[_queue_->rear] = element;
    _queue_->rear = (_queue_->rear == _queue_->capacity - 1) ? 0 : _queue_->rear + 1;
    _queue_->count++;
    return true;
}
bool q_dequeue(queue *_queue_)
{
    if (q_empty(_queue_))
        return false;
    _queue_->buffer[_queue_->front] = q_impl_default_value();
    _queue_->front = (_queue_->front == _queue_->capacity - 1) ? 0 : _queue_->front + 1;
    _queue_->count--;
    return true;
}
bool q_enqueue_if(queue *_queue_, size_t element, bool condition)
{
    if (condition)
        return q_enqueue(_queue_, element);
    return false;
}
bool q_dequeue_if(queue *_queue_, bool condition)
{
    if (condition)
        return q_dequeue(_queue_);
    return false;
}
size_t q_peek(queue *_queue_)
{
    if (q_empty(_queue_))
        return q_impl_default_value();
    return _queue_->buffer[_queue_->front];
}
bool q_contains(queue *_queue_, size_t element, int (*comparator)(size_t, size_t))
{
    for (size_t i = _queue_->front, j = 0; j < _queue_->count; i = (i + 1) % _queue_->count, j++)
    {
        if (comparator(_queue_->buffer[i], element) == 0)
            return true;
    }
    return false;
}
bool q_empty(queue *_queue_) { return _queue_->count == 0; }
bool q_full(queue *_queue_) { return _queue_->count >= _queue_->capacity; }
size_t q_count(queue *_queue_) { return _queue_->count; }
size_t q_capacity(queue *_queue_) { return _queue_->capacity; }
queue_iter *q_iter_new(queue *target)
{
    queue_iter *iter = malloc(sizeof(queue_iter));
    if (!iter)
        return NULL;
    q_iter_init(iter, target);
    return iter;
}
void q_iter_free(queue_iter *iter) { free(iter); }
void q_iter_init(queue_iter *iter, queue *target)
{
    iter->target = target;
    iter->cursor = target->front;
    iter->index = 0;
    iter->start = true;
    iter->end = q_empty(target);
}
bool q_iter_start(queue_iter *iter) { return q_empty(iter->target) || iter->start; }
bool q_iter_end(queue_iter *iter) { return q_empty(iter->target) || iter->end; }
void q_iter_to_start(queue_iter *iter)
{
    iter->cursor = iter->target->front;
    iter->index = 0;
    iter->start = true;
    iter->end = q_empty(iter->target);
}
void q_iter_to_end(queue_iter *iter)
{
    if (q_empty(iter->target))
        iter->cursor = 0;
    else
        iter->cursor = (iter->target->rear == 0) ? iter->target->capacity - 1 : iter->target->rear - 1;
    iter->index = iter->target->count - 1;
    iter->start = q_empty(iter->target);
    iter->end = true;
}
bool q_iter_next(queue_iter *iter)
{
    if (iter->end)
        return false;
    iter->start = q_empty(iter->target);
    if (iter->index == iter->target->count - 1)
        iter->end = true;
    else
    {
        iter->cursor = (iter->cursor + 1) % (iter->target->capacity);
        iter->index++;
    }
    return true;
}
bool q_iter_prev(queue_iter *iter)
{
    if (iter->start)
        return false;
    iter->end = q_empty(iter->target);
    if (iter->index == 0)
        iter->start = true;
    else
    {
        iter->cursor = (iter->cursor == 0) ? iter->target->capacity - 1 : iter->cursor - 1;
        iter->index--;
    }
    return true;
}
size_t q_iter_value(queue_iter *iter)
{
    if (q_empty(iter->target))
        return q_impl_default_value();
    return iter->target->buffer[iter->cursor];
}
size_t *q_iter_rvalue(queue_iter *iter)
{
    if (q_empty(iter->target))
        return NULL;
    return &(iter->target->buffer[iter->cursor]);
}
size_t q_iter_index(queue_iter *iter) { return iter->index; }
static bool q_impl_grow(queue *_queue_)
{
    size_t new_capacity = _queue_->capacity * 2;
    size_t *new_buffer = malloc(sizeof(size_t) * new_capacity);
    if (!new_buffer)
        return false;
    for (size_t i = _queue_->front, j = 0; j < _queue_->count; i = (i + 1) % _queue_->capacity, j++)
    {
        new_buffer[j] = _queue_->buffer[i];
    }
    free(_queue_->buffer);
    _queue_->buffer = new_buffer;
    _queue_->capacity = new_capacity;
    _queue_->front = 0;
    _queue_->rear = _queue_->count;
    return true;
}
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
