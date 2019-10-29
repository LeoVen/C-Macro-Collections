#include <cmc/linkedlist.h>

//CMC_GENERATE_LINKEDLIST(ll, linkedlist, size_t)

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
    _Bool start;
    _Bool end;
} linkedlist_iter, *linkedlist_iter_ptr;
linkedlist *ll_new(void);
void ll_clear(linkedlist *_list_, void (*deallocator)(size_t));
void ll_free(linkedlist *_list_, void (*deallocator)(size_t));
_Bool ll_push_front(linkedlist *_list_, size_t element);
_Bool ll_push_at(linkedlist *_list_, size_t element, size_t index);
_Bool ll_push_back(linkedlist *_list_, size_t element);
_Bool ll_pop_front(linkedlist *_list_);
_Bool ll_pop_at(linkedlist *_list_, size_t index);
_Bool ll_pop_back(linkedlist *_list_);
size_t ll_front(linkedlist *_list_);
size_t ll_get(linkedlist *_list_, size_t index);
size_t *ll_get_ref(linkedlist *_list_, size_t index);
size_t ll_back(linkedlist *_list_);
_Bool ll_contains(linkedlist *_list_, size_t element, int (*comparator)(size_t, size_t));
_Bool ll_empty(linkedlist *_list_);
size_t ll_count(linkedlist *_list_);
linkedlist *ll_copy_of(linkedlist *_list_, size_t (*copy_func)(size_t));
_Bool ll_equals(linkedlist *_list1_, linkedlist *_list2_, int (*comparator)(size_t, size_t));
struct cmc_string ll_to_string(linkedlist *_list_);
linkedlist_node *ll_new_node(size_t element);
void ll_free_node(linkedlist_node *_node_);
linkedlist_node *ll_head(linkedlist *_list_);
linkedlist_node *ll_get_node(linkedlist *_list_, size_t index);
linkedlist_node *ll_tail(linkedlist *_list_);
_Bool ll_insert_after(linkedlist *_owner_, linkedlist_node *_node_, size_t element);
_Bool ll_insert_before(linkedlist *_owner_, linkedlist_node *_node_, size_t element);
_Bool ll_remove_after(linkedlist *_owner_, linkedlist_node *_node_);
_Bool ll_remove_current(linkedlist *_owner_, linkedlist_node *_node_);
_Bool ll_remove_before(linkedlist *_owner_, linkedlist_node *_node_);
linkedlist_node *ll_next_node(linkedlist_node *_node_);
linkedlist_node *ll_prev_node(linkedlist_node *_node_);
linkedlist_iter *ll_iter_new(linkedlist *target);
void ll_iter_free(linkedlist_iter *iter);
void ll_iter_init(linkedlist_iter *iter, linkedlist *target);
_Bool ll_iter_start(linkedlist_iter *iter);
_Bool ll_iter_end(linkedlist_iter *iter);
void ll_iter_to_start(linkedlist_iter *iter);
void ll_iter_to_end(linkedlist_iter *iter);
_Bool ll_iter_next(linkedlist_iter *iter);
_Bool ll_iter_prev(linkedlist_iter *iter);
_Bool ll_iter_advance(linkedlist_iter *iter, size_t steps);
_Bool ll_iter_rewind(linkedlist_iter *iter, size_t steps);
_Bool ll_iter_go_to(linkedlist_iter *iter, size_t index);
size_t ll_iter_value(linkedlist_iter *iter);
size_t *ll_iter_rvalue(linkedlist_iter *iter);
size_t ll_iter_index(linkedlist_iter *iter);
linkedlist_node *ll_iter_node(linkedlist_iter *iter);
static linkedlist_iter ll_impl_it_start(linkedlist *_list_);
static linkedlist_iter ll_impl_it_end(linkedlist *_list_);
linkedlist *ll_new(void)
{
    linkedlist *_list_ = malloc(sizeof(linkedlist));
    if (!_list_)
        return ((void *)0);
    _list_->count = 0;
    _list_->head = ((void *)0);
    _list_->tail = ((void *)0);
    _list_->it_start = ll_impl_it_start;
    _list_->it_end = ll_impl_it_end;
    return _list_;
}
void ll_clear(linkedlist *_list_, void (*deallocator)(size_t))
{
    linkedlist_node *scan = _list_->head;
    while (_list_->head != ((void *)0))
    {
        _list_->head = _list_->head->next;
        if (deallocator)
        {
            deallocator(scan->data);
        }
        free(scan);
        scan = _list_->head;
    }
    _list_->count = 0;
    _list_->head = ((void *)0);
    _list_->tail = ((void *)0);
}
void ll_free(linkedlist *_list_, void (*deallocator)(size_t))
{
    ll_clear(_list_, deallocator);
    free(_list_);
}
_Bool ll_push_front(linkedlist *_list_, size_t element)
{
    linkedlist_node *_node_ = ll_new_node(element);
    if (!_node_)
        return 0;
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
    return 1;
}
_Bool ll_push_at(linkedlist *_list_, size_t element, size_t index)
{
    if (index > _list_->count)
        return 0;
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
        return 0;
    linkedlist_node *scan = ll_get_node(_list_, index - 1);
    _node_->next = scan->next;
    _node_->prev = scan;
    _node_->next->prev = _node_;
    _node_->prev->next = _node_;
    _list_->count++;
    return 1;
}
_Bool ll_push_back(linkedlist *_list_, size_t element)
{
    linkedlist_node *_node_ = ll_new_node(element);
    if (!_node_)
        return 0;
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
    return 1;
}
_Bool ll_pop_front(linkedlist *_list_)
{
    if (ll_empty(_list_))
        return 0;
    linkedlist_node *_node_ = _list_->head;
    _list_->head = _list_->head->next;
    free(_node_);
    if (_list_->head == ((void *)0))
        _list_->tail = ((void *)0);
    else
        _list_->head->prev = ((void *)0);
    _list_->count--;
    return 1;
}
_Bool ll_pop_at(linkedlist *_list_, size_t index)
{
    if (ll_empty(_list_))
        return 0;
    if (index >= _list_->count)
        return 0;
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
        return 0;
    _node_->next->prev = _node_->prev;
    _node_->prev->next = _node_->next;
    free(_node_);
    _list_->count--;
    return 1;
}
_Bool ll_pop_back(linkedlist *_list_)
{
    if (ll_empty(_list_))
        return 0;
    linkedlist_node *_node_ = _list_->tail;
    _list_->tail = _list_->tail->prev;
    free(_node_);
    if (_list_->tail == ((void *)0))
        _list_->head = ((void *)0);
    else
        _list_->tail->next = ((void *)0);
    _list_->count--;
    return 1;
}
size_t ll_front(linkedlist *_list_)
{
    if (ll_empty(_list_))
        return (size_t){0};
    return _list_->head->data;
}
size_t ll_get(linkedlist *_list_, size_t index)
{
    if (index >= _list_->count || ll_empty(_list_))
        return (size_t){0};
    linkedlist_node *scan = ll_get_node(_list_, index);
    if (scan == ((void *)0))
        return (size_t){0};
    return scan->data;
}
size_t *ll_get_ref(linkedlist *_list_, size_t index)
{
    if (index >= _list_->count)
        return ((void *)0);
    if (ll_empty(_list_))
        return ((void *)0);
    linkedlist_node *scan = ll_get_node(_list_, index);
    if (scan == ((void *)0))
        return ((void *)0);
    return &(scan->data);
}
size_t ll_back(linkedlist *_list_)
{
    if (ll_empty(_list_))
        return (size_t){0};
    return _list_->tail->data;
}
_Bool ll_contains(linkedlist *_list_, size_t element, int (*comparator)(size_t, size_t))
{
    linkedlist_node *scan = _list_->head;
    while (scan != ((void *)0))
    {
        if (comparator(scan->data, element) == 0)
            return 1;
        scan = scan->next;
    }
    return 0;
}
_Bool ll_empty(linkedlist *_list_) { return _list_->count == 0; }
size_t ll_count(linkedlist *_list_) { return _list_->count; }
linkedlist *ll_copy_of(linkedlist *_list_, size_t (*copy_func)(size_t))
{
    linkedlist *result = ll_new();
    if (!result)
        return ((void *)0);
    linkedlist_node *scan = _list_->head;
    while (scan != ((void *)0))
    {
        linkedlist_node *new_node;
        if (copy_func)
            new_node = ll_new_node(copy_func(scan->data));
        else
            new_node = ll_new_node(scan->data);
        if (!result->head)
        {
            result->head = new_node;
            result->tail = new_node;
        }
        else
        {
            new_node->prev = result->tail;
            result->tail->next = new_node;
            result->tail = new_node;
        }
        scan = scan->next;
    }
    result->count = _list_->count;
    return result;
}
_Bool ll_equals(linkedlist *_list1_, linkedlist *_list2_, int (*comparator)(size_t, size_t))
{
    if (ll_count(_list1_) != ll_count(_list2_))
        return 0;
    linkedlist_node *scan1 = _list1_->head;
    linkedlist_node *scan2 = _list2_->head;
    while (scan1 != ((void *)0) && scan2 != ((void *)0))
    {
        if (comparator(scan1->data, scan2->data) != 0)
            return 0;
        scan1 = scan1->next;
        scan2 = scan2->next;
    }
    return 1;
}
struct cmc_string ll_to_string(linkedlist *_list_)
{
    struct cmc_string str;
    linkedlist *l_ = _list_;
    const char *name = "linkedlist";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_linkedlist, name, l_, l_->count, l_->head, l_->tail);
    return str;
}
linkedlist_node *ll_new_node(size_t element)
{
    linkedlist_node *_node_ = malloc(sizeof(linkedlist_node));
    if (!_node_)
        return ((void *)0);
    _node_->data = element;
    _node_->next = ((void *)0);
    _node_->prev = ((void *)0);
    return _node_;
}
void ll_free_node(linkedlist_node *_node_) { free(_node_); }
linkedlist_node *ll_head(linkedlist *_list_) { return _list_->head; }
linkedlist_node *ll_get_node(linkedlist *_list_, size_t index)
{
    if (index >= _list_->count)
        return ((void *)0);
    if (ll_empty(_list_))
        return ((void *)0);
    linkedlist_node *_node_ = ((void *)0);
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
_Bool ll_insert_after(linkedlist *_owner_, linkedlist_node *_node_, size_t element)
{
    linkedlist_node *new_node = ll_new_node(element);
    if (!new_node)
        return 0;
    new_node->next = _node_->next;
    if (_node_->next != ((void *)0))
        _node_->next->prev = new_node;
    else
        _owner_->tail = new_node;
    new_node->prev = _node_;
    _node_->next = new_node;
    _owner_->count++;
    return 1;
}
_Bool ll_insert_before(linkedlist *_owner_, linkedlist_node *_node_, size_t element)
{
    linkedlist_node *new_node = ll_new_node(element);
    if (!new_node)
        return 0;
    new_node->prev = _node_->prev;
    if (_node_->prev != ((void *)0))
        _node_->prev->next = new_node;
    else
        _owner_->head = new_node;
    new_node->next = _node_;
    _node_->prev = new_node;
    _owner_->count++;
    return 1;
}
_Bool ll_remove_after(linkedlist *_owner_, linkedlist_node *_node_)
{
    if (_node_->next == ((void *)0))
        return 0;
    linkedlist_node *tmp = _node_->next;
    if (tmp->next != ((void *)0))
    {
        _node_->next = _node_->next->next;
        _node_->next->prev = _node_;
    }
    else
        _owner_->tail = _node_;
    free(tmp);
    _owner_->count--;
    return 1;
}
_Bool ll_remove_current(linkedlist *_owner_, linkedlist_node *_node_)
{
    if (_node_->prev != ((void *)0))
        _node_->prev->next = _node_->next;
    else
        _owner_->head = _node_->next;
    if (_node_->next != ((void *)0))
        _node_->next->prev = _node_->prev;
    else
        _owner_->tail = _node_->prev;
    free(_node_);
    _owner_->count--;
    return 1;
}
_Bool ll_remove_before(linkedlist *_owner_, linkedlist_node *_node_)
{
    if (_node_->prev == ((void *)0))
        return 0;
    linkedlist_node *tmp = _node_->prev;
    if (tmp->prev != ((void *)0))
    {
        _node_->prev = _node_->prev->prev;
        _node_->prev->next = _node_;
    }
    else
        _owner_->head = _node_;
    free(tmp);
    _owner_->count--;
    return 1;
}
linkedlist_node *ll_next_node(linkedlist_node *_node_) { return _node_->next; }
linkedlist_node *ll_prev_node(linkedlist_node *_node_) { return _node_->prev; }
linkedlist_iter *ll_iter_new(linkedlist *target)
{
    linkedlist_iter *iter = malloc(sizeof(linkedlist_iter));
    if (!iter)
        return ((void *)0);
    ll_iter_init(iter, target);
    return iter;
}
void ll_iter_free(linkedlist_iter *iter) { free(iter); }
void ll_iter_init(linkedlist_iter *iter, linkedlist *target)
{
    iter->target = target;
    iter->cursor = target->head;
    iter->index = 0;
    iter->start = 1;
    iter->end = ll_empty(target);
}
_Bool ll_iter_start(linkedlist_iter *iter) { return ll_empty(iter->target) || iter->start; }
_Bool ll_iter_end(linkedlist_iter *iter) { return ll_empty(iter->target) || iter->end; }
void ll_iter_to_start(linkedlist_iter *iter)
{
    if (!ll_empty(iter->target))
    {
        iter->cursor = iter->target->head;
        iter->index = 0;
        iter->start = 1;
        iter->end = ll_empty(iter->target);
    }
}
void ll_iter_to_end(linkedlist_iter *iter)
{
    if (!ll_empty(iter->target))
    {
        iter->cursor = iter->target->tail;
        iter->index = iter->target->count - 1;
        iter->start = ll_empty(iter->target);
        iter->end = 1;
    }
}
_Bool ll_iter_next(linkedlist_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor->next == ((void *)0))
    {
        iter->end = 1;
        return 0;
    }
    iter->start = ll_empty(iter->target);
    iter->cursor = iter->cursor->next;
    iter->index++;
    return 1;
}
_Bool ll_iter_prev(linkedlist_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor->prev == ((void *)0))
    {
        iter->start = 1;
        return 0;
    }
    iter->end = ll_empty(iter->target);
    iter->cursor = iter->cursor->prev;
    iter->index--;
    return 1;
}
_Bool ll_iter_advance(linkedlist_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->cursor->next == ((void *)0))
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= ll_count(iter->target))
        return 0;
    iter->start = ll_empty(iter->target);
    iter->index += steps;
    for (size_t i = 0; i < steps; i++)
        iter->cursor = iter->cursor->next;
    return 1;
}
_Bool ll_iter_rewind(linkedlist_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->cursor->prev == ((void *)0))
    {
        iter->start = 1;
        return 0;
    }
    if (steps == 0 || iter->index < steps)
        return 0;
    iter->end = ll_empty(iter->target);
    iter->index -= steps;
    for (size_t i = 0; i < steps; i++)
        iter->cursor = iter->cursor->prev;
    return 1;
}
_Bool ll_iter_go_to(linkedlist_iter *iter, size_t index)
{
    if (index >= ll_count(iter->target))
        return 0;
    if (iter->index > index)
        return ll_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return ll_iter_advance(iter, index - iter->index);
    return 1;
}
size_t ll_iter_value(linkedlist_iter *iter)
{
    if (ll_empty(iter->target))
        return (size_t){0};
    return iter->cursor->data;
}
size_t *ll_iter_rvalue(linkedlist_iter *iter)
{
    if (ll_empty(iter->target))
        return ((void *)0);
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
