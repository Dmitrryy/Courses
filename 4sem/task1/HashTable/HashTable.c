
#include "HashTable.h"

#include <assert.h>
#include <inttypes.h>
#include <sys/types.h>
#include <limits.h>

#include "../ForvardList/forvard_list.h"


static uint HashZero(char* key, size_t size, uint max) { return 0; }
static uint HashDefault(char* key, size_t size, uint max) { return 0; }//todo


struct Hash_t
{
    uint       (* m_pHash )(char* key, size_t size, uint max);

    forward_list* m_arrLists;
    size_t        m_size;
};


Hash hsCreate (size_t size)
{
    assert(size > 0);
    Hash result = (Hash)malloc(sizeof(struct Hash_t));

    result->m_size = size;
    result->m_pHash = &HashDefault;

    result->m_arrLists = (forward_list*)calloc(size, sizeof(forward_list));
    for (size_t k = 0; k < size; ++k)
    {
        result->m_arrLists[k] = flCreate();
    }

    return result;
}

void hsDestroy(Hash ht)
{
    assert(ht != NULL);
    for (size_t k = 0; k < ht->m_size; ++k)
    {
        flDestroy(ht->m_arrLists[k]);
    }
    free(ht->m_arrLists);
    free(ht);
}


void hsInsert    (Hash ht, hs_key_type key, hs_val_type val)
{
    assert(ht != NULL);

    size_t hashKey = ht->m_pHash(&key, sizeof(hs_key_type), ht->m_size - 1) % ht->m_size;

}


size_t hsSize(Hash ht)
{
    assert(ht != NULL);
    return ht->m_size;
}



void hsSetHashFunc       (Hash ht, uint(*hashCompute)(void* key, size_t size, uint max))
{
    assert(ht != NULL);
    assert(hashCompute != NULL);

    ht->m_pHash = hashCompute;
}
void hsSetDefaultHashFunc(Hash ht)
{
    assert(ht != NULL);
    ht->m_pHash = &HashDefault;
}

///////////////////////////////////////////////////////

static size_t findNextNotEmpty(forward_list* lists, size_t size, size_t curListId)
{
    assert(lists != NULL);
    assert(curListId < size);

    size_t k = 0;
    for (k = curListId; k < size; ++k)
    {
        if (!flEmpty(lists[k]))
        {
            break;
        }
    }
    return k;
}


///////////////////////////////////////////////////////
//iterator

struct htIt_t
{
    forward_list* m_lists;
    size_t        m_nLists;
    size_t        m_curList;
    flIt          m_listIt;
};

hsIt hsBegin(Hash ht)
{
    assert(ht != NULL);
    hsIt result = (hsIt)malloc(sizeof(struct htIt_t));
    result->m_lists = ht->m_arrLists;
    result->m_nLists = ht->m_size;

    result->m_curList = findNextNotEmpty(ht->m_arrLists, ht->m_size, 0);
    if (result->m_curList != result->m_nLists)
    {
        result->m_listIt = flBegin(result->m_lists[result->m_curList]);
    }
    else {
        result->m_listIt = NULL;
    }

    return result;
}

int hsItIsEnd  (hsIt it)
{
    if (it->m_curList == it->m_nLists) {
        return 1;
    }
    return 0;
}

void hsItNext(hsIt it)
{
    if (hsItIsEnd(it)) {
        return ;
    }
    flItNext(it->m_listIt);
    if(flItIsEnd(it->m_listIt))
    {
        flItDestroy(it->m_listIt);

        it->m_curList = findNextNotEmpty(it->m_lists, it->m_nLists, it->m_curList + 1);
        if (it->m_curList < it->m_nLists) {
            it->m_listIt = flBegin(it->m_lists[it->m_curList]);
        }
        else
        {
            it->m_listIt = NULL;
        }
    }
}

hs_key_type hsItKey(hsIt it); //todo
hs_val_type hsItVal(hsIt it); //todo

void hsItDestroy(hsIt it);