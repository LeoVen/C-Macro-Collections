/**
 * Example of In-Order Depth-First Search using:
 *  - A Stack
 *  - Recursion
 *  - Without both
 */

#define CMC_CAMEL_CASE

#include "macro_collections.h"

C_MACRO_COLLECTIONS_GEN(CMC, TREESET, (tree, BinaryTree, , , int32_t), (ITER, STR))

#define MYSTACK (stk, Stack, , , struct BinaryTreeNode *)

C_MACRO_COLLECTIONS(CMC, STACK, CORE, STRUCT, MYSTACK)
C_MACRO_COLLECTIONS(CMC, STACK, CORE, HEADER, MYSTACK)
C_MACRO_COLLECTIONS(CMC, STACK, CORE, SOURCE, MYSTACK)

void DFS(struct BinaryTreeNode *root)
{
    if (!root)
        return;

    DFS(root->left);

    fprintf(stdout, "%d ", root->value);

    DFS(root->right);
}

int main(void)
{
    struct BinaryTree *tree = tree_new(&(struct BinaryTreeFVal){ .cmp = cmc_i32_cmp, .str = cmc_i32_str, NULL });

    for (int i = 1; i <= 10; i++)
        tree_insert(tree, i);

    int32_t result = 1;

    cmc_assert(tree_min(tree, &result));
    cmc_assert_equals(int32_t, 1, result);

    cmc_assert(tree_max(tree, &result));
    cmc_assert_equals(int32_t, 10, result);

    printf("Without Recursion or Stack:\n");
    tree_print(tree, stdout, "[ ", " ", " ]\n");

    printf("Recursion:\n");
    fprintf(stdout, "[ ");
    DFS(tree->root);
    fprintf(stdout, "]\n");

    printf("Stack:\n");
    fprintf(stdout, "[ ");
    struct Stack *stack = stk_new(100, &(struct StackFVal){ NULL });

    struct BinaryTreeNode *node = tree->root;

    while (node || !stk_empty(stack))
    {
        if (node)
        {
            if (!stk_push(stack, node))
                cmc_log_error("Could not push to stack %d\n", cmc_flags_to_str[stk_flag(stack)]);

            node = node->left;
        }
        else
        {
            node = stk_top(stack);

            if (!stk_pop(stack))
                cmc_log_error("Could not pop from the stack %s\n", cmc_flags_to_str[stk_flag(stack)]);

            fprintf(stdout, "%d ", node->value);

            node = node->right;
        }
    }

    stk_free(stack);
    fprintf(stdout, "]\n");

    tree_free(tree);
}
