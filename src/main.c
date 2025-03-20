#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

void print_tree(tNode *root) {
    if (root->left) print_tree(root->left);
    printf("%.*s", (int)root->item->len, root->item->text);
    if (root->right) print_tree(root->right);
}

int main(int argc, char *argv[]) {
    stringstream ss;
    ss.data = argv[1];
    ss.offset = 0;
    ss.isempty = stringstreamIsEmpty;
    tNode* root = createSyntaxTree(&ss); 
    print_tree(root);
    return EXIT_SUCCESS;
}
