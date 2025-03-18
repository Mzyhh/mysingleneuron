#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

void print_tree(tNode *root) {
    if (root->left) print_tree(root->left);
    printf("%.*s", (int)root->item->len, root->item->text);
    if (root->right) print_tree(root->right);
}

int main(int argc, char *argv[]) {
    char c[1];
    return EXIT_SUCCESS;
}
