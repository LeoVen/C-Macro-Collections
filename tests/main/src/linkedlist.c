#include "cmc/linkedlist.h"

typedef struct linkedlist_s
{
    struct linkedlist_node_s *head;
    struct linkedlist_node_s *tail;
    size_t count;
    struct linkedlist_iter_s (*it_start)(struct linkedlist_s *);
    struct linkedlist_iter_s (*it_end)(struct linkedlist_s *);
} linkedlist, *linkedlist_ptr;
typedef struct linkedlist_node_s
{
    size_t data;
    struct linkedlist_node_s *next;
    struct linkedlist_node_s *prev;
} linkedlist_node, *linkedlist_node_ptr;
typedef struct linkedlist_iter_s
{
    struct linkedlist_s *target;
    struct linkedlist_node_s *cursor;
    size_t index;
    bool start;
    bool end;
} linkedlist_iter, *linkedlist_iter_ptr;
linkedlist *ll_new(void);
void ll_clear(linkedlist *_list_);
void ll_free(linkedlist *_list_);
bool ll_push_front(linkedlist *_list_, size_t element);
bool ll_push(linkedlist *_list_, size_t element, size_t index);
bool ll_push_back(linkedlist *_list_, size_t element);
bool ll_pop_front(linkedlist *_list_);
bool ll_pop(linkedlist *_list_, size_t index);
bool ll_pop_back(linkedlist *_list_);
bool ll_push_if(linkedlist *_list_, size_t element, size_t index, bool condition);
bool ll_pop_if(linkedlist *_list_, size_t index, bool condition);
size_t ll_front(linkedlist *_list_);
size_t ll_get(linkedlist *_list_, size_t index);
size_t *ll_get_ref(linkedlist *_list_, size_t index);
size_t ll_back(linkedlist *_list_);
bool ll_contains(linkedlist *_list_, size_t element, int (*comparator)(size_t, size_t));
bool ll_empty(linkedlist *_list_);
size_t ll_count(linkedlist *_list_);
linkedlist_node *ll_new_node(size_t element);
void ll_free_node(linkedlist_node *_node_);
linkedlist_node *ll_head(linkedlist *_list_);
linkedlist_node *ll_get_node(linkedlist *_list_, size_t index);
linkedlist_node *ll_tail(linkedlist *_list_);
bool ll_insert_after(linkedlist *_owner_, linkedlist_node *_node_, size_t element);
bool ll_insert_before(linkedlist *_owner_, linkedlist_node *_node_, size_t element);
bool ll_remove_after(linkedlist *_owner_, linkedlist_node *_node_);
bool ll_remove_current(linkedlist *_owner_, linkedlist_node *_node_);
bool ll_remove_before(linkedlist *_owner_, linkedlist_node *_node_);
linkedlist_node *ll_next_node(linkedlist_node *_node_);
linkedlist_node *ll_prev_node(linkedlist_node *_node_);
linkedlist_iter *ll_iter_new(linkedlist *target);
void ll_iter_free(linkedlist_iter *iter);
void ll_iter_init(linkedlist_iter *iter, linkedlist *target);
bool ll_iter_start(linkedlist_iter *iter);
bool ll_iter_end(linkedlist_iter *iter);
void ll_iter_to_start(linkedlist_iter *iter);
void ll_iter_to_end(linkedlist_iter *iter);
bool ll_iter_next(linkedlist_iter *iter);
bool ll_iter_prev(linkedlist_iter *iter);
size_t ll_iter_value(linkedlist_iter *iter);
size_t *ll_iter_rvalue(linkedlist_iter *iter);
size_t ll_iter_index(linkedlist_iter *iter);
linkedlist_node *ll_iter_node(linkedlist_iter *iter);
static inline size_t ll_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
static linkedlist_iter ll_impl_it_start(linkedlist *_list_);
static linkedlist_iter ll_impl_it_end(linkedlist *_list_);
linkedlist *ll_new(void)
{
    linkedlist *_list_ = malloc(sizeof(linkedlist));
    if (!_list_)
        return NULL;
    _list_->count = 0;
    _list_->head = NULL;
    _list_->tail = NULL;
    _list_->it_start = ll_impl_it_start;
    _list_->it_end = ll_impl_it_end;
    return _list_;
}
void ll_clear(linkedlist *_list_)
{
    linkedlist_node *scan = _list_->head;
    while (_list_->head != NULL)
    {
        _list_->head = _list_->head->next;
        free(scan);
        scan = _list_->head;
    }
    _list_->count = 0;
    _list_->head = NULL;
    _list_->tail = NULL;
}
void ll_free(linkedlist *_list_)
{
    ll_clear(_list_);
    free(_list_);
}
bool ll_push_front(linkedlist *_list_, size_t element)
{
    linkedlist_node *_node_ = ll_new_node(element);
    if (!_node_)
        return false;
    if (ll_empty(_list_))
    {
        _list_->head = _node_;
        _list_->tail = _node_;
    }
    else
    {
        _node_->next = _list_->head;
        _list_->head->prev = _node_;
        _list_->head = _node_;
    }
    _list_->count++;
    return true;
}
bool ll_push(linkedlist *_list_, size_t element, size_t index)
{
    if (index > _list_->count)
        return false;
    if (index == 0)
    {
        return ll_push_front(_list_, element);
    }
    else if (index == _list_->count)
    {
        return ll_push_back(_list_, element);
    }
    linkedlist_node *_node_ = ll_new_node(element);
    if (!_node_)
        return false;
    linkedlist_node *scan = ll_get_node(_list_, index - 1);
    _node_->next = scan->next;
    _node_->prev = scan;
    _node_->next->prev = _node_;
    _node_->prev->next = _node_;
    _list_->count++;
    return true;
}
bool ll_push_back(linkedlist *_list_, size_t element)
{
    linkedlist_node *_node_ = ll_new_node(element);
    if (!_node_)
        return false;
    if (ll_empty(_list_))
    {
        _list_->head = _node_;
        _list_->tail = _node_;
    }
    else
    {
        _node_->prev = _list_->tail;
        _list_->tail->next = _node_;
        _list_->tail = _node_;
    }
    _list_->count++;
    return true;
}
bool ll_pop_front(linkedlist *_list_)
{
    if (ll_empty(_list_))
        return false;
    linkedlist_node *_node_ = _list_->head;
    _list_->head = _list_->head->next;
    free(_node_);
    if (_list_->head == NULL)
        _list_->tail = NULL;
    else
        _list_->head->prev = NULL;
    _list_->count--;
    return true;
}
bool ll_pop(linkedlist *_list_, size_t index)
{
    if (ll_empty(_list_))
        return false;
    if (index >= _list_->count)
        return false;
    if (index == 0)
    {
        return ll_pop_front(_list_);
    }
    else if (index == _list_->count - 1)
    {
        return ll_pop_back(_list_);
    }
    linkedlist_node *_node_ = ll_get_node(_list_, index);
    if (!_node_)
        return false;
    _node_->next->prev = _node_->prev;
    _node_->prev->next = _node_->next;
    free(_node_);
    _list_->count--;
    return true;
}
bool ll_pop_back(linkedlist *_list_)
{
    if (ll_empty(_list_))
        return false;
    linkedlist_node *_node_ = _list_->tail;
    _list_->tail = _list_->tail->prev;
    free(_node_);
    if (_list_->tail == NULL)
        _list_->head = NULL;
    else
        _list_->tail->next = NULL;
    _list_->count--;
    return true;
}
bool ll_push_if(linkedlist *_list_, size_t element, size_t index, bool condition)
{
    if (condition)
        return ll_push(_list_, element, index);
    return false;
}
bool ll_pop_if(linkedlist *_list_, size_t index, bool condition)
{
    if (condition)
        return ll_pop(_list_, index);
    return false;
}
size_t ll_front(linkedlist *_list_)
{
    if (ll_empty(_list_))
        return ll_impl_default_value();
    return _list_->head->data;
}
size_t ll_get(linkedlist *_list_, size_t index)
{
    if (index >= _list_->count || ll_empty(_list_))
        return ll_impl_default_value();
    linkedlist_node *scan = ll_get_node(_list_, index);
    if (scan == NULL)
        return ll_impl_default_value();
    return scan->data;
}
size_t *ll_get_ref(linkedlist *_list_, size_t index)
{
    if (index >= _list_->count)
        return NULL;
    if (ll_empty(_list_))
        return NULL;
    linkedlist_node *scan = ll_get_node(_list_, index);
    if (scan == NULL)
        return NULL;
    return &(scan->data);
}
size_t ll_back(linkedlist *_list_)
{
    if (ll_empty(_list_))
        return ll_impl_default_value();
    return _list_->tail->data;
}
bool ll_contains(linkedlist *_list_, size_t element, int (*comparator)(size_t, size_t))
{
    linkedlist_node *scan = _list_->head;
    while (scan != NULL)
    {
        if (comparator(scan->data, element) == 0)
            return true;
        scan = scan->next;
    }
    return false;
}
bool ll_empty(linkedlist *_list_) { return _list_->count == 0; }
size_t ll_count(linkedlist *_list_) { return _list_->count; }
linkedlist_node *ll_new_node(size_t element)
{
    linkedlist_node *_node_ = malloc(sizeof(linkedlist_node));
    if (!_node_)
        return NULL;
    _node_->data = element;
    _node_->next = NULL;
    _node_->prev = NULL;
    return _node_;
}
void ll_free_node(linkedlist_node *_node_) { free(_node_); }
linkedlist_node *ll_head(linkedlist *_list_) { return _list_->head; }
linkedlist_node *ll_get_node(linkedlist *_list_, size_t index)
{
    if (index >= _list_->count)
        return NULL;
    if (ll_empty(_list_))
        return NULL;
    linkedlist_node *_node_ = NULL;
    if (index <= _list_->count / 2)
    {
        _node_ = _list_->head;
        for (size_t i = 0; i < index; i++)
        {
            _node_ = _node_->next;
        }
    }
    else
    {
        _node_ = _list_->tail;
        for (size_t i = _list_->count - 1; i > index; i--)
        {
            _node_ = _node_->prev;
        }
    }
    return _node_;
}
linkedlist_node *ll_tail(linkedlist *_list_) { return _list_->tail; }
bool ll_insert_after(linkedlist *_owner_, linkedlist_node *_node_, size_t element)
{
    linkedlist_node *new_node = ll_new_node(element);
    if (!new_node)
        return false;
    new_node->next = _node_->next;
    if (_node_->next != NULL)
        _node_->next->prev = new_node;
    else
        _owner_->tail = new_node;
    new_node->prev = _node_;
    _node_->next = new_node;
    _owner_->count++;
    return true;
}
bool ll_insert_before(linkedlist *_owner_, linkedlist_node *_node_, size_t element)
{
    linkedlist_node *new_node = ll_new_node(element);
    if (!new_node)
        return false;
    new_node->prev = _node_->prev;
    if (_node_->prev != NULL)
        _node_->prev->next = new_node;
    else
        _owner_->head = new_node;
    new_node->next = _node_;
    _node_->prev = new_node;
    _owner_->count++;
    return true;
}
bool ll_remove_after(linkedlist *_owner_, linkedlist_node *_node_)
{
    if (_node_->next == NULL)
        return false;
    linkedlist_node *tmp = _node_->next;
    if (tmp->next != NULL)
    {
        _node_->next = _node_->next->next;
        _node_->next->prev = _node_;
    }
    else
        _owner_->tail = _node_;
    free(tmp);
    _owner_->count--;
    return true;
}
bool ll_remove_current(linkedlist *_owner_, linkedlist_node *_node_)
{
    if (_node_->prev != NULL)
        _node_->prev->next = _node_->next;
    else
        _owner_->head = _node_->next;
    if (_node_->next != NULL)
        _node_->next->prev = _node_->prev;
    else
        _owner_->tail = _node_->prev;
    free(_node_);
    _owner_->count--;
    return true;
}
bool ll_remove_before(linkedlist *_owner_, linkedlist_node *_node_)
{
    if (_node_->prev == NULL)
        return false;
    linkedlist_node *tmp = _node_->prev;
    if (tmp->prev != NULL)
    {
        _node_->prev = _node_->prev->prev;
        _node_->prev->next = _node_;
    }
    else
        _owner_->head = _node_;
    free(tmp);
    _owner_->count--;
    return true;
}
linkedlist_node *ll_next_node(linkedlist_node *_node_) { return _node_->next; }
linkedlist_node *ll_prev_node(linkedlist_node *_node_) { return _node_->prev; }
linkedlist_iter *ll_iter_new(linkedlist *target)
{
    linkedlist_iter *iter = malloc(sizeof(linkedlist_iter));
    if (!iter)
        return NULL;
    ll_iter_init(iter, target);
    return iter;
}
void ll_iter_free(linkedlist_iter *iter) { free(iter); }
void ll_iter_init(linkedlist_iter *iter, linkedlist *target)
{
    iter->target = target;
    iter->cursor = target->head;
    iter->index = 0;
    iter->start = true;
    iter->end = ll_empty(target);
}
bool ll_iter_start(linkedlist_iter *iter) { return ll_empty(iter->target) || iter->start; }
bool ll_iter_end(linkedlist_iter *iter) { return ll_empty(iter->target) || iter->end; }
void ll_iter_to_start(linkedlist_iter *iter)
{
    iter->cursor = iter->target->head;
    iter->index = 0;
    iter->start = true;
    iter->end = ll_empty(iter->target);
}
void ll_iter_to_end(linkedlist_iter *iter)
{
    iter->cursor = iter->target->tail;
    iter->index = iter->target->count - 1;
    iter->start = ll_empty(iter->target);
    iter->end = true;
}
bool ll_iter_next(linkedlist_iter *iter)
{
    if (iter->end)
        return false;
    iter->start = ll_empty(iter->target);
    if (iter->cursor->next == NULL)
        iter->end = true;
    else
    {
        iter->cursor = iter->cursor->next;
        iter->index++;
    }
    return true;
}
bool ll_iter_prev(linkedlist_iter *iter)
{
    if (iter->start)
        return false;
    iter->end = ll_empty(iter->target);
    if (iter->cursor->prev == NULL)
        iter->start = true;
    else
    {
        iter->cursor = iter->cursor->prev;
        iter->index--;
    }
    return true;
}
size_t ll_iter_value(linkedlist_iter *iter)
{
    if (ll_empty(iter->target))
        return ll_impl_default_value();
    return iter->cursor->data;
}
size_t *ll_iter_rvalue(linkedlist_iter *iter)
{
    if (ll_empty(iter->target))
        return NULL;
    return &(iter->cursor->data);
}
size_t ll_iter_index(linkedlist_iter *iter) { return iter->index; }
linkedlist_node *ll_iter_node(linkedlist_iter *iter) { return iter->cursor; }
static linkedlist_iter ll_impl_it_start(linkedlist *_list_)
{
    linkedlist_iter iter;
    ll_iter_init(&iter, _list_);
    ll_iter_to_start(&iter);
    return iter;
}
static linkedlist_iter ll_impl_it_end(linkedlist *_list_)
{
    linkedlist_iter iter;
    ll_iter_init(&iter, _list_);
    ll_iter_to_end(&iter);
    return iter;
}
