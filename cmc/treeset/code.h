/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Library.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* Implementation Detail Functions */
static struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_new_node)(struct SNAME *_set_, V value);
static struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_get_node)(struct SNAME *_set_, V value);
static unsigned char CMC_(PFX, _impl_h)(struct CMC_DEF_NODE(SNAME) * node);
static unsigned char CMC_(PFX, _impl_hupdate)(struct CMC_DEF_NODE(SNAME) * node);
static void CMC_(PFX, _impl_rotate_right)(struct CMC_DEF_NODE(SNAME) * *Z);
static void CMC_(PFX, _impl_rotate_left)(struct CMC_DEF_NODE(SNAME) * *Z);
static void CMC_(PFX, _impl_rebalance)(struct SNAME *_set_, struct CMC_DEF_NODE(SNAME) * node);

struct SNAME *CMC_(PFX, _new)(struct CMC_DEF_FVAL(SNAME) * f_val)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return CMC_(PFX, _new_custom)(f_val, NULL, NULL);
}

struct SNAME *CMC_(PFX, _new_custom)(struct CMC_DEF_FVAL(SNAME) * f_val, CMC_ALLOC_TYPE alloc,
                                     CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (!f_val)
        return NULL;

    if (!alloc)
        alloc = &cmc_alloc_node_default;

    struct SNAME *_set_ = alloc->malloc(sizeof(struct SNAME));

    if (!_set_)
        return NULL;

    _set_->count = 0;
    _set_->root = NULL;
    _set_->flag = CMC_FLAG_OK;
    _set_->f_val = f_val;
    _set_->alloc = alloc;
    CMC_CALLBACKS_ASSIGN(_set_, callbacks);

    return _set_;
}

void CMC_(PFX, _clear)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_NODE(SNAME) *scan = _set_->root;
    struct CMC_DEF_NODE(SNAME) *up = NULL;

    while (scan != NULL)
    {
        if (scan->left != NULL)
        {
            struct CMC_DEF_NODE(SNAME) *left = scan->left;

            scan->left = up;
            up = scan;
            scan = left;
        }
        else if (scan->right != NULL)
        {
            struct CMC_DEF_NODE(SNAME) *right = scan->right;

            scan->left = up;
            scan->right = NULL;
            up = scan;
            scan = right;
        }
        else
        {
            if (up == NULL)
            {
                if (_set_->f_val->free)
                    _set_->f_val->free(scan->value);

                _set_->alloc->free(scan);
                scan = NULL;
            }

            while (up != NULL)
            {
                if (_set_->f_val->free)
                    _set_->f_val->free(scan->value);

                _set_->alloc->free(scan);

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
    _set_->flag = CMC_FLAG_OK;
}

void CMC_(PFX, _free)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_(PFX, _clear)(_set_);

    _set_->alloc->free(_set_);
}

void CMC_(PFX, _customize)(struct SNAME *_set_, CMC_ALLOC_TYPE alloc, CMC_CALLBACK_TYPE callbacks)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    CMC_CALLBACKS_MAYBE_UNUSED(callbacks);

    if (!alloc)
        _set_->alloc = &cmc_alloc_node_default;
    else
        _set_->alloc = alloc;

    CMC_CALLBACKS_ASSIGN(_set_, callbacks);

    _set_->flag = CMC_FLAG_OK;
}

bool CMC_(PFX, _insert)(struct SNAME *_set_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_set_))
    {
        _set_->root = CMC_(PFX, _impl_new_node)(_set_, value);

        if (!_set_->root)
        {
            _set_->flag = CMC_FLAG_ALLOC;
            CMC_CALLBACKS_CALL(_set_);
            return false;
        }
    }
    else
    {
        struct CMC_DEF_NODE(SNAME) *scan = _set_->root;
        struct CMC_DEF_NODE(SNAME) *parent = scan;

        while (scan != NULL)
        {
            parent = scan;

            if (_set_->f_val->cmp(scan->value, value) > 0)
                scan = scan->left;
            else if (_set_->f_val->cmp(scan->value, value) < 0)
                scan = scan->right;
            else
            {
                _set_->flag = CMC_FLAG_DUPLICATE;
                return false;
            }
        }

        struct CMC_DEF_NODE(SNAME) * node;

        if (_set_->f_val->cmp(parent->value, value) > 0)
        {
            parent->left = CMC_(PFX, _impl_new_node)(_set_, value);

            if (!parent->left)
            {
                _set_->flag = CMC_FLAG_ALLOC;
                CMC_CALLBACKS_CALL(_set_);
                return false;
            }

            parent->left->parent = parent;
            node = parent->left;
        }
        else
        {
            parent->right = CMC_(PFX, _impl_new_node)(_set_, value);

            if (!parent->right)
            {
                _set_->flag = CMC_FLAG_ALLOC;
                CMC_CALLBACKS_CALL(_set_);
                return false;
            }

            parent->right->parent = parent;
            node = parent->right;
        }

        CMC_(PFX, _impl_rebalance)(_set_, node);
    }

    _set_->count++;
    _set_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_set_);

    return true;
}

bool CMC_(PFX, _remove)(struct SNAME *_set_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        CMC_CALLBACKS_CALL(_set_);
        return false;
    }

    struct CMC_DEF_NODE(SNAME) *node = CMC_(PFX, _impl_get_node)(_set_, value);

    if (!node)
    {
        _set_->flag = CMC_FLAG_NOT_FOUND;
        CMC_CALLBACKS_CALL(_set_);
        return false;
    }

    struct CMC_DEF_NODE(SNAME) *temp = NULL, *unbalanced = NULL;

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

        _set_->alloc->free(node);
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

        _set_->alloc->free(node);
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

        _set_->alloc->free(node);
    }
    else
    {
        temp = node->right;
        while (temp->left != NULL)
            temp = temp->left;

        V temp_value = temp->value;

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

        _set_->alloc->free(temp);

        node->value = temp_value;
    }

    if (unbalanced != NULL)
        CMC_(PFX, _impl_rebalance)(_set_, unbalanced);

    if (_set_->count == 0)
        _set_->root = NULL;

    _set_->count--;
    _set_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_set_);

    return true;
}

bool CMC_(PFX, _max)(struct SNAME *_set_, V *value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    struct CMC_DEF_NODE(SNAME) *scan = _set_->root;

    while (scan->right != NULL)
        scan = scan->right;

    if (value)
        *value = scan->value;

    _set_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_set_);

    return true;
}

bool CMC_(PFX, _min)(struct SNAME *_set_, V *value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (CMC_(PFX, _empty)(_set_))
    {
        _set_->flag = CMC_FLAG_EMPTY;
        return false;
    }

    struct CMC_DEF_NODE(SNAME) *scan = _set_->root;

    while (scan->left != NULL)
        scan = scan->left;

    if (value)
        *value = scan->value;

    _set_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_CALL(_set_);

    return true;
}

bool CMC_(PFX, _contains)(struct SNAME *_set_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    bool result = CMC_(PFX, _impl_get_node)(_set_, value) != NULL;

    CMC_CALLBACKS_CALL(_set_);

    return result;
}

bool CMC_(PFX, _empty)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _set_->count == 0;
}

size_t CMC_(PFX, _count)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _set_->count;
}

int CMC_(PFX, _flag)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    return _set_->flag;
}

struct SNAME *CMC_(PFX, _copy_of)(struct SNAME *_set_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    /* Callback will be added later */
    struct SNAME *result = CMC_(PFX, _new_custom)(_set_->f_val, _set_->alloc, NULL);

    if (!result)
    {
        _set_->flag = CMC_FLAG_ERROR;
        return NULL;
    }

    struct CMC_DEF_NODE(SNAME) *root = _set_->root;

    bool left_done = false;

    while (root)
    {
        if (!left_done)
        {
            while (root->left)
                root = root->left;
        }

        V value;

        if (_set_->f_val->cpy)
            value = _set_->f_val->cpy(root->value);
        else
            value = root->value;

        /* TODO check this for errors */
        CMC_(PFX, _insert)(result, value);

        left_done = true;

        if (root->right)
        {
            left_done = false;
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

    _set_->flag = CMC_FLAG_OK;

    CMC_CALLBACKS_ASSIGN(result, _set_->callbacks);

    return result;
}

bool CMC_(PFX, _equals)(struct SNAME *_set1_, struct SNAME *_set2_)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    _set1_->flag = CMC_FLAG_OK;
    _set2_->flag = CMC_FLAG_OK;

    if (_set1_->count != _set2_->count)
        return false;

    struct CMC_DEF_NODE(SNAME) *root = _set1_->root;

    bool left_done = false;

    while (root)
    {
        if (!left_done)
        {
            while (root->left)
                root = root->left;
        }

        /* TODO this can be optimized by doing two in-order traversals */
        struct CMC_DEF_NODE(SNAME) *node = CMC_(PFX, _impl_get_node)(_set2_, root->value);

        if (node == NULL)
            return false;

        left_done = true;

        if (root->right)
        {
            left_done = false;
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

    return true;
}

static struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_new_node)(struct SNAME *_set_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_NODE(SNAME) *node = _set_->alloc->malloc(sizeof(struct CMC_DEF_NODE(SNAME)));

    if (!node)
        return NULL;

    node->value = value;
    node->right = NULL;
    node->left = NULL;
    node->parent = NULL;
    node->height = 0;

    return node;
}

static struct CMC_DEF_NODE(SNAME) * CMC_(PFX, _impl_get_node)(struct SNAME *_set_, V value)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_NODE(SNAME) *scan = _set_->root;

    while (scan != NULL)
    {
        if (_set_->f_val->cmp(scan->value, value) > 0)
            scan = scan->left;
        else if (_set_->f_val->cmp(scan->value, value) < 0)
            scan = scan->right;
        else
            return scan;
    }

    return NULL;
}

static unsigned char CMC_(PFX, _impl_h)(struct CMC_DEF_NODE(SNAME) * node)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (node == NULL)
        return 0;

    return node->height;
}

static unsigned char CMC_(PFX, _impl_hupdate)(struct CMC_DEF_NODE(SNAME) * node)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    if (node == NULL)
        return 0;

    unsigned char h_l = CMC_(PFX, _impl_h)(node->left);
    unsigned char h_r = CMC_(PFX, _impl_h)(node->right);

    return 1 + (h_l > h_r ? h_l : h_r);
}

static void CMC_(PFX, _impl_rotate_right)(struct CMC_DEF_NODE(SNAME) * *Z)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_NODE(SNAME) *root = *Z;
    struct CMC_DEF_NODE(SNAME) *new_root = root->left;

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

    root->height = CMC_(PFX, _impl_hupdate)(root);
    new_root->height = CMC_(PFX, _impl_hupdate)(new_root);

    *Z = new_root;
}

static void CMC_(PFX, _impl_rotate_left)(struct CMC_DEF_NODE(SNAME) * *Z)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_NODE(SNAME) *root = *Z;
    struct CMC_DEF_NODE(SNAME) *new_root = root->right;

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

    root->height = CMC_(PFX, _impl_hupdate)(root);
    new_root->height = CMC_(PFX, _impl_hupdate)(new_root);

    *Z = new_root;
}

static void CMC_(PFX, _impl_rebalance)(struct SNAME *_set_, struct CMC_DEF_NODE(SNAME) * node)
{
#ifdef CMC_DEV
    CMC_DEV_FCALL;
#endif

    struct CMC_DEF_NODE(SNAME) *scan = node, *child = NULL;

    int balance;
    bool is_root = false;

    while (scan != NULL)
    {
        if (scan->parent == NULL)
            is_root = true;

        scan->height = CMC_(PFX, _impl_hupdate)(scan);
        balance = CMC_(PFX, _impl_h)(scan->right) - CMC_(PFX, _impl_h)(scan->left);

        if (balance >= 2)
        {
            child = scan->right;

            if (CMC_(PFX, _impl_h)(child->right) < CMC_(PFX, _impl_h)(child->left))
                CMC_(PFX, _impl_rotate_right)(&(scan->right));

            CMC_(PFX, _impl_rotate_left)(&scan);
        }
        else if (balance <= -2)
        {
            child = scan->left;

            if (CMC_(PFX, _impl_h)(child->left) < CMC_(PFX, _impl_h)(child->right))
                CMC_(PFX, _impl_rotate_left)(&(scan->left));

            CMC_(PFX, _impl_rotate_right)(&scan);
        }

        if (is_root)
        {
            _set_->root = scan;
            is_root = false;
        }

        scan = scan->parent;
    }
}
