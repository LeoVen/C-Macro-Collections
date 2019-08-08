/**
 * counter.c
 *
 * Creation Date: 05/07/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * A multi-threading word counter in C using the C Macro Collections library
 * and the pthread library.
 */

#include <cmc/hashmap.h>
#include <cmc/heap.h>
#include <cmc/list.h>
#include <ctype.h>
#include <inttypes.h>
#include <pthread.h>
#include <sac/stack.h>
#include <stdio.h>
#define CMC_LOG_COLOR
#include <utl/log.h>

// Maximum number of threads
#define n_threads 100

/* HashMap mutex */
pthread_mutex_t table_mutex = PTHREAD_MUTEX_INITIALIZER;

/* printf, fprintf mutex */
pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

/* A pair of word and number of how many times that word has appeared */
/* This will be used by Heap */
typedef struct wc_pair_s
{
    char *word;
    size_t count;
} wc_pair;

/* A stack allocated stack to keep track of each thread id */
SAC_STACK_GENERATE(tl, TList, , pthread_t, n_threads)

/* A list used as a dynamic string */
LIST_GENERATE(str, String, , char)

/* A mapping between word and word count */
HASHMAP_GENERATE(hm, HashMap, , char *, size_t)

/* Used by HashMap to hash its keys (char *) */
size_t djb2(char *str)
{
    size_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/* Used by HashMap to compare its keys (char *) */
int chrcmp(char *a, char *b)
{
    return strcmp(a, b);
}

/* Heap used to sort the { word : word_count } pairs */
HEAP_GENERATE(hp, Heap, , wc_pair)

/* Used by Heap to sort wc_pair */
int cmp_wcpair(wc_pair p1, wc_pair p2)
{
    /* Sort by count, then by word */
    int cmp =  (p1.count > p2.count) - (p1.count < p2.count);

    if (cmp == 0)
        /* I swapped p1 and p2 so that in the output the result is alphabetically ordered */
        return chrcmp(p2.word, p1.word);

    return cmp;
}

/* Argument type passed to void *counter_routine(void *data) */
typedef struct cr_arg_s
{
    HashMap *table;
    const char *file_name;
} cr_arg;

/* Word Counter */
void *counter_routine(void *data)
{
    /* Acquire data */
    cr_arg *thread_arg = data;
    HashMap *table = thread_arg->table;
    const char *file_name = thread_arg->file_name;

    FILE *file = fopen(file_name, "r");

    if (!file)
    {
        pthread_mutex_lock(&print_mutex);

        fprintf(stderr, "Failed to open file %s\n", file_name);

        pthread_mutex_unlock(&print_mutex);
        return NULL;
    }

    int ch = 0;
    String *word = str_new(100);

    /* Begin reading file */
    /* Whenever a word is found, add it to the HashMap */
    while ((ch = fgetc(file)) != EOF)
    {
        /* Normalize */
        ch = tolower(ch);

        /* Stop when is a space character or punctuation */
        /* Only count alphabetical letter */
        if ( ( isspace(ch) || ispunct(ch) ) && !str_empty(word) )
        {
            str_push_back(word, 0);

            char *key = strdup(word->buffer);

            pthread_mutex_lock(&table_mutex);

            size_t *num = hm_get_ref(table, key);

            if (num == NULL)
                hm_insert(table, key, 1);
            else
            {
                *num += 1;
                free(key);
                key = NULL;
            }

            pthread_mutex_unlock(&table_mutex);

            str_clear(word);
        }
        else if (isalpha(ch))
        {
            str_push_back(word, ch);
        }
    }

    str_free(word);
    fclose(file);

    return NULL;
}

int main(int argc, char const *argv[])
{
    /* Check arguments*/
    if (argc > n_threads)
    {
        fprintf(stderr, "Sorry but no more than %d arguments\n", n_threads);
        return 2;
    }
    else if (argc == 1)
    {
        fprintf(stderr, "Usage:\n %s path/to/file.txt path/to/another/file.txt\n", argv[0]);
        return 1;
    }

    cr_arg *thread_args = calloc(n_threads, sizeof(cr_arg));

    /* Threads id storage */
    TList ts = tl_new();

    /* Resulting word - word_count mapping */
    HashMap *result = hm_new(100000, 0.6, chrcmp, djb2);

    /* Create one thread for each file */
    for (int i = 1; i < argc; i++)
    {
        thread_args[i - 1].table = result;
        thread_args[i - 1].file_name = argv[i];

        pthread_t T;
        if (pthread_create(&T, NULL, counter_routine, &thread_args[i - 1]) != 0)
        {
            pthread_mutex_lock(&print_mutex);

            log_error("Could not create thread for %s\n", argv[i]);

            pthread_mutex_unlock(&print_mutex);
        }

        if (!tl_push(&ts, T))
        {
            pthread_mutex_lock(&print_mutex);

            log_error("Could not push to stack %lu", T);

            pthread_mutex_unlock(&print_mutex);
        }
    }

    /* Wait for all threads to finish */
    for (TList_iter iter = ts.it_start(&ts); !tl_iter_end(&iter); tl_iter_next(&iter))
    {
        pthread_t T = tl_iter_value(&iter);

        if (pthread_join(T, NULL) != 0)
        {
            log_error("Failed to join thread %lu\n", T);
        }
    }

    /* Sorting using a Heap */
    Heap *sort = hp_new(hm_count(result), cmc_max_heap, cmp_wcpair);

    /* Add key-value pairs to Heap for sorting */
    for (HashMap_iter iter = result->it_start(result);
        !hm_iter_end(&iter);
        hm_iter_next(&iter))
    {
        wc_pair wc = {hm_iter_key(&iter), hm_iter_value(&iter)};

        hp_insert(sort, wc);
    }

    /* Open file for output */
    FILE *out = fopen("out.txt", "w");

    if (!out)
    {
        fprintf(stderr, "Failed to create file %s\n", "out.txt");
    }
    else
    {
        fprintf(out, "A total of %" PRIuMAX " unique words were found\n\n", hp_count(sort));

        /* Until heap is exausted, print the pair into the file */
        while (!hp_empty(sort))
        {
            wc_pair wc;

            if (!hp_remove(sort, &wc))
            {
                fprintf(stderr, "Failed to remove element from heap");
            }
            else
            {
                fprintf(out, "%s, %" PRIuMAX "\n", wc.word, wc.count);
                free(wc.word);
            }
        }

        fclose(out);
    }

    hp_free(sort);
    hm_free(result);
    free(thread_args);

    return 0;
}

