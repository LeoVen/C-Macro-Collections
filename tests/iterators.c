#include "../src/macro_collections.h"
#include <stdio.h>
#include <assert.h>

int intcmp(int a, int b)
{
    return a - b;
}

size_t inthash(int t)
{
    size_t a = t;
    a += ~(a << 15);
    a ^= (a >> 10);
    a += (a << 3);
    a ^= (a >> 6);
    a += ~(a << 11);
    a ^= (a >> 16);
    return a;
}

COLLECTION_GENERATE(LIST, PUBLIC, l, list, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(STACK, PUBLIC, s, stack, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(QUEUE, PUBLIC, q, queue, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(DEQUE, PUBLIC, d, deque, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(LINKEDLIST, PUBLIC, ll, linked, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(HEAP, PUBLIC, h, heap, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(TREESET, PUBLIC, ts, tset, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(TREEMAP, PUBLIC, tm, tmap, /* FMOD */, int, double)
COLLECTION_GENERATE(HASHSET, PUBLIC, hs, hset, /* FMOD */, /* K */, int)
COLLECTION_GENERATE(HASHMAP, PUBLIC, hm, hmap, /* FMOD */, int, double)

void deque_iter_test(void)
{
    size_t i;
    int r, sum = 0;

    // Initialize deque and add some elements
    deque *d = d_new(10);
    for (int i = 0; i < 10; i++)
    {
        i % 2 == 0 ? d_push_back(d, i) : d_push_front(d, i);
    }

    // Initialize iterator
    deque_iter iter;
    d_iter_new(&iter, d);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (d_iter_next(&iter, &r, &i))
            sum += r;
        while (d_iter_prev(&iter, &r, &i))
            sum += r;
    }

    for (d_iter_tostart(&iter); !d_iter_end(&iter); /**/)
    {
        d_iter_next(&iter, &r, &i);
        sum += r;
    }

    for (d_iter_toend(&iter); !d_iter_start(&iter); /**/)
    {
        d_iter_prev(&iter, &r, &i);
        sum += r;
    }

    d_free(d);

    if (sum == 450)
        printf("%10s PASSED\n", "DEQUE");
}

void hashmap_iter_test(void)
{
    size_t i;
    int k, sum = 0;
    double dsum = 0.0, v, varr[10] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    // Initialize hashmap and add some element
    hmap *hm = hm_new(50, 0.9, intcmp, inthash);
    for (int i = 0; i < 10; i++)
        hm_insert(hm, i, varr[i]);

    // Initialize iterator
    hmap_iter iter;
    hm_iter_new(&iter, hm);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (hm_iter_next(&iter, &k, &v, &i))
        {
            sum += k;
            dsum += v;
        }
        while (hm_iter_prev(&iter, &k, &v, &i))
        {
            sum += k;
            dsum += v;
        }
    }

    for (hm_iter_tostart(&iter); !hm_iter_end(&iter); /**/)
    {
        hm_iter_next(&iter, &k, &v, &i);
        sum += k;
        dsum += v;
    }

    for (hm_iter_toend(&iter); !hm_iter_start(&iter); /**/)
    {
        hm_iter_prev(&iter, &k, &v, &i);
        {
            sum += k;
            dsum += v;
        }
    }

    hm_free(hm);

    if (sum == 450 && dsum == 45.0)
        printf("%10s PASSED\n", "HASHMAP");
}

void hashset_iter_test(void)
{
    size_t i;
    int r, sum = 0;

    // Initialize hash set and add some elements
    hset *hs = hs_new(10, 0.9, intcmp, inthash);
    for (int i = 0; i < 10; i++)
        hs_insert(hs, i);

    // Initialize iterator
    hset_iter iter;
    hs_iter_new(&iter, hs);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (hs_iter_next(&iter, &r, &i))
            sum += r;
        while (hs_iter_prev(&iter, &r, &i))
            sum += r;
    }

    for (hs_iter_tostart(&iter); !hs_iter_end(&iter); /**/)
    {
        hs_iter_next(&iter, &r, &i);
        sum += r;
    }

    for (hs_iter_toend(&iter); !hs_iter_start(&iter); /**/)
    {
        hs_iter_prev(&iter, &r, &i);
        sum += r;
    }

    hs_free(hs);

    if (sum == 450)
        printf("%10s PASSED\n", "HASHSET");
}

void heap_iter_test(void)
{
    size_t i;
    int r, sum = 0;

    // Initialize heap and add some elements
    heap *h = h_new(10, MinHeap, intcmp);
    for (int i = 9; i >= 0; i--)
        h_insert(h, i);

    // Initialize iterator
    heap_iter iter;
    h_iter_new(&iter, h);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (h_iter_next(&iter, &r, &i))
            sum += r;
        while (h_iter_prev(&iter, &r, &i))
            sum += r;
    }

    for (h_iter_tostart(&iter); !h_iter_end(&iter); /**/)
    {
        h_iter_next(&iter, &r, &i);
        sum += r;
    }

    for (h_iter_toend(&iter); !h_iter_start(&iter); /**/)
    {
        h_iter_prev(&iter, &r, &i);
        sum += r;
    }

    h_free(h);

    if (sum == 450)
        printf("%10s PASSED\n", "HEAP");
}

void linkedlist_iter_test(void)
{
    size_t i;
    int r, sum = 0;

    // Initialize linked list and add some elements
    linked *ll = ll_new();
    for (int i = 0; i < 10; i++)
        ll_push_back(ll, i);

    // Initialize iterator
    linked_iter iter;
    ll_iter_new(&iter, ll);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (ll_iter_next(&iter, &r, &i))
            sum += r;
        while (ll_iter_prev(&iter, &r, &i))
            sum += r;
    }

    for (ll_iter_tostart(&iter); !ll_iter_end(&iter); /**/)
    {
        ll_iter_next(&iter, &r, &i);
        sum += r;
    }

    for (ll_iter_toend(&iter); !ll_iter_start(&iter); /**/)
    {
        ll_iter_prev(&iter, &r, &i);
        sum += r;
    }

    ll_free(ll);

    if (sum == 450)
        printf("%10s PASSED\n", "LINKEDLIST");
}

void list_iter_test(void)
{
    size_t i;
    int r, sum = 0;

    // Initialize list and add some elements
    list *l = l_new(10);
    for (int i = 0; i < 10; i++)
        l_push_back(l, i);

    // Initialize iterator
    list_iter iter;
    l_iter_new(&iter, l);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (l_iter_next(&iter, &r, &i))
            sum += r;
        while (l_iter_prev(&iter, &r, &i))
            sum += r;
    }

    for (l_iter_tostart(&iter); !l_iter_end(&iter); /**/)
    {
        l_iter_next(&iter, &r, &i);
        sum += r;
    }

    for (l_iter_toend(&iter); !l_iter_start(&iter); /**/)
    {
        l_iter_prev(&iter, &r, &i);
        sum += r;
    }

    l_free(l);

    if (sum == 450)
        printf("%10s PASSED\n", "LIST");
}

void queue_iter_test(void)
{
    size_t i;
    int r, sum = 0;

    // Initialize queue and add some elements
    queue *q = q_new(10);
    for (int i = 0; i < 10; i++)
        q_enqueue(q, i);

    // Initialize iterator
    queue_iter iter;
    q_iter_new(&iter, q);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (q_iter_next(&iter, &r, &i))
            sum += r;
        while (q_iter_prev(&iter, &r, &i))
            sum += r;
    }

    for (q_iter_tostart(&iter); !q_iter_end(&iter); /**/)
    {
        q_iter_next(&iter, &r, &i);
        sum += r;
    }

    for (q_iter_toend(&iter); !q_iter_start(&iter); /**/)
    {
        q_iter_prev(&iter, &r, &i);
        sum += r;
    }

    q_free(q);

    if (sum == 450)
        printf("%10s PASSED\n", "QUEUE");
}

void stack_iter_test(void)
{
    size_t i;
    int r, sum = 0;

    // Initialize stack and add some elements
    stack *s = s_new(10);
    for (int i = 0; i < 10; i++)
        s_push(s, i);

    // Initialize iterator
    stack_iter iter;
    s_iter_new(&iter, s);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (s_iter_next(&iter, &r, &i))
            sum += r;
        while (s_iter_prev(&iter, &r, &i))
            sum += r;
    }

    for (s_iter_tostart(&iter); !s_iter_end(&iter); /**/)
    {
        s_iter_next(&iter, &r, &i);
        sum += r;
    }

    for (s_iter_toend(&iter); !s_iter_start(&iter); /**/)
    {
        s_iter_prev(&iter, &r, &i);
        sum += r;
    }

    s_free(s);

    if (sum == 450)
        printf("%10s PASSED\n", "STACK");
}

void treemap_iter_test(void)
{
    size_t i;
    int k, sum = 0;
    double dsum = 0, v, varr[10] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};

    // Initialize treemap and add some element
    tmap *tm = tm_new(intcmp);
    for (int i = 0; i < 10; i++)
        tm_insert(tm, i, varr[i]);

    // Initialize iterator
    tmap_iter iter;
    tm_iter_new(&iter, tm);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (tm_iter_next(&iter, &k, &v, &i))
        {
            sum += k;
            dsum += v;
        }
        while (tm_iter_prev(&iter, &k, &v, &i))
        {
            sum += k;
            dsum += v;
        }
    }

    for (tm_iter_tostart(&iter); !tm_iter_end(&iter); /**/)
    {
        tm_iter_next(&iter, &k, &v, &i);
        sum += k;
        dsum += v;
    }

    for (tm_iter_toend(&iter); !tm_iter_start(&iter); /**/)
    {
        tm_iter_prev(&iter, &k, &v, &i);
        sum += k;
        dsum += v;
    }

    tm_free(tm);

    if (sum == 450 && dsum == 45.0)
        printf("%10s PASSED\n", "TREEMAP");
}

void treeset_iter_test(void)
{
    size_t i;
    int r, sum = 0;

    // Initialize treeset and add some elements
    tset *ts = ts_new(intcmp);
    for (int i = 0; i < 10; i++)
        ts_insert(ts, i);

    // Initialize iterator
    tset_iter iter;
    ts_iter_new(&iter, ts);

    // Make the iterator go back and forward
    for (int j = 0; j < 4; j++)
    {
        while (ts_iter_next(&iter, &r, &i))
            sum += r;
        while (ts_iter_prev(&iter, &r, &i))
            sum += r;
    }

    for (ts_iter_tostart(&iter); !ts_iter_end(&iter); /**/)
    {
        ts_iter_next(&iter, &r, &i);
        sum += r;
    }

    for (ts_iter_toend(&iter); !ts_iter_start(&iter); /**/)
    {
        ts_iter_prev(&iter, &r, &i);
        sum += r;
    }

    ts_free(ts);

    if (sum == 450)
        printf("%10s PASSED\n", "TREESET");
}

int main(void)
{
    printf("\n-------------------- ITERATORS --------------------\n");
    deque_iter_test();
    hashmap_iter_test();
    hashset_iter_test();
    heap_iter_test();
    linkedlist_iter_test();
    list_iter_test();
    queue_iter_test();
    stack_iter_test();
    treemap_iter_test();
    treeset_iter_test();

    return 0;
}
