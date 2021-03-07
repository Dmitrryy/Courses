
#include "forvard_list.h"
#include <assert.h>

///////////////////////////////////////////////////////
//list node

struct flNode_t
{
    fl_key_type m_key;
    fl_val_type m_data;

    struct flNode_t* m_pNext;
};
typedef struct flNode_t* flNode;

static flNode flCreateNode(fl_key_type key, fl_val_type val)
{
    flNode result = (flNode)malloc(sizeof(struct flNode_t));
    result->m_data = val;
    result->m_key = key;
    result->m_pNext = NULL;
    return result;
}
static void flDestroyNode(flNode node)
{
    free(node);
}

///////////////////////////////////////////////////////
//list

struct forward_list_t
{
    flNode m_pFront;
};



forward_list flCreate()
{
    forward_list result = (forward_list)malloc(sizeof(forward_list));
    assert(result != NULL);
    return result;
}
void flDestroy(forward_list fl)
{
    free(fl);
}


fl_val_type flFront(forward_list fl)
{
    assert(fl != NULL);
    return fl->m_pFront->m_data;
}

int flEmpty(forward_list fl)
{
    assert(fl != NULL);
    return fl->m_pFront == NULL;
}

void flPushFront(forward_list fl, fl_key_type key, fl_val_type val)
{
    assert(fl != NULL);
    flNode node = flCreateNode(key, val);
    node->m_pNext = fl->m_pFront;
    fl->m_pFront = node;
}
void flPopFront (forward_list fl)
{
    assert(fl != NULL);
    flNode tmp = fl->m_pFront;
    fl->m_pFront = fl->m_pFront->m_pNext;
    flDestroyNode(tmp);
}


///////////////////////////////////////////////////////
//iterator

struct flIt_t
{
    flNode m_node;
};


flIt flBegin(forward_list fl)
{
    assert(fl != NULL);
    flIt it = (flIt)malloc(sizeof(struct flIt_t));
    it->m_node = fl->m_pFront;
    return it;
}

void        flItNext (flIt it)
{
    assert(it != NULL);
    if (it->m_node != NULL) {
        it->m_node = it->m_node->m_pNext;
    }
}

fl_val_type flItKey  (flIt it)
{
    assert(it != NULL);
    assert(it->m_node != NULL);
    return it->m_node->m_key;
}

fl_val_type flItVal  (flIt it)
{
    assert(it != NULL);
    assert(it->m_node != NULL);
    return it->m_node->m_data;
}

int         flItIsEnd(flIt it)
{
    assert(it != NULL);
    return it->m_node == NULL;
}

///////////////////////////////////////////////////////

void erase_after(flIt it)
{
    assert(it != NULL);
    flNode tmp = it->m_node->m_pNext;
    it->m_node->m_pNext = it->m_node->m_pNext->m_pNext;
    flDestroyNode(tmp);
}

void insert_after(flIt it, fl_key_type key, fl_val_type val)
{
    assert(it != NULL);
    flNode node = flCreateNode(key, val);
    node->m_pNext = it->m_node->m_pNext;
    it->m_node->m_pNext = node;
}