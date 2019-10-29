#include <cmc/treeset.h>

//CMC_GENERATE_TREESET(ts, treeset, size_t)

typedef struct treeset_s
{
    struct treeset_node_s *root;
    size_t count;
    int (*cmp)(size_t, size_t);
    struct treeset_iter_s (*it_start)(struct treeset_s *);
    struct treeset_iter_s (*it_end)(struct treeset_s *);
} treeset, *treeset_ptr;
typedef struct treeset_node_s
{
    size_t value;
    unsigned char height;
    struct treeset_node_s *right;
    struct treeset_node_s *left;
    struct treeset_node_s *parent;
} treeset_node, *treeset_node_ptr;
typedef struct treeset_iter_s
{
    struct treeset_s *target;
    struct treeset_node_s *cursor;
    struct treeset_node_s *first;
    struct treeset_node_s *last;
    size_t index;
    _Bool start;
    _Bool end;
} treeset_iter, *treeset_iter_ptr;
treeset *ts_new(int (*compare)(size_t, size_t));
void ts_clear(treeset *_set_, void (*deallocator)(size_t));
void ts_free(treeset *_set_, void (*deallocator)(size_t));
_Bool ts_insert(treeset *_set_, size_t element);
_Bool ts_remove(treeset *_set_, size_t element);
_Bool ts_max(treeset *_set_, size_t *value);
_Bool ts_min(treeset *_set_, size_t *value);
_Bool ts_contains(treeset *_set_, size_t element);
_Bool ts_empty(treeset *_set_);
size_t ts_count(treeset *_set_);
treeset *ts_copy_of(treeset *_set_, size_t (*copy_func)(size_t));
_Bool ts_equals(treeset *_set1_, treeset *_set2_);
struct cmc_string ts_to_string(treeset *_set_);
treeset *ts_union(treeset *_set1_, treeset *_set2_);
treeset *ts_intersection(treeset *_set1_, treeset *_set2_);
treeset *ts_difference(treeset *_set1_, treeset *_set2_);
treeset *ts_symmetric_difference(treeset *_set1_, treeset *_set2_);
_Bool ts_is_subset(treeset *_set1_, treeset *_set2_);
_Bool ts_is_superset(treeset *_set1_, treeset *_set2_);
_Bool ts_is_proper_subset(treeset *_set1_, treeset *_set2_);
_Bool ts_is_proper_superset(treeset *_set1_, treeset *_set2_);
_Bool ts_is_disjointset(treeset *_set1_, treeset *_set2_);
treeset_iter *ts_iter_new(treeset *target);
void ts_iter_free(treeset_iter *iter);
void ts_iter_init(treeset_iter *iter, treeset *target);
_Bool ts_iter_start(treeset_iter *iter);
_Bool ts_iter_end(treeset_iter *iter);
void ts_iter_to_start(treeset_iter *iter);
void ts_iter_to_end(treeset_iter *iter);
_Bool ts_iter_next(treeset_iter *iter);
_Bool ts_iter_prev(treeset_iter *iter);
_Bool ts_iter_advance(treeset_iter *iter, size_t steps);
_Bool ts_iter_rewind(treeset_iter *iter, size_t steps);
_Bool ts_iter_go_to(treeset_iter *iter, size_t index);
size_t ts_iter_value(treeset_iter *iter);
size_t ts_iter_index(treeset_iter *iter);
static inline size_t ts_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
static treeset_node *ts_impl_new_node(size_t element);
static treeset_node *ts_impl_get_node(treeset *_set_, size_t element);
static unsigned char ts_impl_h(treeset_node *node);
static unsigned char ts_impl_hupdate(treeset_node *node);
static void ts_impl_rotate_right(treeset_node **Z);
static void ts_impl_rotate_left(treeset_node **Z);
static void ts_impl_rebalance(treeset *_set_, treeset_node *node);
static treeset_iter ts_impl_it_start(treeset *_set_);
static treeset_iter ts_impl_it_end(treeset *_set_);
treeset *ts_new(int (*compare)(size_t, size_t))
{
    treeset *_set_ = malloc(sizeof(treeset));
    if (!_set_)
        return ((void *)0);
    _set_->count = 0;
    _set_->root = ((void *)0);
    _set_->cmp = compare;
    _set_->it_start = ts_impl_it_start;
    _set_->it_end = ts_impl_it_end;
    return _set_;
}
void ts_clear(treeset *_set_, void (*deallocator)(size_t))
{
    treeset_node *scan = _set_->root;
    treeset_node *up = ((void *)0);
    while (scan != ((void *)0))
    {
        if (scan->left != ((void *)0))
        {
            treeset_node *left = scan->left;
            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != ((void *)0))
        {
            treeset_node *right = scan->right;
            scan->left = up;
            scan->right = ((void *)0);
            up = scan;
            scan = right;
        }
        else
        {
            if (up == ((void *)0))
            {
                if (deallocator)
                    deallocator(scan->value);
                free(scan);
                scan = ((void *)0);
            }
            while (up != ((void *)0))
            {
                if (deallocator)
                    deallocator(scan->value);
                free(scan);
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
}
void ts_free(treeset *_set_, void (*deallocator)(size_t))
{
    ts_clear(_set_, deallocator);
    free(_set_);
}
_Bool ts_insert(treeset *_set_, size_t element)
{
    if (ts_empty(_set_))
    {
        _set_->root = ts_impl_new_node(element);
        if (!_set_->root)
            return 0;
    }
    else
    {
        treeset_node *scan = _set_->root;
        treeset_node *parent = scan;
        while (scan != ((void *)0))
        {
            parent = scan;
            if (_set_->cmp(scan->value, element) > 0)
                scan = scan->left;
            else if (_set_->cmp(scan->value, element) < 0)
                scan = scan->right;
            else
                return 0;
        }
        treeset_node *node;
        if (_set_->cmp(parent->value, element) > 0)
        {
            parent->left = ts_impl_new_node(element);
            if (!parent->left)
                return 0;
            parent->left->parent = parent;
            node = parent->left;
        }
        else
        {
            parent->right = ts_impl_new_node(element);
            if (!parent->right)
                return 0;
            parent->right->parent = parent;
            node = parent->right;
        }
        ts_impl_rebalance(_set_, node);
    }
    _set_->count++;
    return 1;
}
_Bool ts_remove(treeset *_set_, size_t element)
{
    treeset_node *node = ts_impl_get_node(_set_, element);
    if (!node)
        return 0;
    treeset_node *temp = ((void *)0), *unbalanced = ((void *)0);
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
        free(node);
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
        free(node);
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
        free(node);
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
        free(temp);
        node->value = temp_value;
    }
    if (unbalanced != ((void *)0))
        ts_impl_rebalance(_set_, unbalanced);
    _set_->count--;
    if (_set_->count == 0)
        _set_->root = ((void *)0);
    return 1;
}
_Bool ts_max(treeset *_set_, size_t *value)
{
    if (ts_empty(_set_))
        return 0;
    treeset_node *scan = _set_->root;
    while (scan->right != ((void *)0))
        scan = scan->right;
    *value = scan->value;
    return 1;
}
_Bool ts_min(treeset *_set_, size_t *value)
{
    if (ts_empty(_set_))
        return 0;
    treeset_node *scan = _set_->root;
    while (scan->left != ((void *)0))
        scan = scan->left;
    *value = scan->value;
    return 1;
}
_Bool ts_contains(treeset *_set_, size_t element)
{
    treeset_node *scan = _set_->root;
    while (scan != ((void *)0))
    {
        if (_set_->cmp(scan->value, element) > 0)
            scan = scan->left;
        else if (_set_->cmp(scan->value, element) < 0)
            scan = scan->right;
        else
            return 1;
    }
    return 0;
}
_Bool ts_empty(treeset *_set_) { return _set_->count == 0; }
size_t ts_count(treeset *_set_) { return _set_->count; }
treeset *ts_copy_of(treeset *_set_, size_t (*copy_func)(size_t))
{
    treeset *result = ts_new(_set_->cmp);
    if (!result)
        return ((void *)0);
    treeset_iter iter;
    ts_iter_init(&iter, _set_);
    if (!ts_empty(_set_))
    {
        for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
        {
            if (copy_func)
                ts_insert(result, copy_func(ts_iter_value(&iter)));
            else
                ts_insert(result, ts_iter_value(&iter));
        }
    }
    return result;
}
_Bool ts_equals(treeset *_set1_, treeset *_set2_)
{
    if (ts_count(_set1_) != ts_count(_set2_))
        return 0;
    treeset_iter iter;
    ts_iter_init(&iter, _set1_);
    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        if (ts_impl_get_node(_set2_, ts_iter_value(&iter)) == ((void *)0))
            return 0;
    }
    return 1;
}
struct cmc_string ts_to_string(treeset *_set_)
{
    struct cmc_string str;
    treeset *s_ = _set_;
    const char *name = "treeset";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_treeset, name, s_, s_->root, s_->count, s_->cmp);
    return str;
}
treeset *ts_union(treeset *_set1_, treeset *_set2_)
{
    treeset *_set_r_ = ts_new(_set1_->cmp);
    if (!_set_r_)
        return ((void *)0);
    treeset_iter iter1, iter2;
    ts_iter_init(&iter1, _set1_);
    ts_iter_init(&iter2, _set2_);
    for (ts_iter_to_start(&iter1); !ts_iter_end(&iter1); ts_iter_next(&iter1))
    {
        ts_insert(_set_r_, ts_iter_value(&iter1));
    }
    for (ts_iter_to_start(&iter2); !ts_iter_end(&iter2); ts_iter_next(&iter2))
    {
        ts_insert(_set_r_, ts_iter_value(&iter2));
    }
    return _set_r_;
}
treeset *ts_intersection(treeset *_set1_, treeset *_set2_)
{
    treeset *_set_r_ = ts_new(_set1_->cmp);
    if (!_set_r_)
        return ((void *)0);
    treeset *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;
    treeset *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;
    treeset_iter iter;
    ts_iter_init(&iter, _set_A_);
    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (ts_impl_get_node(_set_B_, value) != ((void *)0))
            ts_insert(_set_r_, value);
    }
    return _set_r_;
}
treeset *ts_difference(treeset *_set1_, treeset *_set2_)
{
    treeset *_set_r_ = ts_new(_set1_->cmp);
    if (!_set_r_)
        return ((void *)0);
    treeset_iter iter;
    ts_iter_init(&iter, _set1_);
    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (ts_impl_get_node(_set2_, value) == ((void *)0))
            ts_insert(_set_r_, value);
    }
    return _set_r_;
}
treeset *ts_symmetric_difference(treeset *_set1_, treeset *_set2_)
{
    treeset *_set_r_ = ts_new(_set1_->cmp);
    if (!_set_r_)
        return ((void *)0);
    treeset_iter iter1, iter2;
    ts_iter_init(&iter1, _set1_);
    ts_iter_init(&iter2, _set2_);
    for (ts_iter_to_start(&iter1); !ts_iter_end(&iter1); ts_iter_next(&iter1))
    {
        size_t value = ts_iter_value(&iter1);
        if (ts_impl_get_node(_set2_, value) == ((void *)0))
            ts_insert(_set_r_, value);
    }
    for (ts_iter_to_start(&iter2); !ts_iter_end(&iter2); ts_iter_next(&iter2))
    {
        size_t value = ts_iter_value(&iter2);
        if (ts_impl_get_node(_set1_, value) == ((void *)0))
            ts_insert(_set_r_, value);
    }
    return _set_r_;
}
_Bool ts_is_subset(treeset *_set1_, treeset *_set2_)
{
    if (ts_count(_set1_) > ts_count(_set2_))
        return 0;
    if (ts_empty(_set1_))
        return 1;
    treeset_iter iter;
    ts_iter_init(&iter, _set1_);
    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (!ts_contains(_set2_, value))
            return 0;
    }
    return 1;
}
_Bool ts_is_superset(treeset *_set1_, treeset *_set2_) { return ts_is_subset(_set2_, _set1_); }
_Bool ts_is_proper_subset(treeset *_set1_, treeset *_set2_)
{
    if (ts_count(_set1_) >= ts_count(_set2_))
        return 0;
    if (ts_empty(_set1_))
    {
        if (!ts_empty(_set2_))
            return 1;
        else
            return 0;
    }
    treeset_iter iter;
    ts_iter_init(&iter, _set1_);
    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (!ts_contains(_set2_, value))
            return 0;
    }
    return 1;
}
_Bool ts_is_proper_superset(treeset *_set1_, treeset *_set2_) { return ts_is_proper_subset(_set2_, _set1_); }
_Bool ts_is_disjointset(treeset *_set1_, treeset *_set2_)
{
    if (ts_empty(_set1_))
        return 1;
    treeset_iter iter;
    ts_iter_init(&iter, _set1_);
    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (ts_contains(_set2_, value))
            return 0;
    }
    return 1;
}
treeset_iter *ts_iter_new(treeset *target)
{
    treeset_iter *iter = malloc(sizeof(treeset_iter));
    if (!iter)
        return ((void *)0);
    ts_iter_init(iter, target);
    return iter;
}
void ts_iter_free(treeset_iter *iter) { free(iter); }
void ts_iter_init(treeset_iter *iter, treeset *target)
{
    memset(iter, 0, sizeof(treeset_iter));
    iter->target = target;
    iter->start = 1;
    iter->end = ts_empty(target);
    iter->cursor = target->root;
    if (!ts_empty(target))
    {
        while (iter->cursor->left != ((void *)0))
            iter->cursor = iter->cursor->left;
        iter->first = iter->cursor;
        iter->last = target->root;
        while (iter->last->right != ((void *)0))
            iter->last = iter->last->right;
    }
}
_Bool ts_iter_start(treeset_iter *iter) { return ts_empty(iter->target) || iter->start; }
_Bool ts_iter_end(treeset_iter *iter) { return ts_empty(iter->target) || iter->end; }
void ts_iter_to_start(treeset_iter *iter)
{
    if (!ts_empty(iter->target))
    {
        iter->index = 0;
        iter->start = 1;
        iter->end = ts_empty(iter->target);
        iter->cursor = iter->first;
    }
}
void ts_iter_to_end(treeset_iter *iter)
{
    if (!ts_empty(iter->target))
    {
        iter->index = iter->target->count - 1;
        iter->start = ts_empty(iter->target);
        iter->end = 1;
        iter->cursor = iter->last;
    }
}
_Bool ts_iter_next(treeset_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor == iter->last)
    {
        iter->end = 1;
        return 1;
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
_Bool ts_iter_prev(treeset_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor == iter->first)
    {
        iter->start = 1;
        return 1;
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
_Bool ts_iter_advance(treeset_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->cursor == iter->last)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= ts_count(iter->target))
        return 0;
    iter->index += steps;
    for (size_t i = 0; i < steps; i++)
        ts_iter_next(iter);
    return 1;
}
_Bool ts_iter_rewind(treeset_iter *iter, size_t steps)
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
    iter->index -= steps;
    for (size_t i = 0; i < steps; i++)
        ts_iter_prev(iter);
    return 1;
}
_Bool ts_iter_go_to(treeset_iter *iter, size_t index)
{
    if (index >= ts_count(iter->target))
        return 0;
    if (iter->index > index)
        return ts_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return ts_iter_advance(iter, index - iter->index);
    return 1;
}
size_t ts_iter_value(treeset_iter *iter)
{
    if (ts_empty(iter->target))
        return (size_t){0};
    return iter->cursor->value;
}
size_t ts_iter_index(treeset_iter *iter) { return iter->index; }
static treeset_node *ts_impl_new_node(size_t element)
{
    treeset_node *node = malloc(sizeof(treeset_node));
    if (!node)
        return ((void *)0);
    node->value = element;
    node->right = ((void *)0);
    node->left = ((void *)0);
    node->parent = ((void *)0);
    node->height = 0;
    return node;
}
static treeset_node *ts_impl_get_node(treeset *_set_, size_t element)
{
    if (ts_empty(_set_))
        return ((void *)0);
    treeset_node *scan = _set_->root;
    while (scan != ((void *)0))
    {
        if (_set_->cmp(scan->value, element) > 0)
            scan = scan->left;
        else if (_set_->cmp(scan->value, element) < 0)
            scan = scan->right;
        else
            return scan;
    }
    return ((void *)0);
}
static unsigned char ts_impl_h(treeset_node *node)
{
    if (node == ((void *)0))
        return 0;
    return node->height;
}
static unsigned char ts_impl_hupdate(treeset_node *node)
{
    if (node == ((void *)0))
        return 0;
    unsigned char h_l = ts_impl_h(node->left);
    unsigned char h_r = ts_impl_h(node->right);
    return 1 + (h_l > h_r ? h_l : h_r);
}
static void ts_impl_rotate_right(treeset_node **Z)
{
    treeset_node *root = *Z;
    treeset_node *new_root = root->left;
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
static void ts_impl_rotate_left(treeset_node **Z)
{
    treeset_node *root = *Z;
    treeset_node *new_root = root->right;
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
static void ts_impl_rebalance(treeset *_set_, treeset_node *node)
{
    treeset_node *scan = node, *child = ((void *)0);
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
static treeset_iter ts_impl_it_start(treeset *_set_)
{
    treeset_iter iter;
    ts_iter_init(&iter, _set_);
    return iter;
}
static treeset_iter ts_impl_it_end(treeset *_set_)
{
    treeset_iter iter;
    ts_iter_init(&iter, _set_);
    ts_iter_to_end(&iter);
    return iter;
}
