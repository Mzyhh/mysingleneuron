#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

tNode *__expr(stringstream *ss);
tNode *__term(stringstream *ss);
tNode *__factor(stringstream *ss);

static const char *EXPR_OPS[NEXPR_OPS] = {"+", "-"};
static const char *TERM_OPS[NTERM_OPS] = {"*", "/"};

int isExprOp(Token t) {
    if (t.type != Op) return 0;
    for (int i = 0; i < NEXPR_OPS; ++i) {
        if (!strncmp(t.text, EXPR_OPS[i], t.len)) return 1;
    }
    return 0;
}

int isTermOp(Token t) {
    if (t.type != Op) return 0;
    for (int i = 0; i < NTERM_OPS; ++i) {
        if (!strncmp(t.text, TERM_OPS[i], t.len)) return 1;
    }
    return 0;
}

int isOp(Token t) {
    return t.type == Op;
}

tNode *__expr(stringstream *ss) {
    if (stringstreamIsEmpty(ss)) 
        return NULL;
    tNode *left = __term(ss);
    Token t = getToken(ss);
    if (isExprOp(t)) {
        tNode *op = (tNode*)malloc(sizeof(tNode));
        op->item = t;
        op->left = left;
        op->right = __expr(ss);
        return op;
    }
    return left;
}

tNode *__factor(stringstream *ss) {
    if (stringstreamIsEmpty(ss)) 
        return NULL;
    Token t = getToken(ss);
    if(t.type == None) {
        return NULL;
    }
    if (t.type == Open) {
        return __expr(ss);
    }
    CREATE_LEAF(result, t);
    return result;
}

tNode *__term(stringstream *ss) {
    tNode *left = __factor(ss);
    Token t = getToken(ss);
    if (isTermOp(t)) {
        tNode *right = __term(ss);
        tNode *op = (tNode*)malloc(sizeof(tNode));
        op->item = t;
        op->left = left;
        op->right = right;
        return op;
    }
    backToken(ss, t);
    return left;
}

tNode *createSyntaxTree(stringstream *ss) {
    tNode* root = NULL;
    if (!stringstreamIsEmpty(ss)) 
        root = __expr(ss);
    return root;
}
