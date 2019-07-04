#include "cmc/treemap.h"

typedef struct treemap_s
{
    struct treemap_node_s *root;
    size_t count;
    int (*cmp)(size_t, size_t);
    struct treemap_iter_s (*it_start)(struct treemap_s *);
    struct treemap_iter_s (*it_end)(struct treemap_s *);
} treemap, *treemap_ptr;
typedef struct treemap_node_s
{
    size_t key;
    size_t value;
    unsigned char height;
    struct treemap_node_s *right;
    struct treemap_node_s *left;
    struct treemap_node_s *parent;
} treemap_node, *treemap_node_ptr;
typedef struct treemap_iter_s
{
    struct treemap_s *target;
    struct treemap_node_s *cursor;
    struct treemap_node_s *first;
    struct treemap_node_s *last;
    size_t index;
    bool start;
    bool end;
} treemap_iter, *treemap_iter_ptr;
treemap *tm_new(int (*compare)(size_t, size_t));
void tm_clear(treemap *_map_);
void tm_free(treemap *_map_);
bool tm_insert(treemap *_map_, size_t key, size_t value);
bool tm_remove(treemap *_map_, size_t key, size_t *value);
bool tm_insert_if(treemap *_map_, size_t key, size_t value, bool condition);
bool tm_remove_if(treemap *_map_, size_t key, size_t *value, bool condition);
bool tm_max(treemap *_map_, size_t *key, size_t *value);
bool tm_min(treemap *_map_, size_t *key, size_t *value);
size_t tm_get(treemap *_map_, size_t key);
size_t *tm_get_ref(treemap *_map_, size_t key);
bool tm_contains(treemap *_map_, size_t key);
bool tm_empty(treemap *_map_);
size_t tm_count(treemap *_map_);
treemap_iter *tm_iter_new(treemap *target);
void tm_iter_free(treemap_iter *iter);
void tm_iter_init(treemap_iter *iter, treemap *target);
bool tm_iter_start(treemap_iter *iter);
bool tm_iter_end(treemap_iter *iter);
void tm_iter_to_start(treemap_iter *iter);
void tm_iter_to_end(treemap_iter *iter);
bool tm_iter_next(treemap_iter *iter);
bool tm_iter_prev(treemap_iter *iter);
size_t tm_iter_key(treemap_iter *iter);
size_t tm_iter_value(treemap_iter *iter);
size_t *tm_iter_rvalue(treemap_iter *iter);
size_t tm_iter_index(treemap_iter *iter);
static inline size_t tm_impl_default_key(void)
{
    size_t _empty_key_;
    memset(&_empty_key_, 0, sizeof(size_t));
    return _empty_key_;
}
static inline size_t tm_impl_default_value(void)
{
    size_t _empty_value_;
    memset(&_empty_value_, 0, sizeof(size_t));
    return _empty_value_;
}
static treemap_node *tm_impl_new_node(size_t key, size_t value);
static treemap_node *tm_impl_get_node(treemap *_map_, size_t key);
static unsigned char tm_impl_h(treemap_node *node);
static unsigned char tm_impl_hupdate(treemap_node *node);
static void tm_impl_rotate_right(treemap_node **Z);
static void tm_impl_rotate_left(treemap_node **Z);
static void tm_impl_rebalance(treemap *_map_, treemap_node *node);
static treemap_iter tm_impl_it_start(treemap *_map_);
static treemap_iter tm_impl_it_end(treemap *_map_);
treemap *tm_new(int (*compare)(size_t, size_t))
{
    treemap *_map_ = malloc(sizeof(treemap));
    if (!_map_)
        return NULL;
    _map_->count = 0;
    _map_->root = NULL;
    _map_->cmp = compare;
    _map_->it_start = tm_impl_it_start;
    _map_->it_end = tm_impl_it_end;
    return _map_;
}
void tm_clear(treemap *_map_)
{
    treemap_node *scan = _map_->root;
    treemap_node *up = NULL;
    while (scan != NULL)
    {
        if (scan->left != NULL)
        {
            treemap_node *left = scan->left;
            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != NULL)
        {
            treemap_node *right = scan->right;
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
    _map_->count = 0;
    _map_->root = NULL;
}
void tm_free(treemap *_map_)
{
    tm_clear(_map_);
    free(_map_);
}
bool tm_insert(treemap *_map_, size_t key, size_t value)
{
    if (tm_empty(_map_))
    {
        _map_->root = tm_impl_new_node(key, value);
        if (!_map_->root)
            return false;
    }
    else
    {
        treemap_node *scan = _map_->root;
        treemap_node *parent = scan;
        while (scan != NULL)
        {
            parent = scan;
            if (_map_->cmp(scan->key, key) > 0)
                scan = scan->left;
            else if (_map_->cmp(scan->key, key) < 0)
                scan = scan->right;
            else
                return false;
        }
        treemap_node *node;
        if (_map_->cmp(parent->key, key) > 0)
        {
            parent->left = tm_impl_new_node(key, value);
            if (!parent->left)
                return false;
            parent->left->parent = parent;
            node = parent->left;
        }
        else
        {
            parent->right = tm_impl_new_node(key, value);
            if (!parent->right)
                return false;
            parent->right->parent = parent;
            node = parent->right;
        }
        tm_impl_rebalance(_map_, node);
    }
    _map_->count++;
    return true;
}
bool tm_remove(treemap *_map_, size_t key, size_t *value)
{
    treemap_node *node = tm_impl_get_node(_map_, key);
    if (!node)
        return false;
    *value = node->value;
    treemap_node *temp = NULL, *unbalanced = NULL;
    bool is_root = node->parent == NULL;
    if (node->left == NULL && node->right == NULL)
    {
        if (is_root)
            _map_->root = NULL;
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
            _map_->root = node->right;
            _map_->root->parent = NULL;
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
            _map_->root = node->left;
            _map_->root->parent = NULL;
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
        size_t temp_key = temp->key;
        size_t temp_val = temp->value;
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
        node->key = temp_key;
        node->value = temp_val;
    }
    if (unbalanced != NULL)
        tm_impl_rebalance(_map_, unbalanced);
    _map_->count--;
    if (_map_->count == 0)
        _map_->root = NULL;
    return true;
}
bool tm_insert_if(treemap *_map_, size_t key, size_t value, bool condition)
{
    if (condition)
        return tm_insert(_map_, key, value);
    return false;
}
bool tm_remove_if(treemap *_map_, size_t key, size_t *value, bool condition)
{
    if (condition)
        return tm_remove(_map_, key, value);
    return false;
}
bool tm_max(treemap *_map_, size_t *key, size_t *value)
{
    if (tm_empty(_map_))
        return false;
    treemap_node *scan = _map_->root;
    while (scan->right != NULL)
        scan = scan->right;
    *key = scan->key;
    *value = scan->value;
    return true;
}
bool tm_min(treemap *_map_, size_t *key, size_t *value)
{
    if (tm_empty(_map_))
        return false;
    treemap_node *scan = _map_->root;
    while (scan->left != NULL)
        scan = scan->left;
    *key = scan->key;
    *value = scan->value;
    return true;
}
size_t tm_get(treemap *_map_, size_t key)
{
    treemap_node *node = tm_impl_get_node(_map_, key);
    if (!node)
        return tm_impl_default_value();
    return node->value;
}
size_t *tm_get_ref(treemap *_map_, size_t key)
{
    treemap_node *node = tm_impl_get_node(_map_, key);
    if (!node)
        return NULL;
    return &(node->value);
}
bool tm_contains(treemap *_map_, size_t key)
{
    treemap_node *scan = _map_->root;
    while (scan != NULL)
    {
        if (_map_->cmp(scan->key, key) > 0)
            scan = scan->left;
        else if (_map_->cmp(scan->key, key) < 0)
            scan = scan->right;
        else
            return true;
    }
    return false;
}
bool tm_empty(treemap *_map_) { return _map_->count == 0; }
size_t tm_count(treemap *_map_) { return _map_->count; }
treemap_iter *tm_iter_new(treemap *target)
{
    treemap_iter *iter = malloc(sizeof(treemap_iter));
    if (!iter)
        return NULL;
    tm_iter_init(iter, target);
    return iter;
}
void tm_iter_free(treemap_iter *iter) { free(iter); }
void tm_iter_init(treemap_iter *iter, treemap *target)
{
    iter->target = target;
    iter->index = 0;
    iter->start = true;
    iter->end = tm_empty(target);
    iter->cursor = target->root;
    while (iter->cursor->left != NULL)
        iter->cursor = iter->cursor->left;
    iter->first = iter->cursor;
    iter->last = target->root;
    while (iter->last->right != NULL)
        iter->last = iter->last->right;
}
bool tm_iter_start(treemap_iter *iter) { return tm_empty(iter->target) || iter->start; }
bool tm_iter_end(treemap_iter *iter) { return tm_empty(iter->target) || iter->end; }
void tm_iter_to_start(treemap_iter *iter)
{
    iter->index = 0;
    iter->start = true;
    iter->end = tm_empty(iter->target);
    iter->cursor = iter->first;
}
void tm_iter_to_end(treemap_iter *iter)
{
    iter->index = iter->target->count - 1;
    iter->start = tm_empty(iter->target);
    iter->end = true;
    iter->cursor = iter->last;
}
bool tm_iter_next(treemap_iter *iter)
{
    if (iter->end)
        return false;
    iter->start = false;
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
bool tm_iter_prev(treemap_iter *iter)
{
    if (iter->start)
        return false;
    iter->end = false;
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
size_t tm_iter_key(treemap_iter *iter)
{
    if (tm_empty(iter->target))
        return tm_impl_default_key();
    return iter->cursor->key;
}
size_t tm_iter_value(treemap_iter *iter)
{
    if (tm_empty(iter->target))
        return tm_impl_default_value();
    return iter->cursor->value;
}
size_t *tm_iter_rvalue(treemap_iter *iter)
{
    if (tm_empty(iter->target))
        return NULL;
    return &(iter->cursor->value);
}
size_t tm_iter_index(treemap_iter *iter) { return iter->index; }
static treemap_node *tm_impl_new_node(size_t key, size_t value)
{
    treemap_node *node = malloc(sizeof(treemap_node));
    if (!node)
        return NULL;
    node->key = key;
    node->value = value;
    node->right = NULL;
    node->left = NULL;
    node->parent = NULL;
    node->height = 0;
    return node;
}
static treemap_node *tm_impl_get_node(treemap *_map_, size_t key)
{
    if (tm_empty(_map_))
        return NULL;
    treemap_node *scan = _map_->root;
    while (scan != NULL)
    {
        if (_map_->cmp(scan->key, key) > 0)
            scan = scan->left;
        else if (_map_->cmp(scan->key, key) < 0)
            scan = scan->right;
        else
            return scan;
    }
    return NULL;
}
static unsigned char tm_impl_h(treemap_node *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}
static unsigned char tm_impl_hupdate(treemap_node *node)
{
    if (node == NULL)
        return 0;
    unsigned char h_l = tm_impl_h(node->left);
    unsigned char h_r = tm_impl_h(node->right);
    return 1 + (h_l > h_r ? h_l : h_r);
}
static void tm_impl_rotate_right(treemap_node **Z)
{
    treemap_node *root = *Z;
    treemap_node *new_root = root->left;
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
    root->height = tm_impl_hupdate(root);
    new_root->height = tm_impl_hupdate(new_root);
    *Z = new_root;
}
static void tm_impl_rotate_left(treemap_node **Z)
{
    treemap_node *root = *Z;
    treemap_node *new_root = root->right;
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
    root->height = tm_impl_hupdate(root);
    new_root->height = tm_impl_hupdate(new_root);
    *Z = new_root;
}
static void tm_impl_rebalance(treemap *_map_, treemap_node *node)
{
    treemap_node *scan = node, *child = NULL;
    int balance;
    bool is_root = false;
    while (scan != NULL)
    {
        if (scan->parent == NULL)
            is_root = true;
        scan->height = tm_impl_hupdate(scan);
        balance = tm_impl_h(scan->right) - tm_impl_h(scan->left);
        if (balance >= 2)
        {
            child = scan->right;
            if (tm_impl_h(child->right) < tm_impl_h(child->left))
                tm_impl_rotate_right(&(scan->right));
            tm_impl_rotate_left(&scan);
        }
        else if (balance <= -2)
        {
            child = scan->left;
            if (tm_impl_h(child->left) < tm_impl_h(child->right))
                tm_impl_rotate_left(&(scan->left));
            tm_impl_rotate_right(&scan);
        }
        if (is_root)
        {
            _map_->root = scan;
            is_root = false;
        }
        scan = scan->parent;
    }
}
static treemap_iter tm_impl_it_start(treemap *_map_)
{
    treemap_iter iter;
    tm_iter_init(&iter, _map_);
    return iter;
}
static treemap_iter tm_impl_it_end(treemap *_map_)
{
    treemap_iter iter;
    tm_iter_init(&iter, _map_);
    tm_iter_to_end(&iter);
    return iter;
}
