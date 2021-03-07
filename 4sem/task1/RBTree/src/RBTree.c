/****************************************************************************************
 *
 *   RBTree.c
 *
 *   Created by dmitry
 *   06.03.2021
 *
 ***/

#include "../include/RBTree.h"



/****************************************************************************************
 *
 *   defining structures
 *
 ***/

/// An attribute of every node in the tree. Used for balancing of tree.
enum color_t {BLACK, RED};


struct rbNode_t
{
    struct rbNode_t* parent, *left, *right; //TODO
    enum color_t     color;
    rbPair           pair;
};
typedef struct rbNode_t* rbNode;


struct rbMap_t
{
    struct rbNode_t* treeRoot;
};
/***
 *
 *   end of defining structures
 *
 ****************************************************************************************/


static void* calloc_h(size_t nnum, size_t size);


static int isInTree (struct rbMap_t* map, rb_key_type key);
static void insert (struct rbNode_t* parent, struct rbNode_t* node);

static void foreach_ (struct rbNode_t* tree, void (*act)(rbPair*, void*), void* data);

static void printTree_ (struct rbNode_t* tree, int indents);
static void printNode_ (rbNode node, int indents);

static struct rbNode_t* findTop (struct rbNode_t* node);
static struct rbNode_t* findParent (struct rbNode_t* tree, rb_key_type key);
static struct rbNode_t* findGrandparent(struct rbNode_t* node);
static struct rbNode_t* findUncle      (struct rbNode_t* node);
static struct rbNode_t* findBrother    (struct rbNode_t* node);

static struct rbNode_t* find_node_with_key_(rbTree tree, rb_key_type key);


static void leftRotation (struct rbNode_t* node);
static void rightRotation(struct rbNode_t* node);


static void replaceWithChild (struct rbNode_t* node, struct rbNode_t* child);

static void deleteTree (struct rbNode_t* tree);
static rbNode deleteNode        (rbNode node);
static void   deleteTheOnlyChild(struct rbNode_t* node);
static void   delete_case1 (struct rbNode_t* node);
static void   delete_case2 (struct rbNode_t* node);
static void   delete_case3 (struct rbNode_t* node);
static void   delete_case4 (struct rbNode_t* node);
static void   delete_case5 (struct rbNode_t* node);
static void   delete_case6 (struct rbNode_t* node);

static void   insert_case1 (struct rbNode_t* node);
static void   insert_case2 (struct rbNode_t* node);
static void   insert_case3 (struct rbNode_t* node);
static void   insert_case4 (struct rbNode_t* node);
static void   insert_case5 (struct rbNode_t* node);


rbResult rbCreate (const rbPair* data, size_t size, rbTree* tree)
{
    *tree = (struct rbMap_t*) CALLOC(1, sizeof(struct rbMap_t));

    if (*tree == NULL) {
        return RB_LACK_OF_MEMORY;
    }

    (*tree)->treeRoot = NULL;

    if (data == NULL || size == 0) {
        return RB_SUCCESS;
    }

    for (size_t i = 0; i < size; ++i) {
        rbResult res = rbInsert(*tree, data[i]);
        if (res != RB_SUCCESS) {
            rbDestroy(*tree);
            return res;
        }
    }

    return RB_SUCCESS;
}


rbResult rbDestroy (rbTree tree)
{
    deleteTree(tree->treeRoot);
    free (tree);
    return RB_SUCCESS;
}


rbResult rbClear (rbTree map) {

    if (map == NULL)
        return RB_INVALID_ARGS;

    deleteTree(map->treeRoot);
    map->treeRoot = NULL;
    return RB_SUCCESS;
}


rbPair* rbFind (rbTree tree, rb_key_type key)
{
    rbNode res = find_node_with_key_(tree, key);
    if (res == NULL)
        return NULL;

    return &res->pair;
}



void* calloc_h(size_t nnum, size_t size) {
    static int a = 0;

    if (a == 0 || a == 2) {
        a++;
        return NULL;
    }

    a++;
    return calloc (nnum,size);
}

rbResult rbInsert (rbTree tree, rbPair pair) {

    if (tree == NULL)
        return RB_INVALID_ARGS;

    struct rbNode_t* rNode = tree->treeRoot;

    if (isInTree(tree, pair.key)) {
        return RB_SUCCESS;
    }

    struct rbNode_t* node = (struct rbNode_t*) CALLOC(1, sizeof(struct rbNode_t));

    if (node == NULL) {
        return RB_LACK_OF_MEMORY;
    }

    //TODO
    rb_key_type * pKey = &node->pair.key;
    *pKey = pair.key;

    node->pair.value = pair.value;
    node->color = RED;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    struct rbNode_t* parent = findParent (rNode, node->pair.key); //TODO

    insert (parent, node); //TODO

    tree->treeRoot = findTop(node); //TODO

    return RB_SUCCESS;
}

rbResult rbErase (struct rbMap_t* tree, int key) {

    if (tree == NULL)
        return RB_INVALID_ARGS;

    struct rbNode_t* node = find_node_with_key_(tree, key);

    if (node != NULL)
        tree->treeRoot = deleteNode(node); //TODO

    return RB_SUCCESS;
}


rbResult isEmpty (struct rbMap_t* map) {

    if (map == NULL)
        return RB_INVALID_ARGS;

    return map->treeRoot == NULL;
}







static struct rbNode_t* find_node_with_key_(rbTree tree, rb_key_type key)
{
    if (tree->treeRoot == NULL)
        return NULL;

    struct rbNode_t* node = tree->treeRoot;

    struct rbNode_t* tmp = node;

    while (tmp) {
        if (tmp->pair.key > key)
            tmp = tmp->left;
        else if (tmp->pair.key < key)
            tmp = tmp->right;
        else
            return tmp;
    }

    return NULL;
}


//                              Find functions
//==========================================================================================

int isInTree (struct rbMap_t* map, int key) {
    return rbFind(map, key) != NULL;
}


struct rbNode_t* findTop (struct rbNode_t* node) {

    if (node == NULL)
        return NULL;

    struct rbNode_t* res = node;

    while (res->parent)
        res = res->parent;

    return res;
}


struct rbNode_t* findParent (struct rbNode_t* tree, int key) {

    if (tree == NULL)
        return NULL;

    if (tree->pair.key > key) {
        if (tree->left)
            return findParent(tree->left, key);
        else
            return tree;
    } else {
        if (tree->right)
            return findParent(tree->right, key);
        else
            return tree;
    }
}

static struct rbNode_t* findGrandparent(struct rbNode_t*node) {

    return node->parent->parent;
}

static struct rbNode_t* findUncle (struct rbNode_t* node) {

    struct rbNode_t* grandpa = findGrandparent(node);

    if (node->parent == grandpa->left)
        return grandpa->right;
    else
        return grandpa->left;
}

static struct rbNode_t* findBrother (struct rbNode_t* node) {

    if (node == NULL || node->parent == NULL) //TODO: if i ever call this function with NULL arguments?
        return NULL;

    if (node == node->parent->left)
        return node->parent->right;
    else
        return node->parent->left;
}

struct rbNode_t* findMax (struct rbNode_t* tree) {

    while (tree->right)
        tree = tree->right;

    return tree;
}

struct rbNode_t* findMin (struct rbNode_t* tree) {

    while (tree->left)
        tree = tree->left;

    return tree;
}


//                              Rotate functions
//==========================================================================================




static void leftRotation (struct rbNode_t* node) {

    struct rbNode_t* pivot = node->right;

    pivot->parent = node->parent; // and pivot can become the root of tree
    if (node->parent != NULL) {
        if (node->parent->left == node)
            node->parent->left = pivot;
        else
            node->parent->right = pivot;
    }

    node->right = pivot->left;
    if (pivot->left != NULL)
        pivot->left->parent = node;

    node->parent = pivot;
    pivot->left = node;
}

static void rightRotation(struct rbNode_t*node) {

    struct rbNode_t* pivot = node->left;

    pivot->parent = node->parent; // and pivot can become the root of tree
    if (node->parent != NULL) {
        if (node->parent->left == node)
            node->parent->left = pivot;
        else
            node->parent->right = pivot;
    }

    node->left = pivot->right;

    if (pivot->right != NULL)
        pivot->right->parent = node;

    node->parent = pivot;
    pivot->right = node;
}



//                              Inserting element
//==========================================================================================



void insert (struct rbNode_t* parent, struct rbNode_t* node) {

    node->parent = parent;

    if (parent == NULL) {
        insert_case1(node);
        return;
    }

    if (parent->pair.key > node->pair.key)
        parent->left = node;
    else
        parent->right = node;

    insert_case2(node);
}

static void insert_case1(struct rbNode_t* node) {

    if (node->parent == NULL)
        node->color = BLACK;
    else
        insert_case2(node);
}

static void insert_case2(struct rbNode_t* node) {

    if (node->parent->color == BLACK)
        return; /* Tree is still valid */
    else
        insert_case3(node);
}

static void insert_case3(struct rbNode_t* node) {

    struct rbNode_t*uncle = findUncle(node), *grandpa;

    if ((uncle != NULL) && (uncle->color == RED)) {

        node->parent->color = BLACK;
        uncle->color = BLACK;

        grandpa = findGrandparent(node);
        grandpa->color = RED;

        insert_case1(grandpa);
    } else {
        insert_case4(node);
    }
}

static void insert_case4(struct rbNode_t* node) {

    struct rbNode_t* grandpa = findGrandparent(node);

    if ((node == node->parent->right) && (node->parent == grandpa->left)) {

        leftRotation(node->parent);
        node = node->left;
    } else if ((node == node->parent->left) && (node->parent == grandpa->right)) {

        rightRotation(node->parent);
        node = node->right;
    }

    insert_case5(node);
}


static void insert_case5(struct rbNode_t* node) {

    struct rbNode_t* grandpa = findGrandparent(node);

    node->parent->color = BLACK;
    grandpa->color = RED;

    if ((node == node->parent->left) && (node->parent == grandpa->left)) {
        rightRotation(grandpa);
    } else {
        leftRotation(grandpa);
    }
}





//                              Deleting element
//==========================================================================================




struct rbNode_t* deleteNode (struct rbNode_t* node) {

    struct rbNode_t* M;
    struct rbNode_t* tmp = node;

    if (node->right)
        M = findMin (node->right);
    else if (node->left)
        M = findMax (node->left);
    else {
        tmp = node->parent;
        M = node;
    }

    node->pair.value = M->pair.value;

    //TODO
    int* pKey = &node->pair.key;
    *pKey = M->pair.key;
    //node->pair.key = M->pair.key;

    deleteTheOnlyChild(M);

    return findTop(tmp);
}


static void deleteTheOnlyChild(struct rbNode_t* node) {

    assert (node->left == NULL || node->right == NULL);

    struct rbNode_t*child;

    if (node->left == NULL && node->right == NULL) {

        if (node->color == BLACK)
            delete_case1(node);

        if (node->parent == NULL) {
            free(node);
            return;
        }

        if (node == node->parent->left)
            node->parent->left = NULL;
        else
            node->parent->right = NULL;

        free(node);
        return;
    }

    child = node->right;

    replaceWithChild (node, child);

    if (node->color == BLACK)//Cause node has only one child, child->color can be only RED
        child->color = BLACK;

    free(node);
}


static void replaceWithChild (struct rbNode_t* node, struct rbNode_t* child) {

    assert (child && node);

    child->parent = node->parent;

    if (node == node->parent->left)
        node->parent->left = child;
    else
        node->parent->right = child;
}


static void delete_case1 (struct rbNode_t* node)
{
    if (node->parent != NULL)
        delete_case2(node);
}

static void delete_case2 (struct rbNode_t* node) {

    struct rbNode_t* brother = findBrother (node);

    if (brother->color == RED) {
        node->parent->color = RED;
        brother->color = BLACK;

        if (node == node->parent->left)
            leftRotation (node->parent);
        else
            rightRotation (node->parent);
    }

    delete_case3 (node);
}

static void delete_case3 (struct rbNode_t* node) {

    struct rbNode_t*brother = findBrother (node);

    if ((node->parent->color == BLACK) &&
        (brother->color == BLACK) &&
        (brother->left == NULL || brother->left->color == BLACK) &&
        (brother->right == NULL || brother->right->color == BLACK)) {

        brother->color = RED;
        delete_case1(node->parent);
    } else
        delete_case4(node);
}

static void delete_case4 (struct rbNode_t* node) {

    struct rbNode_t*brother = findBrother (node);

    if ((node->parent->color == RED) &&
        (brother->color == BLACK) &&
        (brother->left == NULL  || brother->left->color == BLACK) &&
        (brother->right == NULL || brother->right->color == BLACK))  {

        brother->color = RED;
        node->parent->color = BLACK;
    } else
        delete_case5(node);
}

static void delete_case5 (struct rbNode_t* node) {

    struct rbNode_t*brother = findBrother (node);

    if  (brother->color == BLACK) {

        if ((node == node->parent->left) &&
            (brother->right == NULL || brother->right->color == BLACK) &&
            (brother->left && brother->left->color == RED)) { /* this last test is trivial too due to cases 2-4. */

            brother->color = RED;
            brother->left->color = BLACK;
            rightRotation(brother);

        } else if ((node == node->parent->right) &&
                   (brother->left == NULL || brother->left->color == BLACK) &&
                   (brother->right && brother->right->color == RED)) { /* this last test is trivial too due to cases 2-4. */

            brother->color = RED;
            brother->right->color = BLACK;
            leftRotation(brother);
        }
    }

    delete_case6(node);
}

static void delete_case6 (struct rbNode_t* node) {

    struct rbNode_t* brother = findBrother (node);

    brother->color = node->parent->color;
    node->parent->color = BLACK;

    if (node == node->parent->left) {
        brother->right->color = BLACK;
        leftRotation (node->parent);
    } else {
        brother->left->color = BLACK;
        rightRotation (node->parent);
    }
}


static void deleteTree (struct rbNode_t* tree) {

    if (tree == NULL)
        return;

    if (tree->left)
        deleteTree(tree->left);
    if (tree->right)
        deleteTree(tree->right);

    free (tree);
}


/****************************************************************************************
 *
 *   foreach functions
 *
 ***/
rbResult rbForeach (struct rbMap_t* tree, void (*act)(rbPair*, void*), void* data) {

    if (tree == NULL || act == NULL)
        return RB_INVALID_ARGS;

    foreach_(tree->treeRoot, act, data);
    return RB_SUCCESS;
}


static void foreach_ (struct rbNode_t* tree, void (*act)(rbPair*, void*), void* data) {

    if (tree == NULL)
        return;

    foreach_(tree->left, act, data);
    foreach_(tree->right, act, data);
    act (&tree->pair, data);
}
/***
 *
 *   end of foreach functions
 *
 ****************************************************************************************/




/****************************************************************************************
 *
 *   dump functions
 *
 ***/
rbResult rbDump (rbTree tree) {

    if (tree == NULL)
        return RB_INVALID_ARGS;

    printTree_(tree->treeRoot, 0);

    return RB_SUCCESS;
}


/// prints the current node and then the children indented one more.
/// \param tree - current node
/// \param indents - отступ
static void printTree_ (rbNode tree, int indents) {

    printNode_(tree, indents);

    if (tree == NULL)
        return;

    printTree_(tree->left, indents + 1);
    printTree_(tree->right, indents + 1);
}


/// prints the contents of the node indented appropriately.
/// \param node    - print node
/// \param indents - отступ
static void printNode_ (rbNode node, int indents)
{
    for (int i = 0; i < indents; ++i)
        printf("    |");

    if (node == NULL) {
        printf("NULL(B)\n");
    }
    else{
        printf("[key: %d, value: %d]", node->pair.key, node->pair.value);

        if (node->color == RED)
            printf("(R)\n");
        else
            printf("(B)\n");
    }
}
/***
 *
 *   end of dump functions
 *
 ****************************************************************************************/