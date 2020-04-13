#include "cmc/stack.h"

struct stack
{
    size_t *buffer;
    size_t capacity;
    size_t count;
    int flag;
    struct stack_ftab_val *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
    struct stack_iter (*it_start)(struct stack *);
    struct stack_iter (*it_end)(struct stack *);
};
struct stack_ftab_val
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct stack_iter
{
    struct stack *target;
    size_t cursor;
    _Bool start;
    _Bool end;
};
struct stack *s_new(size_t capacity, struct stack_ftab_val *f_val);
struct stack *s_new_custom(size_t capacity, struct stack_ftab_val *f_val,
                           struct cmc_alloc_node *alloc,
                           struct cmc_callbacks *callbacks);
void s_clear(struct stack *_stack_);
void s_free(struct stack *_stack_);
void s_customize(struct stack *_stack_, struct cmc_alloc_node *alloc,
                 struct cmc_callbacks *callbacks);
_Bool s_push(struct stack *_stack_, size_t element);
_Bool s_pop(struct stack *_stack_);
size_t s_top(struct stack *_stack_);
_Bool s_contains(struct stack *_stack_, size_t element);
_Bool s_empty(struct stack *_stack_);
_Bool s_full(struct stack *_stack_);
size_t s_count(struct stack *_stack_);
size_t s_capacity(struct stack *_stack_);
int s_flag(struct stack *_stack_);
_Bool s_resize(struct stack *_stack_, size_t capacity);
struct stack *s_copy_of(struct stack *_stack_);
_Bool s_equals(struct stack *_stack1_, struct stack *_stack2_);
struct cmc_string s_to_string(struct stack *_stack_);
_Bool s_print(struct stack *_stack_, FILE *fptr);
struct stack_iter *s_iter_new(struct stack *target);
void s_iter_free(struct stack_iter *iter);
void s_iter_init(struct stack_iter *iter, struct stack *target);
_Bool s_iter_start(struct stack_iter *iter);
_Bool s_iter_end(struct stack_iter *iter);
void s_iter_to_start(struct stack_iter *iter);
void s_iter_to_end(struct stack_iter *iter);
_Bool s_iter_next(struct stack_iter *iter);
_Bool s_iter_prev(struct stack_iter *iter);
_Bool s_iter_advance(struct stack_iter *iter, size_t steps);
_Bool s_iter_rewind(struct stack_iter *iter, size_t steps);
_Bool s_iter_go_to(struct stack_iter *iter, size_t index);
size_t s_iter_value(struct stack_iter *iter);
size_t *s_iter_rvalue(struct stack_iter *iter);
size_t s_iter_index(struct stack_iter *iter);
static struct stack_iter s_impl_it_start(struct stack *_stack_);
static struct stack_iter s_impl_it_end(struct stack *_stack_);
struct stack *s_new(size_t capacity, struct stack_ftab_val *f_val)
{
    struct cmc_alloc_node *alloc = &cmc_alloc_node_default;
    if (capacity < 1)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    struct stack *_stack_ = alloc->malloc(sizeof(struct stack));
    if (!_stack_)
        return ((void *)0);
    _stack_->buffer = alloc->calloc(capacity, sizeof(size_t));
    if (!_stack_->buffer)
    {
        alloc->free(_stack_);
        return ((void *)0);
    }
    _stack_->capacity = capacity;
    _stack_->count = 0;
    _stack_->flag = cmc_flags.OK;
    _stack_->f_val = f_val;
    _stack_->alloc = alloc;
    _stack_->callbacks = ((void *)0);
    _stack_->it_start = s_impl_it_start;
    _stack_->it_end = s_impl_it_end;
    return _stack_;
}
struct stack *s_new_custom(size_t capacity, struct stack_ftab_val *f_val,
                           struct cmc_alloc_node *alloc,
                           struct cmc_callbacks *callbacks)
{
    if (capacity < 1)
        return ((void *)0);
    if (!f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct stack *_stack_ = alloc->malloc(sizeof(struct stack));
    if (!_stack_)
        return ((void *)0);
    _stack_->buffer = alloc->calloc(capacity, sizeof(size_t));
    if (!_stack_->buffer)
    {
        alloc->free(_stack_);
        return ((void *)0);
    }
    _stack_->capacity = capacity;
    _stack_->count = 0;
    _stack_->flag = cmc_flags.OK;
    _stack_->f_val = f_val;
    _stack_->alloc = alloc;
    _stack_->callbacks = callbacks;
    _stack_->it_start = s_impl_it_start;
    _stack_->it_end = s_impl_it_end;
    return _stack_;
}
void s_clear(struct stack *_stack_)
{
    if (_stack_->f_val->free)
    {
        for (size_t i = 0; i < _stack_->count; i++)
            _stack_->f_val->free(_stack_->buffer[i]);
    }
    memset(_stack_->buffer, 0, sizeof(size_t) * _stack_->capacity);
    _stack_->count = 0;
    _stack_->flag = cmc_flags.OK;
}
void s_free(struct stack *_stack_)
{
    if (_stack_->f_val->free)
    {
        for (size_t i = 0; i < _stack_->count; i++)
            _stack_->f_val->free(_stack_->buffer[i]);
    }
    _stack_->alloc->free(_stack_->buffer);
    _stack_->alloc->free(_stack_);
}
void s_customize(struct stack *_stack_, struct cmc_alloc_node *alloc,
                 struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _stack_->alloc = &cmc_alloc_node_default;
    else
        _stack_->alloc = alloc;
    _stack_->callbacks = callbacks;
    _stack_->flag = cmc_flags.OK;
}
_Bool s_push(struct stack *_stack_, size_t element)
{
    if (s_full(_stack_))
    {
        if (!s_resize(_stack_, _stack_->capacity * 2))
            return 0;
    }
    _stack_->buffer[_stack_->count++] = element;
    _stack_->flag = cmc_flags.OK;
    if (_stack_->callbacks && _stack_->callbacks->create)
        _stack_->callbacks->create();
    return 1;
}
_Bool s_pop(struct stack *_stack_)
{
    if (s_empty(_stack_))
    {
        _stack_->flag = cmc_flags.EMPTY;
        return 0;
    }
    _stack_->buffer[--_stack_->count] = (size_t){ 0 };
    _stack_->flag = cmc_flags.OK;
    if (_stack_->callbacks && _stack_->callbacks->delete)
        _stack_->callbacks->delete ();
    return 1;
}
size_t s_top(struct stack *_stack_)
{
    if (s_empty(_stack_))
    {
        _stack_->flag = cmc_flags.EMPTY;
        return (size_t){ 0 };
    }
    _stack_->flag = cmc_flags.OK;
    if (_stack_->callbacks && _stack_->callbacks->read)
        _stack_->callbacks->read();
    return _stack_->buffer[_stack_->count - 1];
}
_Bool s_contains(struct stack *_stack_, size_t element)
{
    _stack_->flag = cmc_flags.OK;
    _Bool result = 0;
    for (size_t i = 0; i < _stack_->count; i++)
    {
        if (_stack_->f_val->cmp(_stack_->buffer[i], element) == 0)
        {
            result = 1;
            break;
        }
    }
    if (_stack_->callbacks && _stack_->callbacks->read)
        _stack_->callbacks->read();
    return result;
}
_Bool s_empty(struct stack *_stack_)
{
    return _stack_->count == 0;
}
_Bool s_full(struct stack *_stack_)
{
    return _stack_->count >= _stack_->capacity;
}
size_t s_count(struct stack *_stack_)
{
    return _stack_->count;
}
size_t s_capacity(struct stack *_stack_)
{
    return _stack_->capacity;
}
int s_flag(struct stack *_stack_)
{
    return _stack_->flag;
}
_Bool s_resize(struct stack *_stack_, size_t capacity)
{
    if (_stack_->capacity == capacity)
        goto success;
    if (capacity < _stack_->count)
    {
        _stack_->flag = cmc_flags.INVALID;
        return 0;
    }
    size_t *new_buffer =
        _stack_->alloc->realloc(_stack_->buffer, sizeof(size_t) * capacity);
    if (!new_buffer)
    {
        _stack_->flag = cmc_flags.ALLOC;
        return 0;
    }
    _stack_->buffer = new_buffer;
    _stack_->capacity = capacity;
success:
    _stack_->flag = cmc_flags.OK;
    if (_stack_->callbacks && _stack_->callbacks->resize)
        _stack_->callbacks->resize();
    return 1;
}
struct stack *s_copy_of(struct stack *_stack_)
{
    struct stack *result = s_new_custom(_stack_->capacity, _stack_->f_val,
                                        _stack_->alloc, _stack_->callbacks);
    if (!result)
    {
        _stack_->flag = cmc_flags.ERROR;
        return ((void *)0);
    }
    if (_stack_->f_val->cpy)
    {
        for (size_t i = 0; i < _stack_->count; i++)
            result->buffer[i] = _stack_->f_val->cpy(_stack_->buffer[i]);
    }
    else
        memcpy(result->buffer, _stack_->buffer,
               sizeof(size_t) * _stack_->count);
    result->count = _stack_->count;
    _stack_->flag = cmc_flags.OK;
    return result;
}
_Bool s_equals(struct stack *_stack1_, struct stack *_stack2_)
{
    _stack1_->flag = cmc_flags.OK;
    _stack2_->flag = cmc_flags.OK;
    if (_stack1_->count != _stack2_->count)
        return 0;
    for (size_t i = 0; i < _stack1_->count; i++)
    {
        if (_stack1_->f_val->cmp(_stack1_->buffer[i], _stack2_->buffer[i]) != 0)
            return 0;
    }
    return 1;
}
struct cmc_string s_to_string(struct stack *_stack_)
{
    struct cmc_string str;
    struct stack *s_ = _stack_;
    int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_stack, "stack",
                     "size_t", s_, s_->buffer, s_->capacity, s_->count,
                     s_->flag, s_->f_val, s_->alloc, s_->callbacks);
    return n >= 0 ? str : (struct cmc_string){ 0 };
}
_Bool s_print(struct stack *_stack_, FILE *fptr)
{
    for (size_t i = 0; i < _stack_->count; i++)
    {
        if (!_stack_->f_val->str(fptr, _stack_->buffer[i]))
            return 0;
    }
    return 1;
}
struct stack_iter *s_iter_new(struct stack *target)
{
    struct stack_iter *iter = target->alloc->malloc(sizeof(struct stack_iter));
    if (!iter)
        return ((void *)0);
    s_iter_init(iter, target);
    return iter;
}
void s_iter_free(struct stack_iter *iter)
{
    iter->target->alloc->free(iter);
}
void s_iter_init(struct stack_iter *iter, struct stack *target)
{
    iter->target = target;
    iter->cursor = s_empty(target) ? 0 : iter->target->count - 1;
    iter->start = 1;
    iter->end = s_empty(target);
}
_Bool s_iter_start(struct stack_iter *iter)
{
    return s_empty(iter->target) || iter->start;
}
_Bool s_iter_end(struct stack_iter *iter)
{
    return s_empty(iter->target) || iter->end;
}
void s_iter_to_start(struct stack_iter *iter)
{
    if (!s_empty(iter->target))
    {
        iter->cursor = iter->target->count - 1;
        iter->start = 1;
        iter->end = s_empty(iter->target);
    }
}
void s_iter_to_end(struct stack_iter *iter)
{
    if (!s_empty(iter->target))
    {
        iter->cursor = 0;
        iter->start = s_empty(iter->target);
        iter->end = 1;
    }
}
_Bool s_iter_next(struct stack_iter *iter)
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
_Bool s_iter_prev(struct stack_iter *iter)
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
_Bool s_iter_advance(struct stack_iter *iter, size_t steps)
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
_Bool s_iter_rewind(struct stack_iter *iter, size_t steps)
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
_Bool s_iter_go_to(struct stack_iter *iter, size_t index)
{
    if (index >= s_count(iter->target))
        return 0;
    if (iter->cursor > index)
        return s_iter_rewind(iter, iter->cursor - index);
    else if (iter->cursor < index)
        return s_iter_advance(iter, index - iter->cursor);
    return 1;
}
size_t s_iter_value(struct stack_iter *iter)
{
    if (s_empty(iter->target))
        return (size_t){ 0 };
    return iter->target->buffer[iter->cursor];
}
size_t *s_iter_rvalue(struct stack_iter *iter)
{
    if (s_empty(iter->target))
        return ((void *)0);
    return &(iter->target->buffer[iter->cursor]);
}
size_t s_iter_index(struct stack_iter *iter)
{
    if (s_empty(iter->target))
        return 0;
    return iter->target->count - 1 - iter->cursor;
}
static struct stack_iter s_impl_it_start(struct stack *_stack_)
{
    struct stack_iter iter;
    s_iter_init(&iter, _stack_);
    s_iter_to_start(&iter);
    return iter;
}
static struct stack_iter s_impl_it_end(struct stack *_stack_)
{
    struct stack_iter iter;
    s_iter_init(&iter, _stack_);
    s_iter_to_end(&iter);
    return iter;
}
