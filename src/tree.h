#ifndef __TREE_H__
#define __TREE_H__

#define MAX_TOKEN_LEN 16
#define CREATE_LEAF(node, item)\
    tNode *node = (tNode*)malloc(sizeof(tNode));\
    node->item = item;\
    node->left = NULL;\
    node->right = NULL;

typedef struct parserToken {
    enum {Op, Num, Var, Open, Close} type;
    union {
        char varname[MAX_TOKEN_LEN];
        double number;
    } data;
} Token;

typedef struct treeNode {
    void *item; 
    struct treeNode* left;
    struct treeNode* right;
} tNode;

#endif // __TREE_H__
