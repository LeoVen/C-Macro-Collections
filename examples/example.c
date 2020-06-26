// This is the master header. This will include every feature of the library.
// This comes with many helper macros (high level API) and utility functions.
#include "macro_collections.h"

// A PARAM is a standard way to pass required parameters to the lower level API.
// It is a tuple of form (PFX, SNAME, SIZE, K, V). In this case, not all of them
// are used, so we can leave them empty. We are creating a list of value int.
#define MY_LIST_PARAMS (intl, int_list, , , int)

// High level API. Generate a LIST from the CMC library with the STR extension
// using our previously defined PARAMs. Every collections has a CORE part that
// needs to be generated first. Then, we can add other parts after. This macro
// does all of that for us. The STR part will provide us with a function that
// will be used later.
C_MACRO_COLLECTIONS_GEN(CMC, LIST, MY_LIST_PARAMS, (STR))

int main(void)
{
    // Our list type is defined by SNAME and all functions are prefixed by PFX
    // (PFX can also be thought as the function's namespace). Also, nodes,
    // entries, iterators and other structures are prefixed by SNAME. So PFX is
    // for functions and SNAME is for structs, where the main one is simply
    // 'struct SNAME'.
    // To initialize a list we need to pass in an initial capacity and something
    // that is called a function table for the V type.
    // This function table is a struct with methods that will extract some
    // (sometimes) necessary behaviour from your custom data type. Things like
    // hash, comparison and printing. A list doesn't require any of these
    // functions. That is, the CORE module doesn't use any of them.
    // But since we are using the STR module, we will need to define the 'str'
    // function. luckily, for the 'int' type, the library already provides such
    // function (cmc_i32_str), provided by the /utl/futils.h header file.
    struct int_list *list = intl_new(32, &(struct int_list_fval){ .str = cmc_i32_str, NULL });

    // Check if the list was successfully initialized. It could fail if the
    // initial capacity is too big or if 'struct int_list_fval *' is NULL,
    // because every data structure must have a valid function table.
    if (!list)
        return 1;

    // Add some items to the list. The CMC data structures are all dynamically
    // sized. So there can be as many items as you want as long as you have
    // enough memory.
    for (int i = 0; i < 100; i++)
    {
        // Try to add an element to the list. If it fails you can check what
        // caused it by getting its flag.
        if (!intl_push_back(list, i))
        {
            enum cmc_flags flag = intl_flag(list);
            // Use cmc_flags_to_str to map the enum error to a string.
            fprintf(stderr, "%s : push_back failed\n", cmc_flags_to_str[flag]);
        }
    }

    // Now we will use the STR module, the _print() function. This is where the
    // '.str' from the function table comes into play. If we haven't defined it,
    // the program would've crashed. We also need to define to which file we
    // will be printing the list's content. In this case, the terminal. Also,
    // some strings need to be defined. They will be printed: before all elements,
    // between each one, and after all elements. This is very usefull and can
    // print a data structure very nicely.
    intl_print(list, stdout, "[ ", ", ", " ]\n");

    // Free all of its resources
    intl_free(list);
}
