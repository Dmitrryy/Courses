/****************************************************************************************
 *
 * main.c
 *
 * Created by dmitry
 * 22.02.2021
 *
 ***/

//
///======================================================================================
/// sem 4
/// task 1: container
///======================================================================================
///======================================================================================
//

#include <stdlib.h>

#include "RBTree/include/RBTree.h"

int main()
{
    rbTree tree;
    rbCreate(NULL, 0, &tree);

    rbPair p = {1, 2};
    rbInsert(tree, p);
    rbClear(tree);

    rbPair* pp = rbFind(tree, 1);


    printf("%d %d\n", p.key, p.value);
    if (pp != NULL)
        printf("%d %d\n", pp->key, pp->value);
}