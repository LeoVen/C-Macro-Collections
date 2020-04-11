#include <cmc/linkedlist.h>

// CMC_GENERATE_LINKEDLIST(ll, linkedlist, size_t)

struct linkedlist
{
    struct linkedlist_node *head;
    struct linkedlist_node *tail;
    size_t count;
    int flag;
    struct linkedlist_ftab_val *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
    struct linkedlist_iter (*it_start)(struct linkedlist *);
    struct linkedlist_iter (*it_end)(struct linkedlist *);
};
struct linkedlist_node
{
    size_t value;
    struct linkedlist_node *next;
    struct linkedlist_node *prev;
};
struct linkedlist_ftab_val
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct linkedlist_iter
{
    struct linkedlist *target;
    struct linkedlist_node *cursor;
    size_t index;
    _Bool start;
    _Bool end;
};
struct linkedlist *ll_new(struct linkedlist_ftab_val *f_val);
struct linkedlist *ll_new_custom(struct linkedlist_ftab_val *f_val,
                                 struct cmc_alloc_node *alloc,
                                 struct cmc_callbacks *callbacks);
void ll_clear(struct linkedlist *_list_);
void ll_free(struct linkedlist *_list_);
void ll_customize(struct linkedlist *_list_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks);
_Bool ll_push_front(struct linkedlist *_list_, size_t element);
_Bool ll_push_at(struct linkedlist *_list_, size_t element, size_t index);
_Bool ll_push_back(struct linkedlist *_list_, size_t element);
_Bool ll_pop_front(struct linkedlist *_list_);
_Bool ll_pop_at(struct linkedlist *_list_, size_t index);
_Bool ll_pop_back(struct linkedlist *_list_);
size_t ll_front(struct linkedlist *_list_);
size_t ll_get(struct linkedlist *_list_, size_t index);
size_t *ll_get_ref(struct linkedlist *_list_, size_t index);
size_t ll_back(struct linkedlist *_list_);
_Bool ll_contains(struct linkedlist *_list_, size_t element);
_Bool ll_empty(struct linkedlist *_list_);
size_t ll_count(struct linkedlist *_list_);
int ll_flag(struct linkedlist *_list_);
struct linkedlist *ll_copy_of(struct linkedlist *_list_);
_Bool ll_equals(struct linkedlist *_list1_, struct linkedlist *_list2_);
struct cmc_string ll_to_string(struct linkedlist *_list_);
_Bool ll_print(struct linkedlist *_list_, FILE *fptr);
struct linkedlist_node *ll_new_node(struct linkedlist *_list_, size_t element);
void ll_free_node(struct linkedlist *_list_, struct linkedlist_node *_node_);
struct linkedlist_node *ll_head(struct linkedlist *_list_);
struct linkedlist_node *ll_get_node(struct linkedlist *_list_, size_t index);
struct linkedlist_node *ll_tail(struct linkedlist *_list_);
_Bool ll_add_next(struct linkedlist *_owner_, struct linkedlist_node *_node_,
                  size_t element);
_Bool ll_add_prev(struct linkedlist *_owner_, struct linkedlist_node *_node_,
                  size_t element);
_Bool ll_del_next(struct linkedlist *_owner_, struct linkedlist_node *_node_);
_Bool ll_del_curr(struct linkedlist *_owner_, struct linkedlist_node *_node_);
_Bool ll_del_prev(struct linkedlist *_owner_, struct linkedlist_node *_node_);
struct linkedlist_node *ll_next_node(struct linkedlist_node *_node_);
struct linkedlist_node *ll_prev_node(struct linkedlist_node *_node_);
struct linkedlist_iter *ll_iter_new(struct linkedlist *target);
void ll_iter_free(struct linkedlist_iter *iter);
void ll_iter_init(struct linkedlist_iter *iter, struct linkedlist *target);
_Bool ll_iter_start(struct linkedlist_iter *iter);
_Bool ll_iter_end(struct linkedlist_iter *iter);
void ll_iter_to_start(struct linkedlist_iter *iter);
void ll_iter_to_end(struct linkedlist_iter *iter);
_Bool ll_iter_next(struct linkedlist_iter *iter);
_Bool ll_iter_prev(struct linkedlist_iter *iter);
_Bool ll_iter_advance(struct linkedlist_iter *iter, size_t steps);
_Bool ll_iter_rewind(struct linkedlist_iter *iter, size_t steps);
_Bool ll_iter_go_to(struct linkedlist_iter *iter, size_t index);
size_t ll_iter_value(struct linkedlist_iter *iter);
size_t *ll_iter_rvalue(struct linkedlist_iter *iter);
size_t ll_iter_index(struct linkedlist_iter *iter);
struct linkedlist_node *ll_iter_node(struct linkedlist_iter *iter);
static struct linkedlist_iter ll_impl_it_start(struct linkedlist *_list_);
static struct linkedlist_iter ll_impl_it_end(struct linkedlist *_list_);
struct linkedlist *ll_new(struct linkedlist_ftab_val *f_val)
{
    if (!f_val)
        return ((void *)0);
    struct cmc_alloc_node *alloc = &cmc_alloc_node_default;
    struct linkedlist *_list_ = alloc->malloc(sizeof(struct linkedlist));
    if (!_list_)
        return ((void *)0);
    _list_->count = 0;
    _list_->head = ((void *)0);
    _list_->tail = ((void *)0);
    _list_->flag = cmc_flags.OK;
    _list_->f_val = f_val;
    _list_->alloc = alloc;
    _list_->callbacks = ((void *)0);
    _list_->it_start = ll_impl_it_start;
    _list_->it_end = ll_impl_it_end;
    return _list_;
}
struct linkedlist *ll_new_custom(struct linkedlist_ftab_val *f_val,
                                 struct cmc_alloc_node *alloc,
                                 struct cmc_callbacks *callbacks)
{
    if (!f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct linkedlist *_list_ = alloc->malloc(sizeof(struct linkedlist));
    if (!_list_)
        return ((void *)0);
    _list_->count = 0;
    _list_->head = ((void *)0);
    _list_->tail = ((void *)0);
    _list_->flag = cmc_flags.OK;
    _list_->f_val = f_val;
    _list_->alloc = alloc;
    _list_->callbacks = callbacks;
    _list_->it_start = ll_impl_it_start;
    _list_->it_end = ll_impl_it_end;
    return _list_;
}
void ll_clear(struct linkedlist *_list_)
{
    struct linkedlist_node *scan = _list_->head;
    while (_list_->head != ((void *)0))
    {
        _list_->head = _list_->head->next;
        if (_list_->f_val->free)
        {
            _list_->f_val->free(scan->value);
        }
        _list_->alloc->free(scan);
        scan = _list_->head;
    }
    _list_->count = 0;
    _list_->head = ((void *)0);
    _list_->tail = ((void *)0);
    _list_->flag = cmc_flags.OK;
}
void ll_free(struct linkedlist *_list_)
{
    ll_clear(_list_);
    _list_->alloc->free(_list_);
}
void ll_customize(struct linkedlist *_list_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _list_->alloc = &cmc_alloc_node_default;
    else
        _list_->alloc = alloc;
    _list_->callbacks = callbacks;
    _list_->flag = cmc_flags.OK;
}
_Bool ll_push_front(struct linkedlist *_list_, size_t element)
{
    struct linkedlist_node *_node_ = ll_new_node(_list_, element);
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
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->create)
        _list_->callbacks->create();
    return 1;
}
_Bool ll_push_at(struct linkedlist *_list_, size_t element, size_t index)
{
    if (index > _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return 0;
    }
    if (index == 0)
    {
        return ll_push_front(_list_, element);
    }
    else if (index == _list_->count)
    {
        return ll_push_back(_list_, element);
    }
    struct linkedlist_node *_node_ = ll_new_node(_list_, element);
    if (!_node_)
        return 0;
    struct linkedlist_node *scan = ll_get_node(_list_, index - 1);
    _node_->next = scan->next;
    _node_->prev = scan;
    _node_->next->prev = _node_;
    _node_->prev->next = _node_;
    _list_->count++;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->create)
        _list_->callbacks->create();
    return 1;
}
_Bool ll_push_back(struct linkedlist *_list_, size_t element)
{
    struct linkedlist_node *_node_ = ll_new_node(_list_, element);
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
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->create)
        _list_->callbacks->create();
    return 1;
}
_Bool ll_pop_front(struct linkedlist *_list_)
{
    if (ll_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct linkedlist_node *_node_ = _list_->head;
    _list_->head = _list_->head->next;
    _list_->alloc->free(_node_);
    if (_list_->head == ((void *)0))
        _list_->tail = ((void *)0);
    else
        _list_->head->prev = ((void *)0);
    _list_->count--;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->delete)
        _list_->callbacks->delete ();
    return 1;
}
_Bool ll_pop_at(struct linkedlist *_list_, size_t index)
{
    if (ll_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return 0;
    }
    if (index >= _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return 0;
    }
    if (index == 0)
    {
        return ll_pop_front(_list_);
    }
    else if (index == _list_->count - 1)
    {
        return ll_pop_back(_list_);
    }
    struct linkedlist_node *_node_ = ll_get_node(_list_, index);
    if (!_node_)
        return 0;
    _node_->next->prev = _node_->prev;
    _node_->prev->next = _node_->next;
    _list_->alloc->free(_node_);
    _list_->count--;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->delete)
        _list_->callbacks->delete ();
    return 1;
}
_Bool ll_pop_back(struct linkedlist *_list_)
{
    if (ll_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct linkedlist_node *_node_ = _list_->tail;
    _list_->tail = _list_->tail->prev;
    _list_->alloc->free(_node_);
    if (_list_->tail == ((void *)0))
        _list_->head = ((void *)0);
    else
        _list_->tail->next = ((void *)0);
    _list_->count--;
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->delete)
        _list_->callbacks->delete ();
    return 1;
}
size_t ll_front(struct linkedlist *_list_)
{
    if (ll_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return (size_t){ 0 };
    }
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->read)
        _list_->callbacks->read();
    return _list_->head->value;
}
size_t ll_get(struct linkedlist *_list_, size_t index)
{
    if (ll_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return (size_t){ 0 };
    }
    if (index >= _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return (size_t){ 0 };
    }
    struct linkedlist_node *scan = ll_get_node(_list_, index);
    if (scan == ((void *)0))
        return (size_t){ 0 };
    if (_list_->callbacks && _list_->callbacks->read)
        _list_->callbacks->read();
    return scan->value;
}
size_t *ll_get_ref(struct linkedlist *_list_, size_t index)
{
    if (ll_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return ((void *)0);
    }
    if (index >= _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return ((void *)0);
    }
    struct linkedlist_node *scan = ll_get_node(_list_, index);
    if (scan == ((void *)0))
        return ((void *)0);
    if (_list_->callbacks && _list_->callbacks->read)
        _list_->callbacks->read();
    return &(scan->value);
}
size_t ll_back(struct linkedlist *_list_)
{
    if (ll_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return (size_t){ 0 };
    }
    _list_->flag = cmc_flags.OK;
    if (_list_->callbacks && _list_->callbacks->read)
        _list_->callbacks->read();
    return _list_->tail->value;
}
_Bool ll_contains(struct linkedlist *_list_, size_t element)
{
    _list_->flag = cmc_flags.OK;
    _Bool result = 0;
    struct linkedlist_node *scan = _list_->head;
    while (scan != ((void *)0))
    {
        if (_list_->f_val->cmp(scan->value, element) == 0)
        {
            result = 1;
            break;
        }
        scan = scan->next;
    }
    if (_list_->callbacks && _list_->callbacks->read)
        _list_->callbacks->read();
    return result;
}
_Bool ll_empty(struct linkedlist *_list_)
{
    return _list_->count == 0;
}
size_t ll_count(struct linkedlist *_list_)
{
    return _list_->count;
}
int ll_flag(struct linkedlist *_list_)
{
    return _list_->flag;
}
struct linkedlist *ll_copy_of(struct linkedlist *_list_)
{
    struct linkedlist *result =
        ll_new_custom(_list_->f_val, _list_->alloc, _list_->callbacks);
    if (!result)
        return ((void *)0);
    struct linkedlist_node *scan = _list_->head;
    while (scan != ((void *)0))
    {
        struct linkedlist_node *new_node;
        if (_list_->f_val->cpy)
            new_node = ll_new_node(_list_, _list_->f_val->cpy(scan->value));
        else
            new_node = ll_new_node(_list_, scan->value);
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
    _list_->flag = cmc_flags.OK;
    return result;
}
_Bool ll_equals(struct linkedlist *_list1_, struct linkedlist *_list2_)
{
    _list1_->flag = cmc_flags.OK;
    _list2_->flag = cmc_flags.OK;
    if (_list1_->count != _list2_->count)
        return 0;
    struct linkedlist_node *scan1 = _list1_->head;
    struct linkedlist_node *scan2 = _list2_->head;
    while (scan1 != ((void *)0) && scan2 != ((void *)0))
    {
        if (_list1_->f_val->cmp(scan1->value, scan2->value) != 0)
            return 0;
        scan1 = scan1->next;
        scan2 = scan2->next;
    }
    return 1;
}
struct cmc_string ll_to_string(struct linkedlist *_list_)
{
    struct cmc_string str;
    struct linkedlist *l_ = _list_;
    int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_linkedlist,
                     "linkedlist", "size_t", l_, l_->count, l_->head, l_->tail,
                     l_->flag, l_->f_val, l_->alloc, l_->callbacks);
    return n >= 0 ? str : (struct cmc_string){ 0 };
}
_Bool ll_print(struct linkedlist *_list_, FILE *fptr)
{
    struct linkedlist_node *scan = _list_->head;
    while (scan != ((void *)0))
    {
        if (!_list_->f_val->str(fptr, scan->value))
            return 0;
        scan = scan->next;
    }
    return 1;
}
struct linkedlist_node *ll_new_node(struct linkedlist *_list_, size_t element)
{
    struct linkedlist_node *_node_ =
        _list_->alloc->malloc(sizeof(struct linkedlist_node));
    if (!_node_)
    {
        _list_->flag = cmc_flags.ALLOC;
        return ((void *)0);
    }
    _node_->value = element;
    _node_->next = ((void *)0);
    _node_->prev = ((void *)0);
    return _node_;
}
void ll_free_node(struct linkedlist *_list_, struct linkedlist_node *_node_)
{
    _list_->alloc->free(_node_);
}
struct linkedlist_node *ll_head(struct linkedlist *_list_)
{
    return _list_->head;
}
struct linkedlist_node *ll_get_node(struct linkedlist *_list_, size_t index)
{
    if (ll_empty(_list_))
    {
        _list_->flag = cmc_flags.EMPTY;
        return ((void *)0);
    }
    if (index >= _list_->count)
    {
        _list_->flag = cmc_flags.RANGE;
        return ((void *)0);
    }
    struct linkedlist_node *_node_ = ((void *)0);
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
    _list_->flag = cmc_flags.OK;
    return _node_;
}
struct linkedlist_node *ll_tail(struct linkedlist *_list_)
{
    return _list_->tail;
}
_Bool ll_add_next(struct linkedlist *_owner_, struct linkedlist_node *_node_,
                  size_t element)
{
    struct linkedlist_node *new_node = ll_new_node(_owner_, element);
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
    _owner_->flag = cmc_flags.OK;
    return 1;
}
_Bool ll_add_prev(struct linkedlist *_owner_, struct linkedlist_node *_node_,
                  size_t element)
{
    struct linkedlist_node *new_node = ll_new_node(_owner_, element);
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
    _owner_->flag = cmc_flags.OK;
    return 1;
}
_Bool ll_del_next(struct linkedlist *_owner_, struct linkedlist_node *_node_)
{
    if (_node_->next == ((void *)0))
    {
        _owner_->flag = cmc_flags.INVALID;
        return 0;
    }
    struct linkedlist_node *tmp = _node_->next;
    _node_->next = _node_->next->next;
    if (tmp->next != ((void *)0))
        tmp->next->prev = _node_;
    else
        _owner_->tail = _node_;
    _owner_->alloc->free(tmp);
    _owner_->count--;
    _owner_->flag = cmc_flags.OK;
    return 1;
}
_Bool ll_del_curr(struct linkedlist *_owner_, struct linkedlist_node *_node_)
{
    if (_node_->prev != ((void *)0))
        _node_->prev->next = _node_->next;
    else
        _owner_->head = _node_->next;
    if (_node_->next != ((void *)0))
        _node_->next->prev = _node_->prev;
    else
        _owner_->tail = _node_->prev;
    _owner_->alloc->free(_node_);
    _owner_->count--;
    _owner_->flag = cmc_flags.OK;
    return 1;
}
_Bool ll_del_prev(struct linkedlist *_owner_, struct linkedlist_node *_node_)
{
    if (_node_->prev == ((void *)0))
    {
        _owner_->flag = cmc_flags.INVALID;
        return 0;
    }
    struct linkedlist_node *tmp = _node_->prev;
    _node_->prev = _node_->prev->prev;
    if (tmp->prev != ((void *)0))
        tmp->prev->next = _node_;
    else
        _owner_->head = _node_;
    _owner_->alloc->free(tmp);
    _owner_->count--;
    _owner_->flag = cmc_flags.OK;
    return 1;
}
struct linkedlist_node *ll_next_node(struct linkedlist_node *_node_)
{
    return _node_->next;
}
struct linkedlist_node *ll_prev_node(struct linkedlist_node *_node_)
{
    return _node_->prev;
}
struct linkedlist_iter *ll_iter_new(struct linkedlist *target)
{
    struct linkedlist_iter *iter =
        target->alloc->malloc(sizeof(struct linkedlist_iter));
    if (!iter)
        return ((void *)0);
    ll_iter_init(iter, target);
    return iter;
}
void ll_iter_free(struct linkedlist_iter *iter)
{
    iter->target->alloc->free(iter);
}
void ll_iter_init(struct linkedlist_iter *iter, struct linkedlist *target)
{
    iter->target = target;
    iter->cursor = target->head;
    iter->index = 0;
    iter->start = 1;
    iter->end = ll_empty(target);
}
_Bool ll_iter_start(struct linkedlist_iter *iter)
{
    return ll_empty(iter->target) || iter->start;
}
_Bool ll_iter_end(struct linkedlist_iter *iter)
{
    return ll_empty(iter->target) || iter->end;
}
void ll_iter_to_start(struct linkedlist_iter *iter)
{
    if (!ll_empty(iter->target))
    {
        iter->cursor = iter->target->head;
        iter->index = 0;
        iter->start = 1;
        iter->end = ll_empty(iter->target);
    }
}
void ll_iter_to_end(struct linkedlist_iter *iter)
{
    if (!ll_empty(iter->target))
    {
        iter->cursor = iter->target->tail;
        iter->index = iter->target->count - 1;
        iter->start = ll_empty(iter->target);
        iter->end = 1;
    }
}
_Bool ll_iter_next(struct linkedlist_iter *iter)
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
_Bool ll_iter_prev(struct linkedlist_iter *iter)
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
_Bool ll_iter_advance(struct linkedlist_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->cursor->next == ((void *)0))
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= iter->target->count)
        return 0;
    iter->start = ll_empty(iter->target);
    iter->index += steps;
    for (size_t i = 0; i < steps; i++)
        iter->cursor = iter->cursor->next;
    return 1;
}
_Bool ll_iter_rewind(struct linkedlist_iter *iter, size_t steps)
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
_Bool ll_iter_go_to(struct linkedlist_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return ll_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return ll_iter_advance(iter, index - iter->index);
    return 1;
}
size_t ll_iter_value(struct linkedlist_iter *iter)
{
    if (ll_empty(iter->target))
        return (size_t){ 0 };
    return iter->cursor->value;
}
size_t *ll_iter_rvalue(struct linkedlist_iter *iter)
{
    if (ll_empty(iter->target))
        return ((void *)0);
    return &(iter->cursor->value);
}
size_t ll_iter_index(struct linkedlist_iter *iter)
{
    return iter->index;
}
struct linkedlist_node *ll_iter_node(struct linkedlist_iter *iter)
{
    return iter->cursor;
}
static struct linkedlist_iter ll_impl_it_start(struct linkedlist *_list_)
{
    struct linkedlist_iter iter;
    ll_iter_init(&iter, _list_);
    ll_iter_to_start(&iter);
    return iter;
}
static struct linkedlist_iter ll_impl_it_end(struct linkedlist *_list_)
{
    struct linkedlist_iter iter;
    ll_iter_init(&iter, _list_);
    ll_iter_to_end(&iter);
    return iter;
}
