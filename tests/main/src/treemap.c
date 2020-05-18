#include "cmc/treemap.h"

struct treemap
{
    struct treemap_node *root;
    size_t count;
    int flag;
    struct treemap_fkey *f_key;
    struct treemap_fval *f_val;
    struct cmc_alloc_node *alloc;
    struct cmc_callbacks *callbacks;
};
struct treemap_node
{
    size_t key;
    size_t value;
    unsigned char height;
    struct treemap_node *right;
    struct treemap_node *left;
    struct treemap_node *parent;
};
struct treemap_fkey
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct treemap_fval
{
    int (*cmp)(size_t, size_t);
    size_t (*cpy)(size_t);
    _Bool (*str)(FILE *, size_t);
    void (*free)(size_t);
    size_t (*hash)(size_t);
    int (*pri)(size_t, size_t);
};
struct treemap_iter
{
    struct treemap *target;
    struct treemap_node *cursor;
    struct treemap_node *first;
    struct treemap_node *last;
    size_t index;
    _Bool start;
    _Bool end;
};
struct treemap *tm_new(struct treemap_fkey *f_key, struct treemap_fval *f_val);
struct treemap *tm_new_custom(struct treemap_fkey *f_key,
                              struct treemap_fval *f_val,
                              struct cmc_alloc_node *alloc,
                              struct cmc_callbacks *callbacks);
void tm_clear(struct treemap *_map_);
void tm_free(struct treemap *_map_);
void tm_customize(struct treemap *_map_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks);
_Bool tm_insert(struct treemap *_map_, size_t key, size_t value);
_Bool tm_update(struct treemap *_map_, size_t key, size_t new_value,
                size_t *old_value);
_Bool tm_remove(struct treemap *_map_, size_t key, size_t *out_value);
_Bool tm_max(struct treemap *_map_, size_t *key, size_t *value);
_Bool tm_min(struct treemap *_map_, size_t *key, size_t *value);
size_t tm_get(struct treemap *_map_, size_t key);
size_t *tm_get_ref(struct treemap *_map_, size_t key);
_Bool tm_contains(struct treemap *_map_, size_t key);
_Bool tm_empty(struct treemap *_map_);
size_t tm_count(struct treemap *_map_);
int tm_flag(struct treemap *_map_);
struct treemap *tm_copy_of(struct treemap *_map_);
_Bool tm_equals(struct treemap *_map1_, struct treemap *_map2_);
struct cmc_string tm_to_string(struct treemap *_map_);
_Bool tm_print(struct treemap *_map_, FILE *fptr);
struct treemap_iter tm_iter_start(struct treemap *target);
struct treemap_iter tm_iter_end(struct treemap *target);
_Bool tm_iter_at_start(struct treemap_iter *iter);
_Bool tm_iter_at_end(struct treemap_iter *iter);
_Bool tm_iter_to_start(struct treemap_iter *iter);
_Bool tm_iter_to_end(struct treemap_iter *iter);
_Bool tm_iter_next(struct treemap_iter *iter);
_Bool tm_iter_prev(struct treemap_iter *iter);
_Bool tm_iter_advance(struct treemap_iter *iter, size_t steps);
_Bool tm_iter_rewind(struct treemap_iter *iter, size_t steps);
_Bool tm_iter_go_to(struct treemap_iter *iter, size_t index);
size_t tm_iter_key(struct treemap_iter *iter);
size_t tm_iter_value(struct treemap_iter *iter);
size_t *tm_iter_rvalue(struct treemap_iter *iter);
size_t tm_iter_index(struct treemap_iter *iter);
static struct treemap_node *tm_impl_new_node(struct treemap *_map_, size_t key,
                                             size_t value);
static struct treemap_node *tm_impl_get_node(struct treemap *_map_, size_t key);
static unsigned char tm_impl_h(struct treemap_node *node);
static unsigned char tm_impl_hupdate(struct treemap_node *node);
static void tm_impl_rotate_right(struct treemap_node **Z);
static void tm_impl_rotate_left(struct treemap_node **Z);
static void tm_impl_rebalance(struct treemap *_map_, struct treemap_node *node);
struct treemap *tm_new(struct treemap_fkey *f_key, struct treemap_fval *f_val)
{
    if (!f_key || !f_val)
        return ((void *)0);
    struct cmc_alloc_node *alloc = &cmc_alloc_node_default;
    struct treemap *_map_ = alloc->malloc(sizeof(struct treemap));
    if (!_map_)
        return ((void *)0);
    _map_->count = 0;
    _map_->root = ((void *)0);
    _map_->flag = cmc_flags.OK;
    _map_->f_key = f_key;
    _map_->f_val = f_val;
    _map_->alloc = alloc;
    _map_->callbacks = ((void *)0);
    return _map_;
}
struct treemap *tm_new_custom(struct treemap_fkey *f_key,
                              struct treemap_fval *f_val,
                              struct cmc_alloc_node *alloc,
                              struct cmc_callbacks *callbacks)
{
    if (!f_key || !f_val)
        return ((void *)0);
    if (!alloc)
        alloc = &cmc_alloc_node_default;
    struct treemap *_map_ = alloc->malloc(sizeof(struct treemap));
    if (!_map_)
        return ((void *)0);
    _map_->count = 0;
    _map_->root = ((void *)0);
    _map_->flag = cmc_flags.OK;
    _map_->f_key = f_key;
    _map_->f_val = f_val;
    _map_->alloc = alloc;
    _map_->callbacks = callbacks;
    return _map_;
}
void tm_clear(struct treemap *_map_)
{
    struct treemap_node *scan = _map_->root;
    struct treemap_node *up = ((void *)0);
    while (scan != ((void *)0))
    {
        if (scan->left != ((void *)0))
        {
            struct treemap_node *left = scan->left;
            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != ((void *)0))
        {
            struct treemap_node *right = scan->right;
            scan->left = up;
            scan->right = ((void *)0);
            up = scan;
            scan = right;
        }
        else
        {
            if (up == ((void *)0))
            {
                if (_map_->f_key->free)
                    _map_->f_key->free(scan->key);
                if (_map_->f_val->free)
                    _map_->f_val->free(scan->value);
                _map_->alloc->free(scan);
                scan = ((void *)0);
            }
            while (up != ((void *)0))
            {
                if (_map_->f_key->free)
                    _map_->f_key->free(scan->key);
                if (_map_->f_val->free)
                    _map_->f_val->free(scan->value);
                _map_->alloc->free(scan);
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
    _map_->flag = cmc_flags.OK;
}
void tm_free(struct treemap *_map_)
{
    tm_clear(_map_);
    _map_->alloc->free(_map_);
}
void tm_customize(struct treemap *_map_, struct cmc_alloc_node *alloc,
                  struct cmc_callbacks *callbacks)
{
    if (!alloc)
        _map_->alloc = &cmc_alloc_node_default;
    else
        _map_->alloc = alloc;
    _map_->callbacks = callbacks;
    _map_->flag = cmc_flags.OK;
}
_Bool tm_insert(struct treemap *_map_, size_t key, size_t value)
{
    if (tm_empty(_map_))
    {
        _map_->root = tm_impl_new_node(_map_, key, value);
        if (!_map_->root)
        {
            _map_->flag = cmc_flags.ALLOC;
            return 0;
        }
    }
    else
    {
        struct treemap_node *scan = _map_->root;
        struct treemap_node *parent = scan;
        while (scan != ((void *)0))
        {
            parent = scan;
            if (_map_->f_key->cmp(scan->key, key) > 0)
                scan = scan->left;
            else if (_map_->f_key->cmp(scan->key, key) < 0)
                scan = scan->right;
            else
            {
                _map_->flag = cmc_flags.DUPLICATE;
                return 0;
            }
        }
        struct treemap_node *node;
        if (_map_->f_key->cmp(parent->key, key) > 0)
        {
            parent->left = tm_impl_new_node(_map_, key, value);
            if (!parent->left)
            {
                _map_->flag = cmc_flags.ALLOC;
                return 0;
            }
            parent->left->parent = parent;
            node = parent->left;
        }
        else
        {
            parent->right = tm_impl_new_node(_map_, key, value);
            if (!parent->right)
            {
                _map_->flag = cmc_flags.ALLOC;
                return 0;
            }
            parent->right->parent = parent;
            node = parent->right;
        }
        tm_impl_rebalance(_map_, node);
    }
    _map_->count++;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->create)
        _map_->callbacks->create();
    return 1;
}
_Bool tm_update(struct treemap *_map_, size_t key, size_t new_value,
                size_t *old_value)
{
    struct treemap_node *node = tm_impl_get_node(_map_, key);
    if (!node)
    {
        _map_->flag = cmc_flags.NOT_FOUND;
        return 0;
    }
    if (old_value)
        *old_value = node->value;
    node->value = new_value;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->update)
        _map_->callbacks->update();
    return 1;
}
_Bool tm_remove(struct treemap *_map_, size_t key, size_t *out_value)
{
    if (tm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct treemap_node *node = tm_impl_get_node(_map_, key);
    if (!node)
    {
        _map_->flag = cmc_flags.NOT_FOUND;
        return 0;
    }
    if (out_value)
        *out_value = node->value;
    struct treemap_node *temp = ((void *)0), *unbalanced = ((void *)0);
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
        _map_->alloc->free(node);
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
        _map_->alloc->free(node);
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
        _map_->alloc->free(node);
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
        _map_->alloc->free(temp);
        node->key = temp_key;
        node->value = temp_val;
    }
    if (unbalanced != ((void *)0))
        tm_impl_rebalance(_map_, unbalanced);
    if (_map_->count == 0)
        _map_->root = ((void *)0);
    _map_->count--;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->delete)
        _map_->callbacks->delete ();
    return 1;
}
_Bool tm_max(struct treemap *_map_, size_t *key, size_t *value)
{
    if (tm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct treemap_node *scan = _map_->root;
    while (scan->right != ((void *)0))
        scan = scan->right;
    if (key)
        *key = scan->key;
    if (value)
        *value = scan->value;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return 1;
}
_Bool tm_min(struct treemap *_map_, size_t *key, size_t *value)
{
    if (tm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct treemap_node *scan = _map_->root;
    while (scan->left != ((void *)0))
        scan = scan->left;
    if (key)
        *key = scan->key;
    if (value)
        *value = scan->value;
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return 1;
}
size_t tm_get(struct treemap *_map_, size_t key)
{
    if (tm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct treemap_node *node = tm_impl_get_node(_map_, key);
    if (!node)
    {
        _map_->flag = cmc_flags.NOT_FOUND;
        return (size_t){ 0 };
    }
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return node->value;
}
size_t *tm_get_ref(struct treemap *_map_, size_t key)
{
    if (tm_empty(_map_))
    {
        _map_->flag = cmc_flags.EMPTY;
        return 0;
    }
    struct treemap_node *node = tm_impl_get_node(_map_, key);
    if (!node)
    {
        _map_->flag = cmc_flags.NOT_FOUND;
        return ((void *)0);
    }
    _map_->flag = cmc_flags.OK;
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return &(node->value);
}
_Bool tm_contains(struct treemap *_map_, size_t key)
{
    _Bool result = tm_impl_get_node(_map_, key) != ((void *)0);
    if (_map_->callbacks && _map_->callbacks->read)
        _map_->callbacks->read();
    return result;
}
_Bool tm_empty(struct treemap *_map_)
{
    return _map_->count == 0;
}
size_t tm_count(struct treemap *_map_)
{
    return _map_->count;
}
int tm_flag(struct treemap *_map_)
{
    return _map_->flag;
}
struct treemap *tm_copy_of(struct treemap *_map_)
{
    struct treemap *result =
        tm_new_custom(_map_->f_key, _map_->f_val, _map_->alloc, ((void *)0));
    if (!result)
    {
        _map_->flag = cmc_flags.ERROR;
        return ((void *)0);
    }
    struct treemap_iter iter = tm_iter_start(_map_);
    for (; !tm_iter_at_end(&iter); tm_iter_next(&iter))
    {
        size_t key = tm_iter_key(&iter);
        size_t value = tm_iter_value(&iter);
        if (_map_->f_key->cpy)
            key = _map_->f_key->cpy(key);
        if (_map_->f_val->cpy)
            value = _map_->f_val->cpy(value);
        tm_insert(result, key, value);
    }
    result->callbacks = _map_->callbacks;
    _map_->flag = cmc_flags.OK;
    return result;
}
_Bool tm_equals(struct treemap *_map1_, struct treemap *_map2_)
{
    _map1_->flag = cmc_flags.OK;
    _map2_->flag = cmc_flags.OK;
    if (_map1_->count != _map2_->count)
        return 0;
    struct treemap_iter iter = tm_iter_start(_map1_);
    for (; !tm_iter_at_end(&iter); tm_iter_next(&iter))
    {
        struct treemap_node *node =
            tm_impl_get_node(_map2_, tm_iter_key(&iter));
        if (node == ((void *)0))
            return 0;
        if (_map1_->f_val->cmp(node->value, tm_iter_value(&iter)) != 0)
            return 0;
    }
    return 1;
}
struct cmc_string tm_to_string(struct treemap *_map_)
{
    struct cmc_string str;
    struct treemap *m_ = _map_;
    int n = snprintf(str.s, cmc_string_len, cmc_string_fmt_treemap, "treemap",
                     "size_t", "size_t", m_, m_->root, m_->count, m_->flag,
                     m_->f_key, m_->f_val, m_->alloc, m_->callbacks);
    return n >= 0 ? str : (struct cmc_string){ 0 };
}
_Bool tm_print(struct treemap *_map_, FILE *fptr)
{
    struct treemap_node *root = _map_->root;
    _Bool left_done = 0;
    while (root)
    {
        if (!left_done)
        {
            while (root->left)
                root = root->left;
        }
        if (!_map_->f_key->str(fptr, root->key) &&
            !_map_->f_val->str(fptr, root->value))
            return 0;
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
    return 1;
}
struct treemap_iter tm_iter_start(struct treemap *target)
{
    struct treemap_iter iter;
    iter.target = target;
    iter.cursor = target->root;
    iter.first = ((void *)0);
    iter.last = ((void *)0);
    iter.index = 0;
    iter.start = 1;
    iter.end = tm_empty(target);
    if (!tm_empty(target))
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
struct treemap_iter tm_iter_end(struct treemap *target)
{
    struct treemap_iter iter;
    iter.target = target;
    iter.cursor = target->root;
    iter.first = ((void *)0);
    iter.last = ((void *)0);
    iter.index = 0;
    iter.start = tm_empty(target);
    iter.end = 1;
    if (!tm_empty(target))
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
_Bool tm_iter_at_start(struct treemap_iter *iter)
{
    return tm_empty(iter->target) || iter->start;
}
_Bool tm_iter_at_end(struct treemap_iter *iter)
{
    return tm_empty(iter->target) || iter->end;
}
_Bool tm_iter_to_start(struct treemap_iter *iter)
{
    if (!tm_empty(iter->target))
    {
        iter->index = 0;
        iter->start = 1;
        iter->end = tm_empty(iter->target);
        iter->cursor = iter->first;
        return 1;
    }
    return 0;
}
_Bool tm_iter_to_end(struct treemap_iter *iter)
{
    if (!tm_empty(iter->target))
    {
        iter->index = iter->target->count - 1;
        iter->start = tm_empty(iter->target);
        iter->end = 1;
        iter->cursor = iter->last;
        return 1;
    }
    return 0;
}
_Bool tm_iter_next(struct treemap_iter *iter)
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
_Bool tm_iter_prev(struct treemap_iter *iter)
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
_Bool tm_iter_advance(struct treemap_iter *iter, size_t steps)
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
        tm_iter_next(iter);
    return 1;
}
_Bool tm_iter_rewind(struct treemap_iter *iter, size_t steps)
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
        tm_iter_prev(iter);
    return 1;
}
_Bool tm_iter_go_to(struct treemap_iter *iter, size_t index)
{
    if (index >= iter->target->count)
        return 0;
    if (iter->index > index)
        return tm_iter_rewind(iter, iter->index - index);
    else if (iter->index < index)
        return tm_iter_advance(iter, index - iter->index);
    return 1;
}
size_t tm_iter_key(struct treemap_iter *iter)
{
    if (tm_empty(iter->target))
        return (size_t){ 0 };
    return iter->cursor->key;
}
size_t tm_iter_value(struct treemap_iter *iter)
{
    if (tm_empty(iter->target))
        return (size_t){ 0 };
    return iter->cursor->value;
}
size_t *tm_iter_rvalue(struct treemap_iter *iter)
{
    if (tm_empty(iter->target))
        return ((void *)0);
    return &(iter->cursor->value);
}
size_t tm_iter_index(struct treemap_iter *iter)
{
    return iter->index;
}
static struct treemap_node *tm_impl_new_node(struct treemap *_map_, size_t key,
                                             size_t value)
{
    struct treemap_node *node =
        _map_->alloc->malloc(sizeof(struct treemap_node));
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
static struct treemap_node *tm_impl_get_node(struct treemap *_map_, size_t key)
{
    struct treemap_node *scan = _map_->root;
    while (scan != ((void *)0))
    {
        if (_map_->f_key->cmp(scan->key, key) > 0)
            scan = scan->left;
        else if (_map_->f_key->cmp(scan->key, key) < 0)
            scan = scan->right;
        else
            return scan;
    }
    return ((void *)0);
}
static unsigned char tm_impl_h(struct treemap_node *node)
{
    if (node == ((void *)0))
        return 0;
    return node->height;
}
static unsigned char tm_impl_hupdate(struct treemap_node *node)
{
    if (node == ((void *)0))
        return 0;
    unsigned char h_l = tm_impl_h(node->left);
    unsigned char h_r = tm_impl_h(node->right);
    return 1 + (h_l > h_r ? h_l : h_r);
}
static void tm_impl_rotate_right(struct treemap_node **Z)
{
    struct treemap_node *root = *Z;
    struct treemap_node *new_root = root->left;
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
static void tm_impl_rotate_left(struct treemap_node **Z)
{
    struct treemap_node *root = *Z;
    struct treemap_node *new_root = root->right;
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
static void tm_impl_rebalance(struct treemap *_map_, struct treemap_node *node)
{
    struct treemap_node *scan = node, *child = ((void *)0);
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
