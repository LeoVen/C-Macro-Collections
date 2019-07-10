#include "cmc/treeset.h"

//TREESET_GENERATE(ts, treeset, , size_t)

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
    bool start;
    bool end;
} treeset_iter, *treeset_iter_ptr;
treeset *ts_new(int (*compare)(size_t, size_t));
void ts_clear(treeset *_set_);
void ts_free(treeset *_set_);
bool ts_insert(treeset *_set_, size_t element);
bool ts_remove(treeset *_set_, size_t element);
bool ts_insert_if(treeset *_set_, size_t element, bool condition);
bool ts_remove_if(treeset *_set_, size_t element, bool condition);
bool ts_max(treeset *_set_, size_t *value);
bool ts_min(treeset *_set_, size_t *value);
bool ts_contains(treeset *_set_, size_t element);
bool ts_empty(treeset *_set_);
size_t ts_count(treeset *_set_);
treeset *ts_union(treeset *_set1_, treeset *_set2_);
treeset *ts_intersection(treeset *_set1_, treeset *_set2_);
treeset *ts_difference(treeset *_set1_, treeset *_set2_);
treeset *ts_symmetric_difference(treeset *_set1_, treeset *_set2_);
bool ts_is_subset(treeset *_set1_, treeset *_set2_);
bool ts_is_superset(treeset *_set1_, treeset *_set2_);
bool ts_is_proper_subset(treeset *_set1_, treeset *_set2_);
bool ts_is_proper_superset(treeset *_set1_, treeset *_set2_);
bool ts_is_disjointset(treeset *_set1_, treeset *_set2_);
treeset_iter *ts_iter_new(treeset *target);
void ts_iter_free(treeset_iter *iter);
void ts_iter_init(treeset_iter *iter, treeset *target);
bool ts_iter_start(treeset_iter *iter);
bool ts_iter_end(treeset_iter *iter);
void ts_iter_to_start(treeset_iter *iter);
void ts_iter_to_end(treeset_iter *iter);
bool ts_iter_next(treeset_iter *iter);
bool ts_iter_prev(treeset_iter *iter);
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
        return NULL;
    _set_->count = 0;
    _set_->root = NULL;
    _set_->cmp = compare;
    _set_->it_start = ts_impl_it_start;
    _set_->it_end = ts_impl_it_end;
    return _set_;
}
void ts_clear(treeset *_set_)
{
    treeset_node *scan = _set_->root;
    treeset_node *up = NULL;
    while (scan != NULL)
    {
        if (scan->left != NULL)
        {
            treeset_node *left = scan->left;
            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != NULL)
        {
            treeset_node *right = scan->right;
            scan->left = up;
            scan->right = NULL;
            up = scan;
            scan = right;
        }
        else
        {
            if (up == NULL)
            {
                free(scan);
                scan = NULL;
            }
            while (up != NULL)
            {
                free(scan);
                if (up->right != NULL)
                {
                    scan = up->right;
                    up->right = NULL;
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
    _set_->root = NULL;
}
void ts_free(treeset *_set_)
{
    ts_clear(_set_);
    free(_set_);
}
bool ts_insert(treeset *_set_, size_t element)
{
    if (ts_empty(_set_))
    {
        _set_->root = ts_impl_new_node(element);
        if (!_set_->root)
            return false;
    }
    else
    {
        treeset_node *scan = _set_->root;
        treeset_node *parent = scan;
        while (scan != NULL)
        {
            parent = scan;
            if (_set_->cmp(scan->value, element) > 0)
                scan = scan->left;
            else if (_set_->cmp(scan->value, element) < 0)
                scan = scan->right;
            else
                return false;
        }
        treeset_node *node;
        if (_set_->cmp(parent->value, element) > 0)
        {
            parent->left = ts_impl_new_node(element);
            if (!parent->left)
                return false;
            parent->left->parent = parent;
            node = parent->left;
        }
        else
        {
            parent->right = ts_impl_new_node(element);
            if (!parent->right)
                return false;
            parent->right->parent = parent;
            node = parent->right;
        }
        ts_impl_rebalance(_set_, node);
    }
    _set_->count++;
    return true;
}
bool ts_remove(treeset *_set_, size_t element)
{
    treeset_node *node = ts_impl_get_node(_set_, element);
    if (!node)
        return false;
    treeset_node *temp = NULL, *unbalanced = NULL;
    bool is_root = node->parent == NULL;
    if (node->left == NULL && node->right == NULL)
    {
        if (is_root)
            _set_->root = NULL;
        else
        {
            unbalanced = node->parent;
            if (node->parent->right == node)
                node->parent->right = NULL;
            else
                node->parent->left = NULL;
        }
        free(node);
    }
    else if (node->left == NULL)
    {
        if (is_root)
        {
            _set_->root = node->right;
            _set_->root->parent = NULL;
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
    else if (node->right == NULL)
    {
        if (is_root)
        {
            _set_->root = node->left;
            _set_->root->parent = NULL;
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
        while (temp->left != NULL)
            temp = temp->left;
        size_t temp_value = temp->value;
        unbalanced = temp->parent;
        if (temp->left == NULL && temp->right == NULL)
        {
            if (temp->parent->right == temp)
                temp->parent->right = NULL;
            else
                temp->parent->left = NULL;
        }
        else if (temp->left == NULL)
        {
            temp->right->parent = temp->parent;
            if (temp->parent->right == temp)
                temp->parent->right = temp->right;
            else
                temp->parent->left = temp->right;
        }
        else if (temp->right == NULL)
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
    if (unbalanced != NULL)
        ts_impl_rebalance(_set_, unbalanced);
    _set_->count--;
    if (_set_->count == 0)
        _set_->root = NULL;
    return true;
}
bool ts_insert_if(treeset *_set_, size_t element, bool condition)
{
    if (condition)
        return ts_insert(_set_, element);
    return false;
}
bool ts_remove_if(treeset *_set_, size_t element, bool condition)
{
    if (condition)
        return ts_remove(_set_, element);
    return false;
}
bool ts_max(treeset *_set_, size_t *value)
{
    if (ts_empty(_set_))
        return false;
    treeset_node *scan = _set_->root;
    while (scan->right != NULL)
        scan = scan->right;
    *value = scan->value;
    return true;
}
bool ts_min(treeset *_set_, size_t *value)
{
    if (ts_empty(_set_))
        return false;
    treeset_node *scan = _set_->root;
    while (scan->left != NULL)
        scan = scan->left;
    *value = scan->value;
    return true;
}
bool ts_contains(treeset *_set_, size_t element)
{
    treeset_node *scan = _set_->root;
    while (scan != NULL)
    {
        if (_set_->cmp(scan->value, element) > 0)
            scan = scan->left;
        else if (_set_->cmp(scan->value, element) < 0)
            scan = scan->right;
        else
            return true;
    }
    return false;
}
bool ts_empty(treeset *_set_) { return _set_->count == 0; }
size_t ts_count(treeset *_set_) { return _set_->count; }
treeset *ts_union(treeset *_set1_, treeset *_set2_)
{
    treeset *_set_r_ = ts_new(_set1_->cmp);
    if (!_set_r_)
        return NULL;
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
        return NULL;
    treeset *_set_A_ = _set1_->count < _set2_->count ? _set1_ : _set2_;
    treeset *_set_B_ = _set_A_ == _set1_ ? _set2_ : _set1_;
    treeset_iter iter;
    ts_iter_init(&iter, _set_A_);
    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (ts_impl_get_node(_set_B_, value) != NULL)
            ts_insert(_set_r_, value);
    }
    return _set_r_;
}
treeset *ts_difference(treeset *_set1_, treeset *_set2_)
{
    treeset *_set_r_ = ts_new(_set1_->cmp);
    if (!_set_r_)
        return NULL;
    treeset_iter iter;
    ts_iter_init(&iter, _set1_);
    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (ts_impl_get_node(_set2_, value) == NULL)
            ts_insert(_set_r_, value);
    }
    return _set_r_;
}
treeset *ts_symmetric_difference(treeset *_set1_, treeset *_set2_)
{
    treeset *_set_r_ = ts_new(_set1_->cmp);
    if (!_set_r_)
        return NULL;
    treeset_iter iter1, iter2;
    ts_iter_init(&iter1, _set1_);
    ts_iter_init(&iter2, _set2_);
    for (ts_iter_to_start(&iter1); !ts_iter_end(&iter1); ts_iter_next(&iter1))
    {
        size_t value = ts_iter_value(&iter1);
        if (ts_impl_get_node(_set2_, value) == NULL)
            ts_insert(_set_r_, value);
    }
    for (ts_iter_to_start(&iter2); !ts_iter_end(&iter2); ts_iter_next(&iter2))
    {
        size_t value = ts_iter_value(&iter2);
        if (ts_impl_get_node(_set1_, value) == NULL)
            ts_insert(_set_r_, value);
    }
    return _set_r_;
}
bool ts_is_subset(treeset *_set1_, treeset *_set2_)
{
    if (ts_count(_set1_) > ts_count(_set2_))
        return false;
    if (ts_empty(_set1_))
        return true;
    treeset_iter iter;
    ts_iter_init(&iter, _set1_);
    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (!ts_contains(_set2_, value))
            return false;
    }
    return true;
}
bool ts_is_superset(treeset *_set1_, treeset *_set2_) { return ts_is_subset(_set2_, _set1_); }
bool ts_is_proper_subset(treeset *_set1_, treeset *_set2_)
{
    if (ts_count(_set1_) >= ts_count(_set2_))
        return false;
    if (ts_empty(_set1_))
    {
        if (!ts_empty(_set2_))
            return true;
        else
            return false;
    }
    treeset_iter iter;
    ts_iter_init(&iter, _set1_);
    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (!ts_contains(_set2_, value))
            return false;
    }
    return true;
}
bool ts_is_proper_superset(treeset *_set1_, treeset *_set2_) { return ts_is_proper_subset(_set2_, _set1_); }
bool ts_is_disjointset(treeset *_set1_, treeset *_set2_)
{
    if (ts_empty(_set1_))
        return true;
    treeset_iter iter;
    ts_iter_init(&iter, _set1_);
    for (ts_iter_to_start(&iter); !ts_iter_end(&iter); ts_iter_next(&iter))
    {
        size_t value = ts_iter_value(&iter);
        if (ts_contains(_set2_, value))
            return false;
    }
    return true;
}
treeset_iter *ts_iter_new(treeset *target)
{
    treeset_iter *iter = malloc(sizeof(treeset_iter));
    if (!iter)
        return NULL;
    ts_iter_init(iter, target);
    return iter;
}
void ts_iter_free(treeset_iter *iter) { free(iter); }
void ts_iter_init(treeset_iter *iter, treeset *target)
{
    iter->target = target;
    iter->index = 0;
    iter->start = true;
    iter->end = ts_empty(target);
    iter->cursor = target->root;
    while (iter->cursor->left != NULL)
        iter->cursor = iter->cursor->left;
    iter->first = iter->cursor;
    iter->last = target->root;
    while (iter->last->right != NULL)
        iter->last = iter->last->right;
}
bool ts_iter_start(treeset_iter *iter) { return ts_empty(iter->target) || iter->start; }
bool ts_iter_end(treeset_iter *iter) { return ts_empty(iter->target) || iter->end; }
void ts_iter_to_start(treeset_iter *iter)
{
    iter->index = 0;
    iter->start = true;
    iter->end = ts_empty(iter->target);
    iter->cursor = iter->first;
}
void ts_iter_to_end(treeset_iter *iter)
{
    iter->index = iter->target->count - 1;
    iter->start = ts_empty(iter->target);
    iter->end = true;
    iter->cursor = iter->last;
}
bool ts_iter_next(treeset_iter *iter)
{
    if (iter->end)
        return false;
    iter->start = ts_empty(iter->target);
    if (iter->cursor->right != NULL)
    {
        iter->cursor = iter->cursor->right;
        while (iter->cursor->left != NULL)
            iter->cursor = iter->cursor->left;
        iter->index++;
        return true;
    }
    while (true)
    {
        if (iter->cursor == iter->last)
        {
            iter->end = true;
            return true;
        }
        if (iter->cursor->parent->left == iter->cursor)
        {
            iter->cursor = iter->cursor->parent;
            iter->index++;
            return true;
        }
        iter->cursor = iter->cursor->parent;
    }
}
bool ts_iter_prev(treeset_iter *iter)
{
    if (iter->start)
        return false;
    iter->end = ts_empty(iter->target);
    if (iter->cursor->left != NULL)
    {
        iter->cursor = iter->cursor->left;
        while (iter->cursor->right != NULL)
            iter->cursor = iter->cursor->right;
        iter->index--;
        return true;
    }
    while (true)
    {
        if (iter->cursor == iter->first)
        {
            iter->start = true;
            return true;
        }
        if (iter->cursor->parent->right == iter->cursor)
        {
            iter->cursor = iter->cursor->parent;
            iter->index--;
            return true;
        }
        iter->cursor = iter->cursor->parent;
    }
}
size_t ts_iter_value(treeset_iter *iter)
{
    if (ts_empty(iter->target))
        return ts_impl_default_value();
    return iter->cursor->value;
}
size_t ts_iter_index(treeset_iter *iter) { return iter->index; }
static treeset_node *ts_impl_new_node(size_t element)
{
    treeset_node *node = malloc(sizeof(treeset_node));
    if (!node)
        return NULL;
    node->value = element;
    node->right = NULL;
    node->left = NULL;
    node->parent = NULL;
    node->height = 0;
    return node;
}
static treeset_node *ts_impl_get_node(treeset *_set_, size_t element)
{
    if (ts_empty(_set_))
        return NULL;
    treeset_node *scan = _set_->root;
    while (scan != NULL)
    {
        if (_set_->cmp(scan->value, element) > 0)
            scan = scan->left;
        else if (_set_->cmp(scan->value, element) < 0)
            scan = scan->right;
        else
            return scan;
    }
    return NULL;
}
static unsigned char ts_impl_h(treeset_node *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}
static unsigned char ts_impl_hupdate(treeset_node *node)
{
    if (node == NULL)
        return 0;
    unsigned char h_l = ts_impl_h(node->left);
    unsigned char h_r = ts_impl_h(node->right);
    return 1 + (h_l > h_r ? h_l : h_r);
}
static void ts_impl_rotate_right(treeset_node **Z)
{
    treeset_node *root = *Z;
    treeset_node *new_root = root->left;
    if (root->parent != NULL)
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
    if (root->parent != NULL)
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
    treeset_node *scan = node, *child = NULL;
    int balance;
    bool is_root = false;
    while (scan != NULL)
    {
        if (scan->parent == NULL)
            is_root = true;
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
            is_root = false;
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
