#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../src/treeset.h"

TREESET_GENERATE(set, set, , , int)

void print_tree(set_node *root, int height)
{
    if (!root)
        return;

    print_tree(root->right, height + 1);

    for (int i = 0; i < height; i++)
        printf("|------- ");

    printf("%d\n", root->key);

    print_tree(root->left, height + 1);
}

int intcmp(int a, int b)
{
    return a - b;
}

int main(int argc, char const *argv[])
{
    srand((unsigned)time(NULL));
    set *set1 = set_new(intcmp);

    for (int i = 0; i < 31; i++)
    {
        set_insert(set1, i);
    }

    print_tree(set1->root, 0);

    set_free(set1);

    return 0;
}
