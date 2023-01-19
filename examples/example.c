// The C Macro Collections takes advantage of headers that can be included
// multiple times. You can check the contents of src/cmc/list.h and there are no
// include guards, meaning they can be included multiple times and this is what
// generates the code.
// Before including it though we need to define a few basic macros. You can
// check the documentation to see which ones are required.
#define V int          // list value data type
#define PFX intl       // list function's prefix
#define SNAME int_list // list struct name (struct int_list)
#define CMC_EXT_STR    // Enables an extra functionality (STR) of the list
#include "cmc/list.h"

// This header file includes a lot of functions that can be used as methods to
// basic data types. In this case, cmc_i32_str is used
#include "cmc/utl/futils.h"

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
    // function. Luckily, for the 'int' type, the library already provides such
    // function (cmc_i32_str), provided by the cmc/utl/futils.h header file.
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
            // Use cmc_flags_to_str (global variable) to map the enum error to a
            // string.
            fprintf(stderr, "%s : push_back failed\n", cmc_flags_to_str[flag]);
        }
    }

    // Now we will use the STR module, the _print() function. This is where the
    // '.str' from the function table comes into play. If we haven't defined it,
    // the program would've crashed. We also need to define to which file we
    // will be printing the list's content. In this case, the terminal. Also,
    // some strings need to be defined. They will be printed: before all
    // elements, between each one, and after all elements. This is very useful
    // for debugging.
    intl_print(list, stdout, "[ ", ", ", " ]\n");

    // Free all of its resources
    intl_free(list);
}
