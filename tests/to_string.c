// A visual check for to_string methods

#include "macro_collections.h"

// clang-format off
C_MACRO_COLLECTIONS_ALL(CMC,        DEQUE, (  d,        deque, ,    , int))
C_MACRO_COLLECTIONS_ALL(CMC,      HASHMAP, ( hm,      hashmap, , int, int))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTIMAP, (hmm, hashmultimap, , int, int))
C_MACRO_COLLECTIONS_ALL(CMC, HASHMULTISET, (hms, hashmultiset, ,    , int))

struct
{
    CMC_DEF_FTAB_CMP(int);
    CMC_DEF_FTAB_CPY(int);
    CMC_DEF_FTAB_STR(int);
    CMC_DEF_FTAB_FREE(int);
    CMC_DEF_FTAB_HASH(int);
    CMC_DEF_FTAB_PRI(int);
} int_ftab = {cmc_i32_cmp, NULL, cmc_i32_str, NULL, cmc_i32_hash, cmc_i32_cmp};

int main(void)
{
    struct deque *d = d_new(100, (struct deque_fval *)&int_ftab);
    struct hashmap *hm = hm_new(100, 0.6, (struct hashmap_fkey *)&int_ftab,
                                          (struct hashmap_fval *)&int_ftab);
    struct hashmultimap *hmm = hmm_new(100, 0.8,
                                       (struct hashmultimap_fkey *)&int_ftab,
                                       (struct hashmultimap_fval *)&int_ftab);
    struct hashmultiset *hms = hms_new(100, 0.6, (struct hashmultiset_fval *)&int_ftab);

    d_to_string(d, stdout);
    d_print(d, stdout, "\n[ ", ", ", " ]\n");
    hm_to_string(hm, stdout);
    hm_print(hm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hmm_to_string(hmm, stdout);
    hmm_print(hmm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hms_to_string(hms, stdout);
    hms_print(hms, stdout, "\n[ {", "}, {", "} ]\n", " : ");

    for (int i = 0; i < 20; i++)
    {
        d_push_back(d, i);
        hm_insert(hm, i, i);
        hmm_insert(hmm, i, i);
        hms_insert(hms, i);
    }

    fprintf(stdout, "\n");

    d_to_string(d, stdout);
    d_print(d, stdout, "\n[ ", ", ", " ]\n");
    hm_to_string(hm, stdout);
    hm_print(hm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hmm_to_string(hmm, stdout);
    hmm_print(hmm, stdout, "\n[ {", "}, {", "} ]\n", " : ");
    hms_to_string(hms, stdout);
    hms_print(hms, stdout, "\n[ {", "}, {", "} ]\n", " : ");

    d_free(d);
    hm_free(hm);
    hmm_free(hmm);
    hms_free(hms);
}

// clang-format on
