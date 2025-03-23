#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"

void infix_tree(tNode *root) {
    if (root->left) infix_tree(root->left);
    printf("%.*s", (int)root->item.len, root->item.text);
    if (root->right) infix_tree(root->right);
}

void prefix_tree(tNode *root) {
    printf("%.*s ", (int)root->item.len, root->item.text);
    if (root->left) prefix_tree(root->left);
    if (root->right) prefix_tree(root->right);
}

void recursivePrintAST(tNode *root, int depth) {
    for (int i = 0; i < depth; ++i)
        printf(" ");
    printf("%.*s ", (int)root->item.len, root->item.text);
    printf("%d", root->item.type);
    if (root->left || root->right) printf(":\n");
    else printf("\n");
    if (root->left) recursivePrintAST(root->left, depth+1);
    if (root->right) recursivePrintAST(root->right, depth+1);
}

void printAST(tNode *root) {
    recursivePrintAST(root, 0);
}

int main(int argc, char *argv[]) {
    stringstream ss;
    ss.data = argv[1];
    ss.offset = 0;
    token t;
    while ((t = getToken(&ss)).type != None) {
        printf("%.*s ", (int)t.len, t.text);
    }
    printf("\n");
    ss.offset = 0;
    tNode* root = createAST(&ss); 
    printAST(root);
    return EXIT_SUCCESS;
}
