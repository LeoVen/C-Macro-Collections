#include <cmc/stack.h>

//CMC_GENERATE_STACK(s, stack, size_t)

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
    _Bool start;
    _Bool end;
} stack_iter, *stack_iter_ptr;
stack *s_new(size_t capacity);
void s_clear(stack *_stack_, void (*deallocator)(size_t));
void s_free(stack *_stack_, void (*deallocator)(size_t));
_Bool s_push(stack *_stack_, size_t element);
_Bool s_pop(stack *_stack_);
size_t s_top(stack *_stack_);
_Bool s_contains(stack *_stack_, size_t element, int (*comparator)(size_t, size_t));
_Bool s_empty(stack *_stack_);
_Bool s_full(stack *_stack_);
size_t s_count(stack *_stack_);
size_t s_capacity(stack *_stack_);
_Bool s_resize(stack *_stack_, size_t capacity);
stack *s_copy_of(stack *_stack_, size_t (*copy_func)(size_t));
_Bool s_equals(stack *_stack1_, stack *_stack2_, int (*comparator)(size_t, size_t));
struct cmc_string s_to_string(stack *_stack_);
stack_iter *s_iter_new(stack *target);
void s_iter_free(stack_iter *iter);
void s_iter_init(stack_iter *iter, stack *target);
_Bool s_iter_start(stack_iter *iter);
_Bool s_iter_end(stack_iter *iter);
void s_iter_to_start(stack_iter *iter);
void s_iter_to_end(stack_iter *iter);
_Bool s_iter_next(stack_iter *iter);
_Bool s_iter_prev(stack_iter *iter);
_Bool s_iter_advance(stack_iter *iter, size_t steps);
_Bool s_iter_rewind(stack_iter *iter, size_t steps);
_Bool s_iter_go_to(stack_iter *iter, size_t index);
size_t s_iter_value(stack_iter *iter);
size_t *s_iter_rvalue(stack_iter *iter);
size_t s_iter_index(stack_iter *iter);
static stack_iter s_impl_it_start(stack *_stack_);
static stack_iter s_impl_it_end(stack *_stack_);
stack *s_new(size_t capacity)
{
    if (capacity < 1)
        return ((void *)0);
    stack *_stack_ = malloc(sizeof(stack));
    if (!_stack_)
        return ((void *)0);
    _stack_->buffer = malloc(sizeof(size_t) * capacity);
    if (!_stack_->buffer)
    {
        free(_stack_);
        return ((void *)0);
    }
    memset(_stack_->buffer, 0, sizeof(size_t) * capacity);
    _stack_->capacity = capacity;
    _stack_->count = 0;
    _stack_->it_start = s_impl_it_start;
    _stack_->it_end = s_impl_it_end;
    return _stack_;
}
void s_clear(stack *_stack_, void (*deallocator)(size_t))
{
    if (deallocator)
    {
        for (size_t i = 0; i < _stack_->count; i++)
            deallocator(_stack_->buffer[i]);
    }
    memset(_stack_->buffer, 0, sizeof(size_t) * _stack_->capacity);
    _stack_->count = 0;
}
void s_free(stack *_stack_, void (*deallocator)(size_t))
{
    free(_stack_->buffer);
    if (deallocator)
    {
        for (size_t i = 0; i < _stack_->count; i++)
            deallocator(_stack_->buffer[i]);
    }
    free(_stack_);
}
_Bool s_push(stack *_stack_, size_t element)
{
    if (s_full(_stack_))
    {
        if (!s_resize(_stack_, s_capacity(_stack_) * 2))
            return 0;
    }
    _stack_->buffer[_stack_->count++] = element;
    return 1;
}
_Bool s_pop(stack *_stack_)
{
    if (s_empty(_stack_))
        return 0;
    _stack_->buffer[--_stack_->count] = (size_t){0};
    return 1;
}
size_t s_top(stack *_stack_)
{
    if (s_empty(_stack_))
        return (size_t){0};
    return _stack_->buffer[_stack_->count - 1];
}
_Bool s_contains(stack *_stack_, size_t element, int (*comparator)(size_t, size_t))
{
    for (size_t i = 0; i < _stack_->count; i++)
    {
        if (comparator(_stack_->buffer[i], element) == 0)
            return 1;
    }
    return 0;
}
_Bool s_empty(stack *_stack_) { return _stack_->count == 0; }
_Bool s_full(stack *_stack_) { return _stack_->count >= _stack_->capacity; }
size_t s_count(stack *_stack_) { return _stack_->count; }
size_t s_capacity(stack *_stack_) { return _stack_->capacity; }
_Bool s_resize(stack *_stack_, size_t capacity)
{
    if (s_capacity(_stack_) == capacity)
        return 1;
    if (capacity < s_count(_stack_))
        return 0;
    size_t *new_buffer = realloc(_stack_->buffer, sizeof(size_t) * capacity);
    if (!new_buffer)
        return 0;
    _stack_->buffer = new_buffer;
    _stack_->capacity = capacity;
    return 1;
}
stack *s_copy_of(stack *_stack_, size_t (*copy_func)(size_t))
{
    stack *result = s_new(_stack_->capacity);
    if (!result)
        return ((void *)0);
    if (copy_func)
    {
        for (size_t i = 0; i < _stack_->count; i++)
            result->buffer[i] = copy_func(_stack_->buffer[i]);
    }
    else
        memcpy(result->buffer, _stack_->buffer, sizeof(size_t) * _stack_->count);
    result->count = _stack_->count;
    return result;
}
_Bool s_equals(stack *_stack1_, stack *_stack2_, int (*comparator)(size_t, size_t))
{
    if (s_count(_stack1_) != s_count(_stack2_))
        return 0;
    for (size_t i = 0; i < s_count(_stack1_); i++)
    {
        if (comparator(_stack1_->buffer[i], _stack2_->buffer[i]) != 0)
            return 0;
    }
    return 1;
}
struct cmc_string s_to_string(stack *_stack_)
{
    struct cmc_string str;
    stack *s_ = _stack_;
    const char *name = "stack";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_stack, name, s_, s_->buffer, s_->capacity, s_->count);
    return str;
}
stack_iter *s_iter_new(stack *target)
{
    stack_iter *iter = malloc(sizeof(stack_iter));
    if (!iter)
        return ((void *)0);
    s_iter_init(iter, target);
    return iter;
}
void s_iter_free(stack_iter *iter) { free(iter); }
void s_iter_init(stack_iter *iter, stack *target)
{
    iter->target = target;
    iter->cursor = s_empty(target) ? 0 : iter->target->count - 1;
    iter->start = 1;
    iter->end = s_empty(target);
}
_Bool s_iter_start(stack_iter *iter) { return s_empty(iter->target) || iter->start; }
_Bool s_iter_end(stack_iter *iter) { return s_empty(iter->target) || iter->end; }
void s_iter_to_start(stack_iter *iter)
{
    if (!s_empty(iter->target))
    {
        iter->cursor = iter->target->count - 1;
        iter->start = 1;
        iter->end = s_empty(iter->target);
    }
}
void s_iter_to_end(stack_iter *iter)
{
    if (!s_empty(iter->target))
    {
        iter->cursor = 0;
        iter->start = s_empty(iter->target);
        iter->end = 1;
    }
}
_Bool s_iter_next(stack_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor == 0)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = s_empty(iter->target);
    iter->cursor--;
    return 1;
}
_Bool s_iter_prev(stack_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor + 1 == s_count(iter->target))
    {
        iter->start = 1;
        return 0;
    }
    iter->end = s_empty(iter->target);
    iter->cursor++;
    return 1;
}
_Bool s_iter_advance(stack_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->cursor == 0)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor < steps)
        return 0;
    iter->start = s_empty(iter->target);
    iter->cursor -= steps;
    return 1;
}
_Bool s_iter_rewind(stack_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->cursor + 1 == s_count(iter->target))
    {
        iter->start = 1;
        return 0;
    }
    if (steps == 0 || iter->cursor + steps >= s_count(iter->target))
        return 0;
    iter->end = s_empty(iter->target);
    iter->cursor += steps;
    return 1;
}
_Bool s_iter_go_to(stack_iter *iter, size_t index)
{
    if (index >= s_count(iter->target))
        return 0;
    if (iter->cursor > index)
        return s_iter_rewind(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return s_iter_advance(iter, index - iter->cursor);
    return 1;
}
size_t s_iter_value(stack_iter *iter)
{
    if (s_empty(iter->target))
        return (size_t){0};
    return iter->target->buffer[iter->cursor];
}
size_t *s_iter_rvalue(stack_iter *iter)
{
    if (s_empty(iter->target))
        return ((void *)0);
    return &(iter->target->buffer[iter->cursor]);
}
size_t s_iter_index(stack_iter *iter)
{
    if (s_empty(iter->target))
        return 0;
    return iter->target->count - 1 - iter->cursor;
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
