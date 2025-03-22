#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

void infix_tree(tNode *root) {
    if (root->left) infix_tree(root->left);
    printf("%.*s", (int)root->item.len, root->item.text);
    if (root->right) infix_tree(root->right);
}

void prefix_tree(tNode *root) {
    printf("%.*s", (int)root->item.len, root->item.text);
    if (root->left) prefix_tree(root->left);
    if (root->right) prefix_tree(root->right);
}

int main(int argc, char *argv[]) {
    stringstream ss;
    ss.data = argv[1];
    ss.offset = 0;
    tNode* root = createSyntaxTree(&ss); 
    prefix_tree(root);
    return EXIT_SUCCESS;
}
