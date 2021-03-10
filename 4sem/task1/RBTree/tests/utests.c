/*************************************************************************************************
 *
 *   utests.c
 *
 *   Created by dmitry
 *   09.03.2021
 *
 ***/

#include <assert.h>
#include "utests.h"
#include "stdlib.h"



int run_all_tests()
{
    test_calloc();
    test_rbCreate();
    //test_rbDestroy();
    test_rbForeach();
    test_rbFind();
    test_rbInsert();
    test_rbErase();
    test_rbEmpty();
    test_rbClear();
    test_rbDump();

    test_random();

    return 0;
}

rbPair g_in_pairs_1[] = {
        {3 , 1},
        {5 , 1},
        {1 , 1},
        {6 , 1},
        {9 , 1},
        {2 , 1},
        {16, 1},
        {10, 1},
        {7 , 1},
        {2 , 1},
        {22, 1},
        {8 , 1}
};
size_t g_size_in1 = 12;
int res_sum1 = 11;

rbPair g_in_pairs_2[] = {
        {4 , 1},
        {3 , 1},
        {7 , 1},
        {5 , 1},
        {10, 1},
        {12, 1}
};
size_t g_size_in2 = 6;
int res_sum2 = 6;

rbPair g_in_pairs_3[] = {
        {10, 1},
        {5 , 1},
        {16, 1},
        {20, 1},
        {2 , 1},
        {8 , 1},
        {12, 1},
        {13, 1}
};
size_t g_size_in3 = 8;
int res_sum3 = 8;


void test_calloc()
{
    rbTree tree = NULL;
    assert(rbCreate(NULL, 0, &tree) == RB_LACK_OF_MEMORY);

    rbPair p = { 1, 2 };
    assert(rbCreate(&p, 1, &tree) == RB_LACK_OF_MEMORY);
}



void test_rbCreate()
{
    rbTree tree = NULL;
    assert(rbCreate(g_in_pairs_1, g_size_in1, &tree) == RB_SUCCESS);
    assert(rbDestroy(tree) == RB_SUCCESS);

    assert(rbCreate(NULL, 0, &tree) == RB_SUCCESS);
    assert(rbDestroy(tree) == RB_SUCCESS);

    assert(rbCreate(NULL, 0, NULL) == RB_INVALID_ARGS);
}


void sum(rbPair* pair, void* data) {
    *((int*)data) += pair->value;
}

void test_rbForeach()
{
    rbTree tree = NULL;
    assert(rbCreate(g_in_pairs_1, g_size_in1, &tree) == RB_SUCCESS);

    int res = 0;
    assert(rbForeach(tree, &sum, &res) == RB_SUCCESS);
    assert(res == res_sum1);
    assert(rbForeach(NULL, &sum, &res) == RB_INVALID_ARGS);
    assert(rbForeach(tree, NULL, &res) == RB_INVALID_ARGS);

    assert(rbDestroy(tree) == RB_SUCCESS);
}


void test_rbFind()
{
    rbTree tree = NULL;
    assert(rbCreate(g_in_pairs_1, g_size_in1, &tree) == RB_SUCCESS);

    for (size_t i = 0; i < g_size_in1; ++i)
    {
        rbPair * res = rbFind(tree, g_in_pairs_1[i].key);
        assert(res != NULL);
        assert(res->key == g_in_pairs_1[i].key);
        assert(res->value == g_in_pairs_1[i].value);
    }
    assert(rbFind(tree, -91276287) == NULL);

    assert(rbDestroy(tree) == RB_SUCCESS);
}


void test_rbInsert()
{
    assert(rbInsert(NULL, g_in_pairs_1[0]) == RB_INVALID_ARGS);

    rbTree tree = NULL;
    assert(rbCreate(NULL, 0, &tree) == RB_SUCCESS);

    for (size_t i = 0; i < g_size_in1; ++i)
    {
        assert(rbInsert(tree, g_in_pairs_1[i]) == RB_SUCCESS);

        rbPair * res = rbFind(tree, g_in_pairs_1[i].key);
        assert(res != NULL);
        assert(res->key == g_in_pairs_1[i].key);
        assert(res->value == g_in_pairs_1[i].value);
    }

    rbPair p = g_in_pairs_1[0];
    p.value += 1;
    assert(rbInsert(tree, p) == RB_SUCCESS);
    rbPair * res = rbFind(tree, p.key);
    assert(res != NULL);
    assert(res->key == p.key);
    assert(res->value == (g_in_pairs_1[0].value + 1));

    assert(rbDestroy(tree) == RB_SUCCESS);
}


void test_rbErase()
{
    assert(rbErase(NULL, 0) == RB_INVALID_ARGS);

    rbTree tree = NULL;
    assert(rbCreate(g_in_pairs_1, g_size_in1, &tree) == RB_SUCCESS);
    assert(rbErase(tree, -213213) == RB_SUCCESS);
    for (size_t i = 0; i < g_size_in1; ++i) {
        rbPair p = g_in_pairs_1[i];
        assert(rbErase(tree, p.key) == RB_SUCCESS);
        assert(rbFind(tree, p.key) == NULL);
    }
    assert(rbDestroy(tree) == RB_SUCCESS);


    assert(rbCreate(g_in_pairs_1, g_size_in1, &tree) == RB_SUCCESS);
    assert(rbErase(tree, -213213) == RB_SUCCESS);
    for (size_t i = g_size_in1; i != 0; --i) {
        rbPair p = g_in_pairs_1[i - 1];
        assert(rbErase(tree, p.key) == RB_SUCCESS);
        assert(rbFind(tree, p.key) == NULL);
    }
    assert(rbDestroy(tree) == RB_SUCCESS);


    assert(rbCreate(g_in_pairs_1, g_size_in1, &tree) == RB_SUCCESS);
    assert(rbErase(tree, g_in_pairs_1[7].key) == RB_SUCCESS);
    assert(rbDestroy(tree) == RB_SUCCESS);

    assert(rbCreate(g_in_pairs_1, g_size_in1, &tree) == RB_SUCCESS);
    assert(rbErase(tree, g_in_pairs_1[1].key) == RB_SUCCESS);
    assert(rbDestroy(tree) == RB_SUCCESS);


    assert(rbCreate(g_in_pairs_2, g_size_in2, &tree) == RB_SUCCESS);
    assert(rbErase(tree, g_in_pairs_2[0].key) == RB_SUCCESS);
    assert(rbDestroy(tree) == RB_SUCCESS);


    assert(rbCreate(g_in_pairs_3, g_size_in3, &tree) == RB_SUCCESS);
    assert(rbErase(tree, g_in_pairs_3[0].key) == RB_SUCCESS);
    assert(rbDestroy(tree) == RB_SUCCESS);

    rbPair arr[] =
            {{1, 1},
             {2, 1},
             {4, 1},
             {5, 1},
             {8, 1},
             {7, 3},
             {9, 1},
             {10, 1}};
    assert(rbCreate(arr, 8, &tree) == RB_SUCCESS);
    assert(rbErase(tree, g_in_pairs_3[7].key) == RB_SUCCESS);
    assert(rbErase(tree, g_in_pairs_3[1].key) == RB_SUCCESS);
    assert(rbDestroy(tree) == RB_SUCCESS);
}


void test_rbEmpty()
{
    assert(rbEmpty(NULL) == RB_INVALID_ARGS);


    rbTree tree = NULL;
    assert(rbCreate(g_in_pairs_1, g_size_in1, &tree) == RB_SUCCESS);
    assert(!rbEmpty(tree));
    assert(rbDestroy(tree) == RB_SUCCESS);



    assert(rbCreate(NULL, 0, &tree) == RB_SUCCESS);
    assert(rbEmpty(tree));
    assert(rbDestroy(tree) == RB_SUCCESS);

}


void test_rbClear()
{
    assert(rbClear(NULL) == RB_INVALID_ARGS);

    rbTree tree = NULL;
    assert(rbCreate(g_in_pairs_1, g_size_in1, &tree) == RB_SUCCESS);
    assert(!rbEmpty(tree));
    assert(rbClear(tree) == RB_SUCCESS);
    assert(rbEmpty(tree));
    assert(rbDestroy(tree) == RB_SUCCESS);

    assert(rbCreate(NULL, 0, &tree) == RB_SUCCESS);
    assert(rbClear(tree) == RB_SUCCESS);
    assert(rbEmpty(tree));
    assert(rbDestroy(tree) == RB_SUCCESS);
}


void test_rbDump()
{
    assert(rbDump(NULL) == RB_INVALID_ARGS);

    rbTree tree = NULL;
    assert(rbCreate(g_in_pairs_1, g_size_in1, &tree) == RB_SUCCESS);
    assert(rbDump(tree) == RB_SUCCESS);
    assert(rbDestroy(tree) == RB_SUCCESS);

    assert(rbCreate(NULL, 0, &tree) == RB_SUCCESS);
    assert(rbDump(tree) == RB_SUCCESS);
    assert(rbDestroy(tree) == RB_SUCCESS);
}


void test_random()
{
    rbTree tree = NULL;
    assert(rbCreate(g_in_pairs_1, g_size_in1, &tree) == RB_SUCCESS);

    for(size_t i = 0; i < 1000000; ++i)
    {
        rbPair p = { rand(), rand() };
        rbInsert(tree, p);
    }

    for(size_t i = 0; i < 500000; ++i)
    {
        rbErase(tree, rand());
    }

    assert(rbDestroy(tree) == RB_SUCCESS);
}