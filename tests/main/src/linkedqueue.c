#include "ext/linkedqueue.h"

//LINKEDQUEUE_GENERATE(lq, linkedqueue, , size_t)

typedef struct linkedqueue_s
{
    struct linkedqueue_node_s *head;
    struct linkedqueue_node_s *tail;
    size_t count;
    struct linkedqueue_iter_s (*it_start)(struct linkedqueue_s *);
} linkedqueue, *linkedqueue_ptr;
typedef struct linkedqueue_node_s
{
    size_t data;
    struct linkedqueue_node_s *next;
} linkedqueue_node, *linkedqueue_node_ptr;
typedef struct linkedqueue_iter_s
{
    struct linkedqueue_s *target;
    struct linkedqueue_node_s *cursor;
    size_t index;
    bool start;
    bool end;
} linkedqueue_iter, *linkedqueue_iter_ptr;
linkedqueue *lq_new(void);
void lq_clear(linkedqueue *_lqueue_);
void lq_free(linkedqueue *_lqueue_);
bool lq_enqueue(linkedqueue *_lqueue_, size_t element);
bool lq_dequeue(linkedqueue *_lqueue_);
bool lq_enqueue_if(linkedqueue *_lqueue_, size_t element, bool condition);
bool lq_dequeue_if(linkedqueue *_lqueue_, bool condition);
size_t lq_peek(linkedqueue *_lqueue_);
bool lq_contains(linkedqueue *_lqueue_, size_t element, int (*comparator)(size_t, size_t));
bool lq_empty(linkedqueue *_lqueue_);
size_t lq_count(linkedqueue *_lqueue_);
linkedqueue_iter *lq_iter_new(linkedqueue *target);
void lq_iter_free(linkedqueue_iter *iter);
void lq_iter_init(linkedqueue_iter *iter, linkedqueue *target);
bool lq_iter_start(linkedqueue_iter *iter);
bool lq_iter_end(linkedqueue_iter *iter);
void lq_iter_to_start(linkedqueue_iter *iter);
void lq_iter_to_end(linkedqueue_iter *iter);
bool lq_iter_next(linkedqueue_iter *iter);
size_t lq_iter_value(linkedqueue_iter *iter);
size_t *lq_iter_rvalue(linkedqueue_iter *iter);
size_t lq_iter_index(linkedqueue_iter *iter);
static inline size_t lq_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
linkedqueue_node *lq_impl_new_node(size_t element);
static linkedqueue_iter lq_impl_it_start(linkedqueue *_lqueue_);
linkedqueue *lq_new(void)
{
    linkedqueue *_lqueue_ = malloc(sizeof(linkedqueue));
    if (!_lqueue_)
        return NULL;
    _lqueue_->count = 0;
    _lqueue_->head = NULL;
    _lqueue_->tail = NULL;
    _lqueue_->it_start = lq_impl_it_start;
    return _lqueue_;
}
void lq_clear(linkedqueue *_lqueue_)
{
    linkedqueue_node *scan = _lqueue_->head;
    while (_lqueue_->head != NULL)
    {
        _lqueue_->head = _lqueue_->head->next;
        free(scan);
        scan = _lqueue_->head;
    }
    _lqueue_->count = 0;
    _lqueue_->head = NULL;
    _lqueue_->tail = NULL;
}
void lq_free(linkedqueue *_lqueue_)
{
    lq_clear(_lqueue_);
    free(_lqueue_);
}
bool lq_enqueue(linkedqueue *_lqueue_, size_t element)
{
    linkedqueue_node *_node_ = lq_impl_new_node(element);
    if (!_node_)
        return false;
    if (lq_empty(_lqueue_))
    {
        _lqueue_->head = _node_;
        _lqueue_->tail = _node_;
    }
    else
    {
        _lqueue_->tail->next = _node_;
        _lqueue_->tail = _node_;
    }
    _lqueue_->count++;
    return true;
}
bool lq_dequeue(linkedqueue *_lqueue_)
{
    if (lq_empty(_lqueue_))
        return false;
    linkedqueue_node *_node_ = _lqueue_->head;
    _lqueue_->head = _lqueue_->head->next;
    free(_node_);
    if (_lqueue_->head == NULL)
        _lqueue_->tail = NULL;
    _lqueue_->count--;
    return true;
}
bool lq_enqueue_if(linkedqueue *_lqueue_, size_t element, bool condition)
{
    if (condition)
        return lq_enqueue(_lqueue_, element);
    return false;
}
bool lq_dequeue_if(linkedqueue *_lqueue_, bool condition)
{
    if (condition)
        return lq_dequeue(_lqueue_);
    return false;
}
size_t lq_peek(linkedqueue *_lqueue_)
{
    if (lq_empty(_lqueue_))
        return lq_impl_default_value();
    return _lqueue_->head->data;
}
bool lq_contains(linkedqueue *_lqueue_, size_t element, int (*comparator)(size_t, size_t))
{
    linkedqueue_node *scan = _lqueue_->head;
    while (scan != NULL)
    {
        if (comparator(scan->data, element) == 0)
            return true;
        scan = scan->next;
    }
    return false;
}
bool lq_empty(linkedqueue *_lqueue_) { return _lqueue_->count == 0; }
size_t lq_count(linkedqueue *_lqueue_) { return _lqueue_->count; }
linkedqueue_iter *lq_iter_new(linkedqueue *target)
{
    linkedqueue_iter *iter = malloc(sizeof(linkedqueue_iter));
    if (!iter)
        return NULL;
    lq_iter_init(iter, target);
    return iter;
}
void lq_iter_free(linkedqueue_iter *iter) { free(iter); }
void lq_iter_init(linkedqueue_iter *iter, linkedqueue *target)
{
    iter->target = target;
    iter->cursor = target->head;
    iter->index = 0;
    iter->start = true;
    iter->end = lq_empty(target);
}
bool lq_iter_start(linkedqueue_iter *iter) { return lq_empty(iter->target) || iter->start; }
bool lq_iter_end(linkedqueue_iter *iter) { return lq_empty(iter->target) || iter->end; }
void lq_iter_to_start(linkedqueue_iter *iter)
{
    iter->cursor = iter->target->head;
    iter->index = 0;
    iter->start = true;
    iter->end = lq_empty(iter->target);
}
void lq_iter_to_end(linkedqueue_iter *iter)
{
    iter->cursor = iter->target->tail;
    iter->index = iter->target->count - 1;
    iter->start = lq_empty(iter->target);
    iter->end = true;
}
bool lq_iter_next(linkedqueue_iter *iter)
{
    if (iter->end)
        return false;
    iter->start = lq_empty(iter->target);
    if (iter->cursor->next == NULL)
        iter->end = true;
    else
    {
        iter->cursor = iter->cursor->next;
        iter->index++;
    }
    return true;
}
size_t lq_iter_value(linkedqueue_iter *iter)
{
    if (lq_empty(iter->target))
        return lq_impl_default_value();
    return iter->cursor->data;
}
size_t *lq_iter_rvalue(linkedqueue_iter *iter)
{
    if (lq_empty(iter->target))
        return NULL;
    return &(iter->cursor->data);
}
size_t lq_iter_index(linkedqueue_iter *iter) { return iter->index; }
linkedqueue_node *lq_impl_new_node(size_t element)
{
    linkedqueue_node *_node_ = malloc(sizeof(linkedqueue_node));
    if (!_node_)
        return NULL;
    _node_->data = element;
    _node_->next = NULL;
    return _node_;
}
static linkedqueue_iter lq_impl_it_start(linkedqueue *_lqueue_)
{
    linkedqueue_iter iter;
    lq_iter_init(&iter, _lqueue_);
    lq_iter_to_start(&iter);
    return iter;
}
