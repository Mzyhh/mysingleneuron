#ifndef __TREE_H__
#define __TREE_H__

#define MAX_TOKEN_LEN 16
#define CREATE_LEAF(node, it)\
    tNode *node = (tNode*)malloc(sizeof(tNode));\
    node->item = (it);\
    node->left = NULL;\
    node->right = NULL;
#define MAKE_LEAF(node, it)\
    node->item = (it);\
    node->left = NULL;\
    node->right = NULL;


typedef struct treeNode {
    void *item; 
    struct treeNode* left;
    struct treeNode* right;
} tNode;

#endif // __TREE_H__
