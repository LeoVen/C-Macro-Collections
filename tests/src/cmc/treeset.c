
#include "cmc/treeset.h"

static struct treeset_node *ts_impl_new_node(struct treeset *_set_, size_t value);
static struct treeset_node *ts_impl_get_node(struct treeset *_set_, size_t value);
static unsigned char ts_impl_h(struct treeset_node *node);
static unsigned char ts_impl_hupdate(struct treeset_node *node);
static void ts_impl_rotate_right(struct treeset_node **Z);
static void ts_impl_rotate_left(struct treeset_node **Z);
static void ts_impl_rebalance(struct treeset *_set_, struct treeset_node *node);
struct treeset *ts_new(struct treeset_fval *f_val)
{
    return ts_new_custom(f_val, ((void *)0), ((void *)0));
}
struct treeset *ts_new_custom(struct treeset_fval *f_val, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    ;
    if (!f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct treeset *_set_ = alloc->malloc(sizeof(struct treeset));
    if (!_set_)
        return ((void *)0);
    _set_->count = 0;
    _set_->root = ((void *)0);
    _set_->flag = CMC_FLAG_OK;
    _set_->f_val = f_val;
    _set_->alloc = alloc;
    (_set_)->callbacks = callbacks;
    return _set_;
}
void ts_clear(struct treeset *_set_)
{
    struct treeset_node *scan = _set_->root;
    struct treeset_node *up = ((void *)0);
    while (scan != ((void *)0))
    {
        if (scan->left != ((void *)0))
        {
            struct treeset_node *left = scan->left;
            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != ((void *)0))
        {
            struct treeset_node *right = scan->right;
            scan->left = up;
            scan->right = ((void *)0);
            up = scan;
            scan = right;
        }
        else
        {
            if (up == ((void *)0))
            {
                if (_set_->f_val->free)
                    _set_->f_val->free(scan->value);
                _set_->alloc->free(scan);
                scan = ((void *)0);
            }
            while (up != ((void *)0))
            {
                if (_set_->f_val->free)
                    _set_->f_val->free(scan->value);
                _set_->alloc->free(scan);
                if (up->right != ((void *)0))
                {
                    scan = up->right;
                    up->right = ((void *)0);
                    break;
                }
                else
                {
                    scan = up;
                    up = up->left;
                }
            }
        }
    }
    _set_->count = 0;
    _set_->root = ((void *)0);
    _set_->flag = CMC_FLAG_OK;
}
void ts_free(struct treeset *_set_)
{
    ts_clear(_set_);
    _set_->alloc->free(_set_);
}
void ts_customize(struct treeset *_set_, struct cmc_alloc_node *alloc, struct cmc_callbacks *callbacks)
{
    ;
    if (!alloc)
        _set_->alloc = &cmc_alloc_node_default;
    else
        _set_->alloc = alloc;
    (_set_)->callbacks = callbacks;
    _set_->flag = CMC_FLAG_OK;
}
_Bool ts_insert(struct treeset *_set_, size_t value)
{
    if (ts_empty(_set_))
    {
        _set_->root = ts_impl_new_node(_set_, value);
        if (!_set_->root)
        {
            _set_->flag = CMC_FLAG_ALLOC;
            return 0;
        }
    }
    else
    {
        struct treeset_node *scan = _set_->root;
        struct treeset_node *parent = scan;
        while (scan != ((void *)0))
        {
            parent = scan;
            if (_set_->f_val->cmp(scan->value, value) > 0)
                scan = scan->left;
            else if (_set_->f_val->cmp(scan->value, value) < 0)
                scan = scan->right;
            else
            {
                _set_->flag = CMC_FLAG_DUPLICATE;
                return 0;
            }
        }
        struct treeset_node *node;
        if (_set_->f_val->cmp(parent->value, value) > 0)
        {
            parent->left = ts_impl_new_node(_set_, value);
            if (!parent->left)
            {
                _set_->flag = CMC_FLAG_ALLOC;
                return 0;
            }
            parent->left->parent = parent;
            node = parent->left;
        }
        else
        {
            parent->right = ts_impl_new_node(_set_, value);
            if (!parent->right)
            {
                _set_->flag = CMC_FLAG_ALLOC;
                return 0;
            }
            parent->right->parent = parent;
            node = parent->right;
        }
        ts_impl_rebalance(_set_, node);
    }
    _set_->count++;
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->create)
        (_set_)->callbacks->create();
    ;
    return 1;
}
_Bool ts_remove(struct treeset *_set_, size_t value)
{
    if (ts_empty(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct treeset_node *node = ts_impl_get_node(_set_, value);
    if (!node)
    {
        _set_->flag = CMC_FLAG_NOT_FOUND;
        return 0;
    }
    struct treeset_node *temp = ((void *)0), *unbalanced = ((void *)0);
    _Bool is_root = node->parent == ((void *)0);
    if (node->left == ((void *)0) && node->right == ((void *)0))
    {
        if (is_root)
            _set_->root = ((void *)0);
        else
        {
            unbalanced = node->parent;
            if (node->parent->right == node)
                node->parent->right = ((void *)0);
            else
                node->parent->left = ((void *)0);
        }
        _set_->alloc->free(node);
    }
    else if (node->left == ((void *)0))
    {
        if (is_root)
        {
            _set_->root = node->right;
            _set_->root->parent = ((void *)0);
        }
        else
        {
            unbalanced = node->parent;
            node->right->parent = node->parent;
            if (node->parent->right == node)
                node->parent->right = node->right;
            else
                node->parent->left = node->right;
        }
        _set_->alloc->free(node);
    }
    else if (node->right == ((void *)0))
    {
        if (is_root)
        {
            _set_->root = node->left;
            _set_->root->parent = ((void *)0);
        }
        else
        {
            unbalanced = node->parent;
            node->left->parent = node->parent;
            if (node->parent->right == node)
                node->parent->right = node->left;
            else
                node->parent->left = node->left;
        }
        _set_->alloc->free(node);
    }
    else
    {
        temp = node->right;
        while (temp->left != ((void *)0))
            temp = temp->left;
        size_t temp_value = temp->value;
        unbalanced = temp->parent;
        if (temp->left == ((void *)0) && temp->right == ((void *)0))
        {
            if (temp->parent->right == temp)
                temp->parent->right = ((void *)0);
            else
                temp->parent->left = ((void *)0);
        }
        else if (temp->left == ((void *)0))
        {
            temp->right->parent = temp->parent;
            if (temp->parent->right == temp)
                temp->parent->right = temp->right;
            else
                temp->parent->left = temp->right;
        }
        else if (temp->right == ((void *)0))
        {
            temp->left->parent = temp->parent;
            if (temp->parent->right == temp)
                temp->parent->right = temp->left;
            else
                temp->parent->left = temp->left;
        }
        _set_->alloc->free(temp);
        node->value = temp_value;
    }
    if (unbalanced != ((void *)0))
        ts_impl_rebalance(_set_, unbalanced);
    if (_set_->count == 0)
        _set_->root = ((void *)0);
    _set_->count--;
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->delete)
        (_set_)->callbacks->delete ();
    ;
    return 1;
}
_Bool ts_max(struct treeset *_set_, size_t *value)
{
    if (ts_empty(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct treeset_node *scan = _set_->root;
    while (scan->right != ((void *)0))
        scan = scan->right;
    if (value)
        *value = scan->value;
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->read)
        (_set_)->callbacks->read();
    ;
    return 1;
}
_Bool ts_min(struct treeset *_set_, size_t *value)
{
    if (ts_empty(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return 0;
    }
    struct treeset_node *scan = _set_->root;
    while (scan->left != ((void *)0))
        scan = scan->left;
    if (value)
        *value = scan->value;
    _set_->flag = CMC_FLAG_OK;
    if ((_set_)->callbacks && (_set_)->callbacks->read)
        (_set_)->callbacks->read();
    ;
    return 1;
}
_Bool ts_contains(struct treeset *_set_, size_t value)
{
    _Bool result = ts_impl_get_node(_set_, value) != ((void *)0);
    if ((_set_)->callbacks && (_set_)->callbacks->read)
        (_set_)->callbacks->read();
    ;
    return result;
}
_Bool ts_empty(struct treeset *_set_)
{
    return _set_->count == 0;
}
size_t ts_count(struct treeset *_set_)
{
    return _set_->count;
}
int ts_flag(struct treeset *_set_)
{
    return _set_->flag;
}
struct treeset *ts_copy_of(struct treeset *_set_)
{
    struct treeset *result = ts_new_custom(_set_->f_val, _set_->alloc, ((void *)0));
    if (!result)
    {
        _set_->flag = CMC_FLAG_ERROR;
        return ((void *)0);
    }
    (result)->callbacks = _set_->callbacks;
    if (!ts_empty(_set_))
    {
        struct treeset_iter iter = ts_iter_start(_set_);
        for (; !ts_iter_at_end(&iter); ts_iter_next(&iter))
        {
            if (_set_->f_val->cpy)
                ts_insert(result, _set_->f_val->cpy(ts_iter_value(&iter)));
            else
                ts_insert(result, ts_iter_value(&iter));
        }
    }
    _set_->flag = CMC_FLAG_OK;
    return result;
}
_Bool ts_equals(struct treeset *_set1_, struct treeset *_set2_)
{
    _set1_->flag = CMC_FLAG_OK;
    _set2_->flag = CMC_FLAG_OK;
    if (_set1_->count != _set2_->count)
        return 0;
    struct treeset_iter iter = ts_iter_start(_set1_);
    for (; !ts_iter_at_end(&iter); ts_iter_next(&iter))
    {
        if (ts_impl_get_node(_set2_, ts_iter_value(&iter)) == ((void *)0))
            return 0;
    }
    return 1;
}
struct treeset *ts_union(struct treeset *_set1_, struct treeset *_set2_)
{
    struct treeset *_set_r_ = ts_new_custom(_set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct treeset_iter iter1 = ts_iter_start(_set1_);
    struct treeset_iter iter2 = ts_iter_start(_set2_);
    for (; !ts_iter_at_end(&iter1); ts_iter_next(&iter1))
    {
        ts_insert(_set_r_, ts_iter_value(&iter1));
    }
    for (; !ts_iter_at_end(&iter2); ts_iter_next(&iter2))
    {
        ts_insert(_set_r_, ts_iter_value(&iter2));
    }
    (_set_r_)->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct treeset *ts_intersection(struct treeset *_set1_, struct treeset *_set2_)
{
    struct treeset *_set_r_ = ts_new_custom(_set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct treeset *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;
    struct treeset *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;
    struct treeset_iter iter = ts_iter_start(_set_A_);
    for (; !ts_iter_at_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (ts_impl_get_node(_set_B_, value) != ((void *)0))
            ts_insert(_set_r_, value);
    }
    (_set_r_)->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct treeset *ts_difference(struct treeset *_set1_, struct treeset *_set2_)
{
    struct treeset *_set_r_ = ts_new_custom(_set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct treeset_iter iter = ts_iter_start(_set1_);
    for (; !ts_iter_at_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (ts_impl_get_node(_set2_, value) == ((void *)0))
            ts_insert(_set_r_, value);
    }
    (_set_r_)->callbacks = _set1_->callbacks;
    return _set_r_;
}
struct treeset *ts_symmetric_difference(struct treeset *_set1_, struct treeset *_set2_)
{
    struct treeset *_set_r_ = ts_new_custom(_set1_->f_val, _set1_->alloc, ((void *)0));
    if (!_set_r_)
        return ((void *)0);
    struct treeset_iter iter1 = ts_iter_start(_set1_);
    struct treeset_iter iter2 = ts_iter_start(_set2_);
    for (; !ts_iter_at_end(&iter1); ts_iter_next(&iter1))
    {
        size_t value = ts_iter_value(&iter1);
        if (ts_impl_get_node(_set2_, value) == ((void *)0))
            ts_insert(_set_r_, value);
    }
    for (; !ts_iter_at_end(&iter2); ts_iter_next(&iter2))
    {
        size_t value = ts_iter_value(&iter2);
        if (ts_impl_get_node(_set1_, value) == ((void *)0))
            ts_insert(_set_r_, value);
    }
    (_set_r_)->callbacks = _set1_->callbacks;
    return _set_r_;
}
_Bool ts_is_subset(struct treeset *_set1_, struct treeset *_set2_)
{
    if (_set1_->count > _set2_->count)
        return 0;
    if (ts_empty(_set1_))
        return 1;
    struct treeset_iter iter = ts_iter_start(_set1_);
    for (; !ts_iter_at_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (ts_impl_get_node(_set2_, value) == ((void *)0))
            return 0;
    }
    return 1;
}
_Bool ts_is_superset(struct treeset *_set1_, struct treeset *_set2_)
{
    return ts_is_subset(_set2_, _set1_);
}
_Bool ts_is_proper_subset(struct treeset *_set1_, struct treeset *_set2_)
{
    if (_set1_->count >= _set2_->count)
        return 0;
    if (ts_empty(_set1_))
    {
        if (!ts_empty(_set2_))
            return 1;
        else
            return 0;
    }
    struct treeset_iter iter = ts_iter_start(_set1_);
    for (; !ts_iter_at_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (ts_impl_get_node(_set2_, value) == ((void *)0))
            return 0;
    }
    return 1;
}
_Bool ts_is_proper_superset(struct treeset *_set1_, struct treeset *_set2_)
{
    return ts_is_proper_subset(_set2_, _set1_);
}
_Bool ts_is_disjointset(struct treeset *_set1_, struct treeset *_set2_)
{
    if (ts_empty(_set1_))
        return 1;
    struct treeset_iter iter = ts_iter_start(_set1_);
    for (; !ts_iter_at_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (ts_impl_get_node(_set2_, value) != ((void *)0))
            return 0;
    }
    return 1;
}
struct treeset_iter ts_iter_start(struct treeset *target)
{
    struct treeset_iter iter;
    iter.target = target;
    iter.cursor = target->root;
    iter.first = ((void *)0);
    iter.last = ((void *)0);
    iter.index = 0;
    iter.start = 1;
    iter.end = ts_empty(target);
    if (!ts_empty(target))
    {
        while (iter.cursor->left != ((void *)0))
            iter.cursor = iter.cursor->left;
        iter.first = iter.cursor;
        iter.last = target->root;
        while (iter.last->right != ((void *)0))
            iter.last = iter.last->right;
    }
    return iter;
}
struct treeset_iter ts_iter_end(struct treeset *target)
{
    struct treeset_iter iter;
    iter.target = target;
    iter.cursor = target->root;
    iter.first = ((void *)0);
    iter.last = ((void *)0);
    iter.index = 0;
    iter.start = ts_empty(target);
    iter.end = 1;
    if (!ts_empty(target))
    {
        while (iter.cursor->right != ((void *)0))
            iter.cursor = iter.cursor->right;
        iter.last = iter.cursor;
        iter.first = target->root;
        while (iter.first->left != ((void *)0))
            iter.first = iter.first->left;
        iter.index = target->count - 1;
    }
    return iter;
}
_Bool ts_iter_at_start(struct treeset_iter *iter)
{
    return ts_empty(iter->target) || iter->start;
}
_Bool ts_iter_at_end(struct treeset_iter *iter)
{
    return ts_empty(iter->target) || iter->end;
}
_Bool ts_iter_to_start(struct treeset_iter *iter)
{
    if (!ts_empty(iter->target))
    {
        iter->index = 0;
        iter->start = 1;
        iter->end = ts_empty(iter->target);
        iter->cursor = iter->first;
        return 1;
    }
    return 0;
}
_Bool ts_iter_to_end(struct treeset_iter *iter)
{
    if (!ts_empty(iter->target))
    {
        iter->index = iter->target->count - 1;
        iter->start = ts_empty(iter->target);
        iter->end = 1;
        iter->cursor = iter->last;
        return 1;
    }
    return 0;
}
_Bool ts_iter_next(struct treeset_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor == iter->last)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = ts_empty(iter->target);
    if (iter->cursor->right != ((void *)0))
    {
        iter->cursor = iter->cursor->right;
        while (iter->cursor->left != ((void *)0))
            iter->cursor = iter->cursor->left;
        iter->index++;
        return 1;
    }
    while (1)
    {
        if (iter->cursor->parent->left == iter->cursor)
        {
            iter->cursor = iter->cursor->parent;
            iter->index++;
            return 1;
        }
        iter->cursor = iter->cursor->parent;
    }
}
_Bool ts_iter_prev(struct treeset_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor == iter->first)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = ts_empty(iter->target);
    if (iter->cursor->left != ((void *)0))
    {
        iter->cursor = iter->cursor->left;
        while (iter->cursor->right != ((void *)0))
            iter->cursor = iter->cursor->right;
        iter->index--;
        return 1;
    }
    while (1)
    {
        if (iter->cursor->parent->right == iter->cursor)
        {
            iter->cursor = iter->cursor->parent;
            iter->index--;
            return 1;
        }
        iter->cursor = iter->cursor->parent;
    }
}
_Bool ts_iter_advance(struct treeset_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->cursor == iter->last)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= iter->target->count)
        return 0;
    for (size_t i = 0; i < steps; i++)
        ts_iter_next(iter);
    return 1;
}
_Bool ts_iter_rewind(struct treeset_iter *iter, size_t steps)
{
    if (iter->start)
        return 0;
    if (iter->cursor == iter->first)
    {
        iter->start = 1;
        return 0;
    }
    if (steps == 0 || iter->index < steps)
        return 0;
    for (size_t i = 0; i < steps; i++)
        ts_iter_prev(iter);
    return 1;
}
_Bool ts_iter_go_to(struct treeset_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return ts_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return ts_iter_advance(iter, index - iter->index);
    return 1;
}
size_t ts_iter_value(struct treeset_iter *iter)
{
    if (ts_empty(iter->target))
        return (size_t){ 0 };
    return iter->cursor->value;
}
size_t ts_iter_index(struct treeset_iter *iter)
{
    return iter->index;
}
static struct treeset_node *ts_impl_new_node(struct treeset *_set_, size_t value)
{
    struct treeset_node *node = _set_->alloc->malloc(sizeof(struct treeset_node));
    if (!node)
        return ((void *)0);
    node->value = value;
    node->right = ((void *)0);
    node->left = ((void *)0);
    node->parent = ((void *)0);
    node->height = 0;
    return node;
}
static struct treeset_node *ts_impl_get_node(struct treeset *_set_, size_t value)
{
    struct treeset_node *scan = _set_->root;
    while (scan != ((void *)0))
    {
        if (_set_->f_val->cmp(scan->value, value) > 0)
            scan = scan->left;
        else if (_set_->f_val->cmp(scan->value, value) < 0)
            scan = scan->right;
        else
            return scan;
    }
    return ((void *)0);
}
static unsigned char ts_impl_h(struct treeset_node *node)
{
    if (node == ((void *)0))
        return 0;
    return node->height;
}
static unsigned char ts_impl_hupdate(struct treeset_node *node)
{
    if (node == ((void *)0))
        return 0;
    unsigned char h_l = ts_impl_h(node->left);
    unsigned char h_r = ts_impl_h(node->right);
    return 1 + (h_l > h_r ? h_l : h_r);
}
static void ts_impl_rotate_right(struct treeset_node **Z)
{
    struct treeset_node *root = *Z;
    struct treeset_node *new_root = root->left;
    if (root->parent != ((void *)0))
    {
        if (root->parent->left == root)
            root->parent->left = new_root;
        else
            root->parent->right = new_root;
    }
    new_root->parent = root->parent;
    root->parent = new_root;
    root->left = new_root->right;
    if (root->left)
        root->left->parent = root;
    new_root->right = root;
    root->height = ts_impl_hupdate(root);
    new_root->height = ts_impl_hupdate(new_root);
    *Z = new_root;
}
static void ts_impl_rotate_left(struct treeset_node **Z)
{
    struct treeset_node *root = *Z;
    struct treeset_node *new_root = root->right;
    if (root->parent != ((void *)0))
    {
        if (root->parent->right == root)
            root->parent->right = new_root;
        else
            root->parent->left = new_root;
    }
    new_root->parent = root->parent;
    root->parent = new_root;
    root->right = new_root->left;
    if (root->right)
        root->right->parent = root;
    new_root->left = root;
    root->height = ts_impl_hupdate(root);
    new_root->height = ts_impl_hupdate(new_root);
    *Z = new_root;
}
static void ts_impl_rebalance(struct treeset *_set_, struct treeset_node *node)
{
    struct treeset_node *scan = node, *child = ((void *)0);
    int balance;
    _Bool is_root = 0;
    while (scan != ((void *)0))
    {
        if (scan->parent == ((void *)0))
            is_root = 1;
        scan->height = ts_impl_hupdate(scan);
        balance = ts_impl_h(scan->right) - ts_impl_h(scan->left);
        if (balance >= 2)
        {
            child = scan->right;
            if (ts_impl_h(child->right) < ts_impl_h(child->left))
                ts_impl_rotate_right(&(scan->right));
            ts_impl_rotate_left(&scan);
        }
        else if (balance <= -2)
        {
            child = scan->left;
            if (ts_impl_h(child->left) < ts_impl_h(child->right))
                ts_impl_rotate_left(&(scan->left));
            ts_impl_rotate_right(&scan);
        }
        if (is_root)
        {
            _set_->root = scan;
            is_root = 0;
        }
        scan = scan->parent;
    }
}
_Bool ts_to_string(struct treeset *_set_, FILE *fptr)
{
    struct treeset *s_ = _set_;
    return 0 <= fprintf(fptr,
                        "struct %s<%s> "
                        "at %p { "
                        "root:%p, "
                        "count:%"
                        "I64u"
                        ", "
                        "flag:%d, "
                        "f_val:%p, "
                        "alloc:%p, "
                        "callbacks:%p }",
                        "treeset", "size_t", s_, s_->root, s_->count, s_->flag, s_->f_val, s_->alloc, (s_)->callbacks);
}
_Bool ts_print(struct treeset *_set_, FILE *fptr, const char *start, const char *separator, const char *end)
{
    fprintf(fptr, "%s", start);
    struct treeset_node *root = _set_->root;
    _Bool left_done = 0;
    size_t i = 0;
    while (root)
    {
        if (!left_done)
        {
            while (root->left)
                root = root->left;
        }
        if (!_set_->f_val->str(fptr, root->value))
            return 0;
        if (++i < _set_->count)
            fprintf(fptr, "%s", separator);
        left_done = 1;
        if (root->right)
        {
            left_done = 0;
            root = root->right;
        }
        else if (root->parent)
        {
            while (root->parent && root == root->parent->right)
                root = root->parent;
            if (!root->parent)
                break;
            root = root->parent;
        }
        else
            break;
    }
    fprintf(fptr, "%s", end);
    return 1;
}
