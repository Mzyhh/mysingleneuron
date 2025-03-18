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
    char buffer[128];
    Token t;
    for (int i = 1; i < 6; i++) {
        buffer[i-1] = i + '0';
        buffer[i] = '\0';
        INIT_SS(ss, buffer);
        printf("origin: %s, ", ss.data + ss.offset);
        getToken(&ss, &t);
        printf("token: %.*s,", i, t.text);
        printf("cmp: %d\n", strncmp(t.text, buffer, t.len));
    }
    return EXIT_SUCCESS;
}
