/**
 * Copyright (c) 2019 Leonardo Vencovsky
 *
 * This file is part of the C Macro Collections Libray.
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

struct SNAME
{
    /* Array 0 is K -> V and array 1 is V -> K */
    struct CMC_DEF_ENTRY(SNAME) * (*buffer)[2];
    /* Current arrays capacity */
    size_t capacity;
    /* Current amount of keys */
    size_t count;
    /* Load factor in range (0.0, 1.0) */
    double load;
    /* Flags indicating errors or success */
    int flag;
    /* Key function table */
    struct CMC_DEF_FKEY(SNAME) * f_key;
    /* Value function table */
    struct CMC_DEF_FVAL(SNAME) * f_val;
    /* Custom allocation functions */
    struct CMC_ALLOC_NODE_NAME *alloc;
    /* Custom callback functions */
    CMC_CALLBACKS_DECL;
    /* Methods */
    /* Returns an iterator to the start of the hashbidimap */
    struct CMC_DEF_ITER(SNAME) (*it_start)(struct SNAME *);
    /* Returns an iterator to the end of the hashbidimap */
    struct CMC_DEF_ITER(SNAME) (*it_end)(struct SNAME *);
};

/* HashBidiMap Entry */
struct CMC_DEF_ENTRY(SNAME)
{
    /* Entry Key */
    K key;
    /* Entry Value */
    V value;
    /* The distance of this node to its original position */
    /* dist[0] is relative to K -> V */
    /* dist[1] is relative to V -> K */
    size_t dist[2];
    /* References to this node in the hashbidimap buffer. Used to */
    /* prevent searching for this node twice for update() and */
    /* remove(). Increases memory overhead but reduces execution time */
    /* ref[0] is relative to K -> V */
    /* ref[1] is relative to V -> K */
    struct CMC_DEF_ENTRY(SNAME) * *ref[2];
};
