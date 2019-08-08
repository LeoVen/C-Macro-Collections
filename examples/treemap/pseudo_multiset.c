/**
 * pseudo_multiset.c
 *
 * Creation Date: 26/04/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

// How to make a multiset using a map

#include "cmc/treemap.h"
#include <stdio.h>
#include <inttypes.h>

// Mapping an integer by size_t which will represent the key's multiplicity
// (key could be anything and multiplicity could be an integer, preferably
// unsigned)
TREEMAP_GENERATE(tm, tmap, , int, size_t)

// Compare function used by the treemap
int intcmp(int a, int b)
{
    return (a > b) - (a < b);
}

// Little helper to print the tree
void print_tree(tmap_node *root, int height)
{
    if (!root)
        return;

    print_tree(root->right, height + 1);

    for (int i = 0; i < height; i++)
        printf("|------- ");

    // Key:Multiplicity
    printf("%d:%" PRIuMAX "\n", root->key, root->value);

    print_tree(root->left, height + 1);
}

int main(int argc, char const *argv[])
{
    tmap *map = tm_new(intcmp);

    // Start adding elements
    for (int i = 0; i < 100; i++)
    {
        // Modulo of 8 so we get some repetition
        int key = i % 8;

        // Get the reference to a value in the map with the key
        size_t *rvalue = tm_get_ref(map, key);

        // If the key exists, rvalue is not null
        if (rvalue != NULL)
        {
            // Incrementing it means we have added another key to the set
            // The value represents the multiplicity, that is, how many times
            // we have added the key in the set
            (*rvalue)++;
        }
        else
            // Create a new entry with key with an initial multiplicity of 1
            tm_insert(map, key, 1);
    }

    print_tree(map->root, 0);

    // Summary
    printf("\n---------- SUMMARY ----------\n\n");

    // Print using iterator
    for (tmap_iter it = map->it_start(map); !tm_iter_end(&it); tm_iter_next(&it))
    {
        // Map[key] = multiplicity
        printf("Map[%2d] = %" PRIuMAX "\n", tm_iter_key(&it), tm_iter_value(&it));
    }

    tm_free(map);

    return 0;
}
