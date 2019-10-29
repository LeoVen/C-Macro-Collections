#include <cmc/treemap.h>

//CMC_GENERATE_TREEMAP(tm, treemap, size_t, size_t)

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
    _Bool start;
    _Bool end;
} treemap_iter, *treemap_iter_ptr;
treemap *tm_new(int (*compare)(size_t, size_t));
void tm_clear(treemap *_map_, void (*deallocator)(size_t, size_t));
void tm_free(treemap *_map_, void (*deallocator)(size_t, size_t));
_Bool tm_insert(treemap *_map_, size_t key, size_t value);
_Bool tm_update(treemap *_map_, size_t key, size_t new_value, size_t *old_value);
_Bool tm_remove(treemap *_map_, size_t key, size_t *out_value);
_Bool tm_max(treemap *_map_, size_t *key, size_t *value);
_Bool tm_min(treemap *_map_, size_t *key, size_t *value);
size_t tm_get(treemap *_map_, size_t key);
size_t *tm_get_ref(treemap *_map_, size_t key);
_Bool tm_contains(treemap *_map_, size_t key);
_Bool tm_empty(treemap *_map_);
size_t tm_count(treemap *_map_);
treemap *tm_copy_of(treemap *_map_, size_t (*key_copy_func)(size_t), size_t (*value_copy_func)(size_t));
_Bool tm_equals(treemap *_map1_, treemap *_map2_, int (*value_comparator)(size_t, size_t));
struct cmc_string tm_to_string(treemap *_map_);
treemap_iter *tm_iter_new(treemap *target);
void tm_iter_free(treemap_iter *iter);
void tm_iter_init(treemap_iter *iter, treemap *target);
_Bool tm_iter_start(treemap_iter *iter);
_Bool tm_iter_end(treemap_iter *iter);
void tm_iter_to_start(treemap_iter *iter);
void tm_iter_to_end(treemap_iter *iter);
_Bool tm_iter_next(treemap_iter *iter);
_Bool tm_iter_prev(treemap_iter *iter);
_Bool tm_iter_advance(treemap_iter *iter, size_t steps);
_Bool tm_iter_rewind(treemap_iter *iter, size_t steps);
_Bool tm_iter_go_to(treemap_iter *iter, size_t index);
size_t tm_iter_key(treemap_iter *iter);
size_t tm_iter_value(treemap_iter *iter);
size_t *tm_iter_rvalue(treemap_iter *iter);
size_t tm_iter_index(treemap_iter *iter);
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
        return ((void *)0);
    _map_->count = 0;
    _map_->root = ((void *)0);
    _map_->cmp = compare;
    _map_->it_start = tm_impl_it_start;
    _map_->it_end = tm_impl_it_end;
    return _map_;
}
void tm_clear(treemap *_map_, void (*deallocator)(size_t, size_t))
{
    treemap_node *scan = _map_->root;
    treemap_node *up = ((void *)0);
    while (scan != ((void *)0))
    {
        if (scan->left != ((void *)0))
        {
            treemap_node *left = scan->left;
            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != ((void *)0))
        {
            treemap_node *right = scan->right;
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
                    deallocator(scan->key, scan->value);
                free(scan);
                scan = ((void *)0);
            }
            while (up != ((void *)0))
            {
                if (deallocator)
                    deallocator(scan->key, scan->value);
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
    _map_->count = 0;
    _map_->root = ((void *)0);
}
void tm_free(treemap *_map_, void (*deallocator)(size_t, size_t))
{
    tm_clear(_map_, deallocator);
    free(_map_);
}
_Bool tm_insert(treemap *_map_, size_t key, size_t value)
{
    if (tm_empty(_map_))
    {
        _map_->root = tm_impl_new_node(key, value);
        if (!_map_->root)
            return 0;
    }
    else
    {
        treemap_node *scan = _map_->root;
        treemap_node *parent = scan;
        while (scan != ((void *)0))
        {
            parent = scan;
            if (_map_->cmp(scan->key, key) > 0)
                scan = scan->left;
            else if (_map_->cmp(scan->key, key) < 0)
                scan = scan->right;
            else
                return 0;
        }
        treemap_node *node;
        if (_map_->cmp(parent->key, key) > 0)
        {
            parent->left = tm_impl_new_node(key, value);
            if (!parent->left)
                return 0;
            parent->left->parent = parent;
            node = parent->left;
        }
        else
        {
            parent->right = tm_impl_new_node(key, value);
            if (!parent->right)
                return 0;
            parent->right->parent = parent;
            node = parent->right;
        }
        tm_impl_rebalance(_map_, node);
    }
    _map_->count++;
    return 1;
}
_Bool tm_update(treemap *_map_, size_t key, size_t new_value, size_t *old_value)
{
    treemap_node *node = tm_impl_get_node(_map_, key);
    if (!node)
        return 0;
    if (old_value)
        *old_value = node->value;
    node->value = new_value;
    return 1;
}
_Bool tm_remove(treemap *_map_, size_t key, size_t *out_value)
{
    treemap_node *node = tm_impl_get_node(_map_, key);
    if (!node)
        return 0;
    if (out_value)
        *out_value = node->value;
    treemap_node *temp = ((void *)0), *unbalanced = ((void *)0);
    _Bool is_root = node->parent == ((void *)0);
    if (node->left == ((void *)0) && node->right == ((void *)0))
    {
        if (is_root)
            _map_->root = ((void *)0);
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
            _map_->root = node->right;
            _map_->root->parent = ((void *)0);
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
            _map_->root = node->left;
            _map_->root->parent = ((void *)0);
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
        size_t temp_key = temp->key;
        size_t temp_val = temp->value;
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
        node->key = temp_key;
        node->value = temp_val;
    }
    if (unbalanced != ((void *)0))
        tm_impl_rebalance(_map_, unbalanced);
    _map_->count--;
    if (_map_->count == 0)
        _map_->root = ((void *)0);
    return 1;
}
_Bool tm_max(treemap *_map_, size_t *key, size_t *value)
{
    if (tm_empty(_map_))
        return 0;
    treemap_node *scan = _map_->root;
    while (scan->right != ((void *)0))
        scan = scan->right;
    *key = scan->key;
    *value = scan->value;
    return 1;
}
_Bool tm_min(treemap *_map_, size_t *key, size_t *value)
{
    if (tm_empty(_map_))
        return 0;
    treemap_node *scan = _map_->root;
    while (scan->left != ((void *)0))
        scan = scan->left;
    *key = scan->key;
    *value = scan->value;
    return 1;
}
size_t tm_get(treemap *_map_, size_t key)
{
    treemap_node *node = tm_impl_get_node(_map_, key);
    if (!node)
        return (size_t){0};
    return node->value;
}
size_t *tm_get_ref(treemap *_map_, size_t key)
{
    treemap_node *node = tm_impl_get_node(_map_, key);
    if (!node)
        return ((void *)0);
    return &(node->value);
}
_Bool tm_contains(treemap *_map_, size_t key)
{
    treemap_node *scan = _map_->root;
    while (scan != ((void *)0))
    {
        if (_map_->cmp(scan->key, key) > 0)
            scan = scan->left;
        else if (_map_->cmp(scan->key, key) < 0)
            scan = scan->right;
        else
            return 1;
    }
    return 0;
}
_Bool tm_empty(treemap *_map_) { return _map_->count == 0; }
size_t tm_count(treemap *_map_) { return _map_->count; }
treemap *tm_copy_of(treemap *_map_, size_t (*key_copy_func)(size_t), size_t (*value_copy_func)(size_t))
{
    treemap *result = tm_new(_map_->cmp);
    if (!result)
        return ((void *)0);
    treemap_iter iter;
    tm_iter_init(&iter, _map_);
    if (!tm_empty(_map_))
    {
        for (tm_iter_to_start(&iter); !tm_iter_end(&iter); tm_iter_next(&iter))
        {
            size_t key = tm_iter_key(&iter);
            size_t value = tm_iter_value(&iter);
            if (key_copy_func)
                key = key_copy_func(key);
            if (value_copy_func)
                value = value_copy_func(value);
            tm_insert(result, key, value);
        }
    }
    return result;
}
_Bool tm_equals(treemap *_map1_, treemap *_map2_, int (*value_comparator)(size_t, size_t))
{
    if (tm_count(_map1_) != tm_count(_map2_))
        return 0;
    treemap_iter iter;
    tm_iter_init(&iter, _map1_);
    for (tm_iter_to_start(&iter); !tm_iter_end(&iter); tm_iter_next(&iter))
    {
        treemap_node *node = tm_impl_get_node(_map2_, tm_iter_key(&iter));
        if (node == ((void *)0))
            return 0;
        if (value_comparator)
        {
            if (value_comparator(node->value, tm_iter_value(&iter)) != 0)
                return 0;
        }
    }
    return 1;
}
struct cmc_string tm_to_string(treemap *_map_)
{
    struct cmc_string str;
    treemap *m_ = _map_;
    const char *name = "treemap";
    snprintf(str.s, cmc_string_len, cmc_string_fmt_treemap, name, m_, m_->root, m_->count, m_->cmp);
    return str;
}
treemap_iter *tm_iter_new(treemap *target)
{
    treemap_iter *iter = malloc(sizeof(treemap_iter));
    if (!iter)
        return ((void *)0);
    tm_iter_init(iter, target);
    return iter;
}
void tm_iter_free(treemap_iter *iter) { free(iter); }
void tm_iter_init(treemap_iter *iter, treemap *target)
{
    memset(iter, 0, sizeof(treemap_iter));
    iter->target = target;
    iter->start = 1;
    iter->end = tm_empty(target);
    iter->cursor = target->root;
    if (!tm_empty(target))
    {
        while (iter->cursor->left != ((void *)0))
            iter->cursor = iter->cursor->left;
        iter->first = iter->cursor;
        iter->last = target->root;
        while (iter->last->right != ((void *)0))
            iter->last = iter->last->right;
    }
}
_Bool tm_iter_start(treemap_iter *iter) { return tm_empty(iter->target) || iter->start; }
_Bool tm_iter_end(treemap_iter *iter) { return tm_empty(iter->target) || iter->end; }
void tm_iter_to_start(treemap_iter *iter)
{
    if (!tm_empty(iter->target))
    {
        iter->index = 0;
        iter->start = 1;
        iter->end = tm_empty(iter->target);
        iter->cursor = iter->first;
    }
}
void tm_iter_to_end(treemap_iter *iter)
{
    if (!tm_empty(iter->target))
    {
        iter->index = iter->target->count - 1;
        iter->start = tm_empty(iter->target);
        iter->end = 1;
        iter->cursor = iter->last;
    }
}
_Bool tm_iter_next(treemap_iter *iter)
{
    if (iter->end)
        return 0;
    if (iter->cursor == iter->last)
    {
        iter->end = 1;
        return 0;
    }
    iter->start = tm_empty(iter->target);
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
_Bool tm_iter_prev(treemap_iter *iter)
{
    if (iter->start)
        return 0;
    if (iter->cursor == iter->first)
    {
        iter->start = 1;
        return 0;
    }
    iter->end = tm_empty(iter->target);
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
_Bool tm_iter_advance(treemap_iter *iter, size_t steps)
{
    if (iter->end)
        return 0;
    if (iter->cursor == iter->last)
    {
        iter->end = 1;
        return 0;
    }
    if (steps == 0 || iter->index + steps >= tm_count(iter->target))
        return 0;
    iter->index += steps;
    for (size_t i = 0; i < steps; i++)
        tm_iter_next(iter);
    return 1;
}
_Bool tm_iter_rewind(treemap_iter *iter, size_t steps)
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
        tm_iter_prev(iter);
    return 1;
}
_Bool tm_iter_go_to(treemap_iter *iter, size_t index)
{
    if (index >= tm_count(iter->target))
        return 0;
    if (iter->index > index)
        return tm_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return tm_iter_advance(iter, index - iter->index);
    return 1;
}
size_t tm_iter_key(treemap_iter *iter)
{
    if (tm_empty(iter->target))
        return (size_t){0};
    return iter->cursor->key;
}
size_t tm_iter_value(treemap_iter *iter)
{
    if (tm_empty(iter->target))
        return (size_t){0};
    return iter->cursor->value;
}
size_t *tm_iter_rvalue(treemap_iter *iter)
{
    if (tm_empty(iter->target))
        return ((void *)0);
    return &(iter->cursor->value);
}
size_t tm_iter_index(treemap_iter *iter) { return iter->index; }
static treemap_node *tm_impl_new_node(size_t key, size_t value)
{
    treemap_node *node = malloc(sizeof(treemap_node));
    if (!node)
        return ((void *)0);
    node->key = key;
    node->value = value;
    node->right = ((void *)0);
    node->left = ((void *)0);
    node->parent = ((void *)0);
    node->height = 0;
    return node;
}
static treemap_node *tm_impl_get_node(treemap *_map_, size_t key)
{
    if (tm_empty(_map_))
        return ((void *)0);
    treemap_node *scan = _map_->root;
    while (scan != ((void *)0))
    {
        if (_map_->cmp(scan->key, key) > 0)
            scan = scan->left;
        else if (_map_->cmp(scan->key, key) < 0)
            scan = scan->right;
        else
            return scan;
    }
    return ((void *)0);
}
static unsigned char tm_impl_h(treemap_node *node)
{
    if (node == ((void *)0))
        return 0;
    return node->height;
}
static unsigned char tm_impl_hupdate(treemap_node *node)
{
    if (node == ((void *)0))
        return 0;
    unsigned char h_l = tm_impl_h(node->left);
    unsigned char h_r = tm_impl_h(node->right);
    return 1 + (h_l > h_r ? h_l : h_r);
}
static void tm_impl_rotate_right(treemap_node **Z)
{
    treemap_node *root = *Z;
    treemap_node *new_root = root->left;
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
    root->height = tm_impl_hupdate(root);
    new_root->height = tm_impl_hupdate(new_root);
    *Z = new_root;
}
static void tm_impl_rotate_left(treemap_node **Z)
{
    treemap_node *root = *Z;
    treemap_node *new_root = root->right;
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
    root->height = tm_impl_hupdate(root);
    new_root->height = tm_impl_hupdate(new_root);
    *Z = new_root;
}
static void tm_impl_rebalance(treemap *_map_, treemap_node *node)
{
    treemap_node *scan = node, *child = ((void *)0);
    int balance;
    _Bool is_root = 0;
    while (scan != ((void *)0))
    {
        if (scan->parent == ((void *)0))
            is_root = 1;
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
            is_root = 0;
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
