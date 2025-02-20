#ifndef __TREE_H__
#define __TREE_H__

#define CREATE_LEAF(node, item)\
    tNode *node = (tNode*)malloc(sizeof(tNode));\
    node->item = item;\
    node->left = NULL;\
    node->right = NULL;

enum Type {Op, Num, Var, Open, Close};
typedef struct parserToken {
    enum Type type;
    char *varname;
    double number;
} Token;

typedef struct treeNode {
    void *item; 
    struct treeNode* left;
    struct treeNode* right;
} tNode;

#endif // __TREE_H__
