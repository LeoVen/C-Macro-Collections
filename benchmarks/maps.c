#include <macro_collections.h>

/**
 * HashMap, MultiMap and BidiMap use hashtables with different strategies. This
 * benchmark is aimed to explore the pros and cons of each implementation.
 *
 * HashMap - A plain hashtable with no indirection using linear probing and
 *           robin hood hashing.
 *      Expectations:
 *          - Constant memory usage which leads to high memory usage per element
 *            ratio when the hashtable has few elements but the ratio is
 *            somewhat good as it starts to fill up;
 *          - Faster lookup due to no indirection and good caching;
 *          - Faster insertion and removal due to no calls to free or malloc.
 *
 * MultiMap - A hashtable using separate chaining where each bucket is a doubly
 *           linked list.
 *      Expectations:
 *          - High memory usage per element ratio due to the double linked
 *            lists;
 *          - Pointer indirection might lead to slower lookup and poor caching
 *            due to using nodes to store the key and the value;
 *          - A bit slower insertion and removal as calls to free and malloc are
 *            needed.
 *
 *  BidiMap - Two arrays, one representing the key set and one the value set.
 *           Each array is made of pointers to an entry. This way one entry can
 *           be referenced by both the key set and the value set using only one
 *           node. Uses robin hood hashing to adjust the pointers.
 *      Expectations:
 *          - Overrall low memory usage compared to other implementations;
 *          - Pointer indirection might lead to slower lookupand poor caching
 *            due to using nodes to store the key and the value;
 *          - Very slow insertion and removal as a node needs to be added or
 *            removed on both arrays.
 */

/* Generate Code */
CMC_COLLECTION_GENERATE(HASHMAP, hmap, hmap, int, int)
CMC_COLLECTION_GENERATE(MULTIMAP, mmap, mmap, int, int)
CMC_COLLECTION_GENERATE(BIDIMAP, bmap, bmap, int, int)

/* Forward declaration of benchmarks */
void benchmark_bidimap(const char *collection, int total_elements,
                       double load_factor);
void benchmark_hashmap(const char *collection, int total_elements,
                       double load_factor);
void benchmark_multimap(const char *collection, int total_elements,
                        double load_factor);

/* Comparator and Hash function */
int cmp(int a, int b)
{
    return a - b;
}

size_t hash(int a)
{
    size_t x = (size_t)a;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

/* Custom allocators to count memory usage */
size_t total_memory = 0;

/* Report Functions */
void print_overview(const char *collection, int total_elements,
                    double load_factor)
{
    printf("Overview\n"
           "  Collection     : %s\n"
           "  Total Elements : %d\n"
           "  Load Factor    : %.4lf\n\n",
           collection, total_elements, load_factor);
}

void print_time(const char *collection, double C, double R, double U, double D)
{
    /* C - Create, R - Read, U - Update, D - Delete */
    printf("Time [%s]:                     \n"
           "    Total  : %8.0lf milliseconds   \n"
           "    Create : %8.0lf milliseconds\n"
           "    Read   : %8.0lf milliseconds\n"
           "    Update : %8.0lf milliseconds\n"
           "    Delete : %8.0lf milliseconds\n\n",
           collection, C + R + U + D, C, R, U, D);
}

void print_memory(const char *s, size_t memory[11], int partition_size)
{
    printf("Each Partition : %d elements\n\n", partition_size);

    for (size_t i = 0; i < 11; i++)
    {
        printf("  %3" PRIuMAX "%% : %12" PRIuMAX " B | %10" PRIuMAX " kB | %8" PRIuMAX " MB (+ %2.0lf%%)\n",
        i * 10, memory[i], memory[i] / 1000, memory[i] / 1000000, (1 - ((double)memory[0] / (double)memory[i])) * 100.0);
    }
}

/* Custom allocation functions */
void *my_malloc(size_t size)
{
    void *result = malloc(size);

    if (result)
        total_memory += size;

    return result;
}

void *my_calloc(size_t count, size_t size)
{
    void *result = calloc(count, size);

    if (result)
        total_memory += count * size;

    return result;
}

void *my_realloc(void *buffer, size_t size)
{
    /* This assumes that this benchmark never uses realloc */
    return realloc(buffer, size);
}

void my_free(void *block)
{
    free(block);
}

/* Default custom allocation node */
struct cmc_alloc_node alloc_default = { .malloc = my_malloc,
                                        .calloc = my_calloc,
                                        .realloc = my_realloc,
                                        .free = my_free };

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        fprintf(
            stderr,
            "Usage:\n"
            "    %s collection_name total_elements load_factor\n"
            "\nOPTIONS\n"
            "    - collection_name : (BIDIMAP|HASHMAP|MULTIMAP|ALL)\n"
            "    - total_elements  : Total elements to be added to the hashtable (int > 0)\n"
            "    - load_factor     : The hashtable's load factor (0 < l < 1.0)\n",
            argv[0]);
        return 1;
    }

    int total_elements = atoi(argv[2]);
    double load_factor = atof(argv[3]);

    bool error = false;

    if (total_elements < 1)
    {
        error = true;
        fprintf(stderr, "Invalid value for total_elements: %s\n", argv[2]);
    }

    if (load_factor <= 0.0 || load_factor >= 1.0)
    {
        error = true;
        fprintf(stderr, "Invalid value for load_factor: %s\n", argv[3]);
    }

    if (error)
        return 1;

    const char *collection = argv[1];

    if (strcmp(collection, "BIDIMAP") == 0)
    {
        benchmark_bidimap(collection, total_elements, load_factor);
    }
    else if (strcmp(collection, "HASHMAP") == 0)
    {
        benchmark_hashmap(collection, total_elements, load_factor);
    }
    else if (strcmp(collection, "MULTIMAP") == 0)
    {
        benchmark_multimap(collection, total_elements, load_factor);
    }
    else if (strcmp(collection, "ALL") == 0)
    {
        benchmark_bidimap("BIDIMAP", total_elements, load_factor);
        benchmark_hashmap("HASHMAP", total_elements, load_factor);
        benchmark_multimap("MULTIMAP", total_elements, load_factor);
    }
    else
    {
        fprintf(stderr, "Unknown parameter collection_name = %s\n", collection);
        return 1;
    }

    return 0;
}

void benchmark_bidimap(const char *collection, int total_elements,
                       double load_factor)
{
    printf("--------------------------------------------------------------------------------\n");

    total_memory = 0;

    /* Normalize total_elements to be a multiple of 10 and calculate partition size */
    total_elements = total_elements % 10 == 0 ? total_elements : total_elements + (10 - total_elements % 10);
    int partition = total_elements / 10;

    print_overview(collection, total_elements, load_factor);

    struct bmap *map = bmap_new_custom(total_elements, load_factor, cmp, hash,
                                       cmp, hash, &alloc_default, NULL);

    /* Create, Read, Update, Delete */
    struct cmc_timer timers[4] = { 0 };

    /* Memory Usage 0%, 10%, 20%, ... 100% */
    size_t memory[11] = {0};

    memory[0] = total_memory;

    /* Create */
    cmc_timer_start(timers[0]);
    for (int i = 0; i < 10; i++)
    {
        for (int j = partition * i; j < partition * (i + 1); j++)
        {
            bmap_insert(map, j, j);
        }

        memory[i + 1] = total_memory;
    }

    cmc_timer_stop(timers[0]);
    cmc_timer_calc(timers[0]);
    /* Create End */

    /* Read */
    cmc_timer_start(timers[1]);
    for (int i = 0; i < total_elements; i++)
        bmap_contains_key(map, i);

    cmc_timer_stop(timers[1]);
    cmc_timer_calc(timers[1]);
    /* Read End */

    /* Update */
    cmc_timer_start(timers[2]);
    for (int i = 0; i < total_elements; i++)
        bmap_update_val_by_key(map, i, total_elements - i - 1);

    cmc_timer_stop(timers[2]);
    cmc_timer_calc(timers[2]);
    /* Update End */

    /* Delete */
    cmc_timer_start(timers[3]);
    for (int i = 0; i < total_elements; i++)
        bmap_remove_by_key(map, i, NULL, NULL);

    cmc_timer_stop(timers[3]);
    cmc_timer_calc(timers[3]);
    /* Delete End */

    print_time(collection, timers[0].result, timers[1].result, timers[2].result,
               timers[3].result);

    print_memory(collection, memory, partition);

    bmap_free(map, NULL);

    printf("--------------------------------------------------------------------------------\n");
}

void benchmark_hashmap(const char *collection, int total_elements,
                       double load_factor)
{
    printf("--------------------------------------------------------------------------------\n");

    total_memory = 0;

    /* Normalize total_elements to be a multiple of 10 and calculate partition size */
    total_elements = total_elements % 10 == 0 ? total_elements : total_elements + (10 - total_elements % 10);
    int partition = total_elements / 10;

    print_overview(collection, total_elements, load_factor);

    struct hmap *map = hmap_new_custom(total_elements, load_factor, cmp, hash, &alloc_default, NULL);

    /* Create, Read, Update, Delete */
    struct cmc_timer timers[4] = { 0 };

    /* Memory Usage 0%, 10%, 20%, ... 100% */
    size_t memory[11] = {0};

    memory[0] = total_memory;

    /* Create */
    cmc_timer_start(timers[0]);
    for (int i = 0; i < 10; i++)
    {
        for (int j = partition * i; j < partition * (i + 1); j++)
        {
            hmap_insert(map, j, j);
        }

        memory[i + 1] = total_memory;
    }

    cmc_timer_stop(timers[0]);
    cmc_timer_calc(timers[0]);
    /* Create End */

    /* Read */
    cmc_timer_start(timers[1]);
    for (int i = 0; i < total_elements; i++)
        hmap_contains(map, i);

    cmc_timer_stop(timers[1]);
    cmc_timer_calc(timers[1]);
    /* Read End */

    /* Update */
    cmc_timer_start(timers[2]);
    for (int i = 0; i < total_elements; i++)
        hmap_update(map, i, total_elements - i - 1, NULL);

    cmc_timer_stop(timers[2]);
    cmc_timer_calc(timers[2]);
    /* Update End */

    /* Delete */
    cmc_timer_start(timers[3]);
    for (int i = 0; i < total_elements; i++)
        hmap_remove(map, i, NULL);

    cmc_timer_stop(timers[3]);
    cmc_timer_calc(timers[3]);
    /* Delete End */

    print_time(collection, timers[0].result, timers[1].result, timers[2].result,
               timers[3].result);

    print_memory(collection, memory, partition);

    hmap_free(map, NULL);

    printf("--------------------------------------------------------------------------------\n");
}

void benchmark_multimap(const char *collection, int total_elements,
                        double load_factor)
{
    printf("--------------------------------------------------------------------------------\n");

    total_memory = 0;

    /* Normalize total_elements to be a multiple of 10 and calculate partition size */
    total_elements = total_elements % 10 == 0 ? total_elements : total_elements + (10 - total_elements % 10);
    int partition = total_elements / 10;

    print_overview(collection, total_elements, load_factor);

    struct mmap *map = mmap_new_custom(total_elements, load_factor, cmp, hash, &alloc_default, NULL);

    /* Create, Read, Update, Delete */
    struct cmc_timer timers[4] = { 0 };

    /* Memory Usage 0%, 10%, 20%, ... 100% */
    size_t memory[11] = {0};

    memory[0] = total_memory;

    /* Create */
    cmc_timer_start(timers[0]);
    for (int i = 0; i < 10; i++)
    {
        for (int j = partition * i; j < partition * (i + 1); j++)
        {
            mmap_insert(map, j, j);
        }

        memory[i + 1] = total_memory;
    }

    cmc_timer_stop(timers[0]);
    cmc_timer_calc(timers[0]);
    /* Create End */

    /* Read */
    cmc_timer_start(timers[1]);
    for (int i = 0; i < total_elements; i++)
        mmap_contains(map, i);

    cmc_timer_stop(timers[1]);
    cmc_timer_calc(timers[1]);
    /* Read End */

    /* Update */
    cmc_timer_start(timers[2]);
    for (int i = 0; i < total_elements; i++)
        mmap_update(map, i, total_elements - i - 1, NULL);

    cmc_timer_stop(timers[2]);
    cmc_timer_calc(timers[2]);
    /* Update End */

    /* Delete */
    cmc_timer_start(timers[3]);
    for (int i = 0; i < total_elements; i++)
        mmap_remove(map, i, NULL);

    cmc_timer_stop(timers[3]);
    cmc_timer_calc(timers[3]);
    /* Delete End */

    print_time(collection, timers[0].result, timers[1].result, timers[2].result,
               timers[3].result);

    print_memory(collection, memory, partition);

    mmap_free(map, NULL);

    printf("--------------------------------------------------------------------------------\n");
}
