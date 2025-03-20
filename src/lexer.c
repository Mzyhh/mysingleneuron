#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

tNode *__expr(stringstream *ss);
tNode *__term(stringstream *ss);
tNode *__factor(stringstream *ss);


int isOp(Token t) {
    return t.type == Op;
}

tNode *__expr(stringstream *ss) {
    if (ss->isempty(ss)) 
        return NULL;
    tNode *left = __term(ss);
    Token t = getToken(ss);
    if (t.type == None){
        return left;
    }
    if(t.text[0] == '+' || t.text[0] == '-') {
        tNode *op = (tNode*)malloc(sizeof(tNode));
        op->item = t;
        op->left = left;
        op->right = __expr(ss);
        return op;
    }
    return left;
}

tNode *__factor(stringstream *ss) {
    if (ss->isempty(ss)) 
        return NULL;
    Token t = getToken(ss);
    if(t.type == None) {
        return NULL;
    }
    if (t.type == Open) {
        return __expr(ss);
    }
    tNode *result = (tNode*)malloc(sizeof(tNode));
    MAKE_LEAF(result, t);
    return result;
}

tNode *__term(stringstream *ss) {
    tNode *left = __factor(ss);
    Token t = getToken(ss);
    if (t.type == None || !isOp(t) || t.text[0] == '+' || t.text[0] == '-') {
        backToken(ss, t);
        return left;
    }
    tNode *right = __term(ss);
    tNode *op = (tNode*)malloc(sizeof(tNode));
    op->item = t;
    op->left = left;
    op->right = right;
    return op;
}

tNode *createSyntaxTree(stringstream *ss) {
    tNode* root = NULL;
    if (!ss->isempty(ss)) 
        root = __expr(ss);
    return root;
}
