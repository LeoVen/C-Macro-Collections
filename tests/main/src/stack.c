#include "cmc/stack.h"

typedef struct stack_s
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    struct stack_iter_s (*it_start)(struct stack_s *);
    struct stack_iter_s (*it_end)(struct stack_s *);
} stack, *stack_ptr;
typedef struct stack_iter_s
{
    struct stack_s *target;
    size_t cursor;
    bool start;
    bool end;
} stack_iter, *stack_iter_ptr;
stack *s_new(size_t capacity);
void s_clear(stack *_stack_);
void s_free(stack *_stack_);
bool s_push(stack *_stack_, size_t element);
bool s_pop(stack *_stack_);
bool s_push_if(stack *_stack_, size_t element, bool condition);
bool s_pop_if(stack *_stack_, bool condition);
size_t s_top(stack *_stack_);
bool s_contains(stack *_stack_, size_t element, int (*comparator)(size_t, size_t));
bool s_empty(stack *_stack_);
bool s_full(stack *_stack_);
size_t s_count(stack *_stack_);
size_t s_capacity(stack *_stack_);
stack_iter *s_iter_new(stack *target);
void s_iter_free(stack_iter *iter);
void s_iter_init(stack_iter *iter, stack *target);
bool s_iter_start(stack_iter *iter);
bool s_iter_end(stack_iter *iter);
void s_iter_to_start(stack_iter *iter);
void s_iter_to_end(stack_iter *iter);
bool s_iter_next(stack_iter *iter);
bool s_iter_prev(stack_iter *iter);
size_t s_iter_value(stack_iter *iter);
size_t *s_iter_rvalue(stack_iter *iter);
size_t s_iter_index(stack_iter *iter);
static inline size_t s_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
static bool s_impl_grow(stack *_stack_);
static stack_iter s_impl_it_start(stack *_stack_);
static stack_iter s_impl_it_end(stack *_stack_);
stack *s_new(size_t capacity)
{
    if (capacity < 1)
        return NULL;
    stack *_stack_ = malloc(sizeof(stack));
    if (!_stack_)
        return NULL;
    _stack_->buffer = malloc(sizeof(size_t) * capacity);
    if (!_stack_->buffer)
    {
        free(_stack_);
        return NULL;
    }
    memset(_stack_->buffer, 0, sizeof(size_t) * capacity);
    _stack_->capacity = capacity;
    _stack_->count = 0;
    _stack_->it_start = s_impl_it_start;
    _stack_->it_end = s_impl_it_end;
    return _stack_;
}
void s_clear(stack *_stack_)
{
    memset(_stack_->buffer, 0, sizeof(size_t) * _stack_->capacity);
    _stack_->count = 0;
}
void s_free(stack *_stack_)
{
    free(_stack_->buffer);
    free(_stack_);
}
bool s_push(stack *_stack_, size_t element)
{
    if (s_full(_stack_))
    {
        if (!s_impl_grow(_stack_))
            return false;
    }
    _stack_->buffer[_stack_->count++] = element;
    return true;
}
bool s_pop(stack *_stack_)
{
    if (s_empty(_stack_))
        return false;
    _stack_->buffer[--_stack_->count] = s_impl_default_value();
    return true;
}
bool s_push_if(stack *_stack_, size_t element, bool condition)
{
    if (condition)
        return s_push(_stack_, element);
    return false;
}
bool s_pop_if(stack *_stack_, bool condition)
{
    if (condition)
        return s_pop(_stack_);
    return false;
}
size_t s_top(stack *_stack_)
{
    if (s_empty(_stack_))
        return s_impl_default_value();
    return _stack_->buffer[_stack_->count - 1];
}
bool s_contains(stack *_stack_, size_t element, int (*comparator)(size_t, size_t))
{
    for (size_t i = 0; i < _stack_->count; i++)
    {
        if (comparator(_stack_->buffer[i], element) == 0)
            return true;
    }
    return false;
}
bool s_empty(stack *_stack_) { return _stack_->count == 0; }
bool s_full(stack *_stack_) { return _stack_->count >= _stack_->capacity; }
size_t s_count(stack *_stack_) { return _stack_->count; }
size_t s_capacity(stack *_stack_) { return _stack_->capacity; }
stack_iter *s_iter_new(stack *target)
{
    stack_iter *iter = malloc(sizeof(stack_iter));
    if (!iter)
        return NULL;
    s_iter_init(iter, target);
    return iter;
}
void s_iter_free(stack_iter *iter) { free(iter); }
void s_iter_init(stack_iter *iter, stack *target)
{
    iter->target = target;
    iter->cursor = iter->target->count - 1;
    iter->start = true;
    iter->end = s_empty(target);
}
bool s_iter_start(stack_iter *iter) { return s_empty(iter->target) || iter->start; }
bool s_iter_end(stack_iter *iter) { return s_empty(iter->target) || iter->end; }
void s_iter_to_start(stack_iter *iter)
{
    iter->cursor = iter->target->count - 1;
    iter->start = true;
    iter->end = s_empty(iter->target);
}
void s_iter_to_end(stack_iter *iter)
{
    iter->cursor = 0;
    iter->start = s_empty(iter->target);
    iter->end = true;
}
bool s_iter_next(stack_iter *iter)
{
    if (iter->end)
        return false;
    iter->start = s_empty(iter->target);
    if (iter->cursor == 0)
        iter->end = true;
    else
        iter->cursor--;
    return true;
}
bool s_iter_prev(stack_iter *iter)
{
    if (iter->start)
        return false;
    iter->end = s_empty(iter->target);
    if (iter->cursor == iter->target->count - 1)
        iter->start = true;
    else
        iter->cursor++;
    return true;
}
size_t s_iter_value(stack_iter *iter)
{
    if (s_empty(iter->target))
        return s_impl_default_value();
    return iter->target->buffer[iter->cursor];
}
size_t *s_iter_rvalue(stack_iter *iter)
{
    if (s_empty(iter->target))
        return NULL;
    return &(iter->target->buffer[iter->cursor]);
}
size_t s_iter_index(stack_iter *iter) { return iter->target->count - 1 - iter->cursor; }
static bool s_impl_grow(stack *_stack_)
{
    size_t new_capacity = _stack_->capacity * 2;
    size_t *new_buffer = realloc(_stack_->buffer, sizeof(size_t) * new_capacity);
    if (!new_buffer)
        return false;
    _stack_->buffer = new_buffer;
    _stack_->capacity = new_capacity;
    return true;
}
static stack_iter s_impl_it_start(stack *_stack_)
{
    stack_iter iter;
    s_iter_init(&iter, _stack_);
    s_iter_to_start(&iter);
    return iter;
}
static stack_iter s_impl_it_end(stack *_stack_)
{
    stack_iter iter;
    s_iter_init(&iter, _stack_);
    s_iter_to_end(&iter);
    return iter;
}
